#!/usr/bin/python
#  =========================================================================
#  ZprotoExample - zproto example protocol
#
#  ** WARNING *************************************************************
#  THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
#  your changes at the next build cycle. This is great for temporary printf
#  statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
#  for commits are:
#
#  * The XML model used for this code generation: zproto_example.xml
#  * The code generation script that built this file: zproto_codec_c
#  ************************************************************************
"""
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
"""

#  These are the ZprotoExample messages:
#
#   LOG - Log an event.
#       sequence            number 2            # 
#       version             number 2            # Version
#       level               number 1            # Log severity level
#       event               number 1            # Type of event
#       node                number 2            # Sending node
#       peer                number 2            # Refers to this peer
#       time                number 8            # Log date/time
#       host                string              # Originating hostname
#       data                longstr             # Actual log message
#
#   STRUCTURES - This message contains a list and a hash.
#       sequence            number 2            # 
#       aliases             strings             # List of strings
#       headers             hash                # Other random properties
#
#   BINARY - Deliver a multi-part message.
#       sequence            number 2            # 
#       flags               octets [4]          # A set of flags
#       public_key          chunk               # Our public key
#       identifier          uuid                # Unique identity
#       address             frame               # Return address as frame
#       content             msg                 # Message to be delivered
#
#   TYPES - Demonstrate custom-defined types
#       sequence            number 2            # 
#       client_forename     string              # Given name
#       client_surname      string              # Family name
#       client_mobile       string              # Mobile phone number
#       client_email        string              # Email address
#       supplier_forename   string              # Given name
#       supplier_surname    string              # Family name
#       supplier_mobile     string              # Mobile phone number
#       supplier_email      string              # Email address


import struct
import uuid
import zmq
import logging

logger = logging.getLogger(__name__)

ZFrame = zmq.Frame

class ZprotoExample(object):

    ZPROTO_EXAMPLE_VERSION          = 1

    LOG                             = 1
    STRUCTURES                      = 2
    BINARY                          = 3
    TYPES                           = 4
    FLAGS_SIZE                      = 4

    def __init__(self, id=None, *args, **kwargs):
        #  Structure of our class
        self.routingId = None               # Routing_id from ROUTER, if any
        self._id = id                       # ZprotoExample message ID
        self._needle = 0                    # Read/write pointer for serialization
        self.struct_data = b''              # holds the binary data

        self._sequence = 0
        self._version = 0
        self._level = 0
        self._event = 0
        self._node = 0
        self._peer = 0
        self._time = 0
        self._host = ""
        self._data = ""
        self._aliases = []
        self._headers = {}
        self._flags = b''
        self._public_key = b''
        self._identifier = uuid.uuid4()
        self._address = None # ZTypes are not implemented
        #self._address = ZFrame()
        self._content = None # ZTypes are not implemented
        #self._content = ZMsg()
        self._client_forename = ""
        self._client_surname = ""
        self._client_mobile = ""
        self._client_email = ""
        self._supplier_forename = ""
        self._supplier_surname = ""
        self._supplier_mobile = ""
        self._supplier_email = ""

    #  --------------------------------------------------------------------------
    #  Network data encoding macros

    #  Put a 1-byte number to the frame
    def _put_number1(self, nr):
        d = struct.pack('>b', nr)
        self.struct_data += d
        print(self.struct_data)

    #  Get a 1-byte number to the frame
    #  then make it unsigned
    def _get_number1(self):
        num = struct.unpack_from('>b', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>b')
        return num[0]

    #  Put a 2-byte number to the frame
    def _put_number2(self, nr):
        d = struct.pack('>H', nr)
        self.struct_data += d
        print(self.struct_data)

    # Get a 2-byte number from the frame
    def _get_number2(self):
        num = struct.unpack_from('>H', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>H')
        return num[0]

    #  Put a 4-byte number to the frame
    def _put_number4(self, nr):
        d = struct.pack('>I', nr)
        self.struct_data += d
        print(self.struct_data)

    #  Get a 4-byte number to the frame
    def _get_number4(self):
        num = struct.unpack_from('>I', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>I')
        return num[0]

    #  Put a 8-byte number to the frame
    def _put_number8(self, nr):
        d = struct.pack('>Q', nr)
        self.struct_data += d   
        print(self.struct_data)

    #  Get a 8-byte number to the frame
    def _get_number8(self):
        num = struct.unpack_from('>Q', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>Q')
        return num[0]

    #  Put a string to the frame
    def _put_string(self, s):
        self._put_number1(len(s))
        d = struct.pack('%is' % len(s), s.encode('UTF-8'))
        self.struct_data += d
        print(self.struct_data)

    #  Get a string from the frame
    def _get_string(self):
        s_len = self._get_number1()
        s = struct.unpack_from(str(s_len) + 's', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('s' * s_len)
        return s[0].decode('UTF-8')

    #  Put a long string to the frame
    def _put_long_string(self, s):
        self._put_number4(len(s))
        d = struct.pack('%is' % len(s), s.encode('UTF-8'))
        self.struct_data += d
        print(self.struct_data)

    #  Get a long string from the frame
    def _get_long_string(self):
        s_len = self._get_number4()
        s = struct.unpack_from(str(s_len) + 's', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('s' * s_len)
        return s[0].decode('UTF-8')
    
    #  Put bytes to the frame
    def _put_bytes(self, b):
        self.struct_data += b
        print(self.struct_data)

    #  Get bytes from the frame
    def _get_bytes(self, size):
        s =  self.struct_data[self._needle:self._needle+size]
        self._needle += size
        return s
    

# Skipped recv from zmsg
# Skipped senf to zmsg

    #  --------------------------------------------------------------------------
    #  Receive a ZprotoExample from the socket. Returns new object or
    #  null if error. Will block if there's no message waiting.
    def recv (self, insocket):
        frames = insocket.recv_multipart()
        if insocket.type == zmq.ROUTER:
            self.routing_id = frames.pop(0)

        self.struct_data = frames.pop(0)
        logger.debug("recv data: {0}".format(self.struct_data))
        if not self.struct_data:
            logger.debug("Malformed msg")        
            return

        # Get and check protocol signature
        self._needle = 0        
        self._ceiling = len(self.struct_data)

# TODO what is zdigest?
        signature = self._get_number2()
        if signature != (0xAAA0 | 0): 
            logger.debug("Invalid signature {0}".format(signature))
            return
   
        self._id = self._get_number1()

        if self._id == ZprotoExample.LOG:
            self._sequence = self._get_number2()
            self._version = self._get_number2()
            if self._version != 3:
                logger.debug("Value {0} != {1}".format(self._version, 3))
            self._level = self._get_number1()
            self._event = self._get_number1()
            self._node = self._get_number2()
            self._peer = self._get_number2()
            self._time = self._get_number8()
            self._host = self._get_string()
            self._data = self._get_long_string()
        elif self._id == ZprotoExample.STRUCTURES:
            self._sequence = self._get_number2()
            list_size = self._get_number4()
            self._aliases = []
            for x in range(list_size):
                self._aliases.append(self._get_long_string());
            hash_size = self._get_number4()
            self._headers = {}
            for x in range(hash_size):
                key = self._get_string()
                val = self._get_long_string()
                self._headers.update({key: val})
        elif self._id == ZprotoExample.BINARY:
            self._sequence = self._get_number2()
            self._flags = self._get_bytes(4)
            self._public_key = self._get_bytes(self.getNumber4());
            uuid_bytes = self._get_bytes(16)
            self._identifier = uuid.UUID(bytes=uuid_bytes)
            # TODO  Get next frame, leave current untouched
            pass
            #if (!input.hasReceiveMore ())
            #    throw new IllegalArgumentException ();
            #self.address = ZFrame.recvFrame (input);
            #TODO self.content = new ZMsg();
            pass
            #if (input.hasReceiveMore ())
            #    self.content.add(ZFrame.recvFrame (input));
        elif self._id == ZprotoExample.TYPES:
            self._sequence = self._get_number2()
            self._client_forename = self._get_string()
            self._client_surname = self._get_string()
            self._client_mobile = self._get_string()
            self._client_email = self._get_string()
            self._supplier_forename = self._get_string()
            self._supplier_surname = self._get_string()
            self._supplier_mobile = self._get_string()
            self._supplier_email = self._get_string()
        else:
            logger.debug("bad message ID")


    #  --------------------------------------------------------------------------
    #  Send the ZprotoExample to the socket, and destroy it
    def send(self, outsocket):
        if outsocket.socket_type == zmq.ROUTER:
            outsocket.send(self.routing_id, zmq.SNDMORE)
        # TDOD: We could generalize to a ZMsg class? ZMsg msg = new ZMsg();

        self.struct_data = b''
        self._needle = 0

        # add signature
        self._put_number2(0xAAA0 | 0)
        self._put_number1(self._id)

        if self._id == ZprotoExample.LOG:
            #  sequence is a 2-byte integer
            self._put_number2(self._sequence)
            #  version is a 2-byte integer
            self._put_number2(3)
            #  level is a 1-byte integer
            self._put_number1(self._level)
            #  event is a 1-byte integer
            self._put_number1(self._event)
            #  node is a 2-byte integer
            self._put_number2(self._node)
            #  peer is a 2-byte integer
            self._put_number2(self._peer)
            #  time is a 8-byte integer
            self._put_number8(self._time)
            if self._host != None:
                self._put_string(self._host)
            else:
                self._put_number1(0)      #  Empty string
            if self._data != None:
                self._put_long_string(self._data)
            else:
                self._put_number4(0)      #  Empty string

        elif self._id == ZprotoExample.STRUCTURES:
            #  sequence is a 2-byte integer
            self._put_number2(self._sequence)
            if self._aliases != None:
                self._put_number4(len(self._aliases))
                for val in self._aliases:
                    self._put_long_string(val)
            else:
                self._put_number4(0);      #  Empty string array
            if self._headers != None:
                self._put_number4(len(self._headers))
                for key, val in self._headers.items():
                    self._put_string(key)
                    self._put_long_string(val)
            else:
                self._put_number4(0)      #  Empty hash

        elif self._id == ZprotoExample.BINARY:
            #  sequence is a 2-byte integer
            self._put_number2(self._sequence)
            # TODO flags is a block of 4 bytes
            self._put_bytes(self._flags)
            # TODO
            if self._public_key != None:
                self._put_number4(len(self._public_key))
                self._put_bytes(self._public_key)
            else:
                self._put_number4(0)
            if self._identifier != None:
                self._put_bytes(self._identifier.bytes)
            else:
                self._put_chunk(b'0'*16)    #  Empty Chunk
            # TODO frame
            # TODO msg

        elif self._id == ZprotoExample.TYPES:
            #  sequence is a 2-byte integer
            self._put_number2(self._sequence)
            if self._client_forename != None:
                self._put_string(self._client_forename)
            else:
                self._put_number1(0)      #  Empty string
            if self._client_surname != None:
                self._put_string(self._client_surname)
            else:
                self._put_number1(0)      #  Empty string
            if self._client_mobile != None:
                self._put_string(self._client_mobile)
            else:
                self._put_number1(0)      #  Empty string
            if self._client_email != None:
                self._put_string(self._client_email)
            else:
                self._put_number1(0)      #  Empty string
            if self._supplier_forename != None:
                self._put_string(self._supplier_forename)
            else:
                self._put_number1(0)      #  Empty string
            if self._supplier_surname != None:
                self._put_string(self._supplier_surname)
            else:
                self._put_number1(0)      #  Empty string
            if self._supplier_mobile != None:
                self._put_string(self._supplier_mobile)
            else:
                self._put_number1(0)      #  Empty string
            if self._supplier_email != None:
                self._put_string(self._supplier_email)
            else:
                self._put_number1(0)      #  Empty string


        #  Now send the data frame
        outsocket.send(self.struct_data)
        # TODO: Now send any frame fields, in order
        #if (self._id == ZPROTO_EXAMPLE.BINARY):
        #  If address isn't set, send an empty frame
        #if (self._address)
        #    zframe_send (&self->address, output, ZFRAME_REUSE + (--nbr_frames? ZFRAME_MORE: 0));
        #else
        #    zmq_send (zsock_resolve (output), NULL, 0, (--nbr_frames? ZMQ_SNDMORE: 0));


    #  --------------------------------------------------------------------------
    #  Print contents of message to stdout

    def dump(self):
        if self.id == Zprotoexample.LOG:
            logger.info("LOG:")
            logger.info("    sequence=%d" %sequence)
            logger.info("    version=3")
            logger.info("    level=%d" %level)
            logger.info("    event=%d" %event)
            logger.info("    node=%d" %node)
            logger.info("    peer=%d" %peer)
            logger.info("    time=%d" %time)
            if self._host != None:
                logger.info("    host='%s'\n" %self._host)
            else:
                logger.info("    host=")
            if self._data != None:
                logger.info("    data='%s'\n" %self._data)
            else:
                logger.info("    data=")
        else:
            logger.info("(NULL)")

        if self.id == Zprotoexample.STRUCTURES:
            logger.info("STRUCTURES:")
            logger.info("    sequence=%d" %sequence)
            logger.info("    aliases={0}".format(self._aliases))
            logger.info("    headers={0}".format(self._headers))
        else:
            logger.info("(NULL)")

        if self.id == Zprotoexample.BINARY:
            logger.info("BINARY:")
            logger.info("    sequence=%d" %sequence)
            logger.info("    flags={0}".format(self._flags))
            logger.info("    public_key={0}".format(self._public_key))
            logger.info("    identifier={0}".format(self._identifier))
            logger.info("    address={0}".format(self._address))
            logger.info("    content={0}".format(self._content))
        else:
            logger.info("(NULL)")

        if self.id == Zprotoexample.TYPES:
            logger.info("TYPES:")
            logger.info("    sequence=%d" %sequence)
            if self._client_forename != None:
                logger.info("    client_forename='%s'\n" %self._client_forename)
            else:
                logger.info("    client_forename=")
            if self._client_surname != None:
                logger.info("    client_surname='%s'\n" %self._client_surname)
            else:
                logger.info("    client_surname=")
            if self._client_mobile != None:
                logger.info("    client_mobile='%s'\n" %self._client_mobile)
            else:
                logger.info("    client_mobile=")
            if self._client_email != None:
                logger.info("    client_email='%s'\n" %self._client_email)
            else:
                logger.info("    client_email=")
            if self._supplier_forename != None:
                logger.info("    supplier_forename='%s'\n" %self._supplier_forename)
            else:
                logger.info("    supplier_forename=")
            if self._supplier_surname != None:
                logger.info("    supplier_surname='%s'\n" %self._supplier_surname)
            else:
                logger.info("    supplier_surname=")
            if self._supplier_mobile != None:
                logger.info("    supplier_mobile='%s'\n" %self._supplier_mobile)
            else:
                logger.info("    supplier_mobile=")
            if self._supplier_email != None:
                logger.info("    supplier_email='%s'\n" %self._supplier_email)
            else:
                logger.info("    supplier_email=")
        else:
            logger.info("(NULL)")


    #  --------------------------------------------------------------------------
    #  Get/set the message routing id

    def routing_id(self):
        return self._routing_id

    def set_routing_id(self, routing_id):
        self._routing_id = routing_id

    #  --------------------------------------------------------------------------
    #  Get/set the zproto_example id

    def id():
        return self._id

    def set_id(self, id):
        self._id = id

    #  --------------------------------------------------------------------------
    #  Return a printable command string
    def command(self):
        if self._id == ZprotoExample.LOG:
            return "LOG"
        if self._id == ZprotoExample.STRUCTURES:
            return "STRUCTURES"
        if self._id == ZprotoExample.BINARY:
            return "BINARY"
        if self._id == ZprotoExample.TYPES:
            return "TYPES"
        return "?";

    #  --------------------------------------------------------------------------
    #  Get/set the sequence field

    def sequence(self):
        return self._sequence;

    def set_sequence(self, sequence):
        self._sequence = sequence

    #  --------------------------------------------------------------------------
    #  Get/set the level field

    def level(self):
        return self._level;

    def set_level(self, level):
        self._level = level

    #  --------------------------------------------------------------------------
    #  Get/set the event field

    def event(self):
        return self._event;

    def set_event(self, event):
        self._event = event

    #  --------------------------------------------------------------------------
    #  Get/set the node field

    def node(self):
        return self._node;

    def set_node(self, node):
        self._node = node

    #  --------------------------------------------------------------------------
    #  Get/set the peer field

    def peer(self):
        return self._peer;

    def set_peer(self, peer):
        self._peer = peer

    #  --------------------------------------------------------------------------
    #  Get/set the time field

    def time(self):
        return self._time;

    def set_time(self, time):
        self._time = time

    #  --------------------------------------------------------------------------
    #  Get/set the host field

    def host(self):
        return self._host;

    def set_host(self, host):
        self._host = host

    #  --------------------------------------------------------------------------
    #  Get/set the data field

    def data(self):
        return self._data;

    def set_data(self, data):
        self._data = data

    #  --------------------------------------------------------------------------
    #  Get the aliases field

    def aliases(self):
        return self._aliases

    def get_aliases(self):
        return self._aliases

    def set_aliases(self, aliases):
        self._aliases = aliases

    #  --------------------------------------------------------------------------
    #  Get the headers field

    def headers(self):
        return self._headers

    def get_headers(self):
        return self._headers

    def set_headers(self, headers):
        self._headers = headers

    #  --------------------------------------------------------------------------
    #  Get/set the flags field

    def flags(self):
        return self._flags;

    def set_flags(self, flags):
        self._flags = flags

    #  --------------------------------------------------------------------------
    #  Get the public_key field

    def public_key(self):
        return self._public_key

    def get_public_key(self):
        return self._public_key

    def set_public_key(self, public_key):
        self._public_key = public_key
    #  --------------------------------------------------------------------------
    #  Get the identifier field

    def identifier(self):
        return self._identifier

    def get_identifier(self):
        return self._identifier

    def set_identifier(self, identifier):
        self._identifier = identifier
    #  --------------------------------------------------------------------------
    #  Get the address field

    def address(self):
        return self._address

    def get_address(self):
        return self._address

    def set_address(self, address):
        self._address = address
    #  --------------------------------------------------------------------------
    #  Get the content field

    def content(self):
        return self._content

    def get_content(self):
        return self._content

    def set_content(self, content):
        self._content = content
    #  --------------------------------------------------------------------------
    #  Get/set the client_forename field

    def client_forename(self):
        return self._client_forename;

    def set_client_forename(self, client_forename):
        self._client_forename = client_forename

    #  --------------------------------------------------------------------------
    #  Get/set the client_surname field

    def client_surname(self):
        return self._client_surname;

    def set_client_surname(self, client_surname):
        self._client_surname = client_surname

    #  --------------------------------------------------------------------------
    #  Get/set the client_mobile field

    def client_mobile(self):
        return self._client_mobile;

    def set_client_mobile(self, client_mobile):
        self._client_mobile = client_mobile

    #  --------------------------------------------------------------------------
    #  Get/set the client_email field

    def client_email(self):
        return self._client_email;

    def set_client_email(self, client_email):
        self._client_email = client_email

    #  --------------------------------------------------------------------------
    #  Get/set the supplier_forename field

    def supplier_forename(self):
        return self._supplier_forename;

    def set_supplier_forename(self, supplier_forename):
        self._supplier_forename = supplier_forename

    #  --------------------------------------------------------------------------
    #  Get/set the supplier_surname field

    def supplier_surname(self):
        return self._supplier_surname;

    def set_supplier_surname(self, supplier_surname):
        self._supplier_surname = supplier_surname

    #  --------------------------------------------------------------------------
    #  Get/set the supplier_mobile field

    def supplier_mobile(self):
        return self._supplier_mobile;

    def set_supplier_mobile(self, supplier_mobile):
        self._supplier_mobile = supplier_mobile

    #  --------------------------------------------------------------------------
    #  Get/set the supplier_email field

    def supplier_email(self):
        return self._supplier_email;

    def set_supplier_email(self, supplier_email):
        self._supplier_email = supplier_email


