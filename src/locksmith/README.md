# locksmith

![Build workflow](https://github.com/vesalvojdani/locksmith/actions/workflows/build.yml/badge.svg)

Locksmith is a static analysis tool that tries to find data races in multithreaded C programs. To do that, it implements a static version of the Lockset algorithm. A race occurs whenever two threads access a memory location without any synchronization, and one of the accesses is a write. Locksmith analyzes multithreaded programs that use locks (or mutexes) to protect accesses to shared locations, and tries to discover which, if any, locks protect each shared location. Whenever an access to a shared memory location is not protected by any lock, it potentially is a race.

See also [http://www.cs.umd.edu/projects/PL/locksmith/].

## Build Instructions

The following should work on a fresh installation of Ubuntu 22.04.
Clone this repository and run the following in the checked out directory:

```console
sudo apt install gcc opam autoconf automake make gperf python3 indent emacs flex libfl-dev bison
git submodule update --init --recursive
opam init -n
opam switch create . 3.11.2
eval $(opam env --switch=. --set-switch)
./configure
make
```

If you get errors, try to rebuild banshee:

```console
cd banshee
make
git checkout -- .
git clean -df
make
```

And now the entire project should build:

```console
cd ..
make
```

Try it with `./locksmith --list-guardedby simple.c`.
