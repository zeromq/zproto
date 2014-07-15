package example

import (
	"reflect"
	"testing"

	zmq "github.com/pebbe/zmq4"
)

// Yay! Test function.
func TestRepeat(t *testing.T) {

	// Create pair of sockets we can send through

	// Output socket
	output, err := zmq.NewSocket(zmq.DEALER)
	if err != nil {
		t.Fatal(err)
	}
	defer output.Close()

	routingId := "Shout"
	output.SetIdentity(routingId)
	err = output.Bind("inproc://selftest-repeat")
	if err != nil {
		t.Fatal(err)
	}
	defer output.Unbind("inproc://selftest-repeat")

	// Input socket
	input, err := zmq.NewSocket(zmq.ROUTER)
	if err != nil {
		t.Fatal(err)
	}
	defer input.Close()

	err = input.Connect("inproc://selftest-repeat")
	if err != nil {
		t.Fatal(err)
	}
	defer input.Disconnect("inproc://selftest-repeat")

	// Create a Repeat message and send it through the wire
	repeat := NewRepeat()
	repeat.Sequence = 123
	no1 := make([]byte, 3)
	no1 = append(no1, 10, 20, 30)
	repeat.No1 = no1
	no2 := make([]uint16, 144)
	no2 = append(no2, 10, 20, 30)
	repeat.No2 = no2
	no4 := make([]uint32, 256)
	no4 = append(no4, 10, 20, 30)
	repeat.No4 = no4
	no8 := make([]uint64, 256)
	no8 = append(no8, 10, 20, 30)
	repeat.No8 = no8
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for strings
	// TODO(armen): Implement repeat for chunk
	// TODO(armen): Implement repeat for uuid
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr

	err = repeat.Send(output)
	if err != nil {
		t.Fatal(err)
	}
	transit, err := Recv(input)
	if err != nil {
		t.Fatal(err)
	}

	tr := transit.(*Repeat)
	if tr.Sequence != 123 {
		t.Fatalf("expected %d, got %d", 123, tr.Sequence)
	}
	if reflect.DeepEqual(tr.No1, no1) {
		t.Fatalf("expected %v, got %v", no1, tr.No1)
	}
	if reflect.DeepEqual(tr.No2, no2) {
		t.Fatalf("expected %v, got %v", no2, tr.No2)
	}
	if reflect.DeepEqual(tr.No4, no4) {
		t.Fatalf("expected %v, got %v", no4, tr.No4)
	}
	if reflect.DeepEqual(tr.No8, no8) {
		t.Fatalf("expected %v, got %v", no8, tr.No8)
	}
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for strings
	// TODO(armen): Implement repeat for chunk
	// TODO(armen): Implement repeat for uuid
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr
	// TODO(armen): Implement repeat for string and longstr

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
