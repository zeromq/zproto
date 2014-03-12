/*  =========================================================================
    zproto_example - zproto example protocol
    
    Generated codec header for zproto_example
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

#ifndef __ZPROTO_EXAMPLE_H_INCLUDED__
#define __ZPROTO_EXAMPLE_H_INCLUDED__

/*  These are the zproto_example messages:

    LOG - Log an event.
        sequence            number 2    
        level               number 1    Log severity level
        event               number 1    Type of event
        node                number 2    Sending node
        peer                number 2    Refers to this peer
        time                number 8    Log date/time
        data                string      Actual log message

    STRUCTURES - This message contains a list and a hash.
        sequence            number 2    
        aliases             strings     List of strings
        headers             dictionary  Other random properties

    BINARY - Deliver a multi-part message.
        sequence            number 2    
        flags               octets [4]  A set of flags
        address             frame       Return address as frame
        content             msg         Message to be delivered
*/

#define ZPROTO_EXAMPLE_VERSION              1

#define ZPROTO_EXAMPLE_LOG                  1
#define ZPROTO_EXAMPLE_STRUCTURES           2
#define ZPROTO_EXAMPLE_BINARY               3
#define ZPROTO_EXAMPLE_FLAGS_SIZE           4

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _zproto_example_t zproto_example_t;

//  @interface
//  Create a new zproto_example
zproto_example_t *
    zproto_example_new (int id);

//  Destroy the zproto_example
void
    zproto_example_destroy (zproto_example_t **self_p);

//  Receive and parse a zproto_example from the input
zproto_example_t *
    zproto_example_recv (void *input);

//  Send the zproto_example to the output, and destroy it
int
    zproto_example_send (zproto_example_t **self_p, void *output);

//  Send the zproto_example to the output, and do not destroy it
int
    zproto_example_send_again (zproto_example_t *self, void *output);

//  Send the LOG to the output in one step
int
    zproto_example_send_log (void *output,
        uint16_t sequence,
        byte level,
        byte event,
        uint16_t node,
        uint16_t peer,
        uint64_t time,
        char *data);
    
//  Send the STRUCTURES to the output in one step
int
    zproto_example_send_structures (void *output,
        uint16_t sequence,
        zlist_t *aliases,
        zhash_t *headers);
    
//  Send the BINARY to the output in one step
int
    zproto_example_send_binary (void *output,
        uint16_t sequence,
        byte *flags,
        zframe_t *address,
        zmsg_t *content);
    
//  Duplicate the zproto_example message
zproto_example_t *
    zproto_example_dup (zproto_example_t *self);

//  Print contents of message to stdout
void
    zproto_example_dump (zproto_example_t *self);

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
char *
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

//  Get/set the data field
char *
    zproto_example_data (zproto_example_t *self);
void
    zproto_example_set_data (zproto_example_t *self, char *format, ...);

//  Get/set the aliases field
zlist_t *
    zproto_example_aliases (zproto_example_t *self);
void
    zproto_example_set_aliases (zproto_example_t *self, zlist_t *aliases);

//  Iterate through the aliases field, and append a aliases value
char *
    zproto_example_aliases_first (zproto_example_t *self);
char *
    zproto_example_aliases_next (zproto_example_t *self);
void
    zproto_example_aliases_append (zproto_example_t *self, char *format, ...);
size_t
    zproto_example_aliases_size (zproto_example_t *self);

//  Get/set the headers field
zhash_t *
    zproto_example_headers (zproto_example_t *self);
void
    zproto_example_set_headers (zproto_example_t *self, zhash_t *headers);
    
//  Get/set a value in the headers dictionary
char *
    zproto_example_headers_string (zproto_example_t *self, char *key, char *default_value);
uint64_t
    zproto_example_headers_number (zproto_example_t *self, char *key, uint64_t default_value);
void
    zproto_example_headers_insert (zproto_example_t *self, char *key, char *format, ...);
size_t
    zproto_example_headers_size (zproto_example_t *self);

//  Get/set the flags field
byte *
    zproto_example_flags (zproto_example_t *self);
void
    zproto_example_set_flags (zproto_example_t *self, byte *flags);

//  Get/set the address field
zframe_t *
    zproto_example_address (zproto_example_t *self);
void
    zproto_example_set_address (zproto_example_t *self, zframe_t *frame);

//  Get/set the content field
zmsg_t *
    zproto_example_content (zproto_example_t *self);
void
    zproto_example_set_content (zproto_example_t *self, zmsg_t *msg);

//  Self test of this class
int
    zproto_example_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
