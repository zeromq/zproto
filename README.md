# zproto - a protocol framework for ZeroMQ

## Man Page

zproto is several things:

* A code generator capable of producing fast and efficient binary codecs for ZeroMQ-based protocols.

* A code generator capable of producing full-featured protocol servers based on a high-level finite-state machine model.

* A base project for your ZeroMQ C protocol clients and servers, that use these two techniques.

To use zproto, clone the repository at https://github.com/zeromq/zproto.

Build and test using the usual commands:

    ./autogen.sh
    ./configure
    make check

And then install the code generators:

    make install

Next, read the model/zproto_example.xml file to learn how to write your own protocol specifications. The binary codec has the same name, and is src/zproto_example.c and include/zproto_example.h.

To rebuild the codec, first build and install https://github.com/imatix/gsl. Then run these commands:

    cd src
    make code check

Finally, to use zproto as the base for your own projects, copy the skeleton directory into a new location, and create a new git repository there:

    git init .

And then manually change all references to 'myproj' to your own project prefix, both in filenames, and inside the sources. We may script this, later.

## The Codec Generator

Goals of the codec generator:

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

## The Server Generator

While ZeroMQ gives you a powerful communications engine to use in many different ways, building a conventional server is still fairly heavy work. We use the ROUTER socket for that, managing the state for each individual client connection. The zproto project includes a tool that generates whole servers, in C, from state machine models.

### Quick Background

zproto is based on work by Hintjens I did for Chapter 7 of the ZeroMQ book, used in FileMQ, Zyre, and several other projects. It's a collection of code generation tools that take models and turn them into perfect code.

iMatix used to do code generation as our main business. We got... very good at it. There are lots of ways to generate code, and the most powerful and sophisticated code generator ever built by mankind lives on Github.com at [imatix/gsl](https://github.com/imatix/gsl). It's an interpreter for programs that eat models (self-describing documents) and spew out text of any shape and form.

The only problem with sophisticated magic like GSL is that it quickly excludes other people. So in ZeroMQ I've been very careful to not do a lot of code generation, only opening that mysterious black box when there was real need.

The first case was in CZMQ, to generate the classes for ZeroMQ socket options. Then in CZMQ, to [generate project files](https://github.com/zeromq/czmq/tree/master/model) (for various build systems) from [a single project.xml](https://github.com/zeromq/czmq/blob/master/model/project.xml) file. Yes, we still use XML models. It's actually a good use case for simple schema-free XML.

Then I started generating binary codecs for protocols, [starting with FILEMQ](https://github.com/zeromq/filemq/blob/master/model/fmq_msg.xml). We used these codecs for a few different projects and they started to be quite solid. Something like a protobufs for ZeroMQ. You can see that the generated code [looks as good](https://github.com/zeromq/filemq/blob/master/include/fmq_msg.h) as hand-written code. It's actually better: [more consistent, with fewer errors](https://github.com/zeromq/filemq/blob/master/src/fmq_msg.c).

Finally, I drew back on an even older iMatix speciality, which was state machines. My first free software tool was [Libero](http://legacy.imatix.com/html/libero/), a great tool for designing code as state machines and producing lovely, robust engines in pretty much any language. Libero predates GSL, so isn't as flexible. However it uses a very elegant and simple state-event-action model.

The Libero model is especially good at designing server-side logic, where you want to capture the exact state machine for a client connection, from start to end. This happens to be one of the heavier problems in ZeroMQ architecture: how to build capable protocol servers. We do a lot of this, it turns out. You can do only so much with low-level patterns like pub-sub and push-pull. Quite soon you need to implement stateful services.

So this is what I made: a GSL code generator that takes a finite-state machine model inspired by Libero, and turns out a full working server. The current code generator produces C (that builds on CZMQ). In this article I'll explain briefly how this works, and how to use it.

### The State Machine Model

State machines are a little unusual, conceptually. If you're not familiar with them it'll take a few days before they click. The Libero model is fairly simple and high level, meant to be designed and understood by humans:

* The machine exists in one of a number of named *states*. By convention the machine starts in the first state.

* In each state, the machine accepts one of a set of named *events*. Unhandled events are either ignored or cause the machine to die, depending on your taste.

* Given an event in a state, the machine executes a list of *actions*, which correspond to your code.

* After executing the actions the machine moves to the *next state*. An empty next state means "stay in same state".

* In the next state, the machine either continues with an *internal event* produced by the previous actions, or waits for an *external event* coming as a protocol command.

* Any action can raise an *exception event* that interrupts the flow through the action list and to the next state.

* A *defaults state* collects events that may occur in any state, together with their actions.

### The zproto Server Model

The zproto_server_c.gsl code generator outputs a single .h file called an *engine* that does the hard work. If needed, it'll also generate you a skeleton .c file for your server, which you edit and build. It doesn't re-create that file again, though it will append new action stubs.

The server is a class that exposes an API like this (taken from the [zeromq/zbroker](https://github.com/zeromq/zbroker/tree/master/src) zpipes_server, a good example):

    //  Create a new zpipes_server
    zpipes_server_t *
        zpipes_server_new (void);

    //  Destroy the zpipes_server
    void
        zpipes_server_destroy (zpipes_server_t **self_p);

    //  Load server configuration data
    void
        zpipes_server_configure (zpipes_server_t *self, const char *config_file);

    //  Set one configuration path value
    void
        zpipes_server_setoption (zpipes_server_t *self, const char *path, const char *value);

    //  Binds the server to an endpoint, formatted as printf string
    long
        zpipes_server_bind (zpipes_server_t *self, const char *format, ...);

    //  Self test of this class
    void
        zpipes_server_test (bool verbose);

Rather than run the server as a main program, you write a main program that creates and works with server objects. These run as background services, accepting clients on a ZMQ ROUTER port. The bind method exposes that port to the outside world.

Here's the state machine for the ZPIPES server:

    <class
        name = "zpipes_server"
        title = "ZPIPES server"
        proto = "zpipes_msg"
        script = "zproto_server_c"
        >
        This is a server implementation for the ZPIPES protocol
        <include filename = "license.xml" />

        <!-- State machine for a client connection -->
        <state name = "start">
            <event name = "INPUT" next = "reading">
                <action name = "open pipe for input" />
                <action name = "send" message = "INPUT OK" />
            </event>
            <event name = "OUTPUT" next = "writing">
                <action name = "open pipe for output" />
                <action name = "send" message = "OUTPUT OK" />
            </event>
        </state>

        This state allows two protocol commands, READ and CLOSE.
        Names of states, events, and actions are case insensitive.
        By convention we use uppercase for protocol events:

        <state name = "reading">
            <event name = "READ" next = "expecting chunk">
                <action name = "expect chunk on pipe" />
            </event>
            <event name = "CLOSE">
                <action name = "close pipe" />
                <action name = "send" message = "CLOSE OK" />
                <action name = "terminate" />
            </event>
        </state>

        This internal state shows how we make a decision in the
        state machine. These are three internal events; they can
        happen come immediately from 'expect chunk on pipe', or
        come later from various places. The server can ask for a
        wakeup event (timeout expired). We can also get events
        from other clients' machines.

        <state name = "expecting chunk">
            <event name = "have chunk" next = "reading">
                <action name = "clear pending reads" />
                <action name = "fetch chunk from pipe" />
                <action name = "send" message = "READ OK" />
            </event>
            <event name = "pipe terminated" next = "reading">
                <action name = "clear pending reads" />
                <action name = "send" message = "END OF PIPE" />
            </event>
            <event name = "timeout expired" next = "reading">
                <action name = "clear pending reads" />
                <action name = "send" message = "TIMEOUT" />
            </event>
        </state>

        <state name = "writing">
            <event name = "WRITE" next = "writing">
                <action name = "store chunk to pipe" />
                <action name = "send" message = "WRITE OK" />
            </event>
            <event name = "CLOSE">
                <action name = "close pipe" />
                <action name = "send" message = "CLOSE OK" />
                <action name = "terminate" />
            </event>
        </state>

        The defaults state handles an 'exception_event' (sends
        FAILED back to the client). It also specifies that any
        unrecognized protocol command (an '$other' event) will
        also cause the engine to send FAILED back to the client.

        <state name = "defaults">
            <event name = "exception">
                <action name = "send" message = "FAILED" />
                <action name = "terminate" />
            </event>
            <event name = "$other">
                <action name = "send" message = "FAILED" />
            </event>
        </state>

        We can define API methods to pass commands back from the
        application through to the engine.

        <!-- API methods
        <method name = "somename" return = "number">
            One-line description of method here
            <argument name = "some value" type = "string">What is this argument?</argument>
            <argument name = "some value" type = "number">What is this argument?</argument>
        </method>
        -->
    </class>

The generated server manages clients automatically. There's various options to expire dead clients, and so on -- read the zproto_server_c.gsl code to understand more. The state machine applies to *single client connection* from start to finish.

There are two predefined actions: "send" and "terminate". Other actions map to functions in your own code. The engine calls these functions as needed, passing the client context:

    static void
    store_chunk_to_pipe (client_t *self)
    {
        //  State machine guarantees we're in a valid state for writing
        assert (self->pipe);
        assert (self->writing);

        //  Always store chunk on list, even to pass to pending reader
        zlist_append (self->pipe->queue, zpipes_msg_get_chunk (self->request));
        if (self->pipe->reader) {
            send_event (self->pipe->reader, have_chunk_event);
            assert (zlist_size (self->pipe->queue) == 0);
        }
    }

There are a set of methods your actions can call:

    //  Set the next event, needed in at least one action in an internal
    //  state; otherwise the state machine will wait for a message on the
    //  router socket and treat that as the event.
    static void set_next_event (client_t *self, event_t event);

    //  Raise an exception with 'event', halting any actions in progress.
    //  Continues execution of actions defined for the exception event.
    static void raise_exception (client_t *self, event_t event);

    //  Set wakeup alarm after 'delay' msecs. The next state should
    //  handle the wakeup event. The alarm is cancelled on any other
    //  event.
    static void set_wakeup_event (client_t *self, size_t delay, event_t event);

    //  Execute 'event' on specified client. Use this to send events to
    //  other clients. Cancels any wakeup alarm on that client.
    static void send_event (client_t *self, event_t event);

### For More Information

Though [the Libero documentation](http://legacy.imatix.com/html/libero/) is quite old now, it's useful as a guide to what's possible with state machines. The Libero model added superstates, substates, and other useful ways to manage larger state machines.

The current working example of the zproto server generator is the [zeromq/zbroker](https://github.com/zeromq/zbroker) project, and specifically the zpipes_server class.

You can find [GSL on Github](https://github.com/imatix/gsl) and there's a [old backgrounder](http://download.imatix.com/mop/) for the so-called "model oriented programming" we used at iMatix.
