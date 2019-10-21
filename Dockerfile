FROM ubuntu:18.04

COPY ./bitcoin.conf /root/.bitcoin/bitcoin.conf
COPY . /tmp/bitcoin-ecc
WORKDIR /tmp/bitcoin-ecc

RUN apt update && \
    apt install -y software-properties-common && \
    apt install -y build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 && \
    apt install -y libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev && \
    apt install -y libboost-test-dev libboost-thread-dev libdb-dev libdb++-dev && \
    apt autoclean

RUN ./autogen.sh
RUN ./configure --with-incompatible-bdb
RUN make clean && make -j2 && make install && make clean

EXPOSE 9776 9777
EXPOSE 19776 19777
EXPOSE 19887 19888

ENTRYPOINT ["bitcoind", "--printtoconsole"]
CMD ["bitcoind", "--printtoconsole"]
