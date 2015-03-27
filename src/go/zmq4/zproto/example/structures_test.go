package example

import (
	"crypto/sha1"
	"fmt"
	"testing"

	zmq "github.com/pebbe/zmq4"
)

// Yay! Test function.
func TestStructures(t *testing.T) {

	var (
		sndMsg    []byte
		rcvMsg    []byte
		rcvDigest string
		sndDigest string
	)

	// Create pair of sockets we can send through

	// Output socket
	output, err := zmq.NewSocket(zmq.DEALER)
	if err != nil {
		t.Fatal(err)
	}
	defer output.Close()

	routingID := "Shout"
	output.SetIdentity(routingID)
	err = output.Bind("inproc://selftest-structures")
	if err != nil {
		t.Fatal(err)
	}
	defer output.Unbind("inproc://selftest-structures")

	// Input socket
	input, err := zmq.NewSocket(zmq.ROUTER)
	if err != nil {
		t.Fatal(err)
	}
	defer input.Close()

	err = input.Connect("inproc://selftest-structures")
	if err != nil {
		t.Fatal(err)
	}
	defer input.Disconnect("inproc://selftest-structures")

	// Create a Structures message and send it through the wire
	structures := NewStructures()
	structures.sequence = 123
	structures.Aliases = []string{
		"First alias",
		"Second alias",
		"Third alias",
	}
	structures.Headers = map[string]string{"endpoint": "tcp://localhost:5665"}

	err = structures.Send(output)
	if err != nil {
		t.Fatal(err)
	}

	sndMsg, err = structures.Marshal()
	if err != nil {
		t.Fatal(err)
	}
	sndDigest = fmt.Sprintf("%x", sha1.Sum(sndMsg))
	if "52d9e295862bc3edad2841c412327a50d2c1b857" != sndDigest {
		fmt.Printf("sndMsg: %x\n", sndMsg)
		t.Fatalf("expected %q digest for structures, got %s", "52d9e295862bc3edad2841c412327a50d2c1b857", sndDigest)
	}

	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Structures)

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
	if "52d9e295862bc3edad2841c412327a50d2c1b857" != rcvDigest {
		t.Fatalf("expected %q digest for structures, got %s", "52d9e295862bc3edad2841c412327a50d2c1b857", rcvDigest)
	}

	// Tests number
	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}
	// Tests strings
	for idx, str := range []string{"First alias","Second alias","Third alias"} {
		if tr.Aliases[idx] != str {
			t.Fatalf("expected %s, got %s", str, tr.Aliases[idx])
		}
	}
	// Tests hash
	for key, val := range map[string]string{"endpoint": "tcp://localhost:5665"} {
		if tr.Headers[key] != val {
			t.Fatalf("expected %s, got %s", val, tr.Headers[key])
		}
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
