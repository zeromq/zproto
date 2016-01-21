
import unittest
import struct
import uuid
import logging
import zmq
from ZprotoExample import ZprotoExample

class ZprotoExampleTest(unittest.TestCase):

    def test_create_destroy(self):
        #  Simple create/destroy test
        print(" * zproto_example: ");
        class1 = ZprotoExample()
        del(class1)
    
    def test_sockets(self):
        #  Create pair of sockets we can send through
        #  We must bind before connect if we wish to remain compatible with ZeroMQ < v4
        outsock = zmq.Socket(ctx, zmq.DEALER)
        outsock.bind("inproc://selftest-zproto_example")

        insock = zmq.Socket(ctx, zmq.ROUTER)
        insock.connect("inproc://selftest-zproto_example")

        #  Encode/send/decode and verify each message type

        msg = ZprotoExample(ZprotoExample.LOG)
        msg.set_sequence(123)
        msg.set_level(123)
        msg.set_event(123)
        msg.set_node(123)
        msg.set_peer(123)
        msg.set_time(123)
        msg.set_host ("Life is short but Now lasts for ever")
        msg.set_data ("Life is short but Now lasts for ever")
        # Send twice?
        msg.send(outsock);
        msg.send(outsock);
        

        msg.recv(insock)
        self.assertEqual (msg.sequence(), 123)
        self.assertEqual (msg.level(), 123)
        self.assertEqual (msg.event(), 123)
        self.assertEqual (msg.node(), 123)
        self.assertEqual (msg.peer(), 123)
        self.assertEqual (msg.time(), 123)
        self.assertEqual(msg.host(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.data(), "Life is short but Now lasts for ever")
        #self.destroy ();

        msg = ZprotoExample(ZprotoExample.STRUCTURES)
        msg.set_sequence(123)
        msg.set_aliases(("Name: %s" %"Brutus","Age: %d" %43))
        msg._headers.update({"Name": "Brutus"})
        msg._headers.update({"Age": "43"});
        # Send twice?
        msg.send(outsock);
        msg.send(outsock);
        

        msg.recv(insock)
        self.assertEqual (msg.sequence(), 123)
        self.assertEqual(len(msg.aliases()), 2)
        self.assertEqual(msg.aliases()[0], "Name: Brutus")
        self.assertEqual(msg.aliases()[1], "Age: %d" %43)
        self.assertEqual(len(msg.headers()), 2)
        self.assertEqual(msg.headers()["Name"], "Brutus")
        self.assertEqual(msg.headers()["Age"], "43")
        #self.destroy ();

        msg = ZprotoExample(ZprotoExample.BINARY)
        msg.set_sequence(123)
        msg.set_flags(b'123')
        msg.set_public_key("Captcha Diem".encode('utf-8'))
        msg.set_identifier(uuid.UUID(bytes=b'1'*16))
        msg.set_address(zmq.Frame(b"Captcha Diem"))
        # Send twice?
        msg.send(outsock);
        msg.send(outsock);
        

        msg.recv(insock)
        self.assertEqual (msg.sequence(), 123)
        self.assertEqual(msg.flags(), b'123')
        self.assertEqual(msg.public_key(), "Captcha Diem".encode('utf-8'))
        self.assertEqual(msg.identifier(), uuid.UUID(bytes=b'1'*16))
        self.assertEqual(msg.address().bytes, zmq.Frame(b"Captcha Diem").bytes)
        #self.destroy ();

        msg = ZprotoExample(ZprotoExample.TYPES)
        msg.set_sequence(123)
        msg.set_client_forename ("Life is short but Now lasts for ever")
        msg.set_client_surname ("Life is short but Now lasts for ever")
        msg.set_client_mobile ("Life is short but Now lasts for ever")
        msg.set_client_email ("Life is short but Now lasts for ever")
        msg.set_supplier_forename ("Life is short but Now lasts for ever")
        msg.set_supplier_surname ("Life is short but Now lasts for ever")
        msg.set_supplier_mobile ("Life is short but Now lasts for ever")
        msg.set_supplier_email ("Life is short but Now lasts for ever")
        # Send twice?
        msg.send(outsock);
        msg.send(outsock);
        

        msg.recv(insock)
        self.assertEqual (msg.sequence(), 123)
        self.assertEqual(msg.client_forename(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.client_surname(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.client_mobile(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.client_email(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.supplier_forename(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.supplier_surname(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.supplier_mobile(), "Life is short but Now lasts for ever")
        self.assertEqual(msg.supplier_email(), "Life is short but Now lasts for ever")
        #self.destroy ();

        ctx.destroy ();
        print("OK")

ctx = zmq.Context()

if __name__ == "__main__":
    logger = logging.getLogger()
    logging.basicConfig(level=logging.DEBUG)
    unittest.main()
