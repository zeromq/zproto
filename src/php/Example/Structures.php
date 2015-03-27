<?php

/**
 * Library Example is 100% generated. If you edit this file, you will
 * lose your changes at the next build cycle.
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

namespace Zproto\Example;

use Zproto\Example;

/**
 * This message contains a list and a hash.
 */
class Structures extends Example
{
    /**
     * Message ID
     */
    const ID = 2;

    /**
     * @var number $sequence
     */
    public $sequence;

    /**
     * @var array $aliases List of strings
     */
    public $aliases;

    /**
     * @var array $headers Other random properties
     */
    public $headers;

    /**
     * Unserializes a STRUCTURES message
     *
     * @access public
     * @return void
     */
    public function unserialize()
    {
        parent::unserialize();

        $this->sequence = $this->getNumber2();
        $this->aliases  = $this->getArray();
        $this->headers  = $this->getHash();
    }

    /**
     * Serializes a STRUCTURES message
     *
     * @access public
     * @return serialized binary data
     */
    public function serialize()
    {
        $this->putNumber2($this->sequence);
        $this->putArray($this->aliases);
        $this->putHash($this->headers);

        return $this->buffer;
    }
}
