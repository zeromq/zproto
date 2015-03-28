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
 * Log an event.
 */
class LogTest extends \PHPUnit_Framework_TestCase
{
    public function testLog()
    {
        $log = new Log();
        $log->sequence = 123;
        $log->level    = 2;
        $log->event    = 3;
        $log->node     = 45536;
        $log->peer     = 65535;
        $log->time     = 1427261426;
        $log->host     = "localhost";
        $log->data     = "This is the message to log";

        $msg = $log->serialize();
        $this->assertSame("b90f4926d4662b319c0ec113794b0f27d9336a23", sha1($msg));

        $logRcvd = new Log($msg);
        $logRcvd->unserialize();

        $msg = $logRcvd->serialize();
        $this->assertSame("b90f4926d4662b319c0ec113794b0f27d9336a23", sha1($msg));

        $this->assertEquals($log, $logRcvd);

    }
}
