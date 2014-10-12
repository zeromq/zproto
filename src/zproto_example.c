/*  =========================================================================
    zproto_example - zproto example protocol

    Codec class for zproto_example.

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

/*
@header
    zproto_example - zproto example protocol
@discuss
@end
*/

#include "../include/zproto_example.h"

//  Structure of our class

struct _zproto_example_t {
    zframe_t *routing_id;               //  Routing_id from ROUTER, if any
    int id;                             //  zproto_example message ID
    byte *needle;                       //  Read/write pointer for serialization
    byte *ceiling;                      //  Valid upper limit for read pointer
    uint16_t sequence;                  //  
    uint16_t version;                   //  Version
    byte level;                         //  Log severity level
    byte event;                         //  Type of event
    uint16_t node;                      //  Sending node
    uint16_t peer;                      //  Refers to this peer
    uint64_t time;                      //  Log date/time
    char *host;                         //  Originating hostname
    char *data;                         //  Actual log message
    zlist_t *aliases;                   //  List of strings
    zhash_t *headers;                   //  Other random properties
    size_t headers_bytes;               //  Size of dictionary content
    byte flags [4];                     //  A set of flags
    zchunk_t *public_key;               //  Our public key
    zuuid_t *identifier;                //  Unique identity
    zframe_t *address;                  //  Return address as frame
    zmsg_t *content;                    //  Message to be delivered
    char *client_forename;              //  Given name
    char *client_surname;               //  Family name
    char *client_mobile;                //  Mobile phone number
    char *client_email;                 //  Email address
    char *supplier_forename;            //  Given name
    char *supplier_surname;             //  Family name
    char *supplier_mobile;              //  Mobile phone number
    char *supplier_email;               //  Email address
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
        zuuid_destroy (&self->identifier);
        zframe_destroy (&self->address);
        zmsg_destroy (&self->content);
        free (self->client_forename);
        free (self->client_surname);
        free (self->client_mobile);
        free (self->client_email);
        free (self->supplier_forename);
        free (self->supplier_surname);
        free (self->supplier_mobile);
        free (self->supplier_email);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Parse a zproto_example from zmsg_t. Returns a new object, or NULL if
//  the message could not be parsed, or was NULL. Destroys msg and 
//  nullifies the msg reference.

zproto_example_t *
zproto_example_decode (zmsg_t **msg_p)
{
    assert (msg_p);
    zmsg_t *msg = *msg_p;
    if (msg == NULL)
        return NULL;
        
    zproto_example_t *self = zproto_example_new (0);
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
            {
                if (self->needle + ZUUID_LEN > (self->ceiling))
                    goto malformed;
                self->identifier = zuuid_new ();
                zuuid_set (self->identifier, self->needle);
                self->needle += ZUUID_LEN;
            }
            {
                //  Get next frame, leave current untouched
                zframe_t *address = zmsg_pop (msg);
                if (!address)
                    goto malformed;
                self->address = address;
            }
            //  Get zero or more remaining frames, leaving current
            //  frame untouched
            self->content = zmsg_new ();
            while (zmsg_size (msg))
                zmsg_add (self->content, zmsg_pop (msg));
            break;

        case ZPROTO_EXAMPLE_TYPES:
            GET_NUMBER2 (self->sequence);
            GET_STRING (self->client_forename);
            GET_STRING (self->client_surname);
            GET_STRING (self->client_mobile);
            GET_STRING (self->client_email);
            GET_STRING (self->supplier_forename);
            GET_STRING (self->supplier_surname);
            GET_STRING (self->supplier_mobile);
            GET_STRING (self->supplier_email);
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
        zsys_error ("malformed message '%d'\n", self->id);
    empty:
        zframe_destroy (&frame);
        zmsg_destroy (msg_p);
        zproto_example_destroy (&self);
        return (NULL);
}


//  --------------------------------------------------------------------------
//  Encode zproto_example into zmsg and destroy it. Returns a newly created
//  object or NULL if error. Use when not in control of sending the message.

zmsg_t *
zproto_example_encode (zproto_example_t **self_p)
{
    assert (self_p);
    assert (*self_p);
    
    zproto_example_t *self = *self_p;
    zmsg_t *msg = zmsg_new ();

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
                char *item = (char *) zhash_first (self->headers);
                while (item) {
                    self->headers_bytes += 1 + strlen ((const char *) zhash_cursor (self->headers));
                    self->headers_bytes += 4 + strlen (item);
                    item = (char *) zhash_next (self->headers);
                }
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
            //  identifier is uuid with 16-byte length
            frame_size += ZUUID_LEN;
            break;
            
        case ZPROTO_EXAMPLE_TYPES:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  client_forename is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->client_forename)
                frame_size += strlen (self->client_forename);
            //  client_surname is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->client_surname)
                frame_size += strlen (self->client_surname);
            //  client_mobile is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->client_mobile)
                frame_size += strlen (self->client_mobile);
            //  client_email is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->client_email)
                frame_size += strlen (self->client_email);
            //  supplier_forename is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->supplier_forename)
                frame_size += strlen (self->supplier_forename);
            //  supplier_surname is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->supplier_surname)
                frame_size += strlen (self->supplier_surname);
            //  supplier_mobile is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->supplier_mobile)
                frame_size += strlen (self->supplier_mobile);
            //  supplier_email is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->supplier_email)
                frame_size += strlen (self->supplier_email);
            break;
            
        default:
            zsys_error ("bad message type '%d', not sent\n", self->id);
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
                char *item = (char *) zhash_first (self->headers);
                while (item) {
                    PUT_STRING ((const char *) zhash_cursor ((zhash_t *) self->headers));
                    PUT_LONGSTR (item);
                    item = (char *) zhash_next (self->headers);
                }
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
            if (self->identifier)
                zuuid_export (self->identifier, self->needle);
            else
                memset (self->needle, 0, ZUUID_LEN);
            self->needle += ZUUID_LEN;
            break;

        case ZPROTO_EXAMPLE_TYPES:
            PUT_NUMBER2 (self->sequence);
            if (self->client_forename) {
                PUT_STRING (self->client_forename);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->client_surname) {
                PUT_STRING (self->client_surname);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->client_mobile) {
                PUT_STRING (self->client_mobile);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->client_email) {
                PUT_STRING (self->client_email);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->supplier_forename) {
                PUT_STRING (self->supplier_forename);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->supplier_surname) {
                PUT_STRING (self->supplier_surname);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->supplier_mobile) {
                PUT_STRING (self->supplier_mobile);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            if (self->supplier_email) {
                PUT_STRING (self->supplier_email);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            break;

    }
    //  Now send the data frame
    if (zmsg_append (msg, &frame)) {
        zmsg_destroy (&msg);
        zproto_example_destroy (self_p);
        return NULL;
    }
    //  Now send any frame fields, in order
    if (self->id == ZPROTO_EXAMPLE_BINARY) {
        //  If address isn't set, send an empty frame
        if (!self->address)
            self->address = zframe_new (NULL, 0);
        if (zmsg_append (msg, &self->address)) {
            zmsg_destroy (&msg);
            zproto_example_destroy (self_p);
            return NULL;
        }
    }
    //  Now send the message field if there is any
    if (self->id == ZPROTO_EXAMPLE_BINARY) {
        if (self->content) {
            zframe_t *frame = zmsg_pop (self->content);
            while (frame) {
                zmsg_append (msg, &frame);
                frame = zmsg_pop (self->content);
            }
        }
        else {
            zframe_t *frame = zframe_new (NULL, 0);
            zmsg_append (msg, &frame);
        }
    }
    //  Destroy zproto_example object
    zproto_example_destroy (self_p);
    return msg;
}


//  --------------------------------------------------------------------------
//  Receive and parse a zproto_example from the socket. Returns new object or
//  NULL if error. Will block if there's no message waiting.

zproto_example_t *
zproto_example_recv (void *input)
{
    assert (input);
    zmsg_t *msg = zmsg_recv (input);
    if (!msg)
        return NULL;            //  Interrupted
    //  If message came from a router socket, first frame is routing_id
    zframe_t *routing_id = NULL;
    if (zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER) {
        routing_id = zmsg_pop (msg);
        //  If message was not valid, forget about it
        if (!routing_id || !zmsg_next (msg))
            return NULL;        //  Malformed or empty
    }
    zproto_example_t *zproto_example = zproto_example_decode (&msg);
    if (zproto_example && zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER)
        zproto_example->routing_id = routing_id;

    return zproto_example;
}


//  --------------------------------------------------------------------------
//  Receive and parse a zproto_example from the socket. Returns new object,
//  or NULL either if there was no input waiting, or the recv was interrupted.

zproto_example_t *
zproto_example_recv_nowait (void *input)
{
    assert (input);
    zmsg_t *msg = zmsg_recv_nowait (input);
    if (!msg)
        return NULL;            //  Interrupted
    //  If message came from a router socket, first frame is routing_id
    zframe_t *routing_id = NULL;
    if (zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER) {
        routing_id = zmsg_pop (msg);
        //  If message was not valid, forget about it
        if (!routing_id || !zmsg_next (msg))
            return NULL;        //  Malformed or empty
    }
    zproto_example_t *zproto_example = zproto_example_decode (&msg);
    if (zproto_example && zsocket_type (zsock_resolve (input)) == ZMQ_ROUTER)
        zproto_example->routing_id = routing_id;

    return zproto_example;
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

    //  Save routing_id if any, as encode will destroy it
    zproto_example_t *self = *self_p;
    zframe_t *routing_id = self->routing_id;
    self->routing_id = NULL;

    //  Encode zproto_example message to a single zmsg
    zmsg_t *msg = zproto_example_encode (self_p);
    
    //  If we're sending to a ROUTER, send the routing_id first
    if (zsocket_type (zsock_resolve (output)) == ZMQ_ROUTER) {
        assert (routing_id);
        zmsg_prepend (msg, &routing_id);
    }
    else
        zframe_destroy (&routing_id);
        
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
//  Encode LOG message

zmsg_t * 
zproto_example_encode_log (
    uint16_t sequence,
    byte level,
    byte event,
    uint16_t node,
    uint16_t peer,
    uint64_t time,
    const char *host,
    const char *data)
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
    return zproto_example_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode STRUCTURES message

zmsg_t * 
zproto_example_encode_structures (
    uint16_t sequence,
    zlist_t *aliases,
    zhash_t *headers)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_STRUCTURES);
    zproto_example_set_sequence (self, sequence);
    zlist_t *aliases_copy = zlist_dup (aliases);
    zproto_example_set_aliases (self, &aliases_copy);
    zhash_t *headers_copy = zhash_dup (headers);
    zproto_example_set_headers (self, &headers_copy);
    return zproto_example_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode BINARY message

zmsg_t * 
zproto_example_encode_binary (
    uint16_t sequence,
    byte *flags,
    zchunk_t *public_key,
    zuuid_t *identifier,
    zframe_t *address,
    zmsg_t *content)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_BINARY);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_flags (self, flags);
    zchunk_t *public_key_copy = zchunk_dup (public_key);
    zproto_example_set_public_key (self, &public_key_copy);
    zuuid_t *identifier_copy = zuuid_dup (identifier);
    zproto_example_set_identifier (self, &identifier_copy);
    zframe_t *address_copy = zframe_dup (address);
    zproto_example_set_address (self, &address_copy);
    zmsg_t *content_copy = zmsg_dup (content);
    zproto_example_set_content (self, &content_copy);
    return zproto_example_encode (&self);
}


//  --------------------------------------------------------------------------
//  Encode TYPES message

zmsg_t * 
zproto_example_encode_types (
    uint16_t sequence,
    const char *client_forename,
    const char *client_surname,
    const char *client_mobile,
    const char *client_email,
    const char *supplier_forename,
    const char *supplier_surname,
    const char *supplier_mobile,
    const char *supplier_email)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_TYPES);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_client_forename (self, client_forename);
    zproto_example_set_client_surname (self, client_surname);
    zproto_example_set_client_mobile (self, client_mobile);
    zproto_example_set_client_email (self, client_email);
    zproto_example_set_supplier_forename (self, supplier_forename);
    zproto_example_set_supplier_surname (self, supplier_surname);
    zproto_example_set_supplier_mobile (self, supplier_mobile);
    zproto_example_set_supplier_email (self, supplier_email);
    return zproto_example_encode (&self);
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
    const char *host,
    const char *data)
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
    zlist_t *aliases_copy = zlist_dup (aliases);
    zproto_example_set_aliases (self, &aliases_copy);
    zhash_t *headers_copy = zhash_dup (headers);
    zproto_example_set_headers (self, &headers_copy);
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
    zuuid_t *identifier,
    zframe_t *address,
    zmsg_t *content)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_BINARY);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_flags (self, flags);
    zchunk_t *public_key_copy = zchunk_dup (public_key);
    zproto_example_set_public_key (self, &public_key_copy);
    zuuid_t *identifier_copy = zuuid_dup (identifier);
    zproto_example_set_identifier (self, &identifier_copy);
    zframe_t *address_copy = zframe_dup (address);
    zproto_example_set_address (self, &address_copy);
    zmsg_t *content_copy = zmsg_dup (content);
    zproto_example_set_content (self, &content_copy);
    return zproto_example_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the TYPES to the socket in one step

int
zproto_example_send_types (
    void *output,
    uint16_t sequence,
    const char *client_forename,
    const char *client_surname,
    const char *client_mobile,
    const char *client_email,
    const char *supplier_forename,
    const char *supplier_surname,
    const char *supplier_mobile,
    const char *supplier_email)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_TYPES);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_client_forename (self, client_forename);
    zproto_example_set_client_surname (self, client_surname);
    zproto_example_set_client_mobile (self, client_mobile);
    zproto_example_set_client_email (self, client_email);
    zproto_example_set_supplier_forename (self, supplier_forename);
    zproto_example_set_supplier_surname (self, supplier_surname);
    zproto_example_set_supplier_mobile (self, supplier_mobile);
    zproto_example_set_supplier_email (self, supplier_email);
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
            copy->identifier = self->identifier? zuuid_dup (self->identifier): NULL;
            copy->address = self->address? zframe_dup (self->address): NULL;
            copy->content = self->content? zmsg_dup (self->content): NULL;
            break;

        case ZPROTO_EXAMPLE_TYPES:
            copy->sequence = self->sequence;
            copy->client_forename = self->client_forename? strdup (self->client_forename): NULL;
            copy->client_surname = self->client_surname? strdup (self->client_surname): NULL;
            copy->client_mobile = self->client_mobile? strdup (self->client_mobile): NULL;
            copy->client_email = self->client_email? strdup (self->client_email): NULL;
            copy->supplier_forename = self->supplier_forename? strdup (self->supplier_forename): NULL;
            copy->supplier_surname = self->supplier_surname? strdup (self->supplier_surname): NULL;
            copy->supplier_mobile = self->supplier_mobile? strdup (self->supplier_mobile): NULL;
            copy->supplier_email = self->supplier_email? strdup (self->supplier_email): NULL;
            break;

    }
    return copy;
}


//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zproto_example_print (zproto_example_t *self)
{
    assert (self);
    switch (self->id) {
        case ZPROTO_EXAMPLE_LOG:
            zsys_debug ("ZPROTO_EXAMPLE_LOG:");
            zsys_debug ("    sequence=%ld", (long) self->sequence);
            zsys_debug ("    version=3");
            zsys_debug ("    level=%ld", (long) self->level);
            zsys_debug ("    event=%ld", (long) self->event);
            zsys_debug ("    node=%ld", (long) self->node);
            zsys_debug ("    peer=%ld", (long) self->peer);
            zsys_debug ("    time=%ld", (long) self->time);
            if (self->host)
                zsys_debug ("    host='%s'", self->host);
            else
                zsys_debug ("    host=");
            if (self->data)
                zsys_debug ("    data='%s'", self->data);
            else
                zsys_debug ("    data=");
            break;
            
        case ZPROTO_EXAMPLE_STRUCTURES:
            zsys_debug ("ZPROTO_EXAMPLE_STRUCTURES:");
            zsys_debug ("    sequence=%ld", (long) self->sequence);
            zsys_debug ("    aliases=");
            if (self->aliases) {
                char *aliases = (char *) zlist_first (self->aliases);
                while (aliases) {
                    zsys_debug ("        '%s'", aliases);
                    aliases = (char *) zlist_next (self->aliases);
                }
            }
            zsys_debug ("    headers=");
            if (self->headers) {
                char *item = (char *) zhash_first (self->headers);
                while (item) {
                    zsys_debug ("        %s=%s", zhash_cursor (self->headers), item);
                    item = (char *) zhash_next (self->headers);
                }
            }
            else
                zsys_debug ("(NULL)");
            break;
            
        case ZPROTO_EXAMPLE_BINARY:
            zsys_debug ("ZPROTO_EXAMPLE_BINARY:");
            zsys_debug ("    sequence=%ld", (long) self->sequence);
            zsys_debug ("    flags=[ ... ]");
            zsys_debug ("    public_key=[ ... ]");
            zsys_debug ("    identifier=");
            if (self->identifier)
                zsys_debug ("        %s", zuuid_str (self->identifier));
            else
                zsys_debug ("        (NULL)");
            zsys_debug ("    address=");
            if (self->address)
                zframe_print (self->address, NULL);
            else
                zsys_debug ("(NULL)");
            zsys_debug ("    content=");
            if (self->content)
                zmsg_print (self->content);
            else
                zsys_debug ("(NULL)");
            break;
            
        case ZPROTO_EXAMPLE_TYPES:
            zsys_debug ("ZPROTO_EXAMPLE_TYPES:");
            zsys_debug ("    sequence=%ld", (long) self->sequence);
            if (self->client_forename)
                zsys_debug ("    client_forename='%s'", self->client_forename);
            else
                zsys_debug ("    client_forename=");
            if (self->client_surname)
                zsys_debug ("    client_surname='%s'", self->client_surname);
            else
                zsys_debug ("    client_surname=");
            if (self->client_mobile)
                zsys_debug ("    client_mobile='%s'", self->client_mobile);
            else
                zsys_debug ("    client_mobile=");
            if (self->client_email)
                zsys_debug ("    client_email='%s'", self->client_email);
            else
                zsys_debug ("    client_email=");
            if (self->supplier_forename)
                zsys_debug ("    supplier_forename='%s'", self->supplier_forename);
            else
                zsys_debug ("    supplier_forename=");
            if (self->supplier_surname)
                zsys_debug ("    supplier_surname='%s'", self->supplier_surname);
            else
                zsys_debug ("    supplier_surname=");
            if (self->supplier_mobile)
                zsys_debug ("    supplier_mobile='%s'", self->supplier_mobile);
            else
                zsys_debug ("    supplier_mobile=");
            if (self->supplier_email)
                zsys_debug ("    supplier_email='%s'", self->supplier_email);
            else
                zsys_debug ("    supplier_email=");
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

const char *
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
        case ZPROTO_EXAMPLE_TYPES:
            return ("TYPES");
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

const char *
zproto_example_host (zproto_example_t *self)
{
    assert (self);
    return self->host;
}

void
zproto_example_set_host (zproto_example_t *self, const char *format, ...)
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

const char *
zproto_example_data (zproto_example_t *self)
{
    assert (self);
    return self->data;
}

void
zproto_example_set_data (zproto_example_t *self, const char *format, ...)
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
//  Get the aliases field, without transferring ownership

zlist_t *
zproto_example_aliases (zproto_example_t *self)
{
    assert (self);
    return self->aliases;
}

//  Get the aliases field and transfer ownership to caller

zlist_t *
zproto_example_get_aliases (zproto_example_t *self)
{
    assert (self);
    zlist_t *aliases = self->aliases;
    self->aliases = NULL;
    return aliases;
}

//  Set the aliases field, transferring ownership from caller

void
zproto_example_set_aliases (zproto_example_t *self, zlist_t **aliases_p)
{
    assert (self);
    assert (aliases_p);
    zlist_destroy (&self->aliases);
    self->aliases = *aliases_p;
    *aliases_p = NULL;
}

//  --------------------------------------------------------------------------
//  Iterate through the aliases field, and append a aliases value

const char *
zproto_example_aliases_first (zproto_example_t *self)
{
    assert (self);
    if (self->aliases)
        return (char *) (zlist_first (self->aliases));
    else
        return NULL;
}

const char *
zproto_example_aliases_next (zproto_example_t *self)
{
    assert (self);
    if (self->aliases)
        return (char *) (zlist_next (self->aliases));
    else
        return NULL;
}

void
zproto_example_aliases_append (zproto_example_t *self, const char *format, ...)
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
//  Get the headers field without transferring ownership

zhash_t *
zproto_example_headers (zproto_example_t *self)
{
    assert (self);
    return self->headers;
}

//  Get the headers field and transfer ownership to caller

zhash_t *
zproto_example_get_headers (zproto_example_t *self)
{
    zhash_t *headers = self->headers;
    self->headers = NULL;
    return headers;
}

//  Set the headers field, transferring ownership from caller

void
zproto_example_set_headers (zproto_example_t *self, zhash_t **headers_p)
{
    assert (self);
    assert (headers_p);
    zhash_destroy (&self->headers);
    self->headers = *headers_p;
    *headers_p = NULL;
}

//  --------------------------------------------------------------------------
//  Get/set a value in the headers dictionary

const char *
zproto_example_headers_string (zproto_example_t *self, const char *key, const char *default_value)
{
    assert (self);
    const char *value = NULL;
    if (self->headers)
        value = (const char *) (zhash_lookup (self->headers, key));
    if (!value)
        value = default_value;

    return value;
}

uint64_t
zproto_example_headers_number (zproto_example_t *self, const char *key, uint64_t default_value)
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
zproto_example_headers_insert (zproto_example_t *self, const char *key, const char *format, ...)
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
//  Get the public_key field without transferring ownership

zchunk_t *
zproto_example_public_key (zproto_example_t *self)
{
    assert (self);
    return self->public_key;
}

//  Get the public_key field and transfer ownership to caller

zchunk_t *
zproto_example_get_public_key (zproto_example_t *self)
{
    zchunk_t *public_key = self->public_key;
    self->public_key = NULL;
    return public_key;
}

//  Set the public_key field, transferring ownership from caller

void
zproto_example_set_public_key (zproto_example_t *self, zchunk_t **chunk_p)
{
    assert (self);
    assert (chunk_p);
    zchunk_destroy (&self->public_key);
    self->public_key = *chunk_p;
    *chunk_p = NULL;
}


//  --------------------------------------------------------------------------
//  Get the identifier field without transferring ownership

zuuid_t *
zproto_example_identifier (zproto_example_t *self)
{
    assert (self);
    return self->identifier;
}

//  Get the identifier field and transfer ownership to caller

zuuid_t *
zproto_example_get_identifier (zproto_example_t *self)
{
    zuuid_t *identifier = self->identifier;
    self->identifier = NULL;
    return identifier;
}

//  Set the identifier field, transferring ownership from caller

void
zproto_example_set_identifier (zproto_example_t *self, zuuid_t **uuid_p)
{
    assert (self);
    assert (uuid_p);
    zuuid_destroy (&self->identifier);
    self->identifier = *uuid_p;
    *uuid_p = NULL;
}


//  --------------------------------------------------------------------------
//  Get the address field without transferring ownership

zframe_t *
zproto_example_address (zproto_example_t *self)
{
    assert (self);
    return self->address;
}

//  Get the address field and transfer ownership to caller

zframe_t *
zproto_example_get_address (zproto_example_t *self)
{
    zframe_t *address = self->address;
    self->address = NULL;
    return address;
}

//  Set the address field, transferring ownership from caller

void
zproto_example_set_address (zproto_example_t *self, zframe_t **frame_p)
{
    assert (self);
    assert (frame_p);
    zframe_destroy (&self->address);
    self->address = *frame_p;
    *frame_p = NULL;
}


//  --------------------------------------------------------------------------
//  Get the content field without transferring ownership

zmsg_t *
zproto_example_content (zproto_example_t *self)
{
    assert (self);
    return self->content;
}

//  Get the content field and transfer ownership to caller

zmsg_t *
zproto_example_get_content (zproto_example_t *self)
{
    zmsg_t *content = self->content;
    self->content = NULL;
    return content;
}

//  Set the content field, transferring ownership from caller

void
zproto_example_set_content (zproto_example_t *self, zmsg_t **msg_p)
{
    assert (self);
    assert (msg_p);
    zmsg_destroy (&self->content);
    self->content = *msg_p;
    *msg_p = NULL;
}


//  --------------------------------------------------------------------------
//  Get/set the client_forename field

const char *
zproto_example_client_forename (zproto_example_t *self)
{
    assert (self);
    return self->client_forename;
}

void
zproto_example_set_client_forename (zproto_example_t *self, const char *format, ...)
{
    //  Format client_forename from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->client_forename);
    self->client_forename = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the client_surname field

const char *
zproto_example_client_surname (zproto_example_t *self)
{
    assert (self);
    return self->client_surname;
}

void
zproto_example_set_client_surname (zproto_example_t *self, const char *format, ...)
{
    //  Format client_surname from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->client_surname);
    self->client_surname = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the client_mobile field

const char *
zproto_example_client_mobile (zproto_example_t *self)
{
    assert (self);
    return self->client_mobile;
}

void
zproto_example_set_client_mobile (zproto_example_t *self, const char *format, ...)
{
    //  Format client_mobile from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->client_mobile);
    self->client_mobile = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the client_email field

const char *
zproto_example_client_email (zproto_example_t *self)
{
    assert (self);
    return self->client_email;
}

void
zproto_example_set_client_email (zproto_example_t *self, const char *format, ...)
{
    //  Format client_email from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->client_email);
    self->client_email = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the supplier_forename field

const char *
zproto_example_supplier_forename (zproto_example_t *self)
{
    assert (self);
    return self->supplier_forename;
}

void
zproto_example_set_supplier_forename (zproto_example_t *self, const char *format, ...)
{
    //  Format supplier_forename from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->supplier_forename);
    self->supplier_forename = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the supplier_surname field

const char *
zproto_example_supplier_surname (zproto_example_t *self)
{
    assert (self);
    return self->supplier_surname;
}

void
zproto_example_set_supplier_surname (zproto_example_t *self, const char *format, ...)
{
    //  Format supplier_surname from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->supplier_surname);
    self->supplier_surname = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the supplier_mobile field

const char *
zproto_example_supplier_mobile (zproto_example_t *self)
{
    assert (self);
    return self->supplier_mobile;
}

void
zproto_example_set_supplier_mobile (zproto_example_t *self, const char *format, ...)
{
    //  Format supplier_mobile from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->supplier_mobile);
    self->supplier_mobile = zsys_vprintf (format, argptr);
    va_end (argptr);
}


//  --------------------------------------------------------------------------
//  Get/set the supplier_email field

const char *
zproto_example_supplier_email (zproto_example_t *self)
{
    assert (self);
    return self->supplier_email;
}

void
zproto_example_set_supplier_email (zproto_example_t *self, const char *format, ...)
{
    //  Format supplier_email from provided arguments
    assert (self);
    va_list argptr;
    va_start (argptr, format);
    free (self->supplier_email);
    self->supplier_email = zsys_vprintf (format, argptr);
    va_end (argptr);
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
    zsock_t *input = zsock_new (ZMQ_ROUTER);
    assert (input);
    zsock_connect (input, "inproc://selftest-zproto_example");

    zsock_t *output = zsock_new (ZMQ_DEALER);
    assert (output);
    zsock_bind (output, "inproc://selftest-zproto_example");

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
    zchunk_t *binary_public_key = zchunk_new ("Captcha Diem", 12);
    zproto_example_set_public_key (self, &binary_public_key);
    zuuid_t *binary_identifier = zuuid_new ();
    zuuid_t *binary_identifier_dup = zuuid_dup (binary_identifier);
    zproto_example_set_identifier (self, &binary_identifier);
    zframe_t *binary_address = zframe_new ("Captcha Diem", 12);
    zproto_example_set_address (self, &binary_address);
    zmsg_t *binary_content = zmsg_new ();
    zproto_example_set_content (self, &binary_content);
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
        zuuid_t *acutal_identifier = zproto_example_identifier (self);
        assert (zuuid_eq (binary_identifier_dup, zuuid_data (acutal_identifier)));
        if (instance == 1) {
            zuuid_destroy (&binary_identifier_dup);
        }
        assert (zframe_streq (zproto_example_address (self), "Captcha Diem"));
        assert (zmsg_size (zproto_example_content (self)) == 1);
        zproto_example_destroy (&self);
    }
    self = zproto_example_new (ZPROTO_EXAMPLE_TYPES);
    
    //  Check that _dup works on empty message
    copy = zproto_example_dup (self);
    assert (copy);
    zproto_example_destroy (&copy);

    zproto_example_set_sequence (self, 123);
    zproto_example_set_client_forename (self, "Life is short but Now lasts for ever");
    zproto_example_set_client_surname (self, "Life is short but Now lasts for ever");
    zproto_example_set_client_mobile (self, "Life is short but Now lasts for ever");
    zproto_example_set_client_email (self, "Life is short but Now lasts for ever");
    zproto_example_set_supplier_forename (self, "Life is short but Now lasts for ever");
    zproto_example_set_supplier_surname (self, "Life is short but Now lasts for ever");
    zproto_example_set_supplier_mobile (self, "Life is short but Now lasts for ever");
    zproto_example_set_supplier_email (self, "Life is short but Now lasts for ever");
    //  Send twice from same object
    zproto_example_send_again (self, output);
    zproto_example_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = zproto_example_recv (input);
        assert (self);
        assert (zproto_example_routing_id (self));
        
        assert (zproto_example_sequence (self) == 123);
        assert (streq (zproto_example_client_forename (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_client_surname (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_client_mobile (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_client_email (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_supplier_forename (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_supplier_surname (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_supplier_mobile (self), "Life is short but Now lasts for ever"));
        assert (streq (zproto_example_supplier_email (self), "Life is short but Now lasts for ever"));
        zproto_example_destroy (&self);
    }

    zsock_destroy (&input);
    zsock_destroy (&output);
    //  @end

    printf ("OK\n");
    return 0;
}
