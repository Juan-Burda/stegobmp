FROM gcc:14-bookworm

WORKDIR /usr/src/app

COPY Makefile Makefile.inc ./
COPY include include
COPY src src

RUN make all

ENTRYPOINT ["./bin/stegobmp"]
