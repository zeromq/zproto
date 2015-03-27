package example

import (
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"testing"

	"github.com/zeromq/goczmq"
)

// Yay! Test function.
func TestBinary(t *testing.T) {

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
	_, err := output.Bind("inproc://selftest-binary")
	if err != nil {
		t.Fatal(err)
	}
	defer output.Unbind("inproc://selftest-binary")

	// Input socket
	input := goczmq.NewSock(goczmq.ROUTER)
	defer input.Destroy()

	err = input.Connect("inproc://selftest-binary")
	if err != nil {
		t.Fatal(err)
	}
	defer input.Disconnect("inproc://selftest-binary")

	// Create a Binary message and send it through the wire
	binary := NewBinary()
	binary.sequence = 123
	binary.Flags = []byte("b38c")
	binary.PublicKey = []byte("89f5ffe70d747869dfe8")
	binary.Identifier, _ = hex.DecodeString("3a60e6850a1e4cc15f3bfd4b42bc6b3e")
	binary.Address = []byte("0206f99f6137d9fe380f")
	binary.Content = []byte("728a92c6749235ba7002")

	err = binary.Send(output)
	if err != nil {
		t.Fatal(err)
	}

	sndMsg, err = binary.Marshal()
	if err != nil {
		t.Fatal(err)
	}
	sndDigest = fmt.Sprintf("%x", sha1.Sum(sndMsg))
	if "740caf04158436b4a81b844135a8a1dc9e4f8a54" != sndDigest {
		fmt.Printf("sndMsg: %x\n", sndMsg)
		t.Fatalf("expected %q digest for binary, got %s", "740caf04158436b4a81b844135a8a1dc9e4f8a54", sndDigest)
	}

	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Binary)

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
	if "740caf04158436b4a81b844135a8a1dc9e4f8a54" != rcvDigest {
		t.Fatalf("expected %q digest for binary, got %s", "740caf04158436b4a81b844135a8a1dc9e4f8a54", rcvDigest)
	}

	// Tests number
	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}
	// Tests octets
	if len(tr.Flags) != 4 {
		t.Fatalf("mismatch octets size for %s", "Flags")
	}
	for idx, b := range []byte("b38c") {
		if tr.Flags[idx] != b {
			t.Fatalf("mismatch octets value for %s", "Flags")
		}
	}
	// Tests chunk
	if string(tr.PublicKey) != "89f5ffe70d747869dfe8" {
		t.Fatalf("expected %s, got %s", "89f5ffe70d747869dfe8", tr.PublicKey)
	}
	// Tests uuid
	if hex.EncodeToString(tr.Identifier) != "3a60e6850a1e4cc15f3bfd4b42bc6b3e" {
		t.Fatalf("expected %s, got %s", "3a60e6850a1e4cc15f3bfd4b42bc6b3e", tr.Identifier)
	}
	// Tests frame
	if string(tr.Address) != "0206f99f6137d9fe380f" {
		t.Fatalf("expected %s, got %s", "0206f99f6137d9fe380f", tr.Address)
	}
	// Tests msg
	if string(tr.Content) != "728a92c6749235ba7002" {
		t.Fatalf("expected %s, got %s", "728a92c6749235ba7002", tr.Content)
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
