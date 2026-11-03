#!/usr/bin/env python3
"""Run with: python3 test.py /path/to/deagle_exe [--output-dir DIR]."""

import argparse
from collections import Counter
import hashlib
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET


NS = {'g': 'http://graphml.graphdrawing.org/xmlns'}
RACE_SPEC = 'CHECK( init(main()), LTL(G ! data-race) )'
CASES = [
    ('write-write', 1, [(('CREATE1',), 'WRITE'), (('CREATE2',), 'WRITE')]),
    ('read-write', 2, [(('CREATE1',), 'WRITE'), (('CREATE_READER',), 'READ')]),
    ('nested', 3, [(('CREATE1',), 'WRITE'), (('CREATE_LAUNCHER', 'NESTED'), 'WRITE')]),
    ('main-child', 4, [(('CREATE1',), 'WRITE'), ((), 'MAIN_WRITE')]),
    ('loop-create', 5, [(('LOOP',), 'WRITE'), (('LOOP',), 'WRITE')]),
    ('safe', 6, []),
    ('multiple-candidates', 7, [(('CREATE2',), 'WRITE'), (('CREATE3',), 'WRITE')]),
    ('conditional-create', 8, [(('CREATE1',), 'WRITE'), (('CREATE2',), 'WRITE')]),
    ('ordinary-assert', 9, None),
    ('reach', 10, None),
    ('creation-order', 11,
     [(('ORDERED_PARENT', 'NESTED'), 'WRITE'), (('ORDERED_WRITER',), 'WRITE')]),
    ('same-line-create', 12,
     [(('SAME_LINE',), 'DIRECT_WRITE'), (('SAME_LINE',), 'DIRECT_WRITE')]),
    ('helper-access', 13,
     [(('FIRST_CREATE',), 'HELPER_WRITE'), (('LAST_CREATE',), 'HELPER_WRITE')]),
    ('function-pointer', 14,
     [(('POINTER_CREATE',), 'DIRECT_WRITE'), (('SECOND_CREATE',), 'DIRECT_WRITE')]),
    ('64bit', 1, [(('CREATE1',), 'WRITE'), (('CREATE2',), 'WRITE')]),
    ('assertions-enabled', 1, [(('CREATE1',), 'WRITE'), (('CREATE2',), 'WRITE')]),
]


def data(element):
    return {item.attrib['key']: item.text or '' for item in element.findall('g:data', NS)}


def check_witness(path, source, expected, race, architecture):
    graph = ET.parse(path).getroot().find('g:graph', NS)
    metadata = data(graph)
    assert metadata['witness-type'] == 'violation_witness'
    assert metadata['specification'] == (
        RACE_SPEC if race else 'CHECK( init(main()), LTL(G ! call(reach_error())) )')
    assert metadata['programfile'] == 'main.c'
    assert metadata['programhash'].lower() == hashlib.sha256(source).hexdigest()
    assert metadata['architecture'] == architecture
    nodes = {node.attrib['id']: data(node) for node in graph.findall('g:node', NS)}
    violations = [node for node, values in nodes.items() if values.get('violation') == 'true']
    assert len(violations) == 1, violations
    if not race:
        return

    # Follow both access orders, rather than relying on XML element ordering.
    entries = [node for node, values in nodes.items() if values.get('entry') == 'true']
    assert len(entries) == 1, entries
    outgoing = {}
    for edge in graph.findall('g:edge', NS):
        outgoing.setdefault(edge.attrib['source'], []).append(edge)
    visited = set()

    def visit(node, ancestors):
        assert node in nodes and node not in ancestors
        visited.add(node)
        if node not in outgoing:
            assert node == violations[0]
            return [[]]
        return [[data(edge)] + suffix for edge in outgoing[node]
                for suffix in visit(edge.attrib['target'], ancestors | {node})]

    paths = visit(entries[0], set())
    assert visited == set(nodes)
    assert len(paths) == 2
    assert paths[0][:-2] == paths[1][:-2]
    assert paths[0][-2:] == list(reversed(paths[1][-2:]))
    edges = paths[0]
    assert edges[0]['threadId'] == edges[0]['createThread'] == '0'
    assert edges[0]['enterFunction'] == 'main'

    labels = {match[1]: line for line, text in enumerate(source.decode().splitlines(), 1)
              if (match := re.search(r'// ([A-Z_0-9]+)$', text))}
    paths, entered, registrations = {0: ()}, {0}, []
    for edge in edges[1:-2]:
        thread = int(edge['threadId'])
        assert thread in paths, ('unregistered thread', thread)
        assert not any(key.startswith('assumption') for key in edge)
        if 'createThread' in edge:
            child = int(edge['createThread'])
            assert child not in paths
            assert thread in entered
            assert edge['originfile'] == 'main.c'
            line = int(edge['startline'])
            paths[child] = paths[thread] + (line,)
            registrations.append(line)
        else:
            assert thread not in entered
            entered.add(thread)
            last_create = paths[thread][-1]
            entry = ('launcher' if last_create in {
                labels.get('CREATE_LAUNCHER'), labels.get('ORDERED_PARENT')}
                else 'reader' if last_create == labels.get('CREATE_READER') else 'writer')
            assert edge['enterFunction'] == entry, (edge, entry)
    assert entered == set(paths)
    targets = []
    for edge in edges[-2:]:
        assert 'createThread' not in edge and 'enterFunction' not in edge
        assert not any(key.startswith('assumption') for key in edge)
        thread = int(edge['threadId'])
        assert thread in entered
        assert edge['originfile'] == 'main.c'
        targets.append((thread, paths[thread], int(edge['startline'])))
    assert len({thread for thread, _, _ in targets}) == 2
    actual = Counter((path, line) for _, path, line in targets)
    wanted = Counter((tuple(labels[label] for label in path), labels[line])
                     for path, line in expected)
    assert actual == wanted, (actual, wanted)
    assert labels.get('SKIPPED') not in registrations
    if labels.get('LOOP') in registrations:
        assert registrations.count(labels['LOOP']) == 2
    if labels.get('ORDERED_PARENT') in registrations:
        assert registrations == [labels[label] for label in
                                 ('ORDERED_PARENT', 'NESTED', 'ORDERED_WRITER')]


def run_cases(executable, output):
    fixtures = Path(__file__).resolve().parent
    for name, case, expected in CASES:
        directory = output / name
        directory.mkdir(parents=True, exist_ok=True)
        source = fixtures / ('../yaml-witness/main.c' if case <= 11 else 'main.c')
        shutil.copyfile(source, directory / 'main.c')
        witness = directory / 'witness.graphml'
        witness.write_text('stale witness must be removed\n')
        flags = ['--datarace'] if case != 10 else []
        if case not in (9, 10) and name != 'assertions-enabled':
            flags.append('--no-assertions')
        flags.append('--64' if name == '64bit' else '--32')
        command = [str(executable), 'main.c', '-DCASE=' + str(case),
                   '--refined-pointer-analysis', '--mm', 'sc', '--unwind', '4',
                   '--graphml-witness', str(witness), *flags]
        result = subprocess.run(command, cwd=directory, capture_output=True,
                                text=True, timeout=60)
        (directory / 'output.log').write_text(result.stdout + result.stderr)
        try:
            assert result.returncode == (0 if case == 6 else 10), result.returncode
            verdict = 'VERIFICATION SUCCESSFUL' if case == 6 else 'VERIFICATION FAILED'
            assert result.stdout.rstrip().splitlines()[-1] == verdict
            assert witness.exists() == (case != 6)
            if witness.exists():
                check_witness(witness, source.read_bytes(), expected, case not in (9, 10),
                              '64bit' if name == '64bit' else '32bit')
        except AssertionError:
            print(name, ' '.join(command))
            print(result.stdout + result.stderr)
            if witness.exists():
                print(witness.read_text())
            raise
        print(name + ': PASS', flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('deagle_exe', type=Path)
    parser.add_argument('--output-dir', type=Path)
    args = parser.parse_args()
    executable = args.deagle_exe.resolve(strict=True)
    if args.output_dir:
        run_cases(executable, args.output_dir.resolve())
    else:
        with tempfile.TemporaryDirectory(prefix='deagle-graphml-race-') as temporary:
            run_cases(executable, Path(temporary))


if __name__ == '__main__':
    main()
