package example

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"

	zmq "github.com/pebbe/zmq4"
)

// Structures struct
// This message contains a list and a hash.
type Structures struct {
	routingID []byte
	sequence  uint16
	Aliases   []string
	Headers   map[string]string
}

// NewStructures creates new Structures message.
func NewStructures() *Structures {
	structures := &Structures{}
	structures.Headers = make(map[string]string)
	return structures
}

// String returns print friendly name.
func (s *Structures) String() string {
	str := "ZPROTO_EXAMPLE_STRUCTURES:\n"
	str += fmt.Sprintf("    sequence = %v\n", s.sequence)
	str += fmt.Sprintf("    Aliases = %v\n", s.Aliases)
	str += fmt.Sprintf("    Headers = %v\n", s.Headers)
	return str
}

// Marshal serializes the message.
func (s *Structures) Marshal() ([]byte, error) {
	// Calculate size of serialized data
	bufferSize := 2 + 1 // Signature and message ID

	// sequence is a 2-byte integer
	bufferSize += 2

	// Aliases is an array of strings
	bufferSize += 4 // Size is 4 bytes
	// Add up size of string contents
	for _, val := range s.Aliases {
		bufferSize += 4 + len(val)
	}

	// Headers is a hash table
	bufferSize += 4 // Size is 4 bytes
	for key, val := range s.Headers {
		bufferSize += 1 + len(key)
		bufferSize += 4 + len(val)
	}

	// Now serialize the message
	tmpBuf := make([]byte, bufferSize)
	tmpBuf = tmpBuf[:0]
	buffer := bytes.NewBuffer(tmpBuf)
	binary.Write(buffer, binary.BigEndian, Signature)
	binary.Write(buffer, binary.BigEndian, StructuresID)

	// sequence
	binary.Write(buffer, binary.BigEndian, s.sequence)

	// Aliases
	binary.Write(buffer, binary.BigEndian, uint32(len(s.Aliases)))
	for _, val := range s.Aliases {
		putLongString(buffer, val)
	}

	// Headers
	binary.Write(buffer, binary.BigEndian, uint32(len(s.Headers)))
	for key, val := range s.Headers {
		putString(buffer, key)
		putLongString(buffer, val)
	}

	return buffer.Bytes(), nil
}

// Unmarshal unmarshals the message.
func (s *Structures) Unmarshal(frames ...[]byte) error {
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
		return fmt.Errorf("invalid signature %X != %X", Signature, signature)
	}

	// Get message id and parse per message type
	var id uint8
	binary.Read(buffer, binary.BigEndian, &id)
	if id != StructuresID {
		return errors.New("malformed Structures message")
	}
	// sequence
	binary.Read(buffer, binary.BigEndian, &s.sequence)
	// Aliases
	var aliasesSize uint32
	binary.Read(buffer, binary.BigEndian, &aliasesSize)
	for ; aliasesSize != 0; aliasesSize-- {
		s.Aliases = append(s.Aliases, getLongString(buffer))
	}
	// Headers
	var headersSize uint32
	binary.Read(buffer, binary.BigEndian, &headersSize)
	for ; headersSize != 0; headersSize-- {
		key := getString(buffer)
		val := getLongString(buffer)
		s.Headers[key] = val
	}

	return nil
}

// Send sends marshaled data through 0mq socket.
func (s *Structures) Send(socket *zmq.Socket) (err error) {
	frame, err := s.Marshal()
	if err != nil {
		return err
	}

	socType, err := socket.GetType()
	if err != nil {
		return err
	}

	// If we're sending to a ROUTER, we send the routingID first
	if socType == zmq.ROUTER {
		_, err = socket.SendBytes(s.routingID, zmq.SNDMORE)
		if err != nil {
			return err
		}
	}

	// Now send the data frame
	_, err = socket.SendBytes(frame, 0)
	if err != nil {
		return err
	}

	return err
}

// RoutingID returns the routingID for this message, routingID should be set
// whenever talking to a ROUTER.
func (s *Structures) RoutingID() []byte {
	return s.routingID
}

// SetRoutingID sets the routingID for this message, routingID should be set
// whenever talking to a ROUTER.
func (s *Structures) SetRoutingID(routingID []byte) {
	s.routingID = routingID
}

// SetSequence sets the sequence.
func (s *Structures) SetSequence(sequence uint16) {
	s.sequence = sequence
}

// Sequence returns the sequence.
func (s *Structures) Sequence() uint16 {
	return s.sequence
}
