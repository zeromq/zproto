/*  =========================================================================
    example_peer - Example Peer

    =========================================================================
*/

/*
@header
    Description of class for man page.
@discuss
    Detailed discussion of the class, if any.
@end
*/

#include "czmq.h"
//  TODO: Change these to match your project's needs
#include "../include/example_peer_msg.h"
#include "../include/example_peer.h"

//  Forward reference to method arguments structure
typedef struct _client_args_t client_args_t;

//  This structure defines the context for a client connection
typedef struct {
    //  These properties must always be present in the client_t
    //  and are set by the generated engine. The cmdpipe gets
    //  messages sent to the actor; the msgpipe may be used for
    //  faster asynchronous message flows.
    zsock_t *cmdpipe;           //  Command pipe to/from caller API
    zsock_t *msgpipe;           //  Message pipe to/from caller API
//    zsock_t *dealer;            //  Socket to talk to server
    zyre_t  * zyre;
    example_peer_msg_t *message;  //  Message to/from server
    client_args_t *args;        //  Arguments from methods

    //  TODO: Add specific properties for your application
} client_t;

//  Include the generated client engine
#include "example_peer_engine.inc"

//  Allocate properties and structures for a new client instance.
//  Return 0 if OK, -1 if failed

static int
client_initialize (client_t *self)
{
    return 0;
}

//  Free properties and structures for a client instance

static void
client_terminate (client_t *self)
{
    //  Destroy properties here
}


//  ---------------------------------------------------------------------------
//  Selftest

void
example_peer_test (bool verbose)
{
    printf (" * example_peer: ");
    if (verbose)
        printf ("\n");

    //  @selftest
    zactor_t *client = zactor_new (example_peer, NULL);
    example_peer_verbose = verbose;
    zactor_destroy (&client);
    //  @end
    printf ("OK\n");
}



//  ---------------------------------------------------------------------------
//  output_chat_message
//

static void
output_chat_message (client_t *self)
{
    zsys_info("%s:%s",example_peer_msg_name(self->message),example_peer_msg_message(self->message));
}


//  ---------------------------------------------------------------------------
//  generate_chat_message
//

static void
generate_chat_message (client_t *self)
{
    example_peer_msg_set_name(self->message,zyre_name(self->zyre));
    example_peer_msg_set_uuid(self->message,zyre_uuid(self->zyre));
    example_peer_msg_set_message(self->message,self->args->message);
    
}
