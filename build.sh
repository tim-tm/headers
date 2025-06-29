#!/usr/bin/env bash

set -xe

mkdir -p build

clang -Wall -Wextra -pedantic -I./include/ -std=c99 -ggdb2 -g examples/ht.c -o build/ht -lcrypto
clang -I./include/ --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -O3 -o build/ht.wasm examples/wasm.c
