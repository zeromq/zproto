<?php

/**
 * Library Example is 100% generated. If you edit this file, you will
 * lose your changes at the next build cycle.
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
 * Demonstrate custom-defined types
 */
class Types extends Example
{
    /**
     * Message ID
     */
    const ID = 4;

    /**
     * @var number $sequence
     */
    public $sequence;

    /**
     * @var string $clientForename Given name
     */
    public $clientForename;

    /**
     * @var string $clientSurname Family name
     */
    public $clientSurname;

    /**
     * @var string $clientMobile Mobile phone number
     */
    public $clientMobile;

    /**
     * @var string $clientEmail Email address
     */
    public $clientEmail;

    /**
     * @var string $supplierForename Given name
     */
    public $supplierForename;

    /**
     * @var string $supplierSurname Family name
     */
    public $supplierSurname;

    /**
     * @var string $supplierMobile Mobile phone number
     */
    public $supplierMobile;

    /**
     * @var string $supplierEmail Email address
     */
    public $supplierEmail;

    /**
     * Unserializes a TYPES message
     *
     * @access public
     * @return void
     */
    public function unserialize()
    {
        parent::unserialize();

        $this->sequence         = $this->getNumber2();
        $this->clientForename   = $this->getString();
        $this->clientSurname    = $this->getString();
        $this->clientMobile     = $this->getString();
        $this->clientEmail      = $this->getString();
        $this->supplierForename = $this->getString();
        $this->supplierSurname  = $this->getString();
        $this->supplierMobile   = $this->getString();
        $this->supplierEmail    = $this->getString();
    }

    /**
     * Serializes a TYPES message
     *
     * @access public
     * @return serialized binary data
     */
    public function serialize()
    {
        $this->putNumber2($this->sequence);
        $this->putString($this->clientForename);
        $this->putString($this->clientSurname);
        $this->putString($this->clientMobile);
        $this->putString($this->clientEmail);
        $this->putString($this->supplierForename);
        $this->putString($this->supplierSurname);
        $this->putString($this->supplierMobile);
        $this->putString($this->supplierEmail);

        return $this->buffer;
    }
}
