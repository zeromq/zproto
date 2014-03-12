# zproto - a protocol framework for ZeroMQ

## Man Page

zproto is two things. First, a code generator capable of producing fast
and efficient binary codecs for ZeroMQ-based protocols. Second, a base
project for your ZeroMQ C protocol clients and servers.

To use zproto, clone the repository at https://github.com/zeromq/zproto.

Build and test using the usual commands:

    ./autogen.sh
    ./configure
    make check

And then install the code generator:

    make install

Next, read the model/zproto_example.xml file to learn how to write your
own protocol specifications. The binary codec has the same name, and is
src/zproto_example.c and include/zproto_example.h.

To rebuild the codec, first build and install https://github.com/imatix/gsl.
Then run these commands:

    cd generate
    ./generate
    cd ..
    make check

Finally, to use zproto as the base for your own projects, clone the git
repository, then:

    rm .git
    rm model
    git init .

And then manually change all references to 'zproto' to your own project
prefix, both in filenames, and inside the sources. We may script this, later.

## More Detail

Goals:

* Very good performance on little-changing data.
* Full flexibility on often-changing data (headers).
* Portable to any programming language.
* Built for ZeroMQ.
* Easy to use.

The origin of this project is [Chapter 7 of the ZeroMQ Guide](http://zguide.zeromq.org/page:all#Serializing-Your-Data).

To use this tool, please build and install the [iMatix GSL code generator](https://github.com/imatix/gsl).

To contribute patches back to this code, please send GitHub pull requests, adding your name to AUTHORS. The full contract for contributions is ZeroMQ RFC 22, http://rfc.zeromq.org/spec:22, with the change of using the MIT license.

To use:

* Write your protocol as an XML file, using model/zproto_example.xml as a starting point.
* Generate your protocol, using model/generate as a starting point.
* Add the generated .h and .c class to your git repository.
* Don't modify generated codecs. Change the model, and regenerate.
