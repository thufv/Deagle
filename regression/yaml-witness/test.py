#!/usr/bin/env python3
"""Run with: python3 test.py /path/to/deagle_exe (requires PyYAML)."""

import argparse
from collections import Counter
import hashlib
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

import yaml


CASES = [
    ("write-write", 1, [(('CREATE1',), 'WRITE'), (('CREATE2',), 'WRITE')]),
    ("read-write", 2, [(('CREATE1',), 'WRITE'), (('CREATE_READER',), 'READ')]),
    ("nested", 3, [(('CREATE1',), 'WRITE'), (('CREATE_LAUNCHER', 'NESTED'), 'WRITE')]),
    ("main-child", 4, [(('CREATE1',), 'WRITE'), ((), 'MAIN_WRITE')]),
    ("loop-create", 5, [(('LOOP',), 'WRITE'), (('LOOP',), 'WRITE')]),
    ("safe", 6, []),
    ("multiple-candidates", 7, [(('CREATE2',), 'WRITE'), (('CREATE3',), 'WRITE')]),
    ("conditional-create", 8, [(('CREATE1',), 'WRITE'), (('CREATE2',), 'WRITE')]),
    ("ordinary-assert", 9, []),
    ("reach", 10, [((), 'REACH')]),
    ("creation-order", 11,
     [(('ORDERED_PARENT', 'NESTED'), 'WRITE'), (('ORDERED_WRITER',), 'WRITE')]),
]


def check_witness(path, source, expected, race):
    entries = yaml.safe_load(path.read_text())
    assert len(entries) == 1
    entry = entries[0]
    assert entry['entry_type'] == 'violation_sequence'
    assert entry['metadata']['format_version'] == '2.2'
    task = entry['metadata']['task']
    assert task['input_files'] == ['main.c']
    assert task['input_file_hashes']['main.c'].lower() == hashlib.sha256(source).hexdigest()
    assert task['specification'] == (
        'CHECK( init(main()), LTL(G ! data-race) )' if race else
        'CHECK( init(main()), LTL(G ! call(reach_error())) )')
    lines = source.decode().splitlines()
    labels = {match[1]: line for line, text in enumerate(lines, 1)
              if (match := re.search(r'// ([A-Z_0-9]+)$', text))}
    paths = {0: ()}
    registrations = []
    targets = []
    segments = entry['content']
    for index, segment in enumerate(segments):
        waypoints = [item['waypoint'] for item in segment['segment']]
        for waypoint in waypoints:
            assert waypoint['action'] == 'follow'
            location = waypoint['location']
            assert location['file_name'] == 'main.c'
            line = location['line']
            column = location.get('column')
            if column is not None:
                assert 0 < column <= len(lines[line - 1])
            thread = int(waypoint.get('thread_id', 0))
            assert thread in paths, ('unregistered thread', thread)
            if waypoint['type'] == 'function_enter':
                assert index < len(segments) - 1
                if column is not None:
                    assert lines[line - 1][column - 1] == ')'
                registrations.append(line)
                paths[len(paths)] = paths[thread] + (line,)
            else:
                assert waypoint['type'] == 'target'
                assert index == len(segments) - 1
                text = lines[line - 1]
                if column is not None:
                    assert column == len(text) - len(text.lstrip()) + 1
                targets.append((thread, paths[thread], line))
    assert len(segments[-1]['segment']) == len(expected)
    if race:
        assert len({thread for thread, _, _ in targets}) == 2
    actual = Counter((path, line) for _, path, line in targets)
    wanted = Counter((tuple(labels[label] for label in path), labels[line])
                     for path, line in expected)
    assert actual == wanted, (actual, wanted)
    assert labels['SKIPPED'] not in registrations
    if labels['LOOP'] in registrations:
        assert registrations.count(labels['LOOP']) == 2
    if labels['ORDERED_PARENT'] in registrations:
        assert registrations == [labels[label] for label in
                                 ('ORDERED_PARENT', 'NESTED', 'ORDERED_WRITER')]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('deagle_exe', type=Path)
    args = parser.parse_args()
    executable = args.deagle_exe.resolve(strict=True)
    source = Path(__file__).with_name('main.c')
    with tempfile.TemporaryDirectory(prefix='deagle-yaml-test-') as temporary:
        for name, case, expected in CASES:
            directory = Path(temporary) / name
            directory.mkdir()
            shutil.copyfile(source, directory / 'main.c')
            witness = directory / 'witness.yml'
            witness.write_text('stale witness must be removed\n')
            flags = ['--datarace'] if case != 10 else []
            if case not in (9, 10):
                flags.append('--no-assertions')
            command = [str(executable), 'main.c', '-DCASE=' + str(case),
                       '--refined-pointer-analysis', '--mm', 'sc', '--unwind', '4',
                       '--yaml-witness', str(witness), *flags]
            result = subprocess.run(command, cwd=directory, capture_output=True,
                                    text=True, timeout=60)
            try:
                assert result.returncode == (0 if case == 6 else 10), result.returncode
                assert witness.exists() == bool(expected)
                if expected:
                    check_witness(witness, source.read_bytes(), expected, case != 10)
            except AssertionError:
                print(name, ' '.join(command))
                print(result.stdout + result.stderr)
                if witness.exists():
                    print(witness.read_text())
                raise
            print(name + ': PASS', flush=True)


if __name__ == '__main__':
    main()
