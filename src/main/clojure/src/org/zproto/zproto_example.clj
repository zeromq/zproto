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
;;    Copyright (C) 2014 the Authors                                         
;;                                                                           
;;    Permission is hereby granted, free of charge, to any person obtaining  
;;    a copy of this software and associated documentation files (the        
;;    "Software"), to deal in the Software without restriction, including    
;;    without limitation the rights to use, copy, modify, merge, publish,    
;;    distribute, sublicense, and/or sell copies of the Software, and to     
;;    permit persons to whom the Software is furnished to do so, subject to  
;;    the following conditions:                                              
;;                                                                           
;;    The above copyright notice and this permission notice shall be included
;;    in all copies or substantial portions of the Software.                 
;;                                                                           
;;    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
;;    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF             
;;    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
;;    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   
;;    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   
;;    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      
;;    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 
;;    =========================================================================

(ns org.zproto.zproto-example
  (:require [zeromq.zmq :as zmq])
  (:import [org.zproto ZprotoExample]))

(defprotocol PZprotoExample
  (log [this sequence level event node peer time host data]
    [this routing-id sequence level event node peer time host data])
  (structures [this sequence aliases headers]
    [this routing-id sequence aliases headers])
  (binary [this sequence flags public-key identifier address content]
    [this routing-id sequence flags public-key identifier address content])
  (types [this sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email]
    [this routing-id sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email]))

(defrecord ZprotoExampleSocket [socket]
  PZprotoExample
  (log [this sequence level event node peer time host data]
    (ZprotoExample/sendLog socket sequence level event node peer time host data))
  (log [this routing-id sequence level event node peer time host data]
    (ZprotoExample/sendLog socket routing-id sequence level event node peer time host data))
  (structures [this sequence aliases headers]
    (ZprotoExample/sendStructures socket sequence aliases headers))
  (structures [this routing-id sequence aliases headers]
    (ZprotoExample/sendStructures socket routing-id sequence aliases headers))
  (binary [this sequence flags public-key identifier address content]
    (ZprotoExample/sendBinary socket sequence flags public-key identifier address content))
  (binary [this routing-id sequence flags public-key identifier address content]
    (ZprotoExample/sendBinary socket routing-id sequence flags public-key identifier address content))
  (types [this sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email]
    (ZprotoExample/sendTypes socket sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email))
  (types [this routing-id sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email]
    (ZprotoExample/sendTypes socket routing-id sequence client-forename client-surname client-mobile client-email supplier-forename supplier-surname supplier-mobile supplier-email)))


;;
;; message setters
;;

(defn id! [^ZprotoExample msg id]
  (.setId msg id))

(defn sequence! [^ZprotoExample msg sequence]
 (.setSequence msg sequence))

(defn level! [^ZprotoExample msg level]
 (.setLevel msg level))

(defn event! [^ZprotoExample msg event]
 (.setEvent msg event))

(defn node! [^ZprotoExample msg node]
 (.setNode msg node))

(defn peer! [^ZprotoExample msg peer]
 (.setPeer msg peer))

(defn time! [^ZprotoExample msg time]
 (.setTime msg time))

(defn host! [^ZprotoExample msg format & opts]
 (.setHost msg format (object-array opts)))

(defn data! [^ZprotoExample msg format & opts]
 (.setData msg format (object-array opts)))

(defn aliases! [^ZprotoExample msg aliases]
 (.setAliases msg aliases))

(defn headers! [^ZprotoExample msg headers]
 (.setHeaders msg headers))

(defn flags! [^ZprotoExample msg flags]
 (.setFlags msg flags))

(defn public-key! [^ZprotoExample msg public-key]
 (.setPublic_Key msg public-key))

(defn identifier! [^ZprotoExample msg identifier]
 (.setIdentifier msg identifier))

(defn address! [^ZprotoExample msg address]
 (.setAddress msg address))

(defn content! [^ZprotoExample msg content]
 (.setContent msg content))

(defn client-forename! [^ZprotoExample msg format & opts]
 (.setClient_Forename msg format (object-array opts)))

(defn client-surname! [^ZprotoExample msg format & opts]
 (.setClient_Surname msg format (object-array opts)))

(defn client-mobile! [^ZprotoExample msg format & opts]
 (.setClient_Mobile msg format (object-array opts)))

(defn client-email! [^ZprotoExample msg format & opts]
 (.setClient_Email msg format (object-array opts)))

(defn supplier-forename! [^ZprotoExample msg format & opts]
 (.setSupplier_Forename msg format (object-array opts)))

(defn supplier-surname! [^ZprotoExample msg format & opts]
 (.setSupplier_Surname msg format (object-array opts)))

(defn supplier-mobile! [^ZprotoExample msg format & opts]
 (.setSupplier_Mobile msg format (object-array opts)))

(defn supplier-email! [^ZprotoExample msg format & opts]
 (.setSupplier_Email msg format (object-array opts)))

(def context (zmq/context))

(defn client-socket [endpoint]
  (let [socket (doto (zmq/socket context :dealer)
                 (zmq/set-receive-timeout 1000)
                 (zmq/connect endpoint))]
    (->ZprotoExampleSocket socket)))

(defn server-socket [endpoint]
  (let [socket (doto (zmq/socket context :router)
                 (zmq/set-receive-timeout 1000)
                 (zmq/bind endpoint))]
     (->ZprotoExampleSocket socket)))

(defn recv [{:keys [socket]}]
  (ZprotoExample/recv socket))
