package org.zproto;

import static org.junit.Assert.*;
import org.junit.Test;
import org.zeromq.ZMQ;
import org.zeromq.ZMQ.Socket;
import org.zeromq.ZFrame;
import org.zeromq.ZContext;

public class TestZprotoExample
{
    @Test
    public void testZprotoExample ()
    {
        System.out.printf (" * zproto_example: ");

        //  Simple create/destroy test
        ZprotoExample self = new ZprotoExample (0);
        assert (self != null);
        self.destroy ();

        //  Create pair of sockets we can send through
        ZContext ctx = new ZContext ();
        assert (ctx != null);

        Socket output = ctx.createSocket (ZMQ.DEALER);
        assert (output != null);
        output.bind ("inproc://selftest");
        Socket input = ctx.createSocket (ZMQ.ROUTER);
        assert (input != null);
        input.connect ("inproc://selftest");

        //  Encode/send/decode and verify each message type

        self = new ZprotoExample (ZprotoExample.LOG);
        self.setSequence ((byte) 123);
        self.setLevel ((byte) 123);
        self.setEvent ((byte) 123);
        self.setNode ((byte) 123);
        self.setPeer ((byte) 123);
        self.setTime ((byte) 123);
        self.setHost ("Life is short but Now lasts for ever");
        self.setData ("Life is short but Now lasts for ever");
        self.send (output);

        self = ZprotoExample.recv (input);
        assert (self != null);
        assertEquals (self.sequence (), 123);
        assertEquals (self.level (), 123);
        assertEquals (self.event (), 123);
        assertEquals (self.node (), 123);
        assertEquals (self.peer (), 123);
        assertEquals (self.time (), 123);
        assertEquals (self.host (), "Life is short but Now lasts for ever");
        assertEquals (self.data (), "Life is short but Now lasts for ever");
        self.destroy ();

        self = new ZprotoExample (ZprotoExample.STRUCTURES);
        self.setSequence ((byte) 123);
        self.appendAliases ("Name: %s", "Brutus");
        self.appendAliases ("Age: %d", 43);
        self.insertHeaders ("Name", "Brutus");
        self.insertHeaders ("Age", "%d", 43);
        self.send (output);

        self = ZprotoExample.recv (input);
        assert (self != null);
        assertEquals (self.sequence (), 123);
        assertEquals (self.aliases ().size (), 2);
        assertEquals (self.aliases ().get (0), "Name: Brutus");
        assertEquals (self.aliases ().get (1), "Age: 43");
        assertEquals (self.headers ().size (), 2);
        assertEquals (self.headersString ("Name", "?"), "Brutus");
        assertEquals (self.headersNumber ("Age", 0), 43);
        self.destroy ();

        self = new ZprotoExample (ZprotoExample.BINARY);
        self.setSequence ((byte) 123);
        byte [] flagsData = new byte [ZprotoExample.FLAGS_SIZE];
        for (int i=0; i < ZprotoExample.FLAGS_SIZE; i++)
            flagsData [i] = 123;
        self.setFlags (flagsData);
        self.setPublic_Key ("Captcha Diem".getBytes());
        self.setAddress (new ZFrame ("Captcha Diem"));
        self.send (output);

        self = ZprotoExample.recv (input);
        assert (self != null);
        assertEquals (self.sequence (), 123);
        assertEquals (self.flags () [0], 123);
        assertEquals (self.flags () [ZprotoExample.FLAGS_SIZE - 1], 123);
        assertTrue (java.util.Arrays.equals("Captcha Diem".getBytes(), self.public_key ()));
        assertTrue (self.address ().streq ("Captcha Diem"));
        self.destroy ();

        self = new ZprotoExample (ZprotoExample.TYPES);
        self.setSequence ((byte) 123);
        self.setClient_Forename ("Life is short but Now lasts for ever");
        self.setClient_Surname ("Life is short but Now lasts for ever");
        self.setClient_Mobile ("Life is short but Now lasts for ever");
        self.setClient_Email ("Life is short but Now lasts for ever");
        self.setSupplier_Forename ("Life is short but Now lasts for ever");
        self.setSupplier_Surname ("Life is short but Now lasts for ever");
        self.setSupplier_Mobile ("Life is short but Now lasts for ever");
        self.setSupplier_Email ("Life is short but Now lasts for ever");
        self.send (output);

        self = ZprotoExample.recv (input);
        assert (self != null);
        assertEquals (self.sequence (), 123);
        assertEquals (self.client_forename (), "Life is short but Now lasts for ever");
        assertEquals (self.client_surname (), "Life is short but Now lasts for ever");
        assertEquals (self.client_mobile (), "Life is short but Now lasts for ever");
        assertEquals (self.client_email (), "Life is short but Now lasts for ever");
        assertEquals (self.supplier_forename (), "Life is short but Now lasts for ever");
        assertEquals (self.supplier_surname (), "Life is short but Now lasts for ever");
        assertEquals (self.supplier_mobile (), "Life is short but Now lasts for ever");
        assertEquals (self.supplier_email (), "Life is short but Now lasts for ever");
        self.destroy ();

        ctx.destroy ();
        System.out.printf ("OK\n");
    }
}

