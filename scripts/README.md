# Build and test helpers

This directory retains helpers used by the build, supported tool features, and regression tests.

- `ls_parse.py`: linker-script support for `goto-cc`; installed by `src/Makefile`.
- `minisat-2.2.1-patch` and `glucose-syrup-patch`: solver download/build targets.
- `minisat2_CMakeLists.txt` and `glucose_CMakeLists.txt`: solver build templates referenced by `src/solvers/CMakeLists.txt`.
- `cpplint.py` and `extract_type_header.py`: helpers used by the retained regression suites.
- `generate_vcxproj`, `vcxproj.1`, `vcxproj.2`, and `vcvars64.ps1`: Visual Studio build helpers.
- `bash-autocomplete/`: command-line completion generation referenced by `src/cbmc/CMakeLists.txt`.

The standard Deagle build remains `cd src && make`.
