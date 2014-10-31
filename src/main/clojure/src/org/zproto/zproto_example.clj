;;  =========================================================================
;;    ZprotoExample - zproto example protocol
;;
;;    ** WARNING *************************************************************
;;    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
;;    your changes at the next build cycle. This is great for temporary printf
;;    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
;;    for commits are:
;;
;;    * The XML model used for this code generation: zproto_example.xml
;;    * The code generation script that built this file: zproto_codec_c
;;    ************************************************************************
    Copyright (C) 2014 the Authors                                         
                                                                           
    Permission is hereby granted, free of charge, to any person obtaining  
    a copy of this software and associated documentation files (the        
    "Software"), to deal in the Software without restriction, including    
    without limitation the rights to use, copy, modify, merge, publish,    
    distribute, sublicense, and/or sell copies of the Software, and to     
    permit persons to whom the Software is furnished to do so, subject to  
    the following conditions:                                              
                                                                           
    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.                 
                                                                           
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF             
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 
;;    =========================================================================

(ns org.zproto.zproto-example
  (:require [zeromq.zmq :as zmq])
  (:import [org.zproto ZprotoExample]))

(defprotocol PZprotoExample
  (log [this sequence level event node peer time host data])
  (structures [this sequence aliases headers])
  (binary [this sequence flags public-key identifier address content])
  (types [this sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email]))

(defrecord ZprotoExampleSocket [socket]
  PZprotoExample
  (log [this sequence level event node peer time host data]
    (ZprotoExample/sendLog socket sequence level event node peer time host data)
    (ZprotoExample/recv socket))
  (structures [this sequence aliases headers]
    (ZprotoExample/sendStructures socket sequence aliases headers)
    (ZprotoExample/recv socket))
  (binary [this sequence flags public-key identifier address content]
    (ZprotoExample/sendBinary socket sequence flags public-key identifier address content)
    (ZprotoExample/recv socket))
  (types [this sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email]
    (ZprotoExample/sendTypes socket sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email)
    (ZprotoExample/recv socket)))

(defn client-socket [endpoint]
  (let [context (zmq/context)
        socket (doto (zmq/socket context :dealer)
                 (zmq/set-receive-timeout 1000)
                 (zmq/connect endpoint))]
    (->ZprotoExampleSocket socket)))

(defn server-socket [endpoint]
  (let [context (zmq/context)
        socket (doto (zmq/socket context :router)
                 (zmq/set-receive-timeout 1000)
                 (zmq/bind endpoint))]
    (->ZprotoExampleSocket socket)))
