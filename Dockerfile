FROM gcc:14-bookworm

RUN apt install libssl-dev -y

WORKDIR /usr/src/app

COPY Makefile Makefile.inc ./
COPY include include
COPY src src
COPY . .

RUN make all

ENTRYPOINT ["./bin/stegobmp"]
