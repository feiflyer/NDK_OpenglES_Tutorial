#!/bin/bash

# 这里需要配置你的NDK路径
NDK_PATH="/Users/fly/Documents/Android/SDK/ndk/21.4.7075529/"
# 注意我这里是苹果系统，如果是Linux系统是不一样的
HOST_PLATFORM="darwin-x86_64"

COMMON_OPTIONS="
    --with-zlib=no
    --with-bzip2=no
    --with-png=no
    --with-harfbuzz=no
    --with-brotli=no
    --with-sysroot=${SYSROOT}
    "
TOOLCHAIN_PREFIX="${NDK_PATH}/toolchains/llvm/prebuilt/${HOST_PLATFORM}/bin"
SYSROOT="${NDK_PATH}/toolchains/llvm/prebuilt/${HOST_PLATFORM}/sysroot"

make clean

FOLDER=`pwd`/android-libs/arm64-v8a
mkdir -p ${FOLDER}
CC=${TOOLCHAIN_PREFIX}/aarch64-linux-android21-clang \
./configure \
    --prefix=${FOLDER} \
    --libdir=${FOLDER} \
    --host=aarch64-linux-android \
    --enable-debug \
    ${COMMON_OPTIONS}

make -j4 && make install


