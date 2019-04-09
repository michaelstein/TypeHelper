#!/bin/bash
SOURCE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

mkdir -p "$SOURCE_DIR/../build-TypeHelper-conan"
cd "$SOURCE_DIR/../build-TypeHelper-conan"

conan install $SOURCE_DIR -pr=gcc820d
source activate.sh && cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" $SOURCE_DIR
source deactivate.sh
