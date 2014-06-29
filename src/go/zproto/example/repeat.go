package example

import (
	zmq "github.com/pebbe/zmq4"

	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
)

// Demonstrates repeating fields
type Repeat struct {
	routingId       []byte
	Sequence        uint16
	No1             []byte
	No2             []uint16
	No4             []uint32
	No8             []uint64
	Str             string
	Lstr            string
	Strs            []string
	Chunks          []byte
	Uuids           []byte
	PersonsForename string
	PersonsSurname  string
	PersonsMobile   string
	PersonsEmail    string
}

// New creates new Repeat message.
func NewRepeat() *Repeat {
	repeat := &Repeat{}
	return repeat
}

// String returns print friendly name.
func (r *Repeat) String() string {
	str := "ZPROTO_EXAMPLE_REPEAT:\n"
	str += fmt.Sprintf("    Sequence = %v\n", r.Sequence)
	str += fmt.Sprintf("    No1 = %v\n", r.No1)
	str += fmt.Sprintf("    No2 = %v\n", r.No2)
	str += fmt.Sprintf("    No4 = %v\n", r.No4)
	str += fmt.Sprintf("    No8 = %v\n", r.No8)
	str += fmt.Sprintf("    Str = %v\n", r.Str)
	str += fmt.Sprintf("    Lstr = %v\n", r.Lstr)
	str += fmt.Sprintf("    Strs = %v\n", r.Strs)
	str += fmt.Sprintf("    Chunks = %v\n", r.Chunks)
	str += fmt.Sprintf("    Uuids = %v\n", r.Uuids)
	str += fmt.Sprintf("    PersonsForename = %v\n", r.PersonsForename)
	str += fmt.Sprintf("    PersonsSurname = %v\n", r.PersonsSurname)
	str += fmt.Sprintf("    PersonsMobile = %v\n", r.PersonsMobile)
	str += fmt.Sprintf("    PersonsEmail = %v\n", r.PersonsEmail)
	return str
}

// Marshal serializes the message.
func (r *Repeat) Marshal() ([]byte, error) {
	// Calculate size of serialized data
	bufferSize := 2 + 1 // Signature and message ID

	// Sequence is a 2-byte integer
	bufferSize += 2

	// No1 is a 1-byte integer
	bufferSize += 1

	// No2 is a 2-byte integer
	bufferSize += 2

	// No4 is a 4-byte integer
	bufferSize += 4

	// No8 is a 8-byte integer
	bufferSize += 8

	// Str is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(r.Str)

	// Lstr is a string with 4-byte length
	bufferSize += 4 // Size is 4 bytes
	bufferSize += len(r.Lstr)

	// Strs is an array of strings
	bufferSize += 4 // Size is 4 bytes
	// Add up size of string contents
	for _, val := range r.Strs {
		bufferSize += 4 + len(val)
	}

	// Chunks is a block of []byte with one byte length
	bufferSize += 1 + len(r.Chunks)

	// Uuids is a block of []byte with one byte length
	bufferSize += 1 + len(r.Uuids)

	// PersonsForename is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(r.PersonsForename)

	// PersonsSurname is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(r.PersonsSurname)

	// PersonsMobile is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(r.PersonsMobile)

	// PersonsEmail is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(r.PersonsEmail)

	// Now serialize the message
	tmpBuf := make([]byte, bufferSize)
	tmpBuf = tmpBuf[:0]
	buffer := bytes.NewBuffer(tmpBuf)
	binary.Write(buffer, binary.BigEndian, Signature)
	binary.Write(buffer, binary.BigEndian, RepeatId)

	// Sequence
	binary.Write(buffer, binary.BigEndian, r.Sequence)

	// No1
	binary.Write(buffer, binary.BigEndian, r.No1)

	// No2
	binary.Write(buffer, binary.BigEndian, r.No2)

	// No4
	binary.Write(buffer, binary.BigEndian, r.No4)

	// No8
	binary.Write(buffer, binary.BigEndian, r.No8)

	// Str
	putString(buffer, r.Str)

	// Lstr
	putLongString(buffer, r.Lstr)

	// Strs
	binary.Write(buffer, binary.BigEndian, uint32(len(r.Strs)))
	for _, val := range r.Strs {
		putLongString(buffer, val)
	}

	// Chunks
	putBytes(buffer, r.Chunks)

	// Uuids
	putBytes(buffer, r.Uuids)

	// PersonsForename
	putString(buffer, r.PersonsForename)

	// PersonsSurname
	putString(buffer, r.PersonsSurname)

	// PersonsMobile
	putString(buffer, r.PersonsMobile)

	// PersonsEmail
	putString(buffer, r.PersonsEmail)

	return buffer.Bytes(), nil
}

// Unmarshals the message.
func (r *Repeat) Unmarshal(frames ...[]byte) error {
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
	if id != RepeatId {
		return errors.New("malformed Repeat message")
	}

	// Sequence
	binary.Read(buffer, binary.BigEndian, &r.Sequence)

	// No1
	binary.Read(buffer, binary.BigEndian, &r.No1)

	// No2
	binary.Read(buffer, binary.BigEndian, &r.No2)

	// No4
	binary.Read(buffer, binary.BigEndian, &r.No4)

	// No8
	binary.Read(buffer, binary.BigEndian, &r.No8)

	// Str
	r.Str = getString(buffer)

	// Lstr
	r.Lstr = getLongString(buffer)

	// Strs
	var strsSize uint32
	binary.Read(buffer, binary.BigEndian, &strsSize)
	for ; strsSize != 0; strsSize-- {
		r.Strs = append(r.Strs, getLongString(buffer))
	}

	// Chunks
	r.Chunks = getBytes(buffer)

	// Uuids
	r.Uuids = getBytes(buffer)

	// PersonsForename
	r.PersonsForename = getString(buffer)

	// PersonsSurname
	r.PersonsSurname = getString(buffer)

	// PersonsMobile
	r.PersonsMobile = getString(buffer)

	// PersonsEmail
	r.PersonsEmail = getString(buffer)

	return nil
}

// Sends marshaled data through 0mq socket.
func (r *Repeat) Send(socket *zmq.Socket) (err error) {
	frame, err := r.Marshal()
	if err != nil {
		return err
	}

	socType, err := socket.GetType()
	if err != nil {
		return err
	}

	// If we're sending to a ROUTER, we send the routingId first
	if socType == zmq.ROUTER {
		_, err = socket.SendBytes(r.routingId, zmq.SNDMORE)
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
func (r *Repeat) RoutingId() []byte {
	return r.routingId
}

// SetRoutingId sets the routingId for this message, routingId should be set
// whenever talking to a ROUTER.
func (r *Repeat) SetRoutingId(routingId []byte) {
	r.routingId = routingId
}
