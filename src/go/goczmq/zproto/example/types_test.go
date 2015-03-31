package example

import (
	"crypto/sha1"
	"fmt"
	"testing"

	"github.com/zeromq/goczmq"
)

// Yay! Test function.
func TestTypes(t *testing.T) {

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
	_, err := output.Bind("inproc://selftest-types")
	if err != nil {
		t.Fatal(err)
	}
	defer output.Unbind("inproc://selftest-types")

	// Input socket
	input := goczmq.NewSock(goczmq.ROUTER)
	defer input.Destroy()

	err = input.Connect("inproc://selftest-types")
	if err != nil {
		t.Fatal(err)
	}
	defer input.Disconnect("inproc://selftest-types")

	// Create a Types message and send it through the wire
	types := NewTypes()
	types.sequence = 123
	types.ClientForename = "Lucius Junius"
	types.ClientSurname = "Brutus"
	types.ClientMobile = "01234567890"
	types.ClientEmail = "brutus@example.com"
	types.SupplierForename = "Leslie"
	types.SupplierSurname = "Lamport"
	types.SupplierMobile = "01987654321"
	types.SupplierEmail = "lamport@example.com"

	err = types.Send(output)
	if err != nil {
		t.Fatal(err)
	}

	sndMsg, err = types.Marshal()
	if err != nil {
		t.Fatal(err)
	}
	sndDigest = fmt.Sprintf("%x", sha1.Sum(sndMsg))
	if "dee674a1bcac455b7cd1801f4008c65d0a37b2ea" != sndDigest {
		fmt.Printf("sndMsg: %x\n", sndMsg)
		t.Fatalf("expected %q digest for types, got %s", "dee674a1bcac455b7cd1801f4008c65d0a37b2ea", sndDigest)
	}

	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Types)

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
	if "dee674a1bcac455b7cd1801f4008c65d0a37b2ea" != rcvDigest {
		t.Fatalf("expected %q digest for types, got %s", "dee674a1bcac455b7cd1801f4008c65d0a37b2ea", rcvDigest)
	}

	// Tests number
	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}
	// Tests string
	if tr.ClientForename != "Lucius Junius" {
		t.Fatalf("expected %s, got %s", "Lucius Junius", tr.ClientForename)
	}
	// Tests string
	if tr.ClientSurname != "Brutus" {
		t.Fatalf("expected %s, got %s", "Brutus", tr.ClientSurname)
	}
	// Tests string
	if tr.ClientMobile != "01234567890" {
		t.Fatalf("expected %s, got %s", "01234567890", tr.ClientMobile)
	}
	// Tests string
	if tr.ClientEmail != "brutus@example.com" {
		t.Fatalf("expected %s, got %s", "brutus@example.com", tr.ClientEmail)
	}
	// Tests string
	if tr.SupplierForename != "Leslie" {
		t.Fatalf("expected %s, got %s", "Leslie", tr.SupplierForename)
	}
	// Tests string
	if tr.SupplierSurname != "Lamport" {
		t.Fatalf("expected %s, got %s", "Lamport", tr.SupplierSurname)
	}
	// Tests string
	if tr.SupplierMobile != "01987654321" {
		t.Fatalf("expected %s, got %s", "01987654321", tr.SupplierMobile)
	}
	// Tests string
	if tr.SupplierEmail != "lamport@example.com" {
		t.Fatalf("expected %s, got %s", "lamport@example.com", tr.SupplierEmail)
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
