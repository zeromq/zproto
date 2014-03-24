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
    uint64_t times [255];               //  Log date/time
    byte times_index;
    char *hosts [255];                  //  Originating hostname
    byte hosts_index;
    zlist_t *aliases;                   //  List of strings
    zhash_t *headers;                   //  Other random properties
    size_t headers_bytes;               //  Size of dictionary content
    byte flags [4];                     //  A set of flags
    zchunk_t *public_key;               //  Our public key
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
    self->times_index = 255;
    self->hosts_index = 255;
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
        int i;
        free (self->host);
        free (self->data);
        if (self->hosts) {
            for (i = 0; i < self->hosts_index; i++)
                if (self->hosts [i])
                    free (self->hosts [i]);
        }
        if (self->aliases)
            zlist_destroy (&self->aliases);
        zhash_destroy (&self->headers);
        zchunk_destroy (&self->public_key);
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

        case ZPROTO_EXAMPLE_LISTS:
            GET_NUMBER2 (self->sequence);
            {
                size_t list_size;
                GET_NUMBER1 (list_size);
                self->times_index = 0;
                while (self->times_index < list_size) {
                    GET_NUMBER8 (self->times [self->times_index]);
                    self->times_index++;    
                }
                //  Decrement to last valid position
                self->times_index--;    
            }
            {
                size_t list_size;
                GET_NUMBER1 (list_size);
                self->hosts_index = 0;
                while (self->hosts_index < list_size) {
                    GET_STRING (self->hosts [self->hosts_index]);
                    self->hosts_index++;    
                }
                //  Decrement to last valid position
                self->hosts_index--;    
            }
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
            
        case ZPROTO_EXAMPLE_LISTS:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  Array with 1-byte length
            frame_size += 1;
            {
                int i;
                for (i = 0; i < self->times_index; i++) {
                    //  times is a 8-byte integer
                    frame_size += 8;
                }
            }
            //  Array with 1-byte length
            frame_size += 1;
            {
                int i;
                for (i = 0; i < self->hosts_index; i++) {
                    //  hosts is a string with 1-byte length
                    frame_size++;       //  Size is one octet
                    if (self->hosts [i])
                        frame_size += strlen (self->hosts [i]);
                }
            }
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

        case ZPROTO_EXAMPLE_LISTS:
            PUT_NUMBER2 (self->sequence);
            {
                PUT_NUMBER1 (self->times_index + 1);
                int i;
                for (i = 0; i < self->times_index; i++) {
                    PUT_NUMBER8 (self->times [i]);
                }
            }    
            {
                PUT_NUMBER1 (self->hosts_index + 1);
                int i;
                for (i = 0; i < self->hosts_index; i++) {
                if (self->hosts [i]) {
                    PUT_STRING (self->hosts [i]);
                }
                else
                    PUT_NUMBER1 (0);    //  Empty string
                }
            }    
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
//  Send the LISTS to the socket in one step

int
zproto_example_send_lists (
    void *output,
    uint16_t sequence,
    uint64_t times [255], byte times_size,
    const char **hosts, byte hosts_size)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_LISTS);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_times (self, times, times_size);
    zproto_example_set_hosts (self, hosts, hosts_size);
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
    zframe_t *address,
    zmsg_t *content)
{
    zproto_example_t *self = zproto_example_new (ZPROTO_EXAMPLE_BINARY);
    zproto_example_set_sequence (self, sequence);
    zproto_example_set_flags (self, flags);
    zchunk_t *public_key_copy = zchunk_dup (public_key);
    zproto_example_set_public_key (self, &public_key_copy);
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

    int i;
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

        case ZPROTO_EXAMPLE_LISTS:
            copy->sequence = self->sequence;
            for (i = 0; i < self->times_index; i++) {
                copy->times [i] = self->times [i];
            }
            for (i = 0; i < self->hosts_index; i++) {
                copy->hosts [i] = self->hosts [i] ? strdup (self->hosts [i]): NULL;
            }
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
    int i;
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
            
        case ZPROTO_EXAMPLE_LISTS:
            puts ("LISTS:");
            printf ("    sequence=%ld\n", (long) self->sequence);
            printf ("   times=[");
            for (i = 0; i < self->times_index; i++) {
                printf ("%ld", (long) self->times);
                if (i < self->times_index - 1)
                    printf (",");
            }
            printf ("]\n");
            printf ("hosts=[");
            for (i = 0; i < self->hosts_index; i++) {
                if (self->hosts [i])
                    printf (" '%s'", self->hosts [i]);
                else
                    printf (" ");
                if (i < self->hosts_index - 1)
                    printf (",");
            }
            printf (" ]\n");
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
            printf ("    flags=[");
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
            
        case ZPROTO_EXAMPLE_TYPES:
            puts ("TYPES:");
            printf ("    sequence=%ld\n", (long) self->sequence);
            if (self->client_forename)
                printf ("    client_forename='%s'\n", self->client_forename);
            else
                printf ("    client_forename=\n");
            if (self->client_surname)
                printf ("    client_surname='%s'\n", self->client_surname);
            else
                printf ("    client_surname=\n");
            if (self->client_mobile)
                printf ("    client_mobile='%s'\n", self->client_mobile);
            else
                printf ("    client_mobile=\n");
            if (self->client_email)
                printf ("    client_email='%s'\n", self->client_email);
            else
                printf ("    client_email=\n");
            if (self->supplier_forename)
                printf ("    supplier_forename='%s'\n", self->supplier_forename);
            else
                printf ("    supplier_forename=\n");
            if (self->supplier_surname)
                printf ("    supplier_surname='%s'\n", self->supplier_surname);
            else
                printf ("    supplier_surname=\n");
            if (self->supplier_mobile)
                printf ("    supplier_mobile='%s'\n", self->supplier_mobile);
            else
                printf ("    supplier_mobile=\n");
            if (self->supplier_email)
                printf ("    supplier_email='%s'\n", self->supplier_email);
            else
                printf ("    supplier_email=\n");
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
        case ZPROTO_EXAMPLE_LISTS:
            return ("LISTS");
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
//  Get/set the times field

uint64_t 
zproto_example_times_index (zproto_example_t *self, byte index)
{
    assert (self);
    if (index > self->times_index)
        return 0;
    return self->times [index];
}

void
zproto_example_set_times (zproto_example_t *self, uint64_t times [255], byte size)
{
    assert (self);
    assert (size <= self->times_index);
    self->times_index = size;
    int i;
    for (i = 0; i < size; i++)
        self->times [i] = times [i];
}


//  --------------------------------------------------------------------------
//  Get/set the hosts field

const char *
zproto_example_hosts_index (zproto_example_t *self, byte index)
{
    assert (self);
    if (index > self->hosts_index)
        return NULL;
    return self->hosts [index];
}

void
zproto_example_set_hosts (zproto_example_t *self, const char **hosts, byte size)
{
    //  Format hosts from provided arguments
    assert (self);
    assert (size <= self->hosts_index);
    self->hosts_index = size;
    int i;
    for (i = 0; i < size; i++) 
        strcpy (self->hosts [i], hosts [i]);
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
    self = zproto_example_new (ZPROTO_EXAMPLE_LISTS);
    
    //  Check that _dup works on empty message
    copy = zproto_example_dup (self);
    assert (copy);
    zproto_example_destroy (&copy);

    zproto_example_set_sequence (self, 123);
    uint64_t times [255];
    times [0] = 10;
    times [1] = 20;
    times [2] = 30;
    zproto_example_set_times (self, times, 3);
    const char **hosts = malloc( sizeof (char**));
    hosts [0] = "Life is short"; 
    hosts [1] = "but Now lasts"; 
    hosts [2] = "for ever"; 
    zproto_example_set_hosts (self, hosts, 3);
    //  Send twice from same object
    zproto_example_send_again (self, output);
    zproto_example_send (&self, output);

    for (instance = 0; instance < 2; instance++) {
        self = zproto_example_recv (input);
        assert (self);
        assert (zproto_example_routing_id (self));
        
        assert (zproto_example_sequence (self) == 123);
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

    zctx_destroy (&ctx);
    //  @end

    printf ("OK\n");
    return 0;
}
