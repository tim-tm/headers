# headers

This is a small collection of stb-style libraries.
They are mostly wasm-compatible.

- [hashtable](include/ht.h)

## Compiling

Compilation requires a POSIX-based system, bash, clang and lld (for wasm-ld).

```bash
./build.sh
```

You may want to run a native example:

```bash
./build/ht
```

or a wasm example (requires python):

```bash
python3 -m http.server
```
