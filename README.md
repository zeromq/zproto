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

* Any action can set an *exception event* that interrupts the flow through the action list and to the next state.

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

    //  Set one configuration option value
    void
        zpipes_server_set (zpipes_server_t *self, const char *path, const char *value);

    //  Binds the server to an endpoint, formatted as printf string
    long
        zpipes_server_bind (zpipes_server_t *self, const char *format, ...);

    //  Self test of this class
    void
        zpipes_server_test (bool verbose);

Rather than run the server as a main program, you write a main program that creates and works with server objects. These run as background services, accepting clients on a ZMQ ROUTER port. The bind method exposes that port to the outside world.

Your input to the code generator is two XML files (without schemas, DTDs, entity encodings!) that defines a set of 'states', and the protocol messages as used to generate the codec. Here is a minimal 'hello_server.xml' example that defines a Hello, World server:

    <class
        name = "hello_server"
        title = "Hello server"
        proto = "hello_msg"
        script = "zproto_server_c"
        >
        A Hello, World server

        <state name = "start">
            <event name = "HELLO">
                <action name = "send" message = "WORLD" />
            </event>
        </state>
    </class>

Names of states, events, and actions are case insensitive. By convention however we use uppercase for protocol events. In this case, HELLO and WORLD are two messages that must be defined in the hello_msg.xml file.

The generated server manages clients automatically. To build this, do:

    gsl -q -trace:1 -animate:1 hello_server.xml

The first time you do this, you'll get a hello_server.c source file. You can edit that; it won't be regenerated. The generated code goes, instead, into hello_server_engine.h. Take a look if you like.

The trace option shows all protocol messages received and sent. The animate option shows the state machine as it moves through states and actions.

There are two predefined actions: "send", which sends a specific protocol message, and "terminate", which ends the client connection. Other actions map to functions in your own code, e.g.:

    <state name = "start">
        <event name = "HELLO" next = "start">
            <action name = "tell the user hello too" />
            <action name = "send" message = "WORLD" />
        </event>
    </state>

    ...
    
    static void
    tell_the_user_hello_too (client_t *self)
    {
        puts ("Hello, World!");
    }

Your server code (the actions) gets a small API to work with:

    //  Set the next event, needed in at least one action in an internal
    //  state; otherwise the state machine will wait for a message on the
    //  router socket and treat that as the event.
    static void
    engine_set_next_event (client_t *self, event_t event);

    //  Raise an exception with 'event', halting any actions in progress.
    //  Continues execution of actions defined for the exception event.
    static void
    engine_set_exception (client_t *self, event_t event);

    //  Set wakeup alarm after 'delay' msecs. The next state should
    //  handle the wakeup event. The alarm is cancelled on any other
    //  event.
    static void
    engine_set_wakeup_event (client_t *self, size_t delay, event_t event);

    //  Execute 'event' on specified client. Use this to send events to
    //  other clients. Cancels any wakeup alarm on that client.
    static void
    engine_send_event (client_t *self, event_t event);

### Message Filtering & Priorities

The generated engine implements a simple yet useful form of message filtering:

* If a protocol message is not valid in the current state (i.e. there is no matching event), then it is queued for later processing.

* When entering a new state, the engine will pull and process protocol messages from oldest to newest, matching events in the state.

You can use this to mix blocking and non-blocking work on the same connection. For instance, one command may involve some work (such as waiting on other connections) that can block for arbitrary periods. At the same time the client may send heartbeat commands that the engine has to respond to:

    <state name = "start">
        <event name = "HELLO" next = "waiting">
            <action name = "wait on other internal event" />
        </event>
        <event name = "PING">
            <action name = "send" message = "PONG" />
        </event>
    </state>

    <state name = "waiting">
        <event name = "ok" next = "start">
            <action name = "tell the user hello too" />
            <action name = "send" message = "WORLD" />
        </event>
        <event name = "error" next = "start">
            <action name = "terminate" />
        </event>
        <event name = "PING">
            <action name = "send" message = "PONG" />
        </event>
    </state>

Note here that while the engine is waiting for an internal event, it will continue to process PING commands, but will not process HELLO commands.

As side-effect of message filtering, the client can 'pipeline' messages, e.g. sending multiple HELLOs before waiting for WORLD replies.

### Superstates

Superstates are a shorthand to reduce the amount of error-prone repetition in a state machine. Here is the same state machine using a superstate:

    <state name = "start" inherit = "defaults">
        <event name = "HELLO" next = "waiting">
            <action name = "wait on other internal event" />
        </event>
    </state>

    <state name = "waiting" inherit = "defaults">
        <event name = "ok" next = "start">
            <action name = "tell the user hello too" />
            <action name = "send" message = "WORLD" />
        </event>
        <event name = "error" next = "start">
            <action name = "terminate" />
        </event>
    </state>

    <state name = "defaults">
        <event name = "PING">
            <action name = "send" message = "PONG" />
        </event>
    </state>

Note the logic of PING, which says, "when the client sends PING, respond by sending PONG, and then stay in the same state." You can't currently use super-superstates.

For complex protocols you can collect error handling together using the wildcard event, "*", which means "all other protocol events in this state". For example:

    <state name = "defaults">
        <event name = "PING">
            <action name = "send" message = "PONG" />
        </event>
        <event name = "*">
            <action name = "log unexpected client message" />
            <action name = "terminate" />
        </event>
    </state>

### Client and Server Properties

In your server code, you have two structures, client_t and server_t. Note that the client_t structure MUST always contain these variables (the request and reply will use whatever protocol name you defined):

    server_t *server;           //  Reference to parent server
    hello_msg_t *request;       //  Last received request
    hello_msg_t *reply;         //  Reply to send out, if any

### Client Connection Expiry

If you define an "expired" event anywhere in your dialog, the server will automatically expire idle clients after a timeout, which defaults to 60 seconds. It's smart to put this into a superstate:

    <state name = "defaults">
        <event name = "PING">
            <action name = "send" message = "PONG" />
        </event>
        <event name = "expired">
            <action name = "terminate" />
        </event>
    </state>

To tune the expiry time, use this method (e.g. to set to 1 second):

    hello_server_set (self, "server/timeout", "1000");

The server timeout can also come from a configuration file, see below. It is good practice to do heartbeating by sending a PING from the client and responding to that with a PONG or suchlike. Do not heartbeat from the server to clients; that is fragile.

### Server Configuration File

You can call the 'configure' method on the server object to configure it, and you can also call the 'set' method later to change individual configuration options. The configuration file format is ZPL (ZeroMQ RFC 5), which looks like this:

    #   Default zbroker configuration
    #
    hello_server
        echo = I: starting hello service on tcp://*:8888
        bind
            endpoint = tcp://*:8888

    #   Apply to all servers that load this config file
    server
        timeout = 10        #   Client connection timeout
        background = 1      #   Run as background process
        workdir = .         #   Working directory for daemon

'echo' and 'bind' in the 'hello_server' section are executed automatically.

### CZMQ Reactor Integration

The generated engine offers zloop integration so you can monitor your own sockets for activity and execute callbacks when messages arrive on them. Use this API method:

    //  Poll socket for activity, invoke handler on any received message.
    //  Handler must be a CZMQ zloop_fn function; receives server as arg.

    static void
    engine_handle_socket (server_t *server, void *socket, zloop_fn handler);

The engine invokes the handler with the 'server' as the argument. Here is the general style of using such a handler. First, in the 'server_initialize' function:

    engine_handle_socket (self, self->some_socket, some_handler);

Where 'some_socket' is a ZeroMQ socket, and where 'some_handler' looks like this:

    static int
    some_handler (zloop_t *loop, zmq_pollitem_t *item, void *argument)
    {
        server_t *self = (server_t *) argument;
        zmsg_t *msg = zmsg_recv (self->some_socket);
        if (!msg)
            return 0;               //  Interrupted; do nothing
        zmsg_dump (msg);            //  Nice during development
        ... process the message
        return 0;                   //  0 = continue, -1 = end reactor
    }

### For More Information

Though [the Libero documentation](http://legacy.imatix.com/html/libero/) is quite old now, it's useful as a guide to what's possible with state machines. The Libero model added superstates, substates, and other useful ways to manage larger state machines.

The current working example of the zproto server generator is the [zeromq/zbroker](https://github.com/zeromq/zbroker) project, and specifically the zpipes_server class.

You can find [GSL on Github](https://github.com/imatix/gsl) and there's a [old backgrounder](http://download.imatix.com/mop/) for the so-called "model oriented programming" we used at iMatix.
