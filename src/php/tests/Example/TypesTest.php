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
 * Demonstrate custom-defined types
 */
class TypesTest extends \PHPUnit_Framework_TestCase
{
    public function testTypes()
    {
        $types = new Types();
        $types->sequence         = 123;
        $types->clientForename   = "Lucius Junius";
        $types->clientSurname    = "Brutus";
        $types->clientMobile     = "01234567890";
        $types->clientEmail      = "brutus@example.com";
        $types->supplierForename = "Leslie";
        $types->supplierSurname  = "Lamport";
        $types->supplierMobile   = "01987654321";
        $types->supplierEmail    = "lamport@example.com";

        $msg = $types->serialize();
        $this->assertSame("dee674a1bcac455b7cd1801f4008c65d0a37b2ea", sha1($msg));

        $typesRcvd = new Types($msg);
        $typesRcvd->unserialize();

        $msg = $typesRcvd->serialize();
        $this->assertSame("dee674a1bcac455b7cd1801f4008c65d0a37b2ea", sha1($msg));

        $this->assertEquals($types, $typesRcvd);

    }
}
