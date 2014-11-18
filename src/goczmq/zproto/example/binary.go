package example

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"

	"github.com/zeromq/goczmq"
)

// Deliver a multi-part message.
type Binary struct {
	routingId  []byte
	sequence   uint16
	Flags      [4]byte
	PublicKey  []byte
	Identifier []byte
	Address    []byte
	Content    []byte
}

// New creates new Binary message.
func NewBinary() *Binary {
	binary := &Binary{}
	return binary
}

// String returns print friendly name.
func (b *Binary) String() string {
	str := "ZPROTO_EXAMPLE_BINARY:\n"
	str += fmt.Sprintf("    sequence = %v\n", b.sequence)
	str += fmt.Sprintf("    Flags = %v\n", b.Flags)
	str += fmt.Sprintf("    PublicKey = %v\n", b.PublicKey)
	str += fmt.Sprintf("    Identifier = %v\n", b.Identifier)
	str += fmt.Sprintf("    Address = %v\n", b.Address)
	str += fmt.Sprintf("    Content = %v\n", b.Content)
	return str
}

// Marshal serializes the message.
func (b *Binary) Marshal() ([]byte, error) {
	// Calculate size of serialized data
	bufferSize := 2 + 1 // Signature and message ID

	// sequence is a 2-byte integer
	bufferSize += 2

	// Flags is a block of [4]byte
	bufferSize += 4

	// PublicKey is a block of []byte with one byte length
	bufferSize += 1 + len(b.PublicKey)

	// Identifier is a block of []byte with one byte length
	bufferSize += 1 + len(b.Identifier)

	// Now serialize the message
	tmpBuf := make([]byte, bufferSize)
	tmpBuf = tmpBuf[:0]
	buffer := bytes.NewBuffer(tmpBuf)
	binary.Write(buffer, binary.BigEndian, Signature)
	binary.Write(buffer, binary.BigEndian, BinaryId)

	// sequence
	binary.Write(buffer, binary.BigEndian, b.sequence)

	// Flags
	binary.Write(buffer, binary.BigEndian, b.Flags)

	// PublicKey
	putBytes(buffer, b.PublicKey)

	// Identifier
	putBytes(buffer, b.Identifier)

	return buffer.Bytes(), nil
}

// Unmarshal unmarshals the message.
func (b *Binary) Unmarshal(frames ...[]byte) error {
	if frames == nil {
		return errors.New("Can't unmarshal empty message")
	}

	frame := frames[0]
	frames = frames[1:]

	buffer := bytes.NewBuffer(frame)

	// Get and check protocol signature
	var signature uint16
	binary.Read(buffer, binary.BigEndian, &signature)
	if signature != Signature {
		return errors.New("invalid signature")
	}

	// Get message id and parse per message type
	var id uint8
	binary.Read(buffer, binary.BigEndian, &id)
	if id != BinaryId {
		return errors.New("malformed Binary message")
	}
	// sequence
	binary.Read(buffer, binary.BigEndian, &b.sequence)
	// Flags
	binary.Read(buffer, binary.BigEndian, &b.Flags)
	// PublicKey
	b.PublicKey = getBytes(buffer)
	// Identifier
	b.Identifier = getBytes(buffer)
	// Address
	if 0 <= len(frames)-1 {
		b.Address = frames[0]
	}
	// Content
	if 1 <= len(frames)-1 {
		b.Content = frames[1]
	}

	return nil
}

// Send sends marshaled data through 0mq socket.
func (b *Binary) Send(sock *goczmq.Sock) (err error) {
	frame, err := b.Marshal()
	if err != nil {
		return err
	}

	socType := sock.GetType()
	if err != nil {
		return err
	}

	// If we're sending to a ROUTER, we send the routingId first
	if socType == goczmq.ROUTER {
		err = sock.SendBytes(b.routingId, goczmq.MORE)
		if err != nil {
			return err
		}
	}

	// Now send the data frame
	err = sock.SendBytes(frame, goczmq.MORE)
	if err != nil {
		return err
	}
	// Now send any frame fields, in order
	err = sock.SendBytes(b.Address, goczmq.MORE)
	err = sock.SendBytes(b.Content, 0)

	return err
}

// RoutingId returns the routingId for this message, routingId should be set
// whenever talking to a ROUTER.
func (b *Binary) RoutingId() []byte {
	return b.routingId
}

// SetRoutingId sets the routingId for this message, routingId should be set
// whenever talking to a ROUTER.
func (b *Binary) SetRoutingId(routingId []byte) {
	b.routingId = routingId
}

// Setsequence sets the sequence.
func (b *Binary) SetSequence(sequence uint16) {
	b.sequence = sequence
}

// sequence returns the sequence.
func (b *Binary) Sequence() uint16 {
	return b.sequence
}
