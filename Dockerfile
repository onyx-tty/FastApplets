FROM ubuntu:latest AS builder

RUN set -ex;                                                                                          \
    apt-get update;                                                                                   \
    apt-get install -y cmake ninja-build g++ git clang qt6-base-dev libgtest-dev libtomlplusplus-dev;

COPY . /app
WORKDIR /app

RUN set -ex;                                \
    cmake --preset Debug -DBUILD_TESTS=OFF; \
    cmake --build build/Debug

#RUN ctest --test-dir /app/build/Debug

