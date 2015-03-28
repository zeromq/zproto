<?php

/**
 * Library Example is 100% generated. If you edit this file,
 * you will lose your changes at the next build cycle.
 * DO NOT MAKE ANY CHANGES YOU WISH TO KEEP.
 *
 * The correct places for commits are:
 *  - The XML model used for this code generation: zproto_example.xml
 *  - The code generation script that built this file: zproto_codec_php
 *
 *  Copyright (C) 2014 the Authors                                         
 *                                                                         
 *  Permission is hereby granted, free of charge, to any person obtaining  
 *  a copy of this software and associated documentation files (the        
 *  "Software"), to deal in the Software without restriction, including    
 *  without limitation the rights to use, copy, modify, merge, publish,    
 *  distribute, sublicense, and/or sell copies of the Software, and to     
 *  permit persons to whom the Software is furnished to do so, subject to  
 *  the following conditions:                                              
 *                                                                         
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.                 
 *                                                                         
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF             
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 
 */

namespace Zproto;

/**
 * A message in zproto example protocol.
 */
class Example
{
    /**
     * Buffer holds the serialized data.
     */
    protected $buffer;

    /**
     * Needle
     */
    protected $needle = 0;

    /**
     * Example constructor
     *
     * @param string $msg The message
     */
    public function __construct($msg=NULL)
    {
        // puts/serializes the message signature if $msg is null
        if ($msg == NULL) {
            // 0xAAA0 is the signature of the messages
            $this->buffer = pack('C*', 0xAA, 0xA0 | 0, static::ID);
        } else {
            $this->buffer = $msg;
        }
    }

    /**
     * Validates the signature of the message.
     */
    public function hasValidSignature()
    {
        if (!is_string($this->buffer)) {
            return FALSE;
        }

        // A message has to be at least three bytes, two bytes for
        // signature and one byte for the ID of the message
        return strlen($this->buffer) >= 3 &&
               ord($this->buffer[0]) == 0xAA &&
               ord($this->buffer[1]) == 0xA0 | 0;
    }

    /**
     * Validates the signature and type/ID of the message.
     */
    public function isValid()
    {
        return $this->getId() == static::ID;
    }

    /**
     * Returns the ID of the message.
     */
    public function getId()
    {
        if ($this->hasValidSignature()) {
            return ord($this->buffer[2]);
        }

        return FALSE;
    }

    /**
     * Unserializes a message. This method is used by unserialize method of
     * the messages in the child class and shouldn't be used directly.
     */
    protected function unserialize()
    {
        if (!$this->isValid()) {
            throw new \Exception(
                'Invalid signature or message type: ' . bin2hex($this->buffer(3))
            );
        }

        // It's a valid message, move the needle to point to the forth byte (0..3)
        $this->needle = 3;
    }

    /**
     * Serializes the given string
     *
     * @param string $string The string being serialized
     */
    protected function putString($string)
    {
        // Put the length of the string first
        $this->putNumber(strlen($string));
        $this->buffer .= $string;
    }

    /**
     * Serializes the given string
     *
     * @param string $string The string being serialized
     */
    protected function putLongString($string)
    {
        $this->putNumber4(strlen($string));
        $this->buffer .= $string;
    }

    /**
     * Unserializes a string. A serialized string consists of one byte length
     * followed by the string itself.
     */
    protected function getString()
    {
        $len = ord($this->buffer());

        return $this->buffer($len);
    }

    /**
     * Unserializes a long string. A serialized long string consists of 4-byte length
     * followed by the string itself.
     */
    protected function getLongString()
    {
        $len = $this->getNumber4();

        return $this->buffer($len);
    }

    /**
     * Serializes a string. A serialized string consists of one byte length
     * followed by the string itself.
     *
     * @param string $string The string being serialized
     */
    protected function putBytes($string)
    {
        $this->putNumber4(strlen($string));
        $this->buffer .= $string;
    }

    /**
     * Serializes octets.
     *
     * @param string $string The string being serialized
     * @param int    $len    The length of the string
     */
    protected function putOctets($string, $len)
    {
        $this->buffer .= substr($string, 0, $len);
    }

    /**
     * Unserializes byte array which is could have a 64-bit long in length which
     * is huge. The 64-bit length is followed by the data itself. We treat the
     * byte array as PHP strings which is a binary safe string.
     */
    protected function getBytes()
    {
        $len = $this->getNumber4();

        return $this->buffer($len);
    }

    /**
     * Unserializes octets.
     */
    protected function getOctets($len)
    {
        return $this->buffer($len);
    }

    /**
     * Unserializes a 8-bit number.
     */
    protected function getNumber()
    {
        $res = unpack('Cnum', $this->buffer(1));

        return $res['num'];
    }

    /**
     * Unserializes a 16-bit number.
     */
    protected function getNumber2()
    {
        $res = unpack('nnum', $this->buffer(2));

        return $res['num'];
    }

    /**
     * Unserializes a 32-bit number.
     */
    protected function getNumber4()
    {
        $res = unpack('Nnum', $this->buffer(4));

        return $res['num'];
    }

    /**
     * Unserializes a 64-bit number.
     */
    protected function getNumber8()
    {
        $num = unpack('Nhigher/Nlower', $this->buffer(8));
        $num = $num['higher'] << 32 | $num['lower'];

        return $num;
    }

    /**
     * Unserializes a hash.
     */
    protected function getHash()
    {
        $hash = [];
        $size = $this->getNumber4();

        for ($i = 0; $i < $size; $i++) {
            $key   = $this->getString();
            $value = $this->getLongString();

            $hash[$key] = $value;
        }

        return $hash;
    }

    /**
     * Unserializes an array of strings.
     */
    protected function getStrings()
    {
        $strings = [];
        $size = $this->getNumber4();

        for ($i = 0; $i < $size; $i++) {
            $strings[] = $this->getLongString();
        }

        return $strings;
    }

    /**
     * Unserializes a UUID.
     */
    protected function getUuid()
    {
        return $this->buffer(16);
    }

    /**
     * Serializes a 8-bit number.
     *
     * @param number $number The number being serialized
     */
    protected function putNumber($number)
    {
        $this->buffer .= chr($number);
    }

    /**
     * Serializes a 16-bit number.
     *
     * @param number $number The number being serialized
     */
    protected function putNumber2($number)
    {
        $this->buffer .= pack('n', $number);
    }

    /**
     * Serializes a 32-bit number.
     *
     * @param number $number The number being serialized
     */
    protected function putNumber4($number)
    {
        $this->buffer .= pack('N', $number);
    }

    /**
     * Serializes a 64-bit number.
     *
     * @param number $number The number being serialized
     */
    protected function putNumber8($number)
    {
        $this->buffer .= pack('NN', $number & 0xffffffff00000000, $number & 0x00000000ffffffff);
    }

    /**
     * Serializes a hash.
     *
     * @param array $hash The hash being serialized
     */
    protected function putHash(array $hash)
    {
        $this->putNumber4(count($hash));
        foreach ($hash as $key => $val) {
            $this->putString($key);
            $this->putLongString($val);
        }
    }

    /**
     * Serializes an array of string.
     *
     * @param array $strings The array of string
     */
    protected function putStrings(array $strings)
    {
        $this->putNumber4(count($strings));
        foreach ($strings as $str) {
            $this->putLongString($str);
        }
    }

    /**
     * Serializes a UUID.
     *
     * @param string $uuid
     */
    protected function putUuid($uuid)
    {
        $this->buffer .= $uuid;
    }

    /**
     * Buffer returns a bytes array size of $size started at $this->needle
     *
     * @param int $size Number of bytes
     */
    protected function buffer($size = 1)
    {
        // Make sure buffer is initialized
        if (!isset($this->buffer[(int) $this->needle+((int) $size-1)])) {
            throw new \Exception('Malformed message');
        }

        $bytes = substr($this->buffer, $this->needle, $size);
        $this->needle += $size;

        return $bytes;
    }
}
