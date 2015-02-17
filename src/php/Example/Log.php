<?php

/**
 * Library Example is 100% generated. If you edit this file,
 * you will lose your changes at the next build cycle.
 * DO NOT MAKE ANY CHANGES YOU WISH TO KEEP.
 *
 * The correct places for commits are:
 *  - The XML model used for this code generation: zproto_example_php.xml
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
 * Log an event.
 */
class Log extends Example
{
    /**
     * Message ID
     */
    const ID = 1;

    /**
     * @var number $sequence
     */
    public $sequence;

    /**
     * @var number $version Version
     */
    public $version;

    /**
     * @var number $level Log severity level
     */
    public $level;

    /**
     * @var number $event Type of event
     */
    public $event;

    /**
     * @var number $node Sending node
     */
    public $node;

    /**
     * @var number $peer Refers to this peer
     */
    public $peer;

    /**
     * @var number $time Log date/time
     */
    public $time;

    /**
     * @var string $host Originating hostname
     */
    public $host;

    /**
     * @var string $data Actual log message
     */
    public $data;

    /**
     * Unserializes a LOG message
     *
     * @access public
     * @return void
     */
    public function unserialize()
    {
        parent::unserialize();

        $this->sequence = $this->getNumber2();
        $this->version  = $this->getNumber2();
        $this->level    = $this->getNumber1();
        $this->event    = $this->getNumber1();
        $this->node     = $this->getNumber2();
        $this->peer     = $this->getNumber2();
        $this->time     = $this->getNumber8();
        $this->host     = $this->getString();
        $this->data     = $this->getString();
    }

    /**
     * Serializes a LOG message
     *
     * @access public
     * @return serialized binary data
     */
    public function serialize()
    {
        $this->putNumber2($this->sequence);
        $this->putNumber2($this->version);
        $this->putNumber1($this->level);
        $this->putNumber1($this->event);
        $this->putNumber2($this->node);
        $this->putNumber2($this->peer);
        $this->putNumber8($this->time);
        $this->putString($this->host);
        $this->putString($this->data);

        return $this->buffer;
    }
}
