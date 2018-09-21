#!/usr/bin/env bash
set -e

OUTPUT_FILENAME="yoga.js"

# clean up build folder
rm -rf build && mkdir -p build

# compile to wasm
docker run --rm -v $(pwd):$(pwd) -w $(pwd) -u emscripten -t trzeci/emscripten:sdk-incoming-64bit emcc \
  yoga/*.cpp bindings/*.cc \
  --bind --memory-init-file 0 --closure 0 --llvm-lto 1 \
  -fno-exceptions \
  --pre-js pre.js \
  -g4 \
  -s WASM=1 \
  -s EXPORTED_RUNTIME_METHODS=[] \
  -s DISABLE_EXCEPTION_CATCHING=1 \
  -s AGGRESSIVE_VARIABLE_ELIMINATION=1 \
  -s NO_EXIT_RUNTIME=1 \
  -s ASSERTIONS=2 \
  -s SINGLE_FILE=1 \
  -s FILESYSTEM=0 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s WASM_MEM_MAX=512MB \
  -s USE_PTHREADS=1 \
  -s PROXY_TO_PTHREAD=1 \
  -s PTHREAD_POOL_SIZE=4 \
  -s EMULATE_FUNCTION_POINTER_CASTS=1 \
  -s RESERVED_FUNCTION_POINTERS=1 \
  -s DEMANGLE_SUPPORT=1 \
  -s "DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=['memcpy','memset','malloc','free','strlen']" \
  -o build/$OUTPUT_FILENAME

cp ./build/yoga.js ./build/yoga.module.js
echo 'import PthreadMain from "../build/pthread-main";' >> build/yoga.module.js
echo 'export default Module' >> build/yoga.module.js

# cp ./build/pthread-main.js ./dist/pthread-main.js