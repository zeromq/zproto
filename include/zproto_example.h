/*  =========================================================================
    zproto_example - zproto example protocol

    Codec header for zproto_example.

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

     * The XML model used for this code generation: zproto_example.xml, or
     * The code generation script that built this file: zproto_codec_c
    ************************************************************************
    Copyright (C) 2014 the Authors                                         
                                                                           
    Permission is hereby granted, free of charge, to any person obtaining  
    a copy of this software and associated documentation files (the        
    "Software"), to deal in the Software without restriction, including    
    without limitation the rights to use, copy, modify, merge, publish,    
    distribute, sublicense, and/or sell copies of the Software, and to     
    permit persons to whom the Software is furnished to do so, subject to  
    the following conditions:                                              
                                                                           
    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.                 
                                                                           
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF             
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 
    =========================================================================
*/

#ifndef ZPROTO_EXAMPLE_H_INCLUDED
#define ZPROTO_EXAMPLE_H_INCLUDED

/*  These are the zproto_example messages:

    LOG - Log an event.
        sequence            number 2    
        version             number 2    Version
        level               number 1    Log severity level
        event               number 1    Type of event
        node                number 2    Sending node
        peer                number 2    Refers to this peer
        time                number 8    Log date/time
        host                string      Originating hostname
        data                longstr     Actual log message

    STRUCTURES - This message contains a list and a hash.
        sequence            number 2    
        aliases             strings     List of strings
        headers             hash        Other random properties

    BINARY - Deliver a multi-part message.
        sequence            number 2    
        flags               octets [4]  A set of flags
        public_key          chunk       Our public key
        identifier          uuid        Unique identity
        address             frame       Return address as frame
        content             msg         Message to be delivered

    TYPES - Demonstrate custom-defined types
        sequence            number 2    
        client_forename     string      Given name
        client_surname      string      Family name
        client_mobile       string      Mobile phone number
        client_email        string      Email address
        supplier_forename   string      Given name
        supplier_surname    string      Family name
        supplier_mobile     string      Mobile phone number
        supplier_email      string      Email address
*/

#define ZPROTO_EXAMPLE_VERSION              1

#define ZPROTO_EXAMPLE_LOG                  1
#define ZPROTO_EXAMPLE_STRUCTURES           2
#define ZPROTO_EXAMPLE_BINARY               3
#define ZPROTO_EXAMPLE_TYPES                4
#define ZPROTO_EXAMPLE_FLAGS_SIZE           4

#include <czmq.h>

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
#ifndef ZPROTO_EXAMPLE_T_DEFINED
typedef struct _zproto_example_t zproto_example_t;
#define ZPROTO_EXAMPLE_T_DEFINED
#endif

//  @interface
//  Create a new empty zproto_example
zproto_example_t *
    zproto_example_new (void);

//  Destroy a zproto_example instance
void
    zproto_example_destroy (zproto_example_t **self_p);

//  Receive a zproto_example from the socket. Returns 0 if OK, -1 if
//  there was an error. Blocks if there is no message waiting.
int
    zproto_example_recv (zproto_example_t *self, zsock_t *input);

//  Send the zproto_example to the output socket, does not destroy it
int
    zproto_example_send (zproto_example_t *self, zsock_t *output);

//  Print contents of message to stdout
void
    zproto_example_print (zproto_example_t *self);

//  Get/set the message routing id
zframe_t *
    zproto_example_routing_id (zproto_example_t *self);
void
    zproto_example_set_routing_id (zproto_example_t *self, zframe_t *routing_id);

//  Get the zproto_example id and printable command
int
    zproto_example_id (zproto_example_t *self);
void
    zproto_example_set_id (zproto_example_t *self, int id);
const char *
    zproto_example_command (zproto_example_t *self);

//  Get/set the sequence field
uint16_t
    zproto_example_sequence (zproto_example_t *self);
void
    zproto_example_set_sequence (zproto_example_t *self, uint16_t sequence);

//  Get/set the level field
byte
    zproto_example_level (zproto_example_t *self);
void
    zproto_example_set_level (zproto_example_t *self, byte level);

//  Get/set the event field
byte
    zproto_example_event (zproto_example_t *self);
void
    zproto_example_set_event (zproto_example_t *self, byte event);

//  Get/set the node field
uint16_t
    zproto_example_node (zproto_example_t *self);
void
    zproto_example_set_node (zproto_example_t *self, uint16_t node);

//  Get/set the peer field
uint16_t
    zproto_example_peer (zproto_example_t *self);
void
    zproto_example_set_peer (zproto_example_t *self, uint16_t peer);

//  Get/set the time field
uint64_t
    zproto_example_time (zproto_example_t *self);
void
    zproto_example_set_time (zproto_example_t *self, uint64_t time);

//  Get/set the host field
const char *
    zproto_example_host (zproto_example_t *self);
void
    zproto_example_set_host (zproto_example_t *self, const char *value);

//  Get/set the data field
const char *
    zproto_example_data (zproto_example_t *self);
void
    zproto_example_set_data (zproto_example_t *self, const char *value);

//  Get/set the aliases field
zlist_t *
    zproto_example_aliases (zproto_example_t *self);
//  Get the aliases field and transfer ownership to caller
zlist_t *
    zproto_example_get_aliases (zproto_example_t *self);
//  Set the aliases field, transferring ownership from caller
void
    zproto_example_set_aliases (zproto_example_t *self, zlist_t **aliases_p);

//  Get a copy of the headers field
zhash_t *
    zproto_example_headers (zproto_example_t *self);
//  Get the headers field and transfer ownership to caller
zhash_t *
    zproto_example_get_headers (zproto_example_t *self);
//  Set the headers field, transferring ownership from caller
void
    zproto_example_set_headers (zproto_example_t *self, zhash_t **hash_p);

//  Get/set the flags field
byte *
    zproto_example_flags (zproto_example_t *self);
void
    zproto_example_set_flags (zproto_example_t *self, byte *flags);

//  Get a copy of the public_key field
zchunk_t *
    zproto_example_public_key (zproto_example_t *self);
//  Get the public_key field and transfer ownership to caller
zchunk_t *
    zproto_example_get_public_key (zproto_example_t *self);
//  Set the public_key field, transferring ownership from caller
void
    zproto_example_set_public_key (zproto_example_t *self, zchunk_t **chunk_p);

//  Get a copy of the identifier field
zuuid_t *
    zproto_example_identifier (zproto_example_t *self);
//  Get the identifier field and transfer ownership to caller
zuuid_t *
    zproto_example_get_identifier (zproto_example_t *self);
//  Set the identifier field, transferring ownership from caller
void
    zproto_example_set_identifier (zproto_example_t *self, zuuid_t **uuid_p);

//  Get a copy of the address field
zframe_t *
    zproto_example_address (zproto_example_t *self);
//  Get the address field and transfer ownership to caller
zframe_t *
    zproto_example_get_address (zproto_example_t *self);
//  Set the address field, transferring ownership from caller
void
    zproto_example_set_address (zproto_example_t *self, zframe_t **frame_p);

//  Get a copy of the content field
zmsg_t *
    zproto_example_content (zproto_example_t *self);
//  Get the content field and transfer ownership to caller
zmsg_t *
    zproto_example_get_content (zproto_example_t *self);
//  Set the content field, transferring ownership from caller
void
    zproto_example_set_content (zproto_example_t *self, zmsg_t **msg_p);

//  Get/set the client_forename field
const char *
    zproto_example_client_forename (zproto_example_t *self);
void
    zproto_example_set_client_forename (zproto_example_t *self, const char *value);

//  Get/set the client_surname field
const char *
    zproto_example_client_surname (zproto_example_t *self);
void
    zproto_example_set_client_surname (zproto_example_t *self, const char *value);

//  Get/set the client_mobile field
const char *
    zproto_example_client_mobile (zproto_example_t *self);
void
    zproto_example_set_client_mobile (zproto_example_t *self, const char *value);

//  Get/set the client_email field
const char *
    zproto_example_client_email (zproto_example_t *self);
void
    zproto_example_set_client_email (zproto_example_t *self, const char *value);

//  Get/set the supplier_forename field
const char *
    zproto_example_supplier_forename (zproto_example_t *self);
void
    zproto_example_set_supplier_forename (zproto_example_t *self, const char *value);

//  Get/set the supplier_surname field
const char *
    zproto_example_supplier_surname (zproto_example_t *self);
void
    zproto_example_set_supplier_surname (zproto_example_t *self, const char *value);

//  Get/set the supplier_mobile field
const char *
    zproto_example_supplier_mobile (zproto_example_t *self);
void
    zproto_example_set_supplier_mobile (zproto_example_t *self, const char *value);

//  Get/set the supplier_email field
const char *
    zproto_example_supplier_email (zproto_example_t *self);
void
    zproto_example_set_supplier_email (zproto_example_t *self, const char *value);

//  Self test of this class
int
    zproto_example_test (bool verbose);
//  @end

//  For backwards compatibility with old codecs
#define zproto_example_dump  zproto_example_print

#ifdef __cplusplus
}
#endif

#endif
