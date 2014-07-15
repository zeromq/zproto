package example

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"strconv"

	zmq "github.com/pebbe/zmq4"
)

// Log an event.
type Log struct {
	routingId []byte
	Sequence  uint16
	Version   uint16
	Level     byte
	Event     byte
	Node      uint16
	Peer      uint16
	Time      uint64
	Host      string
	Data      string
}

// New creates new Log message.
func NewLog() *Log {
	log := &Log{}
	return log
}

// String returns print friendly name.
func (l *Log) String() string {
	str := "ZPROTO_EXAMPLE_LOG:\n"
	str += fmt.Sprintf("    Sequence = %v\n", l.Sequence)
	str += fmt.Sprintf("    Version = %v\n", l.Version)
	str += fmt.Sprintf("    Level = %v\n", l.Level)
	str += fmt.Sprintf("    Event = %v\n", l.Event)
	str += fmt.Sprintf("    Node = %v\n", l.Node)
	str += fmt.Sprintf("    Peer = %v\n", l.Peer)
	str += fmt.Sprintf("    Time = %v\n", l.Time)
	str += fmt.Sprintf("    Host = %v\n", l.Host)
	str += fmt.Sprintf("    Data = %v\n", l.Data)
	return str
}

// Marshal serializes the message.
func (l *Log) Marshal() ([]byte, error) {
	// Calculate size of serialized data
	bufferSize := 2 + 1 // Signature and message ID

	// Sequence is a 2-byte integer
	bufferSize += 2

	// Version is a 2-byte integer
	bufferSize += 2

	// Level is a 1-byte integer
	bufferSize += 1

	// Event is a 1-byte integer
	bufferSize += 1

	// Node is a 2-byte integer
	bufferSize += 2

	// Peer is a 2-byte integer
	bufferSize += 2

	// Time is a 8-byte integer
	bufferSize += 8

	// Host is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(l.Host)

	// Data is a string with 4-byte length
	bufferSize += 4 // Size is 4 bytes
	bufferSize += len(l.Data)

	// Now serialize the message
	tmpBuf := make([]byte, bufferSize)
	tmpBuf = tmpBuf[:0]
	buffer := bytes.NewBuffer(tmpBuf)
	binary.Write(buffer, binary.BigEndian, Signature)
	binary.Write(buffer, binary.BigEndian, LogId)

	// Sequence
	binary.Write(buffer, binary.BigEndian, l.Sequence)

	// Version
	value, _ := strconv.ParseUint("3", 10, 2*8)
	binary.Write(buffer, binary.BigEndian, uint16(value))

	// Level
	binary.Write(buffer, binary.BigEndian, l.Level)

	// Event
	binary.Write(buffer, binary.BigEndian, l.Event)

	// Node
	binary.Write(buffer, binary.BigEndian, l.Node)

	// Peer
	binary.Write(buffer, binary.BigEndian, l.Peer)

	// Time
	binary.Write(buffer, binary.BigEndian, l.Time)

	// Host
	putString(buffer, l.Host)

	// Data
	putLongString(buffer, l.Data)

	return buffer.Bytes(), nil
}

// Unmarshals the message.
func (l *Log) Unmarshal(frames ...[]byte) error {
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
	if id != LogId {
		return errors.New("malformed Log message")
	}

	// Sequence
	binary.Read(buffer, binary.BigEndian, &l.Sequence)

	// Version
	binary.Read(buffer, binary.BigEndian, &l.Version)
	if l.Version != 3 {
		return errors.New("malformed Version message")
	}

	// Level
	binary.Read(buffer, binary.BigEndian, &l.Level)

	// Event
	binary.Read(buffer, binary.BigEndian, &l.Event)

	// Node
	binary.Read(buffer, binary.BigEndian, &l.Node)

	// Peer
	binary.Read(buffer, binary.BigEndian, &l.Peer)

	// Time
	binary.Read(buffer, binary.BigEndian, &l.Time)

	// Host
	l.Host = getString(buffer)

	// Data
	l.Data = getLongString(buffer)

	return nil
}

// Sends marshaled data through 0mq socket.
func (l *Log) Send(socket *zmq.Socket) (err error) {
	frame, err := l.Marshal()
	if err != nil {
		return err
	}

	socType, err := socket.GetType()
	if err != nil {
		return err
	}

	// If we're sending to a ROUTER, we send the routingId first
	if socType == zmq.ROUTER {
		_, err = socket.SendBytes(l.routingId, zmq.SNDMORE)
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

// RoutingId returns the routingId for this message, routingId should be set
// whenever talking to a ROUTER.
func (l *Log) RoutingId() []byte {
	return l.routingId
}

// SetRoutingId sets the routingId for this message, routingId should be set
// whenever talking to a ROUTER.
func (l *Log) SetRoutingId(routingId []byte) {
	l.routingId = routingId
}
