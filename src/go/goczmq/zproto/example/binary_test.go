package example

import (
	"testing"

	"github.com/zeromq/goczmq"
)

// Yay! Test function.
func TestBinary(t *testing.T) {

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

	binary.Flags = []byte{'z', 'z', 'z', 'z'}

	binary.PublicKey = []byte("Captcha Diem")

	binary.Identifier = []byte("Captcha Diem")

	binary.Address = []byte("Captcha Diem")

	binary.Content = []byte("Captcha Diem")

	err = binary.Send(output)
	if err != nil {
		t.Fatal(err)
	}
	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Binary)

	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}

	if len(tr.Flags) != 4 {
		t.Fatalf("mismatch octets size for %s", "Flags")
	}
	for idx, b := range []byte{'z', 'z', 'z', 'z'} {
		if tr.Flags[idx] != b {
			t.Fatalf("mismatch octets value for %s", "Flags")
		}
	}

	if string(tr.PublicKey) != "Captcha Diem" {
		t.Fatalf("expected %s, got %s", "Captcha Diem", tr.PublicKey)
	}

	if string(tr.Identifier) != "Captcha Diem" {
		t.Fatalf("expected %s, got %s", "Captcha Diem", tr.Identifier)
	}

	if string(tr.Address) != "Captcha Diem" {
		t.Fatalf("expected %s, got %s", "Captcha Diem", tr.Address)
	}

	if string(tr.Content) != "Captcha Diem" {
		t.Fatalf("expected %s, got %s", "Captcha Diem", tr.Content)
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
