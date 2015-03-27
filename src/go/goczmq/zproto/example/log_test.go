package example

import (
	"crypto/sha1"
	"fmt"
	"testing"

	"github.com/zeromq/goczmq"
)

// Yay! Test function.
func TestLog(t *testing.T) {

	var (
		sndMsg    []byte
		rcvMsg    []byte
		rcvDigest string
		sndDigest string
	)

	// Create pair of sockets we can send through

	// Output socket
	output := goczmq.NewSock(goczmq.DEALER)
	defer output.Destroy()

	routingID := "Shout"
	output.SetIdentity(routingID)
	_, err := output.Bind("inproc://selftest-log")
	if err != nil {
		t.Fatal(err)
	}
	defer output.Unbind("inproc://selftest-log")

	// Input socket
	input := goczmq.NewSock(goczmq.ROUTER)
	defer input.Destroy()

	err = input.Connect("inproc://selftest-log")
	if err != nil {
		t.Fatal(err)
	}
	defer input.Disconnect("inproc://selftest-log")

	// Create a Log message and send it through the wire
	log := NewLog()
	log.sequence = 123
	log.Level = 2
	log.Event = 3
	log.Node = 45536
	log.Peer = 65535
	log.Time = 1427261426
	log.Host = "localhost"
	log.Data = "This is the message to log"

	err = log.Send(output)
	if err != nil {
		t.Fatal(err)
	}

	sndMsg, err = log.Marshal()
	if err != nil {
		t.Fatal(err)
	}
	sndDigest = fmt.Sprintf("%x", sha1.Sum(sndMsg))
	if "b90f4926d4662b319c0ec113794b0f27d9336a23" != sndDigest {
		fmt.Printf("sndMsg: %x\n", sndMsg)
		t.Fatalf("expected %q digest for log, got %s", "b90f4926d4662b319c0ec113794b0f27d9336a23", sndDigest)
	}

	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Log)

	rcvMsg, err = tr.Marshal()
	if err != nil {
		t.Fatal(err)
	}
	rcvDigest = fmt.Sprintf("%x", sha1.Sum(rcvMsg))
	if sndDigest != rcvDigest {
		fmt.Printf("sndMsg: %x\n", sndMsg)
		fmt.Printf("rcvMsg: %x\n", rcvMsg)
		t.Fatalf("inconsistent digest after sending and after receiving msg: %q != %q", sndDigest, rcvDigest)
	}
	if "b90f4926d4662b319c0ec113794b0f27d9336a23" != rcvDigest {
		t.Fatalf("expected %q digest for log, got %s", "b90f4926d4662b319c0ec113794b0f27d9336a23", rcvDigest)
	}

	// Tests number
	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}
	// Tests number
	if tr.Level != 2 {
		t.Fatalf("expected %d, got %d", 2, tr.Level)
	}
	// Tests number
	if tr.Event != 3 {
		t.Fatalf("expected %d, got %d", 3, tr.Event)
	}
	// Tests number
	if tr.Node != 45536 {
		t.Fatalf("expected %d, got %d", 45536, tr.Node)
	}
	// Tests number
	if tr.Peer != 65535 {
		t.Fatalf("expected %d, got %d", 65535, tr.Peer)
	}
	// Tests number
	if tr.Time != 1427261426 {
		t.Fatalf("expected %d, got %d", 1427261426, tr.Time)
	}
	// Tests string
	if tr.Host != "localhost" {
		t.Fatalf("expected %s, got %s", "localhost", tr.Host)
	}
	// Tests longstr
	if tr.Data != "This is the message to log" {
		t.Fatalf("expected %s, got %s", "This is the message to log", tr.Data)
	}

	err = tr.Send(input)
	if err != nil {
		t.Fatal(err)
	}

	transit, err = Recv(output)
	if err != nil {
		t.Fatal(err)
	}

	if routingID != string(tr.RoutingID()) {
		t.Fatalf("expected %s, got %s", routingID, string(tr.RoutingID()))
	}
}
