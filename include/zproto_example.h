/*  =========================================================================
    zproto_example - zproto example protocol
    
    Codec header for zproto_example.

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

#ifndef __ZPROTO_EXAMPLE_H_INCLUDED__
#define __ZPROTO_EXAMPLE_H_INCLUDED__

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
        headers             dictionary  Other random properties

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

    REPEAT - Demonstrates repeating fields
        sequence            number 2    
        no1                 number 1    Repeating byte
        no2                 number 2    Repeating 2-bytes
        no4                 number 4    Repeating 4-bytes
        no8                 number 8    Repeating 8-bytes
        str                 string      Repeating 1-byte string
        lstr                longstr     Repeating 4-byte string
        strs                strings     Repeating strings
        chunks              chunk       Repeating chunks
        uuids               uuid        Repeating uuids
        persons_forename    string      Given name
        persons_surname     string      Family name
        persons_mobile      string      Mobile phone number
        persons_email       string      Email address
*/

#define ZPROTO_EXAMPLE_VERSION              1

#define ZPROTO_EXAMPLE_LOG                  1
#define ZPROTO_EXAMPLE_STRUCTURES           2
#define ZPROTO_EXAMPLE_BINARY               3
#define ZPROTO_EXAMPLE_TYPES                4
#define ZPROTO_EXAMPLE_REPEAT               5
#define ZPROTO_EXAMPLE_FLAGS_SIZE           4

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _zproto_example_t zproto_example_t;

//  @interface
//  Create a new zproto_example
CZMQ_EXPORT zproto_example_t *
    zproto_example_new (int id);

//  Destroy the zproto_example
CZMQ_EXPORT void
    zproto_example_destroy (zproto_example_t **self_p);

//  Parse a zproto_example from zmsg_t. Returns a new object, or NULL if
//  the message could not be parsed, or was NULL. If the socket type is
//  ZMQ_ROUTER, then parses the first frame as a routing_id. Destroys msg
//  and nullifies the msg refernce.
CZMQ_EXPORT zproto_example_t *
    zproto_example_decode (zmsg_t **msg_p);

//  Encode zproto_example into zmsg and destroy it. Returns a newly created
//  object or NULL if error. Use when not in control of sending the message.
//  If the socket_type is ZMQ_ROUTER, then stores the routing_id as the
//  first frame of the resulting message.
CZMQ_EXPORT zmsg_t *
    zproto_example_encode (zproto_example_t **self_p);

//  Receive and parse a zproto_example from the socket. Returns new object, 
//  or NULL if error. Will block if there's no message waiting.
CZMQ_EXPORT zproto_example_t *
    zproto_example_recv (void *input);

//  Receive and parse a zproto_example from the socket. Returns new object, 
//  or NULL either if there was no input waiting, or the recv was interrupted.
CZMQ_EXPORT zproto_example_t *
    zproto_example_recv_nowait (void *input);

//  Send the zproto_example to the output, and destroy it
CZMQ_EXPORT int
    zproto_example_send (zproto_example_t **self_p, void *output);

//  Send the zproto_example to the output, and do not destroy it
CZMQ_EXPORT int
    zproto_example_send_again (zproto_example_t *self, void *output);

//  Encode the LOG 
CZMQ_EXPORT zmsg_t *
    zproto_example_encode_log (
        uint16_t sequence,
        byte level,
        byte event,
        uint16_t node,
        uint16_t peer,
        uint64_t time,
        const char *host,
        const char *data);

//  Encode the STRUCTURES 
CZMQ_EXPORT zmsg_t *
    zproto_example_encode_structures (
        uint16_t sequence,
        zlist_t *aliases,
        zhash_t *headers);

//  Encode the BINARY 
CZMQ_EXPORT zmsg_t *
    zproto_example_encode_binary (
        uint16_t sequence,
        byte *flags,
        zchunk_t *public_key,
        zuuid_t *identifier,
        zframe_t *address,
        zmsg_t *content);

//  Encode the TYPES 
CZMQ_EXPORT zmsg_t *
    zproto_example_encode_types (
        uint16_t sequence,
        const char *client_forename,
        const char *client_surname,
        const char *client_mobile,
        const char *client_email,
        const char *supplier_forename,
        const char *supplier_surname,
        const char *supplier_mobile,
        const char *supplier_email);

//  Encode the REPEAT 
CZMQ_EXPORT zmsg_t *
    zproto_example_encode_repeat (
        uint16_t sequence,
        byte no1 [3], byte no1_size,
        uint16_t no2 [144], byte no2_size,
        uint32_t no4 [256], byte no4_size,
        uint64_t no8 [256], byte no8_size,
        char **str, byte str_size,
        char **lstr, byte lstr_size,
        zlist_t **strs, byte strs_size,
        zchunk_t **chunks, byte chunks_size,
        zuuid_t **uuids, byte uuids_size,
        char **persons_forename, byte persons_forename_size,
        char **persons_surname, byte persons_surname_size,
        char **persons_mobile, byte persons_mobile_size,
        char **persons_email, byte persons_email_size);


//  Send the LOG to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
CZMQ_EXPORT int
    zproto_example_send_log (void *output,
        uint16_t sequence,
        byte level,
        byte event,
        uint16_t node,
        uint16_t peer,
        uint64_t time,
        const char *host,
        const char *data);
    
//  Send the STRUCTURES to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
CZMQ_EXPORT int
    zproto_example_send_structures (void *output,
        uint16_t sequence,
        zlist_t *aliases,
        zhash_t *headers);
    
//  Send the BINARY to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
CZMQ_EXPORT int
    zproto_example_send_binary (void *output,
        uint16_t sequence,
        byte *flags,
        zchunk_t *public_key,
        zuuid_t *identifier,
        zframe_t *address,
        zmsg_t *content);
    
//  Send the TYPES to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
CZMQ_EXPORT int
    zproto_example_send_types (void *output,
        uint16_t sequence,
        const char *client_forename,
        const char *client_surname,
        const char *client_mobile,
        const char *client_email,
        const char *supplier_forename,
        const char *supplier_surname,
        const char *supplier_mobile,
        const char *supplier_email);
    
//  Send the REPEAT to the output in one step
//  WARNING, this call will fail if output is of type ZMQ_ROUTER.
CZMQ_EXPORT int
    zproto_example_send_repeat (void *output,
        uint16_t sequence,
        byte no1 [3], byte no1_size,
        uint16_t no2 [144], byte no2_size,
        uint32_t no4 [256], byte no4_size,
        uint64_t no8 [256], byte no8_size,
        char **str, byte str_size,
        char **lstr, byte lstr_size,
        zlist_t **strs, byte strs_size,
        zchunk_t **chunks, byte chunks_size,
        zuuid_t **uuids, byte uuids_size,
        char **persons_forename, byte persons_forename_size,
        char **persons_surname, byte persons_surname_size,
        char **persons_mobile, byte persons_mobile_size,
        char **persons_email, byte persons_email_size);
    
//  Duplicate the zproto_example message
CZMQ_EXPORT zproto_example_t *
    zproto_example_dup (zproto_example_t *self);

//  Print contents of message to stdout
CZMQ_EXPORT void
    zproto_example_print (zproto_example_t *self);

//  Get/set the message routing id
CZMQ_EXPORT zframe_t *
    zproto_example_routing_id (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_routing_id (zproto_example_t *self, zframe_t *routing_id);

//  Get the zproto_example id and printable command
CZMQ_EXPORT int
    zproto_example_id (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_id (zproto_example_t *self, int id);
CZMQ_EXPORT const char *
    zproto_example_command (zproto_example_t *self);

//  Get/set the sequence field
CZMQ_EXPORT uint16_t
    zproto_example_sequence (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_sequence (zproto_example_t *self, uint16_t sequence);

//  Get/set the level field
CZMQ_EXPORT byte
    zproto_example_level (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_level (zproto_example_t *self, byte level);

//  Get/set the event field
CZMQ_EXPORT byte
    zproto_example_event (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_event (zproto_example_t *self, byte event);

//  Get/set the node field
CZMQ_EXPORT uint16_t
    zproto_example_node (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_node (zproto_example_t *self, uint16_t node);

//  Get/set the peer field
CZMQ_EXPORT uint16_t
    zproto_example_peer (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_peer (zproto_example_t *self, uint16_t peer);

//  Get/set the time field
CZMQ_EXPORT uint64_t
    zproto_example_time (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_time (zproto_example_t *self, uint64_t time);

//  Get/set the host field
CZMQ_EXPORT const char *
    zproto_example_host (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_host (zproto_example_t *self, const char *format, ...);

//  Get/set the data field
CZMQ_EXPORT const char *
    zproto_example_data (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_data (zproto_example_t *self, const char *format, ...);

//  Get/set the aliases field
CZMQ_EXPORT zlist_t *
    zproto_example_aliases (zproto_example_t *self);
//  Get the aliases field and transfer ownership to caller
CZMQ_EXPORT zlist_t *
    zproto_example_get_aliases (zproto_example_t *self);
//  Set the aliases field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_aliases (zproto_example_t *self, zlist_t **aliases_p);

//  Iterate through the aliases field, and append a aliases value
CZMQ_EXPORT const char *
    zproto_example_aliases_first (zproto_example_t *self);
CZMQ_EXPORT const char *
    zproto_example_aliases_next (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_aliases_append (zproto_example_t *self, const char *format, ...);
CZMQ_EXPORT size_t
    zproto_example_aliases_size (zproto_example_t *self);

//  Get/set the headers field
CZMQ_EXPORT zhash_t *
    zproto_example_headers (zproto_example_t *self);
//  Get the headers field and transfer ownership to caller
CZMQ_EXPORT zhash_t *
    zproto_example_get_headers (zproto_example_t *self);
//  Set the headers field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_headers (zproto_example_t *self, zhash_t **headers_p);
    
//  Get/set a value in the headers dictionary
CZMQ_EXPORT const char *
    zproto_example_headers_string (zproto_example_t *self,
        const char *key, const char *default_value);
CZMQ_EXPORT uint64_t
    zproto_example_headers_number (zproto_example_t *self,
        const char *key, uint64_t default_value);
CZMQ_EXPORT void
    zproto_example_headers_insert (zproto_example_t *self,
        const char *key, const char *format, ...);
CZMQ_EXPORT size_t
    zproto_example_headers_size (zproto_example_t *self);

//  Get/set the flags field
CZMQ_EXPORT byte *
    zproto_example_flags (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_flags (zproto_example_t *self, byte *flags);

//  Get a copy of the public_key field
CZMQ_EXPORT zchunk_t *
    zproto_example_public_key (zproto_example_t *self);
//  Get the public_key field and transfer ownership to caller
CZMQ_EXPORT zchunk_t *
    zproto_example_get_public_key (zproto_example_t *self);
//  Set the public_key field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_public_key (zproto_example_t *self, zchunk_t **chunk_p);

//  Get a copy of the identifier field
CZMQ_EXPORT zuuid_t *
    zproto_example_identifier (zproto_example_t *self);
//  Get the identifier field and transfer ownership to caller
CZMQ_EXPORT zuuid_t *
    zproto_example_get_identifier (zproto_example_t *self);
//  Set the identifier field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_identifier (zproto_example_t *self, zuuid_t **uuid_p);

//  Get a copy of the address field
CZMQ_EXPORT zframe_t *
    zproto_example_address (zproto_example_t *self);
//  Get the address field and transfer ownership to caller
CZMQ_EXPORT zframe_t *
    zproto_example_get_address (zproto_example_t *self);
//  Set the address field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_address (zproto_example_t *self, zframe_t **frame_p);

//  Get a copy of the content field
CZMQ_EXPORT zmsg_t *
    zproto_example_content (zproto_example_t *self);
//  Get the content field and transfer ownership to caller
CZMQ_EXPORT zmsg_t *
    zproto_example_get_content (zproto_example_t *self);
//  Set the content field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_content (zproto_example_t *self, zmsg_t **msg_p);

//  Get/set the client_forename field
CZMQ_EXPORT const char *
    zproto_example_client_forename (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_client_forename (zproto_example_t *self, const char *format, ...);

//  Get/set the client_surname field
CZMQ_EXPORT const char *
    zproto_example_client_surname (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_client_surname (zproto_example_t *self, const char *format, ...);

//  Get/set the client_mobile field
CZMQ_EXPORT const char *
    zproto_example_client_mobile (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_client_mobile (zproto_example_t *self, const char *format, ...);

//  Get/set the client_email field
CZMQ_EXPORT const char *
    zproto_example_client_email (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_client_email (zproto_example_t *self, const char *format, ...);

//  Get/set the supplier_forename field
CZMQ_EXPORT const char *
    zproto_example_supplier_forename (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_supplier_forename (zproto_example_t *self, const char *format, ...);

//  Get/set the supplier_surname field
CZMQ_EXPORT const char *
    zproto_example_supplier_surname (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_supplier_surname (zproto_example_t *self, const char *format, ...);

//  Get/set the supplier_mobile field
CZMQ_EXPORT const char *
    zproto_example_supplier_mobile (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_supplier_mobile (zproto_example_t *self, const char *format, ...);

//  Get/set the supplier_email field
CZMQ_EXPORT const char *
    zproto_example_supplier_email (zproto_example_t *self);
CZMQ_EXPORT void
    zproto_example_set_supplier_email (zproto_example_t *self, const char *format, ...);

//  Get/set the no1 field
CZMQ_EXPORT byte
    zproto_example_no1_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_no1 (zproto_example_t *self, byte no1 [3], byte size);

//  Get/set the no2 field
CZMQ_EXPORT uint16_t
    zproto_example_no2_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_no2 (zproto_example_t *self, uint16_t no2 [144], byte size);

//  Get/set the no4 field
CZMQ_EXPORT uint32_t
    zproto_example_no4_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_no4 (zproto_example_t *self, uint32_t no4 [256], byte size);

//  Get/set the no8 field
CZMQ_EXPORT uint64_t
    zproto_example_no8_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_no8 (zproto_example_t *self, uint64_t no8 [256], byte size);

//  Get/set the str field
CZMQ_EXPORT const char *
    zproto_example_str_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_str (zproto_example_t *self, char **, byte size);

//  Get/set the lstr field
CZMQ_EXPORT const char *
    zproto_example_lstr_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_lstr (zproto_example_t *self, char **, byte size);

//  Get/set the strs field
CZMQ_EXPORT zlist_t *
    zproto_example_strs_index (zproto_example_t *self, byte index);
//  Get the strs field and transfer ownership to caller
CZMQ_EXPORT void
    zproto_example_set_strs (zproto_example_t *self, zlist_t **strs, byte size);

//  Get a copy of the chunks field
CZMQ_EXPORT zchunk_t *
    zproto_example_chunks_index (zproto_example_t *self, byte index);
//  Set the chunks field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_chunks (zproto_example_t *self, zchunk_t **chunk, byte size);

//  Get a copy of the uuids field
CZMQ_EXPORT zuuid_t *
    zproto_example_uuids_index (zproto_example_t *self, byte index);
//  Set the uuids field, transferring ownership from caller
CZMQ_EXPORT void
    zproto_example_set_uuids (zproto_example_t *self, zuuid_t **uuid, byte size);

//  Get/set the persons_forename field
CZMQ_EXPORT const char *
    zproto_example_persons_forename_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_persons_forename (zproto_example_t *self, char **, byte size);

//  Get/set the persons_surname field
CZMQ_EXPORT const char *
    zproto_example_persons_surname_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_persons_surname (zproto_example_t *self, char **, byte size);

//  Get/set the persons_mobile field
CZMQ_EXPORT const char *
    zproto_example_persons_mobile_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_persons_mobile (zproto_example_t *self, char **, byte size);

//  Get/set the persons_email field
CZMQ_EXPORT const char *
    zproto_example_persons_email_index (zproto_example_t *self, byte index);
CZMQ_EXPORT void
    zproto_example_set_persons_email (zproto_example_t *self, char **, byte size);

//  Self test of this class
CZMQ_EXPORT int
    zproto_example_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
