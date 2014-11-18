package example

import (
	"testing"

	"github.com/zeromq/goczmq"
)

// Yay! Test function.
func TestStructures(t *testing.T) {

	// Create pair of sockets we can send through

	// Output socket
	output := goczmq.NewSock(goczmq.DEALER)
	defer output.Destroy()

	routingId := "Shout"
	output.SetIdentity(routingId)
	_, err := output.Bind("inproc://selftest-structures")
	if err != nil {
		t.Fatal(err)
	}
	defer output.Unbind("inproc://selftest-structures")

	// Input socket
	input := goczmq.NewSock(goczmq.ROUTER)
	defer input.Destroy()

	err = input.Connect("inproc://selftest-structures")
	if err != nil {
		t.Fatal(err)
	}
	defer input.Disconnect("inproc://selftest-structures")

	// Create a Structures message and send it through the wire
	structures := NewStructures()

	structures.sequence = 123

	structures.Aliases = []string{"Name: Brutus", "Age: 43"}

	structures.Headers = map[string]string{"Name": "Brutus", "Age": "43"}

	err = structures.Send(output)
	if err != nil {
		t.Fatal(err)
	}
	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Structures)

	if tr.sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.sequence)
	}

	for idx, str := range []string{"Name: Brutus", "Age: 43"} {
		if tr.Aliases[idx] != str {
			t.Fatalf("expected %s, got %s", str, tr.Aliases[idx])
		}
	}

	for key, val := range map[string]string{"Name": "Brutus", "Age": "43"} {
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

	if routingId != string(tr.RoutingId()) {
		t.Fatalf("expected %s, got %s", routingId, string(tr.RoutingId()))
	}
}
