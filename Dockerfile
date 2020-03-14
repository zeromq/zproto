FROM zeromqorg/gsl

RUN DEBIAN_FRONTEND=noninteractive apt-get update -y -q
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q --force-yes build-essential autoconf automake libtool pkg-config

COPY packaging/docker/run_zproto.sh /usr/local/bin/run_zproto.sh

COPY . /tmp/zproto
WORKDIR /tmp/zproto
RUN ( ./autogen.sh; ./configure; make check; make install )
RUN rm -rf /tmp/zproto
ENTRYPOINT ["run_zproto.sh"]
