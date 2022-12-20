FROM gcc:latest
RUN apt update -y
RUN apt install -y cmake cppcheck clang-tidy clang-format libgtest-dev libc6-dbg gdb python3-pip
RUN pip install cpplint
RUN wget https://sourceware.org/pub/valgrind/valgrind-3.19.0.tar.bz2 && \
    tar xfv valgrind-3.19.0.tar.bz2 && \
    cd valgrind-3.19.0 && \
    ./autogen.sh && \
    ./configure && \
    make && \
    make install
RUN cd /tmp && \
    wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.bz2 && \
    tar --bzip2 -xf boost_1_76_0.tar.bz2 && \
    cd boost_1_76_0 && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    rm -rf /tmp/*

