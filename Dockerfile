FROM debian:stable-slim
RUN apt-get update
RUN apt-get install -y gcc git
RUN git clone https://github.com/ytaki0801/kilo-lisp.git
WORKDIR /kilo-lisp
RUN cc -static -DJMC -o kilo-lisp kilo-lisp.c jmclisp.c

FROM busybox
WORKDIR /
COPY --from=0 /kilo-lisp/kilo-lisp .
COPY --from=0 /kilo-lisp/sample.jmc .
CMD ["./kilo-lisp", "sample.jmc"]

