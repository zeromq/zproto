package example

import (
	"testing"

	"github.com/zeromq/goczmq"
)

// Yay! Test function.
func TestLog(t *testing.T) {

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

	log.Level = 123

	log.Event = 123

	log.Node = 123

	log.Peer = 123

	log.Time = 123

	log.Host = "Life is short but Now lasts for ever"

	log.Data = "Life is short but Now lasts for ever"

	err = log.Send(output)
	if err != nil {
		t.Fatal(err)
	}
	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Log)

	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}

	if tr.Level != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.Level)
	}

	if tr.Event != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.Event)
	}

	if tr.Node != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.Node)
	}

	if tr.Peer != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.Peer)
	}

	if tr.Time != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.Time)
	}

	if tr.Host != "Life is short but Now lasts for ever" {
		t.Fatalf("expected %s, got %s", "Life is short but Now lasts for ever", tr.Host)
	}

	if tr.Data != "Life is short but Now lasts for ever" {
		t.Fatalf("expected %s, got %s", "Life is short but Now lasts for ever", tr.Data)
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
