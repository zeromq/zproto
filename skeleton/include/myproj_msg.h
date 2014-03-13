/*  =========================================================================
    myproj_msg - myproj example protocol
    
    Generated codec header for myproj_msg
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

#ifndef __MYPROJ_MSG_H_INCLUDED__
#define __MYPROJ_MSG_H_INCLUDED__

/*  These are the myproj_msg messages:

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

#define MYPROJ_MSG_VERSION                  1

#define MYPROJ_MSG_LOG                      1
#define MYPROJ_MSG_STRUCTURES               2
#define MYPROJ_MSG_BINARY                   3
#define MYPROJ_MSG_FLAGS_SIZE               4

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _myproj_msg_t myproj_msg_t;

//  @interface
//  Create a new myproj_msg
myproj_msg_t *
    myproj_msg_new (int id);

//  Destroy the myproj_msg
void
    myproj_msg_destroy (myproj_msg_t **self_p);

//  Receive and parse a myproj_msg from the input
myproj_msg_t *
    myproj_msg_recv (void *input);

//  Send the myproj_msg to the output, and destroy it
int
    myproj_msg_send (myproj_msg_t **self_p, void *output);

//  Send the LOG to the output in one step
int
    myproj_msg_send_log (void *output,
        uint16_t sequence,
        byte level,
        byte event,
        uint16_t node,
        uint16_t peer,
        uint64_t time,
        char *data);
    
//  Send the STRUCTURES to the output in one step
int
    myproj_msg_send_structures (void *output,
        uint16_t sequence,
        zlist_t *aliases,
        zhash_t *headers);
    
//  Send the BINARY to the output in one step
int
    myproj_msg_send_binary (void *output,
        uint16_t sequence,
        byte *flags,
        zframe_t *address,
        zmsg_t *content);
    
//  Duplicate the myproj_msg message
myproj_msg_t *
    myproj_msg_dup (myproj_msg_t *self);

//  Print contents of message to stdout
void
    myproj_msg_dump (myproj_msg_t *self);

//  Get/set the message routing id
zframe_t *
    myproj_msg_routing_id (myproj_msg_t *self);
void
    myproj_msg_set_routing_id (myproj_msg_t *self, zframe_t *routing_id);

//  Get the myproj_msg id and printable command
int
    myproj_msg_id (myproj_msg_t *self);
void
    myproj_msg_set_id (myproj_msg_t *self, int id);
char *
    myproj_msg_command (myproj_msg_t *self);

//  Get/set the sequence field
uint16_t
    myproj_msg_sequence (myproj_msg_t *self);
void
    myproj_msg_set_sequence (myproj_msg_t *self, uint16_t sequence);

//  Get/set the level field
byte
    myproj_msg_level (myproj_msg_t *self);
void
    myproj_msg_set_level (myproj_msg_t *self, byte level);

//  Get/set the event field
byte
    myproj_msg_event (myproj_msg_t *self);
void
    myproj_msg_set_event (myproj_msg_t *self, byte event);

//  Get/set the node field
uint16_t
    myproj_msg_node (myproj_msg_t *self);
void
    myproj_msg_set_node (myproj_msg_t *self, uint16_t node);

//  Get/set the peer field
uint16_t
    myproj_msg_peer (myproj_msg_t *self);
void
    myproj_msg_set_peer (myproj_msg_t *self, uint16_t peer);

//  Get/set the time field
uint64_t
    myproj_msg_time (myproj_msg_t *self);
void
    myproj_msg_set_time (myproj_msg_t *self, uint64_t time);

//  Get/set the data field
char *
    myproj_msg_data (myproj_msg_t *self);
void
    myproj_msg_set_data (myproj_msg_t *self, char *format, ...);

//  Get/set the aliases field
zlist_t *
    myproj_msg_aliases (myproj_msg_t *self);
void
    myproj_msg_set_aliases (myproj_msg_t *self, zlist_t *aliases);

//  Iterate through the aliases field, and append a aliases value
char *
    myproj_msg_aliases_first (myproj_msg_t *self);
char *
    myproj_msg_aliases_next (myproj_msg_t *self);
void
    myproj_msg_aliases_append (myproj_msg_t *self, char *format, ...);
size_t
    myproj_msg_aliases_size (myproj_msg_t *self);

//  Get/set the headers field
zhash_t *
    myproj_msg_headers (myproj_msg_t *self);
void
    myproj_msg_set_headers (myproj_msg_t *self, zhash_t *headers);
    
//  Get/set a value in the headers dictionary
char *
    myproj_msg_headers_string (myproj_msg_t *self, char *key, char *default_value);
uint64_t
    myproj_msg_headers_number (myproj_msg_t *self, char *key, uint64_t default_value);
void
    myproj_msg_headers_insert (myproj_msg_t *self, char *key, char *format, ...);
size_t
    myproj_msg_headers_size (myproj_msg_t *self);

//  Get/set the flags field
byte *
    myproj_msg_flags (myproj_msg_t *self);
void
    myproj_msg_set_flags (myproj_msg_t *self, byte *flags);

//  Get/set the address field
zframe_t *
    myproj_msg_address (myproj_msg_t *self);
void
    myproj_msg_set_address (myproj_msg_t *self, zframe_t *frame);

//  Get/set the content field
zmsg_t *
    myproj_msg_content (myproj_msg_t *self);
void
    myproj_msg_set_content (myproj_msg_t *self, zmsg_t *msg);

//  Self test of this class
int
    myproj_msg_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
