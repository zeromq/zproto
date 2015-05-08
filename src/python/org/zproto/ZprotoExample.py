/*  =========================================================================
    ZprotoExample - zproto example protocol

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

/*  These are the ZprotoExample messages:

    LOG - Log an event.
        sequence            number 2    
        version             number 2    
        Version
    
        level               number 1    
        Log severity level
        
        event               number 1    
        Type of event
        
        node                number 2    
        Sending node
        
        peer                number 2    
        Refers to this peer
        
        time                number 8    
        Log date/time
        
        host                string      
        Originating hostname
        
        data                longstr     
        Actual log message
        

    STRUCTURES - This message contains a list and a hash.
        sequence            number 2    
        aliases             strings     
        List of strings
        
        headers             hash        
        Other random properties
        

    BINARY - Deliver a multi-part message.
        sequence            number 2    
        flags               octets [4]  
        A set of flags
        
        public_key          chunk       
        Our public key
        
        identifier          uuid        
        Unique identity
        
        address             frame       
        Return address as frame
        
        content             msg         
        Message to be delivered
        

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

package org.zproto;

#import java.util.*;
#import java.nio.ByteBuffer;

#import org.zeromq.ZFrame;
#import org.zeromq.ZMsg;
#import org.zeromq.ZMQ;
#import org.zeromq.ZMQ.Socket;
import zmq

class ZprotoExample(Object):

    ZPROTO_EXAMPLE_VERSION          = 1

    LOG                   = 1
    STRUCTURES            = 2
    BINARY                = 3
    TYPES                 = 4
    FLAGS_SIZE            = 4

    def __init__(self, id, *args, **kwargs):
        #  Structure of our class
        self.routingId = None               # Routing_id from ROUTER, if any
        self.id = id                        # ZprotoExample message ID
        self._needle = 0                    # Read/write pointer for serialization
        self.struct_data = b''

    private int sequence;
    private int version;
    private int level;
    private int event;
    private int node;
    private int peer;
    private long time;
    private String host;
    private String data;
    private List <String> aliases;
    private Map <String, String> headers;
    private int headersBytes;
    private byte [] flags = new byte [4];
    private byte[] public_key;
    private UUID identifier;
    private ZFrame address;
    private ZMsg content;
    private String client_forename;
    private String client_surname;
    private String client_mobile;
    private String client_email;
    private String supplier_forename;
    private String supplier_surname;
    private String supplier_mobile;
    private String supplier_email;

    def __del__()
    {
        close();
    }

    @Override
    public void close()
    {
        //  Destroy frame fields
        if (address != null)
            address.destroy();
        address = null;
    }
    #  --------------------------------------------------------------------------
    #  Network data encoding macros


    #  Put a 1-byte number to the frame
    def _put_number1(self, nr):
        d = struct.pack('>b', nr)
        self.struct_data += d

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

    # Get a 2-byte number from the frame
    def _get_number2(self):
        num = struct.unpack_from('>H', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>H')
        return num[0]

    #  Put a 4-byte number to the frame
    def _put_number4(self, nr):
        d = struct.pack('>I', nr)
        self.struct_data += d

    #  Get a 4-byte number to the frame
    def _get_number4(self):
        num = struct.unpack_from('>I', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>I')
        return num[0]

    #  Put a 8-byte number to the frame
    def _put_number8(self, nr):
        d = struct.pack('>Q', nr)
        self.struct_data += d   

    #  Get a 8-byte number to the frame
    def _get_number8(self):
        num = struct.unpack_from('>Q', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('>Q')
        return num[0]

    //  Put a block to the frame
    private void putBlock (byte [] value, int size)
    {
        needle.put (value, 0, size);
    }

    private byte [] getBlock (int size)
    {
        byte [] value = new byte [size];
        needle.get (value);

        return value;
    }

    #  Put a string to the frame
    def _put_string(self, s):
        self._put_number1(len(s))
        d = struct.pack('%is' % len(s), s.encode('UTF-8'))
        self.struct_data += d

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

    #  Get a string from the frame
    def _get_long_string(self):
        s_len = self._get_number4()
        s = struct.unpack_from(str(s_len) + 's', self.struct_data, offset=self._needle)
        self._needle += struct.calcsize('s' * s_len)
        return s[0].decode('UTF-8')

    #  --------------------------------------------------------------------------
    #  Receive and parse a ZprotoExample from the socket. Returns new object or
    #  null if error. Will block if there's no message waiting.
    def recv (insocket):
    {
        frames = insocket.recv_multipart()
        if insocket.type == zmq.ROUTER:
            self.routing_id = frames.pop(0)

        self.struct_data = frames.pop(0)
        logger.debug("recv data: {0}".format(self.struct_data))
        if not self.struct_data:
            logger.debug("Malformed msg")        
            return

        # reset needle
        self._needle = 0        
        self._ceiling = len(self.struct_data)

        signature = self._get_number2()
        if signature != (0xAAA0 | 0): 
            logger.debug("Invalid signature {0}".format(signature))
            return

        
        self.id = self._get_number1()
        version = self._get_number1()

        if self.id == ZPROTO_EXAMPLE.LOG:
                self.sequence = self._get_number2()
                self.version = self._get_number2()
                if self.version != 3
                    #??? raise IllegalArgumentException()
                    logger.debug("Value {0} != {1}".format(self.version, 3))
                self.level = self._get_number1()
                self.event = self._get_number1()
                self.node = self._get_number2()
                self.peer = self._get_number2()
                self.time = self._get_number8()
                self.host = self._get_string()
                self.data = self._get_long_string ();
                break;
        if self.id == ZPROTO_EXAMPLE.STRUCTURES:
                self.sequence = self._get_number2()
                list_size = self._get_number4()
                self.aliases = []
                for x in range(list_size):
                    self.aliases.append(self._get_long_string());
                hash_size = self.get_number4()
                self.headers = {}
                for x in range(hash_size):
                    key = self._get_string()
                    val = self._get_long_string()
                    self.headers.update({key: val})
                break;
        if self.id == ZPROTO_EXAMPLE.BINARY:
                self.sequence = self._get_number2()
                self.flags = self.getBlock (4);
                #TODO
                self.public_key = self.getBlock((int) self.getNumber4());
                uuid_bytes = self._get_block(16)
                self.identifier = uuid.UUID(bytes=uuid_bytes)
                # TODO  Get next frame, leave current untouched
                if (!input.hasReceiveMore ())
                    throw new IllegalArgumentException ();
                self.address = ZFrame.recvFrame (input);
                #TODO self.content = new ZMsg();
                if (input.hasReceiveMore ())
                    self.content.add(ZFrame.recvFrame (input));
                break;
        if self.id == ZPROTO_EXAMPLE.TYPES:
                self.sequence = self._get_number2()
                self.client_forename = self._get_string()
                self.client_surname = self._get_string()
                self.client_mobile = self._get_string()
                self.client_email = self._get_string()
                self.supplier_forename = self._get_string()
                self.supplier_surname = self._get_string()
                self.supplier_mobile = self._get_string()
                self.supplier_email = self._get_string()
                break;
        else:
            logger.debug("bad message ID")

    #  --------------------------------------------------------------------------
    #  Send the ZprotoExample to the socket, and destroy it

    def send(self, outsocket):
        if outsocket.socket_type == zmq.ROUTER:
            outsocket.send(self.routing_id, zmq.SNDMORE)
        # We could generalize to a ZMsg class? ZMsg msg = new ZMsg();

        self.struct_data = b''
        self._needle = 0

        # add signature
        self._put_number2(0xAAA0 | 0)
        self._put_number1(self.id)

        if id == ZPROTO_EXAMPLE.LOG:
            #  sequence is a 2-byte integer
            self._put_number2(self.sequence)
            #  version is a 2-byte integer
            self._put_number2(3)
            #  level is a 1-byte integer
            self._put_number1(self.level)
            #  event is a 1-byte integer
            self._put_number1(self.event)
            #  node is a 2-byte integer
            self._put_number2(self.node)
            #  peer is a 2-byte integer
            self._put_number2(self.peer)
            #  time is a 8-byte integer
            self._put_number8(self.time)
            if host != None:
                self._put_string(host)
            else
                self._put_number1(0)      #  Empty string
            if data != None:
                self._put_long_string(data)
            else
                self._put_number4(0)      #  Empty string

        if id == ZPROTO_EXAMPLE.STRUCTURES:
            #  sequence is a 2-byte integer
            self._put_number2(self.sequence)
            if aliases != None:
                self._put_number4(len(aliases)
                for (String value : aliases) {
                    self._put_long_string(value)
                }
            }
            else
                self._put_number4(0);      #  Empty string array
            if headers != None:
                self._put_number4(len(headers)
                for key, val in headers.items():
                    self._put_string(key)
                    self._put_long_string(val)
                }
            }
            else
                self._put_number4(0)      #  Empty hash

        if id == ZPROTO_EXAMPLE.BINARY:
            #  sequence is a 2-byte integer
            self._put_number2(self.sequence)
            #  flags is a block of 4 bytes
            frameSize += 4;
              # TODO
              if public_key != None:
                  self._put_number4(len(public_key):
                  needle.put(public_key, 0, public_key.length);
              } else {
                  self._put_number4(0)
              }
              if identifier != None:
                  #TODO
                  ByteBuffer bb = ByteBuffer.wrap(new byte[16]);
                  bb.putLong(identifier.getMostSignificantBits());
                  bb.putLong(identifier.getLeastSignificantBits());
                  needle.put(bb.array());
              } else {
                  self._put_chunk(b'0'*16)    #  Empty Chunk
              }

        if id == ZPROTO_EXAMPLE.TYPES:
            #  sequence is a 2-byte integer
            self._put_number2(self.sequence)
            if client_forename != None:
                self._put_string(client_forename)
            else
                self._put_number1(0)      #  Empty string
            if client_surname != None:
                self._put_string(client_surname)
            else
                self._put_number1(0)      #  Empty string
            if client_mobile != None:
                self._put_string(client_mobile)
            else
                self._put_number1(0)      #  Empty string
            if client_email != None:
                self._put_string(client_email)
            else
                self._put_number1(0)      #  Empty string
            if supplier_forename != None:
                self._put_string(supplier_forename)
            else
                self._put_number1(0)      #  Empty string
            if supplier_surname != None:
                self._put_string(supplier_surname)
            else
                self._put_number1(0)      #  Empty string
            if supplier_mobile != None:
                self._put_string(supplier_mobile)
            else
                self._put_number1(0)      #  Empty string
            if supplier_email != None:
                self._put_string(supplier_email)
            else
                self._put_number1(0)      #  Empty string

        }
        //  Now send the data frame
        .add(frame);
        outsocket.send(self.struct_data)


//  --------------------------------------------------------------------------
//  Send the LOG to the socket in one step

    public static void sendLog (
        Socket output,
        int sequence,
        int level,
        int event,
        int node,
        int peer,
        long time,
        String host,
        String data)
    {
	sendLog (
		    output,
		    null,
		    sequence,
		    level,
		    event,
		    node,
		    peer,
		    time,
		    host,
		    data);
    }

//  --------------------------------------------------------------------------
//  Send the LOG to a router socket in one step

    public static void sendLog (
        Socket output,
	ZFrame routingId,
        int sequence,
        int level,
        int event,
        int node,
        int peer,
        long time,
        String host,
        String data)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.LOG);
        if (routingId != null)
        {
	        self.setRoutingId (routingId);
        }
        self.setSequence (sequence);
        self.setLevel (level);
        self.setEvent (event);
        self.setNode (node);
        self.setPeer (peer);
        self.setTime (time);
        self.setHost (host);
        self.setData (data);
        self.send (output);
    }

//  --------------------------------------------------------------------------
//  Send the STRUCTURES to the socket in one step

    public static void sendStructures (
        Socket output,
        int sequence,
        List <String> aliases,
        Map <String, String> headers)
    {
	sendStructures (
		    output,
		    null,
		    sequence,
		    aliases,
		    headers);
    }

//  --------------------------------------------------------------------------
//  Send the STRUCTURES to a router socket in one step

    public static void sendStructures (
        Socket output,
	ZFrame routingId,
        int sequence,
        List <String> aliases,
        Map <String, String> headers)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.STRUCTURES);
        if (routingId != null)
        {
	        self.setRoutingId (routingId);
        }
        self.setSequence (sequence);
        self.setAliases (new ArrayList <String> (aliases));
        self.setHeaders (new HashMap <String, String> (headers));
        self.send (output);
    }

//  --------------------------------------------------------------------------
//  Send the BINARY to the socket in one step

    public static void sendBinary (
        Socket output,
        int sequence,
        byte [] flags,
        byte[] public_key,
        UUID identifier,
        ZFrame address,
        ZMsg content)
    {
	sendBinary (
		    output,
		    null,
		    sequence,
		    flags,
		    public_key,
		    identifier,
		    address,
		    content);
    }

//  --------------------------------------------------------------------------
//  Send the BINARY to a router socket in one step

    public static void sendBinary (
        Socket output,
	ZFrame routingId,
        int sequence,
        byte [] flags,
        byte[] public_key,
        UUID identifier,
        ZFrame address,
        ZMsg content)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.BINARY);
        if (routingId != null)
        {
	        self.setRoutingId (routingId);
        }
        self.setSequence (sequence);
        self.setFlags (flags);
        self.setPublic_Key (public_key);
        self.setIdentifier (identifier);
        self.setAddress (address.duplicate ());
        self.setContent (content.duplicate ());
        self.send (output);
    }

//  --------------------------------------------------------------------------
//  Send the TYPES to the socket in one step

    public static void sendTypes (
        Socket output,
        int sequence,
        String client_forename,
        String client_surname,
        String client_mobile,
        String client_email,
        String supplier_forename,
        String supplier_surname,
        String supplier_mobile,
        String supplier_email)
    {
	sendTypes (
		    output,
		    null,
		    sequence,
		    client_forename,
		    client_surname,
		    client_mobile,
		    client_email,
		    supplier_forename,
		    supplier_surname,
		    supplier_mobile,
		    supplier_email);
    }

//  --------------------------------------------------------------------------
//  Send the TYPES to a router socket in one step

    public static void sendTypes (
        Socket output,
	ZFrame routingId,
        int sequence,
        String client_forename,
        String client_surname,
        String client_mobile,
        String client_email,
        String supplier_forename,
        String supplier_surname,
        String supplier_mobile,
        String supplier_email)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.TYPES);
        if (routingId != null)
        {
	        self.setRoutingId (routingId);
        }
        self.setSequence (sequence);
        self.setClient_Forename (client_forename);
        self.setClient_Surname (client_surname);
        self.setClient_Mobile (client_mobile);
        self.setClient_Email (client_email);
        self.setSupplier_Forename (supplier_forename);
        self.setSupplier_Surname (supplier_surname);
        self.setSupplier_Mobile (supplier_mobile);
        self.setSupplier_Email (supplier_email);
        self.send (output);
    }


    //  --------------------------------------------------------------------------
    //  Duplicate the ZprotoExample message

    public ZprotoExample dup ()
    {
        ZprotoExample copy = new ZprotoExample (this.id);
        if (this.routingId != null)
            copy.routingId = this.routingId.duplicate ();
        switch (this.id) {
        case LOG:
            copy.sequence = this.sequence;
            copy.version = this.version;
            copy.level = this.level;
            copy.event = this.event;
            copy.node = this.node;
            copy.peer = this.peer;
            copy.time = this.time;
            copy.host = this.host;
            copy.data = this.data;
        break;
        case STRUCTURES:
            copy.sequence = this.sequence;
            copy.aliases = new ArrayList <String> (this.aliases);
            copy.headers = new HashMap <String, String> (this.headers);
        break;
        case BINARY:
            copy.sequence = this.sequence;
            System.arraycopy (copy.flags, 0, this.flags, 0, 4);
            copy.public_key = this.public_key;
            copy.address = this.address.duplicate ();
        break;
        case TYPES:
            copy.sequence = this.sequence;
            copy.client_forename = this.client_forename;
            copy.client_surname = this.client_surname;
            copy.client_mobile = this.client_mobile;
            copy.client_email = this.client_email;
            copy.supplier_forename = this.supplier_forename;
            copy.supplier_surname = this.supplier_surname;
            copy.supplier_mobile = this.supplier_mobile;
            copy.supplier_email = this.supplier_email;
        break;
        }
        return copy;
    }

    //  Dump headers key=value pair to stdout
    public static void headersDump (Map.Entry <String, String> entry, ZprotoExample self)
    {
        System.out.printf ("        %s=%s\n", entry.getKey (), entry.getValue ());
    }


    //  --------------------------------------------------------------------------
    //  Print contents of message to stdout

    public void dump ()
    {
        switch (id) {
        case LOG:
            System.out.println ("LOG:");
            System.out.printf ("    sequence=%d\n", (long)sequence);
            System.out.printf ("    version=3\n");
            System.out.printf ("    level=%d\n", (long)level);
            System.out.printf ("    event=%d\n", (long)event);
            System.out.printf ("    node=%d\n", (long)node);
            System.out.printf ("    peer=%d\n", (long)peer);
            System.out.printf ("    time=%d\n", (long)time);
            if (host != null)
                System.out.printf ("    host='%s'\n", host);
            else
                System.out.printf ("    host=\n");
            if (data != null)
                System.out.printf ("    data='%s'\n", data);
            else
                System.out.printf ("    data=\n");
            break;

        case STRUCTURES:
            System.out.println ("STRUCTURES:");
            System.out.printf ("    sequence=%d\n", (long)sequence);
            System.out.printf ("    aliases={");
            if (aliases != null) {
                for (String value : aliases) {
                    System.out.printf (" '%s'", value);
                }
            }
            System.out.printf (" }\n");
            System.out.printf ("    headers={\n");
            if (headers != null) {
                for (Map.Entry <String, String> entry : headers.entrySet ())
                    headersDump (entry, this);
            }
            System.out.printf ("    }\n");
            break;

        case BINARY:
            System.out.println ("BINARY:");
            System.out.printf ("    sequence=%d\n", (long)sequence);
            System.out.printf ("    flags=");
            int flagsIndex;
            for (flagsIndex = 0; flagsIndex < 4; flagsIndex++) {
                if (flagsIndex != 0 && (flagsIndex % 4 == 0))
                    System.out.printf ("-");
                System.out.printf ("%02X", flags [flagsIndex]);
            }
            System.out.println();
            System.out.printf ("    address={\n");
            if (address != null) {
                int size = address.size ();
                byte [] data = address.getData ();
                System.out.printf ("        size=%d\n", address.size ());
                if (size > 32)
                    size = 32;
                int addressIndex;
                for (addressIndex = 0; addressIndex < size; addressIndex++) {
                    if (addressIndex != 0 && (addressIndex % 4 == 0))
                        System.out.printf ("-");
                    System.out.printf ("%02X", data [addressIndex]);
                }
            }
            System.out.printf ("    }\n");
            break;

        case TYPES:
            System.out.println ("TYPES:");
            System.out.printf ("    sequence=%d\n", (long)sequence);
            if (client_forename != null)
                System.out.printf ("    client_forename='%s'\n", client_forename);
            else
                System.out.printf ("    client_forename=\n");
            if (client_surname != null)
                System.out.printf ("    client_surname='%s'\n", client_surname);
            else
                System.out.printf ("    client_surname=\n");
            if (client_mobile != null)
                System.out.printf ("    client_mobile='%s'\n", client_mobile);
            else
                System.out.printf ("    client_mobile=\n");
            if (client_email != null)
                System.out.printf ("    client_email='%s'\n", client_email);
            else
                System.out.printf ("    client_email=\n");
            if (supplier_forename != null)
                System.out.printf ("    supplier_forename='%s'\n", supplier_forename);
            else
                System.out.printf ("    supplier_forename=\n");
            if (supplier_surname != null)
                System.out.printf ("    supplier_surname='%s'\n", supplier_surname);
            else
                System.out.printf ("    supplier_surname=\n");
            if (supplier_mobile != null)
                System.out.printf ("    supplier_mobile='%s'\n", supplier_mobile);
            else
                System.out.printf ("    supplier_mobile=\n");
            if (supplier_email != null)
                System.out.printf ("    supplier_email='%s'\n", supplier_email);
            else
                System.out.printf ("    supplier_email=\n");
            break;

        }
    }


    //  --------------------------------------------------------------------------
    //  Get/set the message routing id

    public ZFrame routingId ()
    {
        return routingId;
    }

    public void setRoutingId (ZFrame routingId)
    {
        if (this.routingId != null)
            this.routingId.destroy ();
        this.routingId = routingId.duplicate ();
    }


    //  --------------------------------------------------------------------------
    //  Get/set the zproto_example id

    public int id ()
    {
        return id;
    }

    public void setId (int id)
    {
        this.id = id;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the sequence field

    public int sequence ()
    {
        return sequence;
    }

    public void setSequence (int sequence)
    {
        this.sequence = sequence;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the level field

    public int level ()
    {
        return level;
    }

    public void setLevel (int level)
    {
        this.level = level;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the event field

    public int event ()
    {
        return event;
    }

    public void setEvent (int event)
    {
        this.event = event;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the node field

    public int node ()
    {
        return node;
    }

    public void setNode (int node)
    {
        this.node = node;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the peer field

    public int peer ()
    {
        return peer;
    }

    public void setPeer (int peer)
    {
        this.peer = peer;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the time field

    public long time ()
    {
        return time;
    }

    public void setTime (long time)
    {
        this.time = time;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the host field

    public String host ()
    {
        return host;
    }

    public void setHost (String format, Object ... args)
    {
        //  Format into newly allocated string
        host = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the data field

    public String data ()
    {
        return data;
    }

    public void setData (String format, Object ... args)
    {
        //  Format into newly allocated string
        data = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Iterate through the aliases field, and append a aliases value

    public List <String> aliases ()
    {
        return aliases;
    }

    public void appendAliases (String format, Object ... args)
    {
        //  Format into newly allocated string

        String string = String.format (format, args);
        //  Attach string to list
        if (aliases == null)
            aliases = new ArrayList <String> ();
        aliases.add (string);
    }

    public void setAliases (List <String> value)
    {
        aliases = new ArrayList (value);
    }

    //  --------------------------------------------------------------------------
    //  Get/set a value in the headers hash

    public Map <String, String> headers ()
    {
        return headers;
    }

    public String headersString (String key, String defaultValue)
    {
        String value = null;
        if (headers != null)
            value = headers.get (key);
        if (value == null)
            value = defaultValue;

        return value;
    }

    public long headersNumber (String key, long defaultValue)
    {
        long value = defaultValue;
        String string = null;
        if (headers != null)
            string = headers.get (key);
        if (string != null)
            value = Long.valueOf (string);

        return value;
    }

    public void insertHeaders (String key, String format, Object ... args)
    {
        //  Format string into buffer
        String string = String.format (format, args);

        //  Store string in hash table
        if (headers == null)
            headers = new HashMap <String, String> ();
        headers.put (key, string);
        headersBytes += key.length () + 1 + string.length ();
    }

    public void setHeaders (Map <String, String> value)
    {
        if (value != null)
            headers = new HashMap <String, String> (value);
        else
            headers = value;
    }

    //  --------------------------------------------------------------------------
    //  Get/set the flags field

    public byte [] flags ()
    {
        return flags;
    }

    public void setFlags (byte [] flags)
    {
        System.arraycopy (flags, 0, this.flags, 0, 4);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the public_key field

    public byte[] public_key ()
    {
        return public_key;
    }

    //  Takes ownership of supplied frame
    public void setPublic_Key (byte[] public_key)
    {
        this.public_key = public_key;
    }
    //  --------------------------------------------------------------------------
    //  Get/set the identifier field

    public UUID identifier ()
    {
        return identifier;
    }

    public void setIdentifier (UUID identifier)
    {
        this.identifier = identifier;
    }
    //  --------------------------------------------------------------------------
    //  Get/set the address field

    public ZFrame address ()
    {
        return address;
    }

    //  Takes ownership of supplied frame
    public void setAddress (ZFrame frame)
    {
        if (address != null)
            address.destroy ();
        address = frame;
    }
    //  --------------------------------------------------------------------------
    //  Get/set the content field

    public ZMsg content ()
    {
        return content;
    }

    //  Takes ownership of supplied frame
    public void setContent (ZMsg frame)
    {
        if (content != null)
            content.destroy ();
        content = frame;
    }
    //  --------------------------------------------------------------------------
    //  Get/set the client_forename field

    public String client_forename ()
    {
        return client_forename;
    }

    public void setClient_Forename (String format, Object ... args)
    {
        //  Format into newly allocated string
        client_forename = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the client_surname field

    public String client_surname ()
    {
        return client_surname;
    }

    public void setClient_Surname (String format, Object ... args)
    {
        //  Format into newly allocated string
        client_surname = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the client_mobile field

    public String client_mobile ()
    {
        return client_mobile;
    }

    public void setClient_Mobile (String format, Object ... args)
    {
        //  Format into newly allocated string
        client_mobile = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the client_email field

    public String client_email ()
    {
        return client_email;
    }

    public void setClient_Email (String format, Object ... args)
    {
        //  Format into newly allocated string
        client_email = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the supplier_forename field

    public String supplier_forename ()
    {
        return supplier_forename;
    }

    public void setSupplier_Forename (String format, Object ... args)
    {
        //  Format into newly allocated string
        supplier_forename = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the supplier_surname field

    public String supplier_surname ()
    {
        return supplier_surname;
    }

    public void setSupplier_Surname (String format, Object ... args)
    {
        //  Format into newly allocated string
        supplier_surname = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the supplier_mobile field

    public String supplier_mobile ()
    {
        return supplier_mobile;
    }

    public void setSupplier_Mobile (String format, Object ... args)
    {
        //  Format into newly allocated string
        supplier_mobile = String.format (format, args);
    }

    //  --------------------------------------------------------------------------
    //  Get/set the supplier_email field

    public String supplier_email ()
    {
        return supplier_email;
    }

    public void setSupplier_Email (String format, Object ... args)
    {
        //  Format into newly allocated string
        supplier_email = String.format (format, args);
    }

}
