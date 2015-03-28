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

/**
 * Deliver a multi-part message.
 */
class BinaryTest extends \PHPUnit_Framework_TestCase
{
    public function testBinary()
    {
        $binary = new Binary();
        $binary->sequence   = 123;
        $binary->flags      = "b38c";
        $binary->publicKey  = "89f5ffe70d747869dfe8";
        $binary->identifier = hex2bin("3a60e6850a1e4cc15f3bfd4b42bc6b3e");

        $msg = $binary->serialize();
        $this->assertSame("740caf04158436b4a81b844135a8a1dc9e4f8a54", sha1($msg));

        $binaryRcvd = new Binary($msg);
        $binaryRcvd->unserialize();

        $msg = $binaryRcvd->serialize();
        $this->assertSame("740caf04158436b4a81b844135a8a1dc9e4f8a54", sha1($msg));

        $this->assertEquals($binary, $binaryRcvd);

    }
}
