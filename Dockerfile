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

