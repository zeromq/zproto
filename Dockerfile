FROM zeromqorg/gsl

RUN apt-get install -y build-essential libtool autoconf automake

COPY . /tmp/zproto
WORKDIR /tmp/zproto
RUN ( ./autogen.sh; ./configure; make check; make install )
