// Package example is 100% generated. If you edit this file,
// you will lose your changes at the next build cycle.
// DO NOT MAKE ANY CHANGES YOU WISH TO KEEP.
//
// The correct places for commits are:
//  - The XML model used for this code generation: zproto_example.xml
//  - The code generation script that built this file: zproto_codec_go
package example

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"

	"github.com/zeromq/goczmq"
)

const (
	// Signature is put into every protocol message and lets us filter bogus
	// or unknown protocols. It is a 4-bit number from 0 to 15. Use a unique value
	// for each protocol you write, at least.
	Signature uint16 = 0xAAA0 | 0
	Version          = 1
)

// Definition of message IDs
const (
	LogID        uint8 = 1
	StructuresID uint8 = 2
	BinaryID     uint8 = 3
	TypesID      uint8 = 4
)

// Transit is a codec interface
type Transit interface {
	Marshal() ([]byte, error)
	Unmarshal(...[]byte) error
	String() string
	Send(*goczmq.Sock) error
	SetRoutingID([]byte)
	RoutingID() []byte
	SetSequence(uint16)
	Sequence() uint16
}

// Unmarshal unmarshals data from raw frames.
func Unmarshal(frames ...[]byte) (t Transit, err error) {
	if frames == nil {
		return nil, errors.New("can't unmarshal an empty message")
	}
	var buffer *bytes.Buffer

	// Check the signature
	var signature uint16
	buffer = bytes.NewBuffer(frames[0])
	binary.Read(buffer, binary.BigEndian, &signature)
	if signature != Signature {
		// Invalid signature
		return nil, fmt.Errorf("invalid signature %X != %X", Signature, signature)
	}

	// Get message id and parse per message type
	var id uint8
	binary.Read(buffer, binary.BigEndian, &id)

	switch id {
	case LogID:
		t = NewLog()
	case StructuresID:
		t = NewStructures()
	case BinaryID:
		t = NewBinary()
	case TypesID:
		t = NewTypes()
	}
	err = t.Unmarshal(frames...)

	return t, err
}

// Recv receives marshaled data from a 0mq socket.
func Recv(sock *goczmq.Sock) (t Transit, err error) {
	return recv(sock, 0)
}

// RecvNoWait receives marshaled data from 0mq socket. It won't wait for input.
func RecvNoWait(sock *goczmq.Sock) (t Transit, err error) {
	return recv(sock, goczmq.DONTWAIT)
}

// recv receives marshaled data from 0mq socket.
func recv(sock *goczmq.Sock, flag int) (t Transit, err error) {
	var frames [][]byte

	if flag == goczmq.DONTWAIT {
		frames, err = sock.RecvMessageNoWait()
	} else {
		frames, err = sock.RecvMessage()
	}

	if err != nil {
		return nil, err
	}

	sType := sock.GetType()
	if err != nil {
		return nil, err
	}

	var routingID []byte
	// If message came from a router socket, first frame is routingID
	if sType == goczmq.ROUTER {
		if len(frames) <= 1 {
			return nil, errors.New("no routingID")
		}
		routingID = frames[0]
		frames = frames[1:]
	}

	t, err = Unmarshal(frames...)
	if err != nil {
		return nil, err
	}

	if sType == goczmq.ROUTER {
		t.SetRoutingID(routingID)
	}
	return t, err
}

// Clone clones a message.
func Clone(t Transit) Transit {

	switch msg := t.(type) {
	case *Log:
		cloned := NewLog()
		routingID := make([]byte, len(msg.RoutingID()))
		copy(routingID, msg.RoutingID())
		cloned.SetRoutingID(routingID)
		cloned.sequence = msg.sequence
		cloned.Version = msg.Version
		cloned.Level = msg.Level
		cloned.Event = msg.Event
		cloned.Node = msg.Node
		cloned.Peer = msg.Peer
		cloned.Time = msg.Time
		cloned.Host = msg.Host
		cloned.Data = msg.Data
		return cloned

	case *Structures:
		cloned := NewStructures()
		routingID := make([]byte, len(msg.RoutingID()))
		copy(routingID, msg.RoutingID())
		cloned.SetRoutingID(routingID)
		cloned.sequence = msg.sequence
		for idx, str := range msg.Aliases {
			cloned.Aliases[idx] = str
		}
		for key, val := range msg.Headers {
			cloned.Headers[key] = val
		}
		return cloned

	case *Binary:
		cloned := NewBinary()
		routingID := make([]byte, len(msg.RoutingID()))
		copy(routingID, msg.RoutingID())
		cloned.SetRoutingID(routingID)
		cloned.sequence = msg.sequence
		copy(cloned.Flags, msg.Flags)
		cloned.PublicKey = append(cloned.PublicKey, msg.PublicKey...)
		cloned.Identifier = append(cloned.Identifier, msg.Identifier...)
		cloned.Address = append(cloned.Address, msg.Address...)
		cloned.Content = append(cloned.Content, msg.Content...)
		return cloned

	case *Types:
		cloned := NewTypes()
		routingID := make([]byte, len(msg.RoutingID()))
		copy(routingID, msg.RoutingID())
		cloned.SetRoutingID(routingID)
		cloned.sequence = msg.sequence
		cloned.ClientForename = msg.ClientForename
		cloned.ClientSurname = msg.ClientSurname
		cloned.ClientMobile = msg.ClientMobile
		cloned.ClientEmail = msg.ClientEmail
		cloned.SupplierForename = msg.SupplierForename
		cloned.SupplierSurname = msg.SupplierSurname
		cloned.SupplierMobile = msg.SupplierMobile
		cloned.SupplierEmail = msg.SupplierEmail
		return cloned
	}

	return nil
}

// putString marshals a string into the buffer.
func putString(buffer *bytes.Buffer, str string) {
	size := len(str)
	binary.Write(buffer, binary.BigEndian, byte(size))
	binary.Write(buffer, binary.BigEndian, []byte(str[0:size]))
}

// getString unmarshals a string from the buffer.
func getString(buffer *bytes.Buffer) string {
	var size byte
	binary.Read(buffer, binary.BigEndian, &size)
	str := make([]byte, size)
	binary.Read(buffer, binary.BigEndian, &str)
	return string(str)
}

// putLongString marshals a string into the buffer.
func putLongString(buffer *bytes.Buffer, str string) {
	size := len(str)
	binary.Write(buffer, binary.BigEndian, uint32(size))
	binary.Write(buffer, binary.BigEndian, []byte(str[0:size]))
}

// getLongString unmarshals a string from the buffer.
func getLongString(buffer *bytes.Buffer) string {
	var size uint32
	binary.Read(buffer, binary.BigEndian, &size)
	str := make([]byte, size)
	binary.Read(buffer, binary.BigEndian, &str)
	return string(str)
}

// putBytes marshals []byte into the buffer.
func putBytes(buffer *bytes.Buffer, data []byte) {
	size := uint32(len(data))
	binary.Write(buffer, binary.BigEndian, size)
	binary.Write(buffer, binary.BigEndian, data)
}

// getBytes unmarshals []byte from the buffer.
func getBytes(buffer *bytes.Buffer) []byte {
	var size uint32
	binary.Read(buffer, binary.BigEndian, &size)
	data := make([]byte, size)
	binary.Read(buffer, binary.BigEndian, &data)
	return data
}
