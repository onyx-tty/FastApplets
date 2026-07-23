FROM ubuntu:latest AS builder

RUN set -ex;                                                                             \
    apt-get update;                                                                      \
    apt-get install -y qt6-base-dev libtomlplusplus-dev cmake git g++ ninja-build clang;

COPY . /app
WORKDIR /app

RUN set -ex;                     \
    cmake --preset Debug;        \
    cmake --build --preset Debug

RUN ctest --preset Debug --output-on-failure

RUN cmake --install build/Debug --prefix /FastApplets

COPY config /FastApplets/

FROM ubuntu:latest AS runner

RUN set -ex;                                                             \
    apt-get update;                                                      \
    apt-get install -y qt6-base-dev libqt6svg6 libtomlplusplus-dev cmake

COPY --from=builder /FastApplets /FastApplets

ENTRYPOINT ["/FastApplets/bin/PowerApplet"]

