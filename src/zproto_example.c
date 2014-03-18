/*  =========================================================================
    zproto_example - zproto example protocol

    Codec class for zproto_example.

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

    * The XML model used for this code generation: zproto_example.xml
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

/*
@header
    zproto_example - zproto example protocol
@discuss
@end
*/

#include <czmq.h>
#include "../include/zproto_example.h"

//  Structure of our class

struct _zproto_example_t {
    zframe_t *routing_id;       //  Routing_id from ROUTER, if any
    int id;                     //  zproto_example message ID
    byte *needle;               //  Read/write pointer for serialization
    byte *ceiling;              //  Valid upper limit for read pointer
    uint16_t sequence;          //  
    uint16_t version;           //  Version
    byte level;                 //  Log severity level
    byte event;                 //  Type of event
    uint16_t node;              //  Sending node
    uint16_t peer;              //  Refers to this peer
    uint64_t time;              //  Log date/time
    char *host;                 //  Originating hostname
    char *data;                 //  Actual log message
    zlist_t *aliases;           //  List of strings
    zhash_t *headers;           //  Other random properties
    size_t headers_bytes;       //  Size of dictionary content
    byte flags [4];             //  A set of flags
    zchunk_t *public_key;       //  Our public key
    zframe_t *address;          //  Return address as frame
    zmsg_t *content;            //  Message to be delivered
};

//  --------------------------------------------------------------------------
//  Network data encoding macros

//  Put a block of octets to the frame
#define PUT_OCTETS(host,size) { \
    memcpy (self->needle, (host), size); \
    self->needle += size; \
}

//  Get a block of octets from the frame
#define GET_OCTETS(host,size) { \
    if (self->needle + size > self->ceiling) \
        goto malformed; \
    memcpy ((host), self->needle, size); \
    self->needle += size; \
}

//  Put a 1-byte number to the frame
#define PUT_NUMBER1(host) { \
    *(byte *) self->needle = (host); \
    self->needle++; \
}

//  Put a 2-byte number to the frame
#define PUT_NUMBER2(host) { \
    self->needle [0] = (byte) (((host) >> 8)  & 255); \
    self->needle [1] = (byte) (((host))       & 255); \
    self->needle += 2; \
}

//  Put a 4-byte number to the frame
#define PUT_NUMBER4(host) { \
    self->needle [0] = (byte) (((host) >> 24) & 255); \
    self->needle [1] = (byte) (((host) >> 16) & 255); \
    self->needle [2] = (byte) (((host) >> 8)  & 255); \
    self->needle [3] = (byte) (((host))       & 255); \
    self->needle += 4; \
}

//  Put a 8-byte number to the frame
#define PUT_NUMBER8(host) { \
    self->needle [0] = (byte) (((host) >> 56) & 255); \
    self->needle [1] = (byte) (((host) >> 48) & 255); \
    self->needle [2] = (byte) (((host) >> 40) & 255); \
    self->needle [3] = (byte) (((host) >> 32) & 255); \
    self->needle [4] = (byte) (((host) >> 24) & 255); \
    self->needle [5] = (byte) (((host) >> 16) & 255); \
    self->needle [6] = (byte) (((host) >> 8)  & 255); \
    self->needle [7] = (byte) (((host))       & 255); \
    self->needle += 8; \
}

//  Get a 1-byte number from the frame
#define GET_NUMBER1(host) { \
    if (self->needle + 1 > self->ceiling) \
        goto malformed; \
    (host) = *(byte *) self->needle; \
    self->needle++; \
}

//  Get a 2-byte number from the frame
#define GET_NUMBER2(host) { \
    if (self->needle + 2 > self->ceiling) \
        goto malformed; \
    (host) = ((uint16_t) (self->needle [0]) << 8) \
           +  (uint16_t) (self->needle [1]); \
    self->needle += 2; \
}

//  Get a 4-byte number from the frame
#define GET_NUMBER4(host) { \
    if (self->needle + 4 > self->ceiling) \
        goto malformed; \
    (host) = ((uint32_t) (self->needle [0]) << 24) \
           + ((uint32_t) (self->needle [1]) << 16) \
           + ((uint32_t) (self->needle [2]) << 8) \
           +  (uint32_t) (self->needle [3]); \
    self->needle += 4; \
}

//  Get a 8-byte number from the frame
#define GET_NUMBER8(host) { \
    if (self->needle + 8 > self->ceiling) \
        goto malformed; \
    (host) = ((uint64_t) (self->needle [0]) << 56) \
           + ((uint64_t) (self->needle [1]) << 48) \
           + ((uint64_t) (self->needle [2]) << 40) \
           + ((uint64_t) (self->needle [3]) << 32) \
           + ((uint64_t) (self->needle [4]) << 24) \
           + ((uint64_t) (self->needle [5]) << 16) \
           + ((uint64_t) (self->needle [6]) << 8) \
           +  (uint64_t) (self->needle [7]); \
    self->needle += 8; \
}

//  Put a string to the frame
#define PUT_STRING(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER1 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a string from the frame
#define GET_STRING(host) { \
    size_t string_size; \
    GET_NUMBER1 (string_size); \
    if (self->needle + string_size > (self->ceiling)) \
        goto malformed; \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}

//  Put a long string to the frame
#define PUT_LONGSTR(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER4 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a long string from the frame
#define GET_LONGSTR(host) { \
    size_t string_size; \
    GET_NUMBER4 (string_size); \
    if (self->needle + string_size > (self->ceiling)) \
        goto malformed; \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}


//  --------------------------------------------------------------------------
//  Create a new zproto_example

zproto_example_t *
zproto_example_new (int id)
{
    zproto_example_t *self = (zproto_example_t *) zmalloc (sizeof (zproto_example_t));
    self->id = id;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zproto_example

void
zproto_example_destroy (zproto_example_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zproto_example_t *self = *self_p;

        //  Free class properties
        zframe_destroy (&self->routing_id);
        free (self->host);
        free (self->data);
        if (self->aliases)
            zlist_destroy (&self->aliases);
        zhash_destroy (&self->headers);
        zchunk_destroy (&self->public_key);
        zframe_destroy (&self->address);
        zmsg_destroy (&self->content);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Parse a zproto_example from zmsg_t. Returns a new object, or NULL if
//  the message could not be parsed, or was NULL. If the socket type is
//  ZMQ_ROUTER, then parses the first frame as a routing_id. Destroys msg
//  and nullifies the msg refernce.

zproto_example_t *
zproto_example_decode (zmsg_t **msg_p, int socket_type)
{
    assert (msg_p);
    zmsg_t *msg = *msg_p;
    if (msg == NULL)
        return NULL;
        
    zproto_example_t *self = zproto_example_new (0);
    //  If message came from a router socket, first frame is routing_id
    if (socket_type == ZMQ_ROUTER) {
        self->routing_id = zmsg_pop (msg);
        //  If message was not valid, forget about it
        if (!self->routing_id || !zmsg_next (msg)) {
            zproto_example_destroy (&self);
            return (NULL);      //  Malformed or empty
        }
    }
    //  Read and parse command in frame
    zframe_t *frame = zmsg_pop (msg);
    if (!frame) 
        goto empty;             //  Malformed or empty

    //  Get and check protocol signature
    self->needle = zframe_data (frame);
    self->ceiling = self->needle + zframe_size (frame);
    uint16_t signature;
    GET_NUMBER2 (signature);
    if (signature != (0xAAA0 | 0))
        goto empty;             //  Invalid signature

    //  Get message id and parse per message type
    GET_NUMBER1 (self->id);

    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            GET_NUMBER2 (self->sequence);
            GET_NUMBER2 (self->version);
            if (self->version != 3)
                goto malformed;
            GET_NUMBER1 (self->level);
            GET_NUMBER1 (self->event);
            GET_NUMBER2 (self->node);
            GET_NUMBER2 (self->peer);
            GET_NUMBER8 (self->time);
            GET_STRING (self->host);
            GET_LONGSTR (self->data);
            break;

        case ZPROTO_EXAMPLE_STRUCTURES:
            GET_NUMBER2 (self->sequence);
            {
                size_t list_size;
                GET_NUMBER4 (list_size);
                self->aliases = zlist_new ();
                zlist_autofree (self->aliases);
                while (list_size--) {
                    char *string;
                    GET_LONGSTR (string);
                    zlist_append (self->aliases, string);
                    free (string);
                }
            }
            {
                size_t hash_size;
                GET_NUMBER4 (hash_size);
                self->headers = zhash_new ();
                zhash_autofree (self->headers);
                while (hash_size--) {
                    char *key, *value;
                    GET_STRING (key);
                    GET_LONGSTR (value);
                    zhash_insert (self->headers, key, value);
                    free (key);
                    free (value);
                }
            }
            break;

        case ZPROTO_EXAMPLE_BINARY:
            GET_NUMBER2 (self->sequence);
            GET_OCTETS (self->flags, 4);
            {
                size_t chunk_size;
                GET_NUMBER4 (chunk_size);
                if (self->needle + chunk_size > (self->ceiling))
                    goto malformed;
                self->public_key = zchunk_new (self->needle, chunk_size);
                self->needle += chunk_size;
            }
            //  Get next frame, leave current untouched
            zframe_t *address = zmsg_pop (msg);
            if (!address)
                goto malformed;
            self->address = address;
            //  Get zero or more remaining frames,
            //  leave current frame untouched
            self->content = zmsg_new ();
            zframe_t *content_part = zmsg_pop (msg);
            while (content_part) {
                zmsg_add (self->content, content_part);
                content_part = zmsg_pop (msg);
            }
            break;

        default:
            goto malformed;
    }
    //  Successful return
    zframe_destroy (&frame);
    zmsg_destroy (msg_p);
    return self;

    //  Error returns
    malformed:
        printf ("E: malformed message '%d'\n", self->id);
    empty:
        zframe_destroy (&frame);
        zmsg_destroy (msg_p);
        zproto_example_destroy (&self);
        return (NULL);
}


//  --------------------------------------------------------------------------
//  Receive and parse a zproto_example from the socket. Returns new object or
//  NULL if error. Will block if there's no message waiting.

zproto_example_t *
zproto_example_recv (void *input)
{
    assert (input);
    zmsg_t *msg = zmsg_recv (input);
    return zproto_example_decode (&msg, zsocket_type (input));
}


//  --------------------------------------------------------------------------
//  Receive and parse a zproto_example from the socket. Returns new object, 
//  or NULL either if there was no input waiting, or the recv was interrupted.

zproto_example_t *
zproto_example_recv_nowait (void *input)
{
    assert (input);
    zmsg_t *msg = zmsg_recv_nowait (input);
    return zproto_example_decode (&msg, zsocket_type (input));
}


//  Count size of key/value pair for serialization
//  Key is encoded as string, value as longstr
static int
s_headers_count (const char *key, void *item, void *argument)
{
    zproto_example_t *self = (zproto_example_t *) argument;
    self->headers_bytes += 1 + strlen (key) + 4 + strlen ((char *) item);
    return 0;
}

//  Serialize headers key=value pair
static int
s_headers_write (const char *key, void *item, void *argument)
{
    zproto_example_t *self = (zproto_example_t *) argument;
    PUT_STRING (key);
    PUT_LONGSTR ((char *) item);
    return 0;
}


//  Encode zproto_example into zmsg and destroy it. Returns a newly created
//  object or NULL if error. Use when not in control of sending the message.
//  If the socket_type is ZMQ_ROUTER, then stores the routing_id as the
//  first frame of the resulting message.

zmsg_t *
zproto_example_encode (zproto_example_t *self, int socket_type)
{
    assert (self);
    zmsg_t *msg = zmsg_new ();

    //  If we're sending to a ROUTER, send the routing_id first
    if (socket_type == ZMQ_ROUTER)
        zmsg_prepend (msg, &self->routing_id);
        
    size_t frame_size = 2 + 1;          //  Signature and message ID
    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  version is a 2-byte integer
            frame_size += 2;
            //  level is a 1-byte integer
            frame_size += 1;
            //  event is a 1-byte integer
            frame_size += 1;
            //  node is a 2-byte integer
            frame_size += 2;
            //  peer is a 2-byte integer
            frame_size += 2;
            //  time is a 8-byte integer
            frame_size += 8;
            //  host is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->host)
                frame_size += strlen (self->host);
            //  data is a string with 4-byte length
            frame_size += 4;
            if (self->data)
                frame_size += strlen (self->data);
            break;
            
        case ZPROTO_EXAMPLE_STRUCTURES:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  aliases is an array of strings
            frame_size += 4;    //  Size is 4 octets
            if (self->aliases) {
                //  Add up size of list contents
                char *aliases = (char *) zlist_first (self->aliases);
                while (aliases) {
                    frame_size += 4 + strlen (aliases);
                    aliases = (char *) zlist_next (self->aliases);
                }
            }
            //  headers is an array of key=value strings
            frame_size += 4;    //  Size is 4 octets
            if (self->headers) {
                self->headers_bytes = 0;
                //  Add up size of dictionary contents
                zhash_foreach (self->headers, s_headers_count, self);
            }
            frame_size += self->headers_bytes;
            break;
            
        case ZPROTO_EXAMPLE_BINARY:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  flags is a block of 4 bytes
            frame_size += 4;
            //  public_key is a chunk with 4-byte length
            frame_size += 4;
            if (self->public_key)
                frame_size += zchunk_size (self->public_key);
            break;
            
        default:
            printf ("E: bad message type '%d', not sent\n", self->id);
            //  No recovery, this is a fatal application error
            assert (false);
    }
    //  Now serialize message into the frame
    zframe_t *frame = zframe_new (NULL, frame_size);
    self->needle = zframe_data (frame);
    PUT_NUMBER2 (0xAAA0 | 0);
    PUT_NUMBER1 (self->id);

    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            PUT_NUMBER2 (self->sequence);
            PUT_NUMBER2 (3);
            PUT_NUMBER1 (self->level);
            PUT_NUMBER1 (self->event);
            PUT_NUMBER2 (self->node);
            PUT_NUMBER2 (self->peer);
            PUT_NUMBER8 (self->time);
            if (self->host) {
                PUT_STRING (self->host);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->data) {
                PUT_LONGSTR (self->data);
            }
            else
                PUT_NUMBER4 (0);    //  Empty string
            break;

        case ZPROTO_EXAMPLE_STRUCTURES:
            PUT_NUMBER2 (self->sequence);
            if (self->aliases) {
                PUT_NUMBER4 (zlist_size (self->aliases));
                char *aliases = (char *) zlist_first (self->aliases);
                while (aliases) {
                    PUT_LONGSTR (aliases);
                    aliases = (char *) zlist_next (self->aliases);
                }
            }
            else
                PUT_NUMBER4 (0);    //  Empty string array
            if (self->headers) {
                PUT_NUMBER4 (zhash_size (self->headers));
                zhash_foreach (self->headers, s_headers_write, self);
            }
            else
                PUT_NUMBER4 (0);    //  Empty dictionary
            break;

        case ZPROTO_EXAMPLE_BINARY:
            PUT_NUMBER2 (self->sequence);
            PUT_OCTETS (self->flags, 4);
            if (self->public_key) {
                PUT_NUMBER4 (zchunk_size (self->public_key));
                memcpy (self->needle,
                        zchunk_data (self->public_key),
                        zchunk_size (self->public_key));
                self->needle += zchunk_size (self->public_key);
            }
            else
                PUT_NUMBER4 (0);    //  Empty chunk
            break;

    }
    //  Now send the data frame
    if (zmsg_append (msg, &frame)) {
        zmsg_destroy (&msg);
        zproto_example_destroy (&self);
        return NULL;
    }
    //  Now send any frame fields, in order
    if (self->id == ZPROTO_EXAMPLE_BINARY) {
        //  If address isn't set, send an empty frame
        if (!self->address)
            self->address = zframe_new (NULL, 0);
        if (zmsg_append (msg, &self->address)) {
            zmsg_destroy (&msg);
            zproto_example_destroy (&self);
            return NULL;
        }
    }
    //  Now send the content field if set
    if (self->id == ZPROTO_EXAMPLE_BINARY) {
        zframe_t *content_part = zmsg_pop (self->content);
        while (content_part) {
            zmsg_append (msg, &content_part);
            content_part = zmsg_pop (self->content);
        }
    }
    //  Destroy zproto_example object
    zproto_example_destroy (&self);
    return msg;

}

//  --------------------------------------------------------------------------
//  Send the zproto_example to the socket, and destroy it
//  Returns 0 if OK, else -1

int
zproto_example_send (zproto_example_t **self_p, void *output)
{
    assert (self_p);
    assert (*self_p);
    assert (output);

    zproto_example_t *self = *self_p;
    zmsg_t *msg = zproto_example_encode (self, zsocket_type (output));
    if (msg && zmsg_send (&msg, output) == 0)
        return 0;
    else
        return -1;              //  Failed to encode, or send
}


//  --------------------------------------------------------------------------
//  Send the zproto_example to the output, and do not destroy it

int
zproto_example_send_again (zproto_example_t *self, void *output)
{
    assert (self);
    assert (output);
    self = zproto_example_dup (self);
    return zproto_example_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the LOG to the socket in one step

int
zproto_example_send_log (
    void *output,
    uint16_t sequence,
    byte level,
    byte event,
    uint16_t node,
    uint16_t peer,
    uint64_t time,
    char *host,
    char *data)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_LOG);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_level (self, level);
    zproto_example_set_event (self, event);
    zproto_example_set_node (self, node);
    zproto_example_set_peer (self, peer);
    zproto_example_set_time (self, time);
    zproto_example_set_host (self, host);
    zproto_example_set_data (self, data);
    return zproto_example_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the STRUCTURES to the socket in one step

int
zproto_example_send_structures (
    void *output,
    uint16_t sequence,
    zlist_t *aliases,
    zhash_t *headers)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_STRUCTURES);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_aliases (self, zlist_dup (aliases));
    zproto_example_set_headers (self, zhash_dup (headers));
    return zproto_example_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the BINARY to the socket in one step

int
zproto_example_send_binary (
    void *output,
    uint16_t sequence,
    byte *flags,
    zchunk_t *public_key,
    zframe_t *address,
    zmsg_t *content)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_BINARY);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_flags (self, flags);
    zproto_example_set_public_key (self, zchunk_dup (public_key));
    zproto_example_set_address (self, zframe_dup (address));
    zproto_example_set_content (self, zmsg_dup (content));
    return zproto_example_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Duplicate the zproto_example message

zproto_example_t *
zproto_example_dup (zproto_example_t *self)
{
    if (!self)
        return NULL;
        
    zproto_example_t *copy = zproto_example_new (self->id);
    if (self->routing_id)
        copy->routing_id = zframe_dup (self->routing_id);

    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            copy->sequence = self->sequence;
            copy->version = self->version;
            copy->level = self->level;
            copy->event = self->event;
            copy->node = self->node;
            copy->peer = self->peer;
            copy->time = self->time;
            copy->host = self->host? strdup (self->host): NULL;
            copy->data = self->data? strdup (self->data): NULL;
            break;

        case ZPROTO_EXAMPLE_STRUCTURES:
            copy->sequence = self->sequence;
            copy->aliases = self->aliases? zlist_dup (self->aliases): NULL;
            copy->headers = self->headers? zhash_dup (self->headers): NULL;
            break;

        case ZPROTO_EXAMPLE_BINARY:
            copy->sequence = self->sequence;
            memcpy (copy->flags, self->flags, 4);
            copy->public_key = self->public_key? zchunk_dup (self->public_key): NULL;
            copy->address = self->address? zframe_dup (self->address): NULL;
            copy->content = self->content? zmsg_dup (self->content): NULL;
            break;

    }
    return copy;
}


//  Dump headers key=value pair to stdout
static int
s_headers_dump (const char *key, void *item, void *argument)
{
    printf ("        %s=%s\n", key, (char *) item);
    return 0;
}


//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zproto_example_dump (zproto_example_t *self)
{
    assert (self);
    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            puts ("LOG:");
            printf ("    sequence=%ld\n", (long) self->sequence);
            printf ("    version=3\n");
            printf ("    level=%ld\n", (long) self->level);
            printf ("    event=%ld\n", (long) self->event);
            printf ("    node=%ld\n", (long) self->node);
            printf ("    peer=%ld\n", (long) self->peer);
            printf ("    time=%ld\n", (long) self->time);
            if (self->host)
                printf ("    host='%s'\n", self->host);
            else
                printf ("    host=\n");
            if (self->data)
                printf ("    data='%s'\n", self->data);
            else
                printf ("    data=\n");
            break;
            
        case ZPROTO_EXAMPLE_STRUCTURES:
            puts ("STRUCTURES:");
            printf ("    sequence=%ld\n", (long) self->sequence);
            printf ("    aliases={");
            if (self->aliases) {
                char *aliases = (char *) zlist_first (self->aliases);
                while (aliases) {
                    printf (" '%s'", aliases);
                    aliases = (char *) zlist_next (self->aliases);
                }
            }
            printf (" }\n");
            printf ("    headers={\n");
            if (self->headers)
                zhash_foreach (self->headers, s_headers_dump, self);
            else
                printf ("(NULL)\n");
            printf ("    }\n");
            break;
            
        case ZPROTO_EXAMPLE_BINARY:
            puts ("BINARY:");
            printf ("    sequence=%ld\n", (long) self->sequence);
            printf ("    flags=");
            int flags_index;
            for (flags_index = 0; flags_index < 4; flags_index++) {
                if (flags_index && (flags_index % 4 == 0))
                    printf ("-");
                printf ("%02X", self->flags [flags_index]);
            }
            printf ("\n");
            printf ("    public_key={\n");
            if (self->public_key)
                zchunk_print (self->public_key);
            else
                printf ("(NULL)\n");
            printf ("    }\n");
            printf ("    address={\n");
            if (self->address)
                zframe_print (self->address, NULL);
            else
                printf ("(NULL)\n");
            printf ("    }\n");
            printf ("    content={\n");
            if (self->content)
                zmsg_dump (self->content);
            else
                printf ("(NULL)\n");
            printf ("    }\n");
            break;
            
    }
}


//  --------------------------------------------------------------------------
//  Get/set the message routing_id

zframe_t *
zproto_example_routing_id (zproto_example_t *self)
{
    assert (self);
    return self->routing_id;
}

void
zproto_example_set_routing_id (zproto_example_t *self, zframe_t *routing_id)
{
    if (self->routing_id)
        zframe_destroy (&self->routing_id);
    self->routing_id = zframe_dup (routing_id);
}


//  --------------------------------------------------------------------------
//  Get/set the zproto_example id

int
zproto_example_id (zproto_example_t *self)
{
    assert (self);
    return self->id;
}

void
zproto_example_set_id (zproto_example_t *self, int id)
{
    self->id = id;
}

//  --------------------------------------------------------------------------
//  Return a printable command string

char *
zproto_example_command (zproto_example_t *self)
{
    assert (self);
    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            return ("LOG");
            break;
        case ZPROTO_EXAMPLE_STRUCTURES:
            return ("STRUCTURES");
            break;
        case ZPROTO_EXAMPLE_BINARY:
            return ("BINARY");
            break;
    }
    return "?";
}

//  --------------------------------------------------------------------------
//  Get/set the sequence field

uint16_t
zproto_example_sequence (zproto_example_t *self)
{
    assert (self);
    return self->sequence;
}

void
zproto_example_set_sequence (zproto_example_t *self, uint16_t sequence)
{
    assert (self);
    self->sequence = sequence;
}


//  --------------------------------------------------------------------------
//  Get/set the level field

byte
zproto_example_level (zproto_example_t *self)
{
    assert (self);
    return self->level;
}

void
zproto_example_set_level (zproto_example_t *self, byte level)
{
    assert (self);
    self->level = level;
}


//  --------------------------------------------------------------------------
//  Get/set the event field

byte
zproto_example_event (zproto_example_t *self)
{
    assert (self);
    return self->event;
}

void
zproto_example_set_event (zproto_example_t *self, byte event)
{
    assert (self);
    self->event = event;
}


//  --------------------------------------------------------------------------
//  Get/set the node field

uint16_t
zproto_example_node (zproto_example_t *self)
{
    assert (self);
    return self->node;
}

void
zproto_example_set_node (zproto_example_t *self, uint16_t node)
{
    assert (self);
    self->node = node;
}


//  --------------------------------------------------------------------------
//  Get/set the peer field

uint16_t
zproto_example_peer (zproto_example_t *self)
{
    assert (self);
    return self->peer;
}

void
zproto_example_set_peer (zproto_example_t *self, uint16_t peer)
{
    assert (self);
    self->peer = peer;
}


//  --------------------------------------------------------------------------
//  Get/set the time field

uint64_t
zproto_example_time (zproto_example_t *self)
{
    assert (self);
    return self->time;
}

void
zproto_example_set_time (zproto_example_t *self, uint64_t time)
{
    assert (self);
    self->time = time;
}


//  --------------------------------------------------------------------------
//  Get/set the host field

char *
zproto_example_host (zproto_example_t *self)
{
    assert (self);
    return self->host;
}

void
zproto_example_set_host (zproto_example_t *self, char *format, ...)
{
    //  Format host from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->host);
    self->host = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the data field

char *
zproto_example_data (zproto_example_t *self)
{
    assert (self);
    return self->data;
}

void
zproto_example_set_data (zproto_example_t *self, char *format, ...)
{
    //  Format data from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->data);
    self->data = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the aliases field

zlist_t *
zproto_example_aliases (zproto_example_t *self)
{
    assert (self);
    return self->aliases;
}

//  Greedy function, takes ownership of aliases; if you don't want that
//  then use zlist_dup() to pass a copy of aliases

void
zproto_example_set_aliases (zproto_example_t *self, zlist_t *aliases)
{
    assert (self);
    zlist_destroy (&self->aliases);
    self->aliases = aliases;
}

//  --------------------------------------------------------------------------
//  Iterate through the aliases field, and append a aliases value

char *
zproto_example_aliases_first (zproto_example_t *self)
{
    assert (self);
    if (self->aliases)
        return (char *) (zlist_first (self->aliases));
    else
        return NULL;
}

char *
zproto_example_aliases_next (zproto_example_t *self)
{
    assert (self);
    if (self->aliases)
        return (char *) (zlist_next (self->aliases));
    else
        return NULL;
}

void
zproto_example_aliases_append (zproto_example_t *self, char *format, ...)
{
    //  Format into newly allocated string
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    char *string = zsys_vprintf (format, argptr);
    va_end (argptr);

    //  Attach string to list
    if (!self->aliases) {
        self->aliases = zlist_new ();
        zlist_autofree (self->aliases);
    }
    zlist_append (self->aliases, string);
    free (string);
}

size_t
zproto_example_aliases_size (zproto_example_t *self)
{
    return zlist_size (self->aliases);
}


//  --------------------------------------------------------------------------
//  Get/set the headers field

zhash_t *
zproto_example_headers (zproto_example_t *self)
{
    assert (self);
    return self->headers;
}

//  Greedy function, takes ownership of headers; if you don't want that
//  then use zhash_dup() to pass a copy of headers

void
zproto_example_set_headers (zproto_example_t *self, zhash_t *headers)
{
    assert (self);
    zhash_destroy (&self->headers);
    self->headers = headers;
}

//  --------------------------------------------------------------------------
//  Get/set a value in the headers dictionary

char *
zproto_example_headers_string (zproto_example_t *self, char *key, char *default_value)
{
    assert (self);
    char *value = NULL;
    if (self->headers)
        value = (char *) (zhash_lookup (self->headers, key));
    if (!value)
        value = default_value;

    return value;
}

uint64_t
zproto_example_headers_number (zproto_example_t *self, char *key, uint64_t default_value)
{
    assert (self);
    uint64_t value = default_value;
    char *string = NULL;
    if (self->headers)
        string = (char *) (zhash_lookup (self->headers, key));
    if (string)
        value = atol (string);

    return value;
}

void
zproto_example_headers_insert (zproto_example_t *self, char *key, char *format, ...)
{
    //  Format into newly allocated string
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    char *string = zsys_vprintf (format, argptr);
    va_end (argptr);

    //  Store string in hash table
    if (!self->headers) {
        self->headers = zhash_new ();
        zhash_autofree (self->headers);
    }
    zhash_update (self->headers, key, string);
    free (string);
}

size_t
zproto_example_headers_size (zproto_example_t *self)
{
    return zhash_size (self->headers);
}


//  --------------------------------------------------------------------------
//  Get/set the flags field

byte *
zproto_example_flags (zproto_example_t *self)
{
    assert (self);
    return self->flags;
}

void
zproto_example_set_flags (zproto_example_t *self, byte *flags)
{
    assert (self);
    memcpy (self->flags, flags, 4);
}


//  --------------------------------------------------------------------------
//  Get/set the public_key field

zchunk_t *
zproto_example_public_key (zproto_example_t *self)
{
    assert (self);
    return self->public_key;
}

//  Takes ownership of supplied chunk
void
zproto_example_set_public_key (zproto_example_t *self, zchunk_t *chunk)
{
    assert (self);
    if (self->public_key)
        zchunk_destroy (&self->public_key);
    self->public_key = chunk;
}

//  --------------------------------------------------------------------------
//  Get/set the address field

zframe_t *
zproto_example_address (zproto_example_t *self)
{
    assert (self);
    return self->address;
}

//  Takes ownership of supplied frame
void
zproto_example_set_address (zproto_example_t *self, zframe_t *frame)
{
    assert (self);
    if (self->address)
        zframe_destroy (&self->address);
    self->address = frame;
}

//  --------------------------------------------------------------------------
//  Get/set the content field

zmsg_t *
zproto_example_content (zproto_example_t *self)
{
    assert (self);
    return self->content;
}

//  Takes ownership of supplied msg
void
zproto_example_set_content (zproto_example_t *self, zmsg_t *msg)
{
    assert (self);
    if (self->content)
        zmsg_destroy (&self->content);
    self->content = msg;
}


//  --------------------------------------------------------------------------
//  Selftest

int
zproto_example_test (bool verbose)
{
    printf (" * zproto_example: ");

    //  @selftest
    //  Simple create/destroy test
    zproto_example_t *self = zproto_example_new (0);
    assert (self);
    zproto_example_destroy (&self);

    //  Create pair of sockets we can send through
    zctx_t *ctx = zctx_new ();
    assert (ctx);

    void *output = zsocket_new (ctx, ZMQ_DEALER);
    assert (output);
    zsocket_bind (output, "inproc://selftest");

    void *input = zsocket_new (ctx, ZMQ_ROUTER);
    assert (input);
    zsocket_connect (input, "inproc://selftest");
    
    //  Encode/send/decode and verify each message type
    int instance;
    zproto_example_t *copy;
    self = zproto_example_new (ZPROTO_EXAMPLE_LOG);
    
    //  Check that _dup works on empty message
    copy = zproto_example_dup (self);
    assert (copy);
    zproto_example_destroy (&copy);

    zproto_example_set_sequence (self, 123);
    zproto_example_set_level (self, 123);
    zproto_example_set_event (self, 123);
    zproto_example_set_node (self, 123);
    zproto_example_set_peer (self, 123);
    zproto_example_set_time (self, 123);
    zproto_example_set_host (self, "Life is short but Now lasts for ever");
    zproto_example_set_data (self, "Life is short but Now lasts for ever");
    //  Send twice from same object
    zproto_example_send_again (self, output);
    zproto_example_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = zproto_example_recv (input);
        assert (self);
        assert (zproto_example_routing_id (self));
        
        assert (zproto_example_sequence (self) == 123);
        assert (zproto_example_level (self) == 123);
        assert (zproto_example_event (self) == 123);
        assert (zproto_example_node (self) == 123);
        assert (zproto_example_peer (self) == 123);
        assert (zproto_example_time (self) == 123);
        assert (streq (zproto_example_host (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_data (self), "Life is short but Now lasts for ever"));
        zproto_example_destroy (&self);
    }
    self = zproto_example_new (ZPROTO_EXAMPLE_STRUCTURES);
    
    //  Check that _dup works on empty message
    copy = zproto_example_dup (self);
    assert (copy);
    zproto_example_destroy (&copy);

    zproto_example_set_sequence (self, 123);
    zproto_example_aliases_append (self, "Name: %s", "Brutus");
    zproto_example_aliases_append (self, "Age: %d", 43);
    zproto_example_headers_insert (self, "Name", "Brutus");
    zproto_example_headers_insert (self, "Age", "%d", 43);
    //  Send twice from same object
    zproto_example_send_again (self, output);
    zproto_example_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = zproto_example_recv (input);
        assert (self);
        assert (zproto_example_routing_id (self));
        
        assert (zproto_example_sequence (self) == 123);
        assert (zproto_example_aliases_size (self) == 2);
        assert (streq (zproto_example_aliases_first (self), "Name: Brutus"));
        assert (streq (zproto_example_aliases_next (self), "Age: 43"));
        assert (zproto_example_headers_size (self) == 2);
        assert (streq (zproto_example_headers_string (self, "Name", "?"), "Brutus"));
        assert (zproto_example_headers_number (self, "Age", 0) == 43);
        zproto_example_destroy (&self);
    }
    self = zproto_example_new (ZPROTO_EXAMPLE_BINARY);
    
    //  Check that _dup works on empty message
    copy = zproto_example_dup (self);
    assert (copy);
    zproto_example_destroy (&copy);

    zproto_example_set_sequence (self, 123);
    byte flags_data [ZPROTO_EXAMPLE_FLAGS_SIZE];
    memset (flags_data, 123, ZPROTO_EXAMPLE_FLAGS_SIZE);
    zproto_example_set_flags (self, flags_data);
    zproto_example_set_public_key (self, zchunk_new ("Captcha Diem", 12));
    zproto_example_set_address (self, zframe_new ("Captcha Diem", 12));
    zproto_example_set_content (self, zmsg_new ());
    zmsg_addstr (zproto_example_content (self), "Hello, World");
    //  Send twice from same object
    zproto_example_send_again (self, output);
    zproto_example_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = zproto_example_recv (input);
        assert (self);
        assert (zproto_example_routing_id (self));
        
        assert (zproto_example_sequence (self) == 123);
        assert (zproto_example_flags (self) [0] == 123);
        assert (zproto_example_flags (self) [ZPROTO_EXAMPLE_FLAGS_SIZE - 1] == 123);
        assert (memcmp (zchunk_data (zproto_example_public_key (self)), "Captcha Diem", 12) == 0);
        assert (zframe_streq (zproto_example_address (self), "Captcha Diem"));
        assert (zmsg_size (zproto_example_content (self)) == 1);
        zproto_example_destroy (&self);
    }

    zctx_destroy (&ctx);
    //  @end

    printf ("OK\n");
    return 0;
}
