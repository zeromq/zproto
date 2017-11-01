module Example

open System
open System.Text
open FsNetMQ
open FsNetMQ.Stream
open FsNetMQ.Stream.Reader

[<LiteralAttribute>]
let LogMessageId = 1uy
[<LiteralAttribute>]
let StructuresMessageId = 2uy
[<LiteralAttribute>]
let BinaryMessageId = 3uy
[<LiteralAttribute>]
let TypesMessageId = 4uy

type Log = {
        sequence : uint16
        level : byte
        event : byte
        node : uint16
        peer : uint16
        time : uint64
        host : string
        data : string
    }

type Structures = {
        sequence : uint16
        aliases : string list
        headers : Map<string,string>
    }

type Binary = {
        sequence : uint16
        flags : byte[]
        publicKey : byte[]
        identifier : System.Guid
    }

type Types = {
        sequence : uint16
        clientForename : string
        clientSurname : string
        clientMobile : string
        clientEmail : string
        supplierForename : string
        supplierSurname : string
        supplierMobile : string
        supplierEmail : string
    }

type T =
    | Log of Log
    | Structures of Structures
    | Binary of Binary
    | Types of Types

module Log =
    let getMessageSize (msg:Log) =
        0 +
            2 +
            2 +
            1 +
            1 +
            2 +
            2 +
            8 +
            1 + String.length msg.host +
            4 + String.length msg.data +
            0

    let write (msg:Log) stream =
        stream
        |> Stream.writeNumber2 msg.sequence
        |> Stream.writeNumber2 3us
        |> Stream.writeNumber1 msg.level
        |> Stream.writeNumber1 msg.event
        |> Stream.writeNumber2 msg.node
        |> Stream.writeNumber2 msg.peer
        |> Stream.writeNumber8 msg.time
        |> Stream.writeString msg.host
        |> Stream.writeLongString msg.data

    let read =
        reader {
            let! sequence = Stream.readNumber2
            let! version = Stream.readNumber2
            do! check (version = 3us)
            let! level = Stream.readNumber1
            let! event = Stream.readNumber1
            let! node = Stream.readNumber2
            let! peer = Stream.readNumber2
            let! time = Stream.readNumber8
            let! host = Stream.readString
            let! data = Stream.readLongString

            return ({
                        sequence = sequence;
                        level = level;
                        event = event;
                        node = node;
                        peer = peer;
                        time = time;
                        host = host;
                        data = data;
                    }: Log)
        }

module Structures =
    let getMessageSize (msg:Structures) =
        0 +
            2 +
            List.fold (fun state (value:string) -> state + 4 + Encoding.UTF8.GetByteCount (value)) 4 msg.aliases +
            Map.fold (fun state (key:string) (value:string) -> state + 5 + Encoding.UTF8.GetByteCount (key) +  Encoding.UTF8.GetByteCount (value)) 4 msg.headers +
            0

    let write (msg:Structures) stream =
        stream
        |> Stream.writeNumber2 msg.sequence
        |> Stream.writeStrings msg.aliases
        |> Stream.writeMap msg.headers

    let read =
        reader {
            let! sequence = Stream.readNumber2
            let! aliases = Stream.readStrings
            let! headers = Stream.readMap

            return ({
                        sequence = sequence;
                        aliases = aliases;
                        headers = headers;
                    }: Structures)
        }

module Binary =
    let flagsSize = 4
    let getMessageSize (msg:Binary) =
        0 +
            2 +
            4 +
            4 + Array.length msg.publicKey +
            16 +
            0

    let write (msg:Binary) stream =
        stream
        |> Stream.writeNumber2 msg.sequence
        |> Stream.writeBytes msg.flags 4
        |> Stream.writeNumber4 (uint32 (Array.length msg.publicKey))
        |> Stream.writeBytes msg.publicKey (Array.length msg.publicKey)
        |> Stream.writeBytes (msg.identifier.ToByteArray()) 16

    let read =
        reader {
            let! sequence = Stream.readNumber2
            let! flags = Stream.readBytes 4
            let! publicKeyLength = Stream.readNumber4
            let! publicKey = Stream.readBytes (int publicKeyLength)
            let! identifierBytes = Stream.readBytes 16
            let identifier = new System.Guid (identifierBytes)

            return ({
                        sequence = sequence;
                        flags = flags;
                        publicKey = publicKey;
                        identifier = identifier;
                    }: Binary)
        }

module Types =
    let getMessageSize (msg:Types) =
        0 +
            2 +
            1 + String.length msg.clientForename +
            1 + String.length msg.clientSurname +
            1 + String.length msg.clientMobile +
            1 + String.length msg.clientEmail +
            1 + String.length msg.supplierForename +
            1 + String.length msg.supplierSurname +
            1 + String.length msg.supplierMobile +
            1 + String.length msg.supplierEmail +
            0

    let write (msg:Types) stream =
        stream
        |> Stream.writeNumber2 msg.sequence
        |> Stream.writeString msg.clientForename
        |> Stream.writeString msg.clientSurname
        |> Stream.writeString msg.clientMobile
        |> Stream.writeString msg.clientEmail
        |> Stream.writeString msg.supplierForename
        |> Stream.writeString msg.supplierSurname
        |> Stream.writeString msg.supplierMobile
        |> Stream.writeString msg.supplierEmail

    let read =
        reader {
            let! sequence = Stream.readNumber2
            let! clientForename = Stream.readString
            let! clientSurname = Stream.readString
            let! clientMobile = Stream.readString
            let! clientEmail = Stream.readString
            let! supplierForename = Stream.readString
            let! supplierSurname = Stream.readString
            let! supplierMobile = Stream.readString
            let! supplierEmail = Stream.readString

            return ({
                        sequence = sequence;
                        clientForename = clientForename;
                        clientSurname = clientSurname;
                        clientMobile = clientMobile;
                        clientEmail = clientEmail;
                        supplierForename = supplierForename;
                        supplierSurname = supplierSurname;
                        supplierMobile = supplierMobile;
                        supplierEmail = supplierEmail;
                    }: Types)
        }


let recv socket =
    let stream, more = Stream.recv socket

    // Drop the rest if any
    if more then Multipart.skip socket

    let readMessage messageId stream =
        match messageId with
        | LogMessageId ->
            match Log.read stream with
            | None,stream -> None,stream
            | Some msg, stream -> Some (Log msg), stream
        | StructuresMessageId ->
            match Structures.read stream with
            | None,stream -> None,stream
            | Some msg, stream -> Some (Structures msg), stream
        | BinaryMessageId ->
            match Binary.read stream with
            | None,stream -> None,stream
            | Some msg, stream -> Some (Binary msg), stream
        | TypesMessageId ->
            match Types.read stream with
            | None,stream -> None,stream
            | Some msg, stream -> Some (Types msg), stream
        | _ -> None, stream

    let r = reader {
                let! signature = Stream.readNumber2
                do! check (signature = (0xAAA0us ||| 0us))
                let! messageId = Stream.readNumber1
                let! message = readMessage messageId
                return message
            }

    run r stream

let send socket msg =
    let writeMessage = function
        | Log msg -> Log.write msg
        | Structures msg -> Structures.write msg
        | Binary msg -> Binary.write msg
        | Types msg -> Types.write msg

    let messageId =
        match msg with
        | Log _ -> LogMessageId
        | Structures _ -> StructuresMessageId
        | Binary _ -> BinaryMessageId
        | Types _ -> TypesMessageId

    let messageSize =
        match msg with
        | Log msg -> Log.getMessageSize msg
        | Structures msg -> Structures.getMessageSize msg
        | Binary msg -> Binary.getMessageSize msg
        | Types msg -> Types.getMessageSize msg

    //  Signature + message ID + message size
    let frameSize = 2 + 1 + messageSize
    let stream = Stream.create frameSize

    let stream' =
        stream
        |> Stream.writeNumber2 (0xAAA0us ||| 0us)
        |> Stream.writeNumber1 messageId
        |> writeMessage msg

    Stream.send socket stream'

