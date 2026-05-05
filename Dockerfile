FROM ubuntu:latest AS builder

RUN set -ex;                                                                                          \
    apt-get update;                                                                                   \
    apt-get install -y cmake ninja-build g++ git clang qt6-base-dev libgtest-dev libtomlplusplus-dev;

COPY . /app
WORKDIR /app

RUN set -ex;                  \
    cmake --preset Debug      \
    cmake --build build/Debug

#RUN ctest --test-dir /app/build/Debug

RUN cmake --install build/Debug --prefix /FastApplets

COPY config /FastApplets/

FROM ubuntu:latest AS runner

RUN set -ex;                                                                      \
    apt-get update;                                                               \
    apt-get install -y cmake qt6-base-dev libtomlplusplus-dev libasan8 libqt6svg6

COPY --from=builder /FastApplets /FastApplets

ENTRYPOINT ["/FastApplets/bin/PowerApplet"]
