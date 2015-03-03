package example

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"

	"github.com/zeromq/goczmq"
)

// Types struct
// Demonstrate custom-defined types
type Types struct {
	routingID        []byte
	sequence         uint16
	ClientForename   string
	ClientSurname    string
	ClientMobile     string
	ClientEmail      string
	SupplierForename string
	SupplierSurname  string
	SupplierMobile   string
	SupplierEmail    string
}

// NewTypes creates new Types message.
func NewTypes() *Types {
	types := &Types{}
	return types
}

// String returns print friendly name.
func (t *Types) String() string {
	str := "ZPROTO_EXAMPLE_TYPES:\n"
	str += fmt.Sprintf("    sequence = %v\n", t.sequence)
	str += fmt.Sprintf("    ClientForename = %v\n", t.ClientForename)
	str += fmt.Sprintf("    ClientSurname = %v\n", t.ClientSurname)
	str += fmt.Sprintf("    ClientMobile = %v\n", t.ClientMobile)
	str += fmt.Sprintf("    ClientEmail = %v\n", t.ClientEmail)
	str += fmt.Sprintf("    SupplierForename = %v\n", t.SupplierForename)
	str += fmt.Sprintf("    SupplierSurname = %v\n", t.SupplierSurname)
	str += fmt.Sprintf("    SupplierMobile = %v\n", t.SupplierMobile)
	str += fmt.Sprintf("    SupplierEmail = %v\n", t.SupplierEmail)
	return str
}

// Marshal serializes the message.
func (t *Types) Marshal() ([]byte, error) {
	// Calculate size of serialized data
	bufferSize := 2 + 1 // Signature and message ID

	// sequence is a 2-byte integer
	bufferSize += 2

	// ClientForename is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.ClientForename)

	// ClientSurname is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.ClientSurname)

	// ClientMobile is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.ClientMobile)

	// ClientEmail is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.ClientEmail)

	// SupplierForename is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.SupplierForename)

	// SupplierSurname is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.SupplierSurname)

	// SupplierMobile is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.SupplierMobile)

	// SupplierEmail is a string with 1-byte length
	bufferSize++ // Size is one byte
	bufferSize += len(t.SupplierEmail)

	// Now serialize the message
	tmpBuf := make([]byte, bufferSize)
	tmpBuf = tmpBuf[:0]
	buffer := bytes.NewBuffer(tmpBuf)
	binary.Write(buffer, binary.BigEndian, Signature)
	binary.Write(buffer, binary.BigEndian, TypesID)

	// sequence
	binary.Write(buffer, binary.BigEndian, t.sequence)

	// ClientForename
	putString(buffer, t.ClientForename)

	// ClientSurname
	putString(buffer, t.ClientSurname)

	// ClientMobile
	putString(buffer, t.ClientMobile)

	// ClientEmail
	putString(buffer, t.ClientEmail)

	// SupplierForename
	putString(buffer, t.SupplierForename)

	// SupplierSurname
	putString(buffer, t.SupplierSurname)

	// SupplierMobile
	putString(buffer, t.SupplierMobile)

	// SupplierEmail
	putString(buffer, t.SupplierEmail)

	return buffer.Bytes(), nil
}

// Unmarshal unmarshals the message.
func (t *Types) Unmarshal(frames ...[]byte) error {
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
	if id != TypesID {
		return errors.New("malformed Types message")
	}
	// sequence
	binary.Read(buffer, binary.BigEndian, &t.sequence)
	// ClientForename
	t.ClientForename = getString(buffer)
	// ClientSurname
	t.ClientSurname = getString(buffer)
	// ClientMobile
	t.ClientMobile = getString(buffer)
	// ClientEmail
	t.ClientEmail = getString(buffer)
	// SupplierForename
	t.SupplierForename = getString(buffer)
	// SupplierSurname
	t.SupplierSurname = getString(buffer)
	// SupplierMobile
	t.SupplierMobile = getString(buffer)
	// SupplierEmail
	t.SupplierEmail = getString(buffer)

	return nil
}

// Send sends marshaled data through 0mq socket.
func (t *Types) Send(sock *goczmq.Sock) (err error) {
	frame, err := t.Marshal()
	if err != nil {
		return err
	}

	socType := sock.GetType()
	if err != nil {
		return err
	}

	// If we're sending to a ROUTER, we send the routingID first
	if socType == goczmq.ROUTER {
		err = sock.SendFrame(t.routingID, goczmq.MORE)
		if err != nil {
			return err
		}
	}

	// Now send the data frame
	err = sock.SendFrame(frame, 0)
	if err != nil {
		return err
	}

	return err
}

// RoutingID returns the routingID for this message, routingID should be set
// whenever talking to a ROUTER.
func (t *Types) RoutingID() []byte {
	return t.routingID
}

// SetRoutingID sets the routingID for this message, routingID should be set
// whenever talking to a ROUTER.
func (t *Types) SetRoutingID(routingID []byte) {
	t.routingID = routingID
}

// SetSequence sets the sequence.
func (t *Types) SetSequence(sequence uint16) {
	t.sequence = sequence
}

// Sequence returns the sequence.
func (t *Types) Sequence() uint16 {
	return t.sequence
}
