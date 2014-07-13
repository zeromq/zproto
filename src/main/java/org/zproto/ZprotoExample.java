/*  =========================================================================
    ZprotoExample - zproto example protocol

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

    * The XML model used for this code generation: zproto_example_java.xml
    * The code generation script that built this file: zproto_codec_java
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

/*  These are the zproto_example messages
    LOG - Log an event.
        sequence      number 2
        sequence      number 2
        version       number 2
        level         number 1
        event         number 1
        node          number 2
        peer          number 2
        time          number 8
        host          string
        data          longstr
    STRUCTURES - This message contains a list and a hash.
        sequence      number 2
        sequence      number 2
        aliases       strings
        headers       dictionary
    BINARY - Deliver a multi-part message.
        sequence      number 2
        sequence      number 2
        flags         octets [4]
        public_key    chunk
        identifier    uuid
        address       frame
        content       msg
    TYPES - Demonstrate custom-defined types
        sequence      number 2
        sequence      number 2
        client_forename  string
        client_surname  string
        client_mobile  string
        client_email  string
        supplier_forename  string
        supplier_surname  string
        supplier_mobile  string
        supplier_email  string
    REPEAT - Demonstrates repeating fields
        sequence      number 2
        sequence      number 2
        no1           number 1
        no2           number 2
        no4           number 4
        no8           number 8
        str           string
        lstr          longstr
        strs          strings
        chunks        chunk
        uuids         uuid
        persons_forename  string
        persons_surname  string
        persons_mobile  string
        persons_email  string
*/

package org.zproto;

import java.util.*;
import java.nio.ByteBuffer;

import org.zeromq.ZFrame;
import org.zeromq.ZMQ;
import org.zeromq.ZMQ.Socket;

public class ZprotoExample implements java.io.Closeable
{
    public static final int ZPROTO_EXAMPLE_VERSION          = 1;

    public static final int LOG                   = 1;
    public static final int STRUCTURES            = 2;
    public static final int BINARY                = 3;
    public static final int TYPES                 = 4;
    public static final int REPEAT                = 5;
    public static final int FLAGS_SIZE            = 4;

    //  Structure of our class
    private ZFrame address;             //  Address of peer if any
    private int id;                     //  ZprotoExample message ID
    private ByteBuffer needle;          //  Read/write pointer for serialization
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
    private ZFrame address;
    private String client_forename;
    private String client_surname;
    private String client_mobile;
    private String client_email;
    private String supplier_forename;
    private String supplier_surname;
    private String supplier_mobile;
    private String supplier_email;
    private int no1;
    private int no2;
    private long no4;
    private long no8;
    private String str;
    private String lstr;
    private List <String> strs;
    private String persons_forename;
    private String persons_surname;
    private String persons_mobile;
    private String persons_email;

    public ZprotoExample( int id )
    {
        this.id = id;
    }

    #Override
    public void close()
    {
        //  Destroy frame fields
        if (address != null)
            address.destroy();
        address = null;
    }
    //  --------------------------------------------------------------------------
    //  Network data encoding macros


    //  Put a 1-byte number to the frame
    private final void putNumber1 (int value)
    {
        needle.put ((byte) value);
    }

    //  Get a 1-byte number to the frame
    //  then make it unsigned
    private int getNumber1 ()
    {
        int value = needle.get ();
        if (value < 0)
            value = (0xff) & value;
        return value;
    }

    //  Put a 2-byte number to the frame
    private final void putNumber2 (int value)
    {
        needle.putShort ((short) value);
    }

    //  Get a 2-byte number to the frame
    private int getNumber2 ()
    {
        int value = needle.getShort ();
        if (value < 0)
            value = (0xffff) & value;
        return value;
    }

    //  Put a 4-byte number to the frame
    private final void putNumber4 (long value)
    {
        needle.putInt ((int) value);
    }

    //  Get a 4-byte number to the frame
    //  then make it unsigned
    private long getNumber4 ()
    {
        long value = needle.getInt ();
        if (value < 0)
            value = (0xffffffff) & value;
        return value;
    }

    //  Put a 8-byte number to the frame
    public void putNumber8 (long value)
    {
        needle.putLong (value);
    }

    //  Get a 8-byte number to the frame
    public long getNumber8 ()
    {
        return needle.getLong ();
    }


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

    //  Put a string to the frame
    public void putString (String value)
    {
        needle.put ((byte) value.length ());
        needle.put (value.getBytes());
    }

    //  Get a string from the frame
    public String getString ()
    {
        int size = getNumber1 ();
        byte [] value = new byte [size];
        needle.get (value);

        return new String (value);
    }
    //  --------------------------------------------------------------------------
    //  Receive and parse a ZprotoExample from the socket. Returns new object or
    //  null if error. Will block if there's no message waiting.

    public static ZprotoExample recv (Socket input)
    {
        assert (input != null);
        ZprotoExample self = new ZprotoExample (0);
        ZFrame frame = null;

        try {
            //  Read valid message frame from socket; we loop over any
            //  garbage data we might receive from badly-connected peers
            while (true) {
                //  If we're reading from a ROUTER socket, get address
                if (input.getType () == ZMQ.ROUTER) {
                    self.address = ZFrame.recvFrame (input);
                    if (self.address == null)
                        return null;         //  Interrupted
                    if (!input.hasReceiveMore ())
                        throw new IllegalArgumentException ();
                }
                //  Read and parse command in frame
                frame = ZFrame.recvFrame (input);
                if (frame == null)
                    return null;             //  Interrupted

                //  Get and check protocol signature
                self.needle = ByteBuffer.wrap (frame.getData ());
                int signature = self.getNumber2 ();
                if (signature == (0xAAA0 | 0))
                    break;                  //  Valid signature

                //  Protocol assertion, drop message
                while (input.hasReceiveMore ()) {
                    frame.destroy ();
                    frame = ZFrame.recvFrame (input);
                }
                frame.destroy ();
            }

            //  Get message id, which is first byte in frame
            self.id = self.getNumber1 ();
            int listSize;
            int hashSize;

            switch (self.id) {
            case LOG:
                self.sequence = self.getNumber2 ();
                self.sequence = self.getNumber2 ();
                self.version = self.getNumber2 ();
                if (self.version != 3)
                    throw new IllegalArgumentException ();
                self.level = self.getNumber1 ();
                self.event = self.getNumber1 ();
                self.node = self.getNumber2 ();
                self.peer = self.getNumber2 ();
                self.time = self.getNumber8 ();
                self.host = self.getString ();
                self.data = self.getString ();
                break;

            case STRUCTURES:
                self.sequence = self.getNumber2 ();
                self.sequence = self.getNumber2 ();
                listSize = self.getNumber1 ();
                self.aliases = new ArrayList<String> ();
                while (listSize-- > 0) {
                    String string = self.getString ();
                    self.aliases.add (string);
                }
                hashSize = self.getNumber1 ();
                self.headers = new HashMap <String, String> ();
                while (hashSize-- > 0) {
                    String string = self.getString ();
                    String [] kv = string.split("=");
                    self.headers.put(kv[0], kv[1]);
                }

                break;

            case BINARY:
                self.sequence = self.getNumber2 ();
                self.sequence = self.getNumber2 ();
                self.flags = self.getBlock (4);
                //  Get next frame, leave current untouched
                if (!input.hasReceiveMore ())
                    throw new IllegalArgumentException ();
                self.address = ZFrame.recvFrame (input);
                break;

            case TYPES:
                self.sequence = self.getNumber2 ();
                self.sequence = self.getNumber2 ();
                self.client_forename = self.getString ();
                self.client_surname = self.getString ();
                self.client_mobile = self.getString ();
                self.client_email = self.getString ();
                self.supplier_forename = self.getString ();
                self.supplier_surname = self.getString ();
                self.supplier_mobile = self.getString ();
                self.supplier_email = self.getString ();
                break;

            case REPEAT:
                self.sequence = self.getNumber2 ();
                self.sequence = self.getNumber2 ();
                self.no1 = self.getNumber1 ();
                self.no2 = self.getNumber2 ();
                self.no4 = self.getNumber4 ();
                self.no8 = self.getNumber8 ();
                self.str = self.getString ();
                self.lstr = self.getString ();
                listSize = self.getNumber1 ();
                self.strs = new ArrayList<String> ();
                while (listSize-- > 0) {
                    String string = self.getString ();
                    self.strs.add (string);
                }
                self.persons_forename = self.getString ();
                self.persons_surname = self.getString ();
                self.persons_mobile = self.getString ();
                self.persons_email = self.getString ();
                break;

            default:
                throw new IllegalArgumentException ();
            }

            return self;

        } catch (Exception e) {
            //  Error returns
            System.out.printf ("E: malformed message '%d'\n", self.id);
            self.destroy ();
            return null;
        } finally {
            if (frame != null)
                frame.destroy ();
        }
    }


    //  Count size of key=value pair
    private static void
    headersCount (final Map.Entry <String, String> entry, ZprotoExample self)
    {
        self.headersBytes += entry.getKey ().length () + 1 + entry.getValue ().length () + 1;
    }

    //  Serialize headers key=value pair
    private static void
    headersWrite (final Map.Entry <String, String> entry, ZprotoExample self)
    {
        String string = entry.getKey () + "=" + entry.getValue ();
        self.putString (string);
    }


    //  --------------------------------------------------------------------------
    //  Send the ZprotoExample to the socket, and destroy it

    public boolean send (Socket socket)
    {
        assert (socket != null);

        //  Calculate size of serialized data
        int frameSize = 2 + 1;          //  Signature and message ID
        switch (id) {
        case LOG:
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  version is a 2-byte integer
            frameSize += 2;
            //  level is a 1-byte integer
            frameSize += 1;
            //  event is a 1-byte integer
            frameSize += 1;
            //  node is a 2-byte integer
            frameSize += 2;
            //  peer is a 2-byte integer
            frameSize += 2;
            //  time is a 8-byte integer
            frameSize += 8;
            //  host is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (host != null)
                frameSize += host.length ();
            //  data is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (data != null)
                frameSize += data.length ();
            break;

        case STRUCTURES:
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  aliases is an array of strings
            frameSize++;       //  Size is one octet
            if (aliases != null) {
                for (String value : aliases)
                    frameSize += 1 + value.length ();
            }
            //  headers is an array of key=value strings
            frameSize++;       //  Size is one octet
            if (headers != null) {
                headersBytes = 0;
                for (Map.Entry <String, String> entry: headers.entrySet ()) {
                    headersCount (entry, this);
                }
                frameSize += headersBytes;
            }
            break;

        case BINARY:
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  flags is a block of 4 bytes
            frameSize += 4;
            break;

        case TYPES:
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  client_forename is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (client_forename != null)
                frameSize += client_forename.length ();
            //  client_surname is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (client_surname != null)
                frameSize += client_surname.length ();
            //  client_mobile is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (client_mobile != null)
                frameSize += client_mobile.length ();
            //  client_email is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (client_email != null)
                frameSize += client_email.length ();
            //  supplier_forename is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (supplier_forename != null)
                frameSize += supplier_forename.length ();
            //  supplier_surname is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (supplier_surname != null)
                frameSize += supplier_surname.length ();
            //  supplier_mobile is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (supplier_mobile != null)
                frameSize += supplier_mobile.length ();
            //  supplier_email is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (supplier_email != null)
                frameSize += supplier_email.length ();
            break;

        case REPEAT:
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  sequence is a 2-byte integer
            frameSize += 2;
            //  no1 is a 1-byte integer
            frameSize += 1;
            //  no2 is a 2-byte integer
            frameSize += 2;
            //  no4 is a 4-byte integer
            frameSize += 4;
            //  no8 is a 8-byte integer
            frameSize += 8;
            //  str is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (str != null)
                frameSize += str.length ();
            //  lstr is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (lstr != null)
                frameSize += lstr.length ();
            //  strs is an array of strings
            frameSize++;       //  Size is one octet
            if (strs != null) {
                for (String value : strs)
                    frameSize += 1 + value.length ();
            }
            //  persons_forename is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (persons_forename != null)
                frameSize += persons_forename.length ();
            //  persons_surname is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (persons_surname != null)
                frameSize += persons_surname.length ();
            //  persons_mobile is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (persons_mobile != null)
                frameSize += persons_mobile.length ();
            //  persons_email is a string with 1-byte length
            frameSize++;       //  Size is one octet
            if (persons_email != null)
                frameSize += persons_email.length ();
            break;

        default:
            System.out.printf ("E: bad message type '%d', not sent\n", id);
            assert (false);
        }
        //  Now serialize message into the frame
        ZFrame frame = new ZFrame (new byte [frameSize]);
        needle = ByteBuffer.wrap (frame.getData ());
        int frameFlags = 0;
        putNumber2 (0xAAA0 | 0);
        putNumber1 ((byte) id);

        switch (id) {
        case LOG:
            putNumber2 (sequence);
            putNumber2 (sequence);
            putNumber2 (3);
            putNumber1 (level);
            putNumber1 (event);
            putNumber2 (node);
            putNumber2 (peer);
            putNumber8 (time);
            if (host != null)
                putString (host);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (data != null)
                putString (data);
            else
                putNumber1 ((byte) 0);      //  Empty string
            break;

        case STRUCTURES:
            putNumber2 (sequence);
            putNumber2 (sequence);
            if (aliases != null) {
                putNumber1 ((byte) aliases.size ());
                for (String value : aliases) {
                    putString (value);
                }
            }
            else
                putNumber1 ((byte) 0);      //  Empty string array
            if (headers != null) {
                putNumber1 ((byte) headers.size ());
                for (Map.Entry <String, String> entry: headers.entrySet ()) {
                    headersWrite (entry, this);
                }
            }
            else
                putNumber1 ((byte) 0);      //  Empty dictionary
            break;

        case BINARY:
            putNumber2 (sequence);
            putNumber2 (sequence);
            putBlock (flags, 4);
            frameFlags = ZMQ.SNDMORE;
            break;

        case TYPES:
            putNumber2 (sequence);
            putNumber2 (sequence);
            if (client_forename != null)
                putString (client_forename);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (client_surname != null)
                putString (client_surname);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (client_mobile != null)
                putString (client_mobile);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (client_email != null)
                putString (client_email);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (supplier_forename != null)
                putString (supplier_forename);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (supplier_surname != null)
                putString (supplier_surname);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (supplier_mobile != null)
                putString (supplier_mobile);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (supplier_email != null)
                putString (supplier_email);
            else
                putNumber1 ((byte) 0);      //  Empty string
            break;

        case REPEAT:
            putNumber2 (sequence);
            putNumber2 (sequence);
            putNumber1 (no1);
            putNumber2 (no2);
            putNumber4 (no4);
            putNumber8 (no8);
            if (str != null)
                putString (str);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (lstr != null)
                putString (lstr);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (strs != null) {
                putNumber1 ((byte) strs.size ());
                for (String value : strs) {
                    putString (value);
                }
            }
            else
                putNumber1 ((byte) 0);      //  Empty string array
            if (persons_forename != null)
                putString (persons_forename);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (persons_surname != null)
                putString (persons_surname);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (persons_mobile != null)
                putString (persons_mobile);
            else
                putNumber1 ((byte) 0);      //  Empty string
            if (persons_email != null)
                putString (persons_email);
            else
                putNumber1 ((byte) 0);      //  Empty string
            break;

        }
        //  If we're sending to a ROUTER, we send the address first
        if (socket.getType () == ZMQ.ROUTER) {
            assert (address != null);
            if (!address.send (socket, ZMQ.SNDMORE)) {
                destroy ();
                return false;
            }
        }
        //  Now send the data frame
        if (!frame.send(socket, frameFlags)) {
            frame.destroy ();
            destroy ();
            return false;
        }

        //  Now send any frame fields, in order
        switch (id) {
        case BINARY:
            //  If address isn't set, send an empty frame
            if (address == null)
                address = new ZFrame ("".getBytes ());
            if (!address.send (socket, 0)) {
                frame.destroy ();
                destroy ();
                return false;
            }
            break;
        }
        //  Destroy ZprotoExample object
        destroy ();
        return true;
    }


//  --------------------------------------------------------------------------
//  Send the LOG to the socket in one step

    public static void sendLog (
        Socket output,
        int sequence,
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
        self.setSequence (sequence);
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
        int sequence,
        List <String> aliases,
        Map <String, String> headers)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.STRUCTURES);
        self.setSequence (sequence);
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
        int sequence,
        byte [] flags,
        chunk public_key,
        uuid identifier,
        ZFrame address,
        msg content)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.BINARY);
        self.setSequence (sequence);
        self.setSequence (sequence);
        self.setFlags (flags);
        self.setAddress (address.duplicate ());
        self.send (output);
    }

//  --------------------------------------------------------------------------
//  Send the TYPES to the socket in one step

    public static void sendTypes (
        Socket output,
        int sequence,
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
        self.setSequence (sequence);
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
//  Send the REPEAT to the socket in one step

    public static void sendRepeat (
        Socket output,
        int sequence,
        int sequence,
        int no1,
        int no2,
        long no4,
        long no8,
        String str,
        String lstr,
        List <String> strs,
        chunk chunks,
        uuid uuids,
        String persons_forename,
        String persons_surname,
        String persons_mobile,
        String persons_email)
    {
        ZprotoExample self = new ZprotoExample (ZprotoExample.REPEAT);
        self.setSequence (sequence);
        self.setSequence (sequence);
        self.setNo1 (no1);
        self.setNo2 (no2);
        self.setNo4 (no4);
        self.setNo8 (no8);
        self.setStr (str);
        self.setLstr (lstr);
        self.setStrs (new ArrayList <String> (strs));
        self.setPersons_Forename (persons_forename);
        self.setPersons_Surname (persons_surname);
        self.setPersons_Mobile (persons_mobile);
        self.setPersons_Email (persons_email);
        self.send (output);
    }


    //  --------------------------------------------------------------------------
    //  Duplicate the ZprotoExample message

    public ZprotoExample dup ()
    {
        ZprotoExample copy = new ZprotoExample (this.id);
        if (this.address != null)
            copy.address = this.address.duplicate ();
        switch (this.id) {
        case LOG:
            copy.sequence = this.sequence;
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
            copy.sequence = this.sequence;
            copy.aliases = new ArrayList <String> (this.aliases);
            copy.headers = new HashMap <String, String> (this.headers);
        break;
        case BINARY:
            copy.sequence = this.sequence;
            copy.sequence = this.sequence;
            System.arraycopy (copy.flags, 0, this.flags, 0, 4);
            copy.address = this.address.duplicate ();
        break;
        case TYPES:
            copy.sequence = this.sequence;
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
        case REPEAT:
            copy.sequence = this.sequence;
            copy.sequence = this.sequence;
            copy.no1 = this.no1;
            copy.no2 = this.no2;
            copy.no4 = this.no4;
            copy.no8 = this.no8;
            copy.str = this.str;
            copy.lstr = this.lstr;
            copy.strs = new ArrayList <String> (this.strs);
            copy.persons_forename = this.persons_forename;
            copy.persons_surname = this.persons_surname;
            copy.persons_mobile = this.persons_mobile;
            copy.persons_email = this.persons_email;
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

        case REPEAT:
            System.out.println ("REPEAT:");
            System.out.printf ("    sequence=%d\n", (long)sequence);
            System.out.printf ("    sequence=%d\n", (long)sequence);
            System.out.printf ("    no1=%d\n", (long)no1);
            System.out.printf ("    no2=%d\n", (long)no2);
            System.out.printf ("    no4=%d\n", (long)no4);
            System.out.printf ("    no8=%d\n", (long)no8);
            if (str != null)
                System.out.printf ("    str='%s'\n", str);
            else
                System.out.printf ("    str=\n");
            if (lstr != null)
                System.out.printf ("    lstr='%s'\n", lstr);
            else
                System.out.printf ("    lstr=\n");
            System.out.printf ("    strs={");
            if (strs != null) {
                for (String value : strs) {
                    System.out.printf (" '%s'", value);
                }
            }
            System.out.printf (" }\n");
            if (persons_forename != null)
                System.out.printf ("    persons_forename='%s'\n", persons_forename);
            else
                System.out.printf ("    persons_forename=\n");
            if (persons_surname != null)
                System.out.printf ("    persons_surname='%s'\n", persons_surname);
            else
                System.out.printf ("    persons_surname=\n");
            if (persons_mobile != null)
                System.out.printf ("    persons_mobile='%s'\n", persons_mobile);
            else
                System.out.printf ("    persons_mobile=\n");
            if (persons_email != null)
                System.out.printf ("    persons_email='%s'\n", persons_email);
            else
                System.out.printf ("    persons_email=\n");
            break;

        }
    }


    //  --------------------------------------------------------------------------
    //  Get/set the message address

    public ZFrame address ()
    {
        return address;
    }

    public void setAddress (ZFrame address)
    {
        if (this.address != null)
            this.address.destroy ();
        this.address = address.duplicate ();
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

    public void setHost (String format, Object[] args)
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

    public void setData (String format, Object[] args)
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

    public void appendAliases (String format, Object[] args)
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
    //  Get/set a value in the headers dictionary

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

    public void insertHeaders (String key, String format, Object[] args)
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
    //  Get/set the client_forename field

    public String client_forename ()
    {
        return client_forename;
    }

    public void setClient_Forename (String format, Object[] args)
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

    public void setClient_Surname (String format, Object[] args)
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

    public void setClient_Mobile (String format, Object[] args)
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

    public void setClient_Email (String format, Object[] args)
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

    public void setSupplier_Forename (String format, Object[] args)
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

    public void setSupplier_Surname (String format, Object[] args)
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

    public void setSupplier_Mobile (String format, Object[] args)
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

    public void setSupplier_Email (String format, Object[] args)
    {
        //  Format into newly allocated string
        supplier_email = String.format (format, args);
    }


    //  --------------------------------------------------------------------------
    //  Get/set the no1 field

    public int no1 ()
    {
        return no1;
    }

    public void setNo1 (int no1)
    {
        this.no1 = no1;
    }


    //  --------------------------------------------------------------------------
    //  Get/set the no2 field

    public int no2 ()
    {
        return no2;
    }

    public void setNo2 (int no2)
    {
        this.no2 = no2;
    }


    //  --------------------------------------------------------------------------
    //  Get/set the no4 field

    public long no4 ()
    {
        return no4;
    }

    public void setNo4 (long no4)
    {
        this.no4 = no4;
    }


    //  --------------------------------------------------------------------------
    //  Get/set the no8 field

    public long no8 ()
    {
        return no8;
    }

    public void setNo8 (long no8)
    {
        this.no8 = no8;
    }


    //  --------------------------------------------------------------------------
    //  Get/set the str field

    public String str ()
    {
        return str;
    }

    public void setStr (String format, Object[] args)
    {
        //  Format into newly allocated string
        str = String.format (format, args);
    }


    //  --------------------------------------------------------------------------
    //  Get/set the lstr field

    public String lstr ()
    {
        return lstr;
    }

    public void setLstr (String format, Object[] args)
    {
        //  Format into newly allocated string
        lstr = String.format (format, args);
    }


    //  --------------------------------------------------------------------------
    //  Iterate through the strs field, and append a strs value

    public List <String> strs ()
    {
        return strs;
    }

    public void appendStrs (String format, Object[] args)
    {
        //  Format into newly allocated string

        String string = String.format (format, args);
        //  Attach string to list
        if (strs == null)
            strs = new ArrayList <String> ();
        strs.add (string);
    }

    public void setStrs (List <String> value)
    {
        strs = new ArrayList (value);
    }




    //  --------------------------------------------------------------------------
    //  Get/set the persons_forename field

    public String persons_forename ()
    {
        return persons_forename;
    }

    public void setPersons_Forename (String format, Object[] args)
    {
        //  Format into newly allocated string
        persons_forename = String.format (format, args);
    }


    //  --------------------------------------------------------------------------
    //  Get/set the persons_surname field

    public String persons_surname ()
    {
        return persons_surname;
    }

    public void setPersons_Surname (String format, Object[] args)
    {
        //  Format into newly allocated string
        persons_surname = String.format (format, args);
    }


    //  --------------------------------------------------------------------------
    //  Get/set the persons_mobile field

    public String persons_mobile ()
    {
        return persons_mobile;
    }

    public void setPersons_Mobile (String format, Object[] args)
    {
        //  Format into newly allocated string
        persons_mobile = String.format (format, args);
    }


    //  --------------------------------------------------------------------------
    //  Get/set the persons_email field

    public String persons_email ()
    {
        return persons_email;
    }

    public void setPersons_Email (String format, Object[] args)
    {
        //  Format into newly allocated string
        persons_email = String.format (format, args);
    }


}

