/*  =========================================================================
    myproj_msg - myproj example protocol

    Generated codec implementation for myproj_msg
    -------------------------------------------------------------------------
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
    myproj_msg - myproj example protocol
@discuss
@end
*/

#include <czmq.h>
#include "../include/myproj_msg.h"

//  Structure of our class

struct _myproj_msg_t {
    zframe_t *routing_id;       //  Routing_id from ROUTER, if any
    int id;                     //  myproj_msg message ID
    byte *needle;               //  Read/write pointer for serialization
    byte *ceiling;              //  Valid upper limit for read pointer
    uint16_t sequence;          //  
    byte level;                 //  Log severity level
    byte event;                 //  Type of event
    uint16_t node;              //  Sending node
    uint16_t peer;              //  Refers to this peer
    uint64_t time;              //  Log date/time
    char *data;                 //  Actual log message
    zlist_t *aliases;           //  List of strings
    zhash_t *headers;           //  Other random properties
    size_t headers_bytes;       //  Size of dictionary content
    byte flags [4];             //  A set of flags
    zframe_t *address;          //  Return address as frame
    zmsg_t *content;            //  Message to be delivered
};

//  --------------------------------------------------------------------------
//  Network data encoding macros

//  Strings are encoded with 1-byte length
#define STRING_MAX  255

//  Put a block to the frame
#define PUT_OCTETS(host,size) { \
    memcpy (self->needle, (host), size); \
    self->needle += size; \
    }

//  Get a block from the frame
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
    string_size = strlen (host); \
    PUT_NUMBER1 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
    }

//  Get a string from the frame
#define GET_STRING(host) { \
    GET_NUMBER1 (string_size); \
    if (self->needle + string_size > (self->ceiling)) \
        goto malformed; \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
    }


//  --------------------------------------------------------------------------
//  Create a new myproj_msg

myproj_msg_t *
myproj_msg_new (int id)
{
    myproj_msg_t *self = (myproj_msg_t *) zmalloc (sizeof (myproj_msg_t));
    self->id = id;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the myproj_msg

void
myproj_msg_destroy (myproj_msg_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        myproj_msg_t *self = *self_p;

        //  Free class properties
        zframe_destroy (&self->routing_id);
        free (self->data);
        if (self->aliases)
            zlist_destroy (&self->aliases);
        zhash_destroy (&self->headers);
        zframe_destroy (&self->address);
        zmsg_destroy (&self->content);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Receive and parse a myproj_msg from the socket. Returns new object or
//  NULL if error. Will block if there's no message waiting.

myproj_msg_t *
myproj_msg_recv (void *input)
{
    assert (input);
    myproj_msg_t *self = myproj_msg_new (0);
    zframe_t *frame = NULL;
    size_t string_size;

    //  Read valid message frame from socket; we loop over any
    //  garbage data we might receive from badly-connected peers
    while (true) {
        //  If we're reading from a ROUTER socket, get routing_id
        if (zsocket_type (input) == ZMQ_ROUTER) {
            zframe_destroy (&self->routing_id);
            self->routing_id = zframe_recv (input);
            if (!self->routing_id)
                goto empty;         //  Interrupted
            if (!zsocket_rcvmore (input))
                goto malformed;
        }
        //  Read and parse command in frame
        frame = zframe_recv (input);
        if (!frame)
            goto empty;             //  Interrupted

        //  Get and check protocol signature
        self->needle = zframe_data (frame);
        self->ceiling = self->needle + zframe_size (frame);
        uint16_t signature;
        GET_NUMBER2 (signature);
        if (signature == (0xAAA0 | 0))
            break;                  //  Valid signature

        //  Protocol assertion, drop message
        while (zsocket_rcvmore (input)) {
            zframe_destroy (&frame);
            frame = zframe_recv (input);
        }
        zframe_destroy (&frame);
    }
    //  Get message id and parse per message type
    GET_NUMBER1 (self->id);

    switch (self->id) {
        case MYPROJ_MSG_LOG:
            GET_NUMBER2 (self->sequence);
            GET_NUMBER1 (self->level);
            GET_NUMBER1 (self->event);
            GET_NUMBER2 (self->node);
            GET_NUMBER2 (self->peer);
            GET_NUMBER8 (self->time);
            free (self->data);
            GET_STRING (self->data);
            break;

        case MYPROJ_MSG_STRUCTURES:
            GET_NUMBER2 (self->sequence);
            size_t list_size;
            GET_NUMBER1 (list_size);
            self->aliases = zlist_new ();
            zlist_autofree (self->aliases);
            while (list_size--) {
                char *string;
                GET_STRING (string);
                zlist_append (self->aliases, string);
                free (string);
            }
            size_t hash_size;
            GET_NUMBER1 (hash_size);
            self->headers = zhash_new ();
            zhash_autofree (self->headers);
            while (hash_size--) {
                char *string;
                GET_STRING (string);
                char *value = strchr (string, '=');
                if (value)
                    *value++ = 0;
                zhash_insert (self->headers, string, value);
                free (string);
            }
            break;

        case MYPROJ_MSG_BINARY:
            GET_NUMBER2 (self->sequence);
            GET_OCTETS (self->flags, 4);
            //  Get next frame, leave current untouched
            if (!zsocket_rcvmore (input))
                goto malformed;
            self->address = zframe_recv (input);
            //  Get zero or more remaining frames,
            //  leave current frame untouched
            self->content = zmsg_new ();
            while (zsocket_rcvmore (input))
                zmsg_add (self->content, zframe_recv (input));
            break;

        default:
            goto malformed;
    }
    //  Successful return
    zframe_destroy (&frame);
    return self;

    //  Error returns
    malformed:
        printf ("E: malformed message '%d'\n", self->id);
    empty:
        zframe_destroy (&frame);
        myproj_msg_destroy (&self);
        return (NULL);
}

//  Count size of key=value pair
static int
s_headers_count (const char *key, void *item, void *argument)
{
    myproj_msg_t *self = (myproj_msg_t *) argument;
    self->headers_bytes += strlen (key) + 1 + strlen ((char *) item) + 1;
    return 0;
}

//  Serialize headers key=value pair
static int
s_headers_write (const char *key, void *item, void *argument)
{
    myproj_msg_t *self = (myproj_msg_t *) argument;
    char string [STRING_MAX + 1];
    snprintf (string, STRING_MAX, "%s=%s", key, (char *) item);
    size_t string_size;
    PUT_STRING (string);
    return 0;
}


//  --------------------------------------------------------------------------
//  Send the myproj_msg to the socket, and destroy it
//  Returns 0 if OK, else -1

int
myproj_msg_send (myproj_msg_t **self_p, void *output)
{
    assert (output);
    assert (self_p);
    assert (*self_p);

    //  Calculate size of serialized data
    myproj_msg_t *self = *self_p;
    size_t frame_size = 2 + 1;          //  Signature and message ID
    switch (self->id) {
        case MYPROJ_MSG_LOG:
            //  sequence is a 2-byte integer
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
            //  data is a string with 1-byte length
            frame_size++;       //  Size is one octet
            if (self->data)
                frame_size += strlen (self->data);
            break;
            
        case MYPROJ_MSG_STRUCTURES:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  aliases is an array of strings
            frame_size++;       //  Size is one octet
            if (self->aliases) {
                //  Add up size of list contents
                char *aliases = (char *) zlist_first (self->aliases);
                while (aliases) {
                    frame_size += 1 + strlen (aliases);
                    aliases = (char *) zlist_next (self->aliases);
                }
            }
            //  headers is an array of key=value strings
            frame_size++;       //  Size is one octet
            if (self->headers) {
                self->headers_bytes = 0;
                //  Add up size of dictionary contents
                zhash_foreach (self->headers, s_headers_count, self);
            }
            frame_size += self->headers_bytes;
            break;
            
        case MYPROJ_MSG_BINARY:
            //  sequence is a 2-byte integer
            frame_size += 2;
            //  flags is a block of 4 bytes
            frame_size += 4;
            break;
            
        default:
            printf ("E: bad message type '%d', not sent\n", self->id);
            //  No recovery, this is a fatal application error
            assert (false);
    }
    //  Now serialize message into the frame
    zframe_t *frame = zframe_new (NULL, frame_size);
    self->needle = zframe_data (frame);
    size_t string_size;
    int frame_flags = 0;
    PUT_NUMBER2 (0xAAA0 | 0);
    PUT_NUMBER1 (self->id);

    switch (self->id) {
        case MYPROJ_MSG_LOG:
            PUT_NUMBER2 (self->sequence);
            PUT_NUMBER1 (self->level);
            PUT_NUMBER1 (self->event);
            PUT_NUMBER2 (self->node);
            PUT_NUMBER2 (self->peer);
            PUT_NUMBER8 (self->time);
            if (self->data) {
                PUT_STRING (self->data);
            }
            else
                PUT_NUMBER1 (0);    //  Empty string
            break;

        case MYPROJ_MSG_STRUCTURES:
            PUT_NUMBER2 (self->sequence);
            if (self->aliases != NULL) {
                PUT_NUMBER1 (zlist_size (self->aliases));
                char *aliases = (char *) zlist_first (self->aliases);
                while (aliases) {
                    PUT_STRING (aliases);
                    aliases = (char *) zlist_next (self->aliases);
                }
            }
            else
                PUT_NUMBER1 (0);    //  Empty string array
            if (self->headers != NULL) {
                PUT_NUMBER1 (zhash_size (self->headers));
                zhash_foreach (self->headers, s_headers_write, self);
            }
            else
                PUT_NUMBER1 (0);    //  Empty dictionary
            break;

        case MYPROJ_MSG_BINARY:
            PUT_NUMBER2 (self->sequence);
            PUT_OCTETS (self->flags, 4);
            frame_flags = ZFRAME_MORE;
            break;

    }
    //  If we're sending to a ROUTER, we send the routing_id first
    if (zsocket_type (output) == ZMQ_ROUTER) {
        assert (self->routing_id);
        if (zframe_send (&self->routing_id, output, ZFRAME_MORE)) {
            zframe_destroy (&frame);
            myproj_msg_destroy (self_p);
            return -1;
        }
    }
    //  Now send the data frame
    if (zframe_send (&frame, output, frame_flags)) {
        zframe_destroy (&frame);
        myproj_msg_destroy (self_p);
        return -1;
    }
    //  Now send any frame fields, in order
    if (self->id == MYPROJ_MSG_BINARY) {
        //  If address isn't set, send an empty frame
        if (!self->address)
            self->address = zframe_new (NULL, 0);
        frame_flags = zmsg_size (self->content)? ZFRAME_MORE: 0;
        if (zframe_send (&self->address, output, frame_flags)) {
            zframe_destroy (&frame);
            myproj_msg_destroy (self_p);
            return -1;
        }
    }
    //  Now send the content field if set
    if (self->id == MYPROJ_MSG_BINARY)
        zmsg_send (&self->content, output);
        
    //  Destroy myproj_msg object
    myproj_msg_destroy (self_p);
    return 0;
}


//  --------------------------------------------------------------------------
//  Send the LOG to the socket in one step

int
myproj_msg_send_log (
    void *output,
    uint16_t sequence,
    byte level,
    byte event,
    uint16_t node,
    uint16_t peer,
    uint64_t time,
    char *data)
{
    myproj_msg_t *self = myproj_msg_new (MYPROJ_MSG_LOG);
    myproj_msg_set_sequence (self, sequence);
    myproj_msg_set_level (self, level);
    myproj_msg_set_event (self, event);
    myproj_msg_set_node (self, node);
    myproj_msg_set_peer (self, peer);
    myproj_msg_set_time (self, time);
    myproj_msg_set_data (self, data);
    return myproj_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the STRUCTURES to the socket in one step

int
myproj_msg_send_structures (
    void *output,
    uint16_t sequence,
    zlist_t *aliases,
    zhash_t *headers)
{
    myproj_msg_t *self = myproj_msg_new (MYPROJ_MSG_STRUCTURES);
    myproj_msg_set_sequence (self, sequence);
    myproj_msg_set_aliases (self, zlist_dup (aliases));
    myproj_msg_set_headers (self, zhash_dup (headers));
    return myproj_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Send the BINARY to the socket in one step

int
myproj_msg_send_binary (
    void *output,
    uint16_t sequence,
    byte *flags,
    zframe_t *address,
    zmsg_t *content)
{
    myproj_msg_t *self = myproj_msg_new (MYPROJ_MSG_BINARY);
    myproj_msg_set_sequence (self, sequence);
    myproj_msg_set_flags (self, flags);
    myproj_msg_set_address (self, zframe_dup (address));
    myproj_msg_set_content (self, zmsg_dup (content));
    return myproj_msg_send (&self, output);
}


//  --------------------------------------------------------------------------
//  Duplicate the myproj_msg message

myproj_msg_t *
myproj_msg_dup (myproj_msg_t *self)
{
    if (!self)
        return NULL;
        
    myproj_msg_t *copy = myproj_msg_new (self->id);
    if (self->routing_id)
        copy->routing_id = zframe_dup (self->routing_id);

    switch (self->id) {
        case MYPROJ_MSG_LOG:
            copy->sequence = self->sequence;
            copy->level = self->level;
            copy->event = self->event;
            copy->node = self->node;
            copy->peer = self->peer;
            copy->time = self->time;
            copy->data = strdup (self->data);
            break;

        case MYPROJ_MSG_STRUCTURES:
            copy->sequence = self->sequence;
            copy->aliases = zlist_dup (self->aliases);
            copy->headers = zhash_dup (self->headers);
            break;

        case MYPROJ_MSG_BINARY:
            copy->sequence = self->sequence;
            memcpy (copy->flags, self->flags, 4);
            copy->address = zframe_dup (self->address);
            copy->content = zmsg_dup (self->content);
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
myproj_msg_dump (myproj_msg_t *self)
{
    assert (self);
    switch (self->id) {
        case MYPROJ_MSG_LOG:
            puts ("LOG:");
            printf ("    sequence=%ld\n", (long) self->sequence);
            printf ("    level=%ld\n", (long) self->level);
            printf ("    event=%ld\n", (long) self->event);
            printf ("    node=%ld\n", (long) self->node);
            printf ("    peer=%ld\n", (long) self->peer);
            printf ("    time=%ld\n", (long) self->time);
            if (self->data)
                printf ("    data='%s'\n", self->data);
            else
                printf ("    data=\n");
            break;
            
        case MYPROJ_MSG_STRUCTURES:
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
            printf ("    }\n");
            break;
            
        case MYPROJ_MSG_BINARY:
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
            printf ("    address={\n");
            if (self->address)
                zframe_print (self->address, NULL);
            printf ("    }\n");
            printf ("    content={\n");
            if (self->content)
                zmsg_dump (self->content);
            printf ("    }\n");
            break;
            
    }
}


//  --------------------------------------------------------------------------
//  Get/set the message routing_id

zframe_t *
myproj_msg_routing_id (myproj_msg_t *self)
{
    assert (self);
    return self->routing_id;
}

void
myproj_msg_set_routing_id (myproj_msg_t *self, zframe_t *routing_id)
{
    if (self->routing_id)
        zframe_destroy (&self->routing_id);
    self->routing_id = zframe_dup (routing_id);
}


//  --------------------------------------------------------------------------
//  Get/set the myproj_msg id

int
myproj_msg_id (myproj_msg_t *self)
{
    assert (self);
    return self->id;
}

void
myproj_msg_set_id (myproj_msg_t *self, int id)
{
    self->id = id;
}

//  --------------------------------------------------------------------------
//  Return a printable command string

char *
myproj_msg_command (myproj_msg_t *self)
{
    assert (self);
    switch (self->id) {
        case MYPROJ_MSG_LOG:
            return ("LOG");
            break;
        case MYPROJ_MSG_STRUCTURES:
            return ("STRUCTURES");
            break;
        case MYPROJ_MSG_BINARY:
            return ("BINARY");
            break;
    }
    return "?";
}

//  --------------------------------------------------------------------------
//  Get/set the sequence field

uint16_t
myproj_msg_sequence (myproj_msg_t *self)
{
    assert (self);
    return self->sequence;
}

void
myproj_msg_set_sequence (myproj_msg_t *self, uint16_t sequence)
{
    assert (self);
    self->sequence = sequence;
}


//  --------------------------------------------------------------------------
//  Get/set the level field

byte
myproj_msg_level (myproj_msg_t *self)
{
    assert (self);
    return self->level;
}

void
myproj_msg_set_level (myproj_msg_t *self, byte level)
{
    assert (self);
    self->level = level;
}


//  --------------------------------------------------------------------------
//  Get/set the event field

byte
myproj_msg_event (myproj_msg_t *self)
{
    assert (self);
    return self->event;
}

void
myproj_msg_set_event (myproj_msg_t *self, byte event)
{
    assert (self);
    self->event = event;
}


//  --------------------------------------------------------------------------
//  Get/set the node field

uint16_t
myproj_msg_node (myproj_msg_t *self)
{
    assert (self);
    return self->node;
}

void
myproj_msg_set_node (myproj_msg_t *self, uint16_t node)
{
    assert (self);
    self->node = node;
}


//  --------------------------------------------------------------------------
//  Get/set the peer field

uint16_t
myproj_msg_peer (myproj_msg_t *self)
{
    assert (self);
    return self->peer;
}

void
myproj_msg_set_peer (myproj_msg_t *self, uint16_t peer)
{
    assert (self);
    self->peer = peer;
}


//  --------------------------------------------------------------------------
//  Get/set the time field

uint64_t
myproj_msg_time (myproj_msg_t *self)
{
    assert (self);
    return self->time;
}

void
myproj_msg_set_time (myproj_msg_t *self, uint64_t time)
{
    assert (self);
    self->time = time;
}


//  --------------------------------------------------------------------------
//  Get/set the data field

char *
myproj_msg_data (myproj_msg_t *self)
{
    assert (self);
    return self->data;
}

void
myproj_msg_set_data (myproj_msg_t *self, char *format, ...)
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
myproj_msg_aliases (myproj_msg_t *self)
{
    assert (self);
    return self->aliases;
}

//  Greedy function, takes ownership of aliases; if you don't want that
//  then use zlist_dup() to pass a copy of aliases

void
myproj_msg_set_aliases (myproj_msg_t *self, zlist_t *aliases)
{
    assert (self);
    zlist_destroy (&self->aliases);
    self->aliases = aliases;
}

//  --------------------------------------------------------------------------
//  Iterate through the aliases field, and append a aliases value

char *
myproj_msg_aliases_first (myproj_msg_t *self)
{
    assert (self);
    if (self->aliases)
        return (char *) (zlist_first (self->aliases));
    else
        return NULL;
}

char *
myproj_msg_aliases_next (myproj_msg_t *self)
{
    assert (self);
    if (self->aliases)
        return (char *) (zlist_next (self->aliases));
    else
        return NULL;
}

void
myproj_msg_aliases_append (myproj_msg_t *self, char *format, ...)
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
myproj_msg_aliases_size (myproj_msg_t *self)
{
    return zlist_size (self->aliases);
}


//  --------------------------------------------------------------------------
//  Get/set the headers field

zhash_t *
myproj_msg_headers (myproj_msg_t *self)
{
    assert (self);
    return self->headers;
}

//  Greedy function, takes ownership of headers; if you don't want that
//  then use zhash_dup() to pass a copy of headers

void
myproj_msg_set_headers (myproj_msg_t *self, zhash_t *headers)
{
    assert (self);
    zhash_destroy (&self->headers);
    self->headers = headers;
}

//  --------------------------------------------------------------------------
//  Get/set a value in the headers dictionary

char *
myproj_msg_headers_string (myproj_msg_t *self, char *key, char *default_value)
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
myproj_msg_headers_number (myproj_msg_t *self, char *key, uint64_t default_value)
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
myproj_msg_headers_insert (myproj_msg_t *self, char *key, char *format, ...)
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
myproj_msg_headers_size (myproj_msg_t *self)
{
    return zhash_size (self->headers);
}


//  --------------------------------------------------------------------------
//  Get/set the flags field

byte *
myproj_msg_flags (myproj_msg_t *self)
{
    assert (self);
    return self->flags;
}

void
myproj_msg_set_flags (myproj_msg_t *self, byte *flags)
{
    assert (self);
    memcpy (self->flags, flags, 4);
}


//  --------------------------------------------------------------------------
//  Get/set the address field

zframe_t *
myproj_msg_address (myproj_msg_t *self)
{
    assert (self);
    return self->address;
}

//  Takes ownership of supplied frame
void
myproj_msg_set_address (myproj_msg_t *self, zframe_t *frame)
{
    assert (self);
    if (self->address)
        zframe_destroy (&self->address);
    self->address = frame;
}


//  --------------------------------------------------------------------------
//  Get/set the content field

zmsg_t *
myproj_msg_content (myproj_msg_t *self)
{
    assert (self);
    return self->content;
}

//  Takes ownership of supplied msg
void
myproj_msg_set_content (myproj_msg_t *self, zmsg_t *msg)
{
    assert (self);
    if (self->content)
        zmsg_destroy (&self->content);
    self->content = msg;
}


//  --------------------------------------------------------------------------
//  Selftest

int
myproj_msg_test (bool verbose)
{
    printf (" * myproj_msg: ");

    //  @selftest
    //  Simple create/destroy test
    myproj_msg_t *self = myproj_msg_new (0);
    assert (self);
    myproj_msg_destroy (&self);

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

    self = myproj_msg_new (MYPROJ_MSG_LOG);
    myproj_msg_set_sequence (self, 123);
    myproj_msg_set_level (self, 123);
    myproj_msg_set_event (self, 123);
    myproj_msg_set_node (self, 123);
    myproj_msg_set_peer (self, 123);
    myproj_msg_set_time (self, 123);
    myproj_msg_set_data (self, "Life is short but Now lasts for ever");
    myproj_msg_send (&self, output);
    
    self = myproj_msg_recv (input);
    assert (self);
    assert (myproj_msg_sequence (self) == 123);
    assert (myproj_msg_level (self) == 123);
    assert (myproj_msg_event (self) == 123);
    assert (myproj_msg_node (self) == 123);
    assert (myproj_msg_peer (self) == 123);
    assert (myproj_msg_time (self) == 123);
    assert (streq (myproj_msg_data (self), "Life is short but Now lasts for ever"));
    myproj_msg_destroy (&self);

    self = myproj_msg_new (MYPROJ_MSG_STRUCTURES);
    myproj_msg_set_sequence (self, 123);
    myproj_msg_aliases_append (self, "Name: %s", "Brutus");
    myproj_msg_aliases_append (self, "Age: %d", 43);
    myproj_msg_headers_insert (self, "Name", "Brutus");
    myproj_msg_headers_insert (self, "Age", "%d", 43);
    myproj_msg_send (&self, output);
    
    self = myproj_msg_recv (input);
    assert (self);
    assert (myproj_msg_sequence (self) == 123);
    assert (myproj_msg_aliases_size (self) == 2);
    assert (streq (myproj_msg_aliases_first (self), "Name: Brutus"));
    assert (streq (myproj_msg_aliases_next (self), "Age: 43"));
    assert (myproj_msg_headers_size (self) == 2);
    assert (streq (myproj_msg_headers_string (self, "Name", "?"), "Brutus"));
    assert (myproj_msg_headers_number (self, "Age", 0) == 43);
    myproj_msg_destroy (&self);

    self = myproj_msg_new (MYPROJ_MSG_BINARY);
    myproj_msg_set_sequence (self, 123);
    byte flags_data [MYPROJ_MSG_FLAGS_SIZE];
    memset (flags_data, 123, MYPROJ_MSG_FLAGS_SIZE);
    myproj_msg_set_flags (self, flags_data);
    myproj_msg_set_address (self, zframe_new ("Captcha Diem", 12));
    myproj_msg_set_content (self, zmsg_new ());
    myproj_msg_send (&self, output);
    
    self = myproj_msg_recv (input);
    assert (self);
    assert (myproj_msg_sequence (self) == 123);
    assert (myproj_msg_flags (self) [0] == 123);
    assert (myproj_msg_flags (self) [MYPROJ_MSG_FLAGS_SIZE - 1] == 123);
    assert (zframe_streq (myproj_msg_address (self), "Captcha Diem"));
    assert (zmsg_size (myproj_msg_content (self)) == 0);
    myproj_msg_destroy (&self);

    zctx_destroy (&ctx);
    //  @end

    printf ("OK\n");
    return 0;
}
