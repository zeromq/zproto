zcodec
======

# Overview

This is a binary codec generator.

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

* Write your protocol as an XML file, using model/zcodec_example.xml as a starting point.
* Generate your protocol, using model/generate as a starting point.
* Add the generated .h and .c class to your git repository.
* Don't modify the generated code. Fix the model, and regenerate.

