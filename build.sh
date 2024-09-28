#!/bin/sh

set -e

BUILD_TYPE="${BUILD_TYPE=Debug}"
BUILD_THREADS="${BUILD_THREADS=4}"

PROJECT_PATH="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
CMAKE_PATH="$PROJECT_PATH/CMakeLists.txt"

BUILD_FOLDER="$PROJECT_PATH/build"
PG_TEMPLATE_BUILD_FOLDER="$BUILD_FOLDER"
EXECUTABLE_NAME="pg-template"


build(){
    if [ ! -d "$PG_TEMPLATE_BUILD_FOLDER" ]; then
        mkdir -p "$PG_TEMPLATE_BUILD_FOLDER"
        cd "$PG_TEMPLATE_BUILD_FOLDER"
    fi

    cd "$PG_TEMPLATE_BUILD_FOLDER"
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $PROJECT_PATH
    make -j$BUILD_THREADS
    cd "$PROJECT_PATH"
}

run(){
    build
    $PG_TEMPLATE_BUILD_FOLDER/$EXECUTABLE_NAME
}



if [ "$1" = "run" ]; then
    run
else
    build
fi
