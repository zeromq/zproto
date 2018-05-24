module Example.Test

open NUnit.Framework
open FsUnit
open FsNetMQ
open Example


[<Test>]
let ``send and recv Log``() =
    let msg = Log {
        sequence = 123us;
        level = 123uy;
        event = 123uy;
        node = 123us;
        peer = 123us;
        time = 123UL;
        host = "Life is short but Now lasts for ever";
        data = "Life is short but Now lasts for ever";
    }

    use server = Socket.dealer ()
    Socket.bind server "inproc://Log.test"

    use client = Socket.dealer ()
    Socket.connect client "inproc://Log.test"

    Example.send server msg

    let msg' = Example.recv client

    msg' |> should equal (Some msg)

[<Test>]
let ``Log size fits stream ``() =
    let log:Log = {
        sequence = 123us;
        level = 123uy;
        event = 123uy;
        node = 123us;
        peer = 123us;
        time = 123UL;
        host = "Life is short but Now lasts for ever";
        data = "Life is short but Now lasts for ever";
    }

    let messageSize = Log.getMessageSize log

    let stream =
        Stream.create messageSize
        |> Log.write log

    let offset = Stream.getOffset stream

    messageSize |> should equal offset

[<Test>]
let ``send and recv Structures``() =
    let msg = Structures {
        sequence = 123us;
        aliases = ["Name: Brutus";"Age: 43"];
        headers =
            Map.empty
            |> Map.add "Name" "Brutus"
            |> Map.add "Age" "43"
    }

    use server = Socket.dealer ()
    Socket.bind server "inproc://Structures.test"

    use client = Socket.dealer ()
    Socket.connect client "inproc://Structures.test"

    Example.send server msg

    let msg' = Example.recv client

    msg' |> should equal (Some msg)

[<Test>]
let ``Structures size fits stream ``() =
    let structures:Structures = {
        sequence = 123us;
        aliases = ["Name: Brutus";"Age: 43"];
        headers =
            Map.empty
            |> Map.add "Name" "Brutus"
            |> Map.add "Age" "43"
    }

    let messageSize = Structures.getMessageSize structures

    let stream =
        Stream.create messageSize
        |> Structures.write structures

    let offset = Stream.getOffset stream

    messageSize |> should equal offset

[<Test>]
let ``send and recv Binary``() =
    let msg = Binary {
        sequence = 123us;
        flags = Array.create 4 123uy;
        publicKey = "Captcha Diem"B;
        identifier = System.Guid.Parse("DED9D22A-FCE1-488B-8761-84752ACC113E");
    }

    use server = Socket.dealer ()
    Socket.bind server "inproc://Binary.test"

    use client = Socket.dealer ()
    Socket.connect client "inproc://Binary.test"

    Example.send server msg

    let msg' = Example.recv client

    msg' |> should equal (Some msg)

[<Test>]
let ``Binary size fits stream ``() =
    let binary:Binary = {
        sequence = 123us;
        flags = Array.create 4 123uy;
        publicKey = "Captcha Diem"B;
        identifier = System.Guid.Parse("DED9D22A-FCE1-488B-8761-84752ACC113E");
    }

    let messageSize = Binary.getMessageSize binary

    let stream =
        Stream.create messageSize
        |> Binary.write binary

    let offset = Stream.getOffset stream

    messageSize |> should equal offset

[<Test>]
let ``send and recv Types``() =
    let msg = Types {
        sequence = 123us;
        clientForename = "Life is short but Now lasts for ever";
        clientSurname = "Life is short but Now lasts for ever";
        clientMobile = "Life is short but Now lasts for ever";
        clientEmail = "Life is short but Now lasts for ever";
        supplierForename = "Life is short but Now lasts for ever";
        supplierSurname = "Life is short but Now lasts for ever";
        supplierMobile = "Life is short but Now lasts for ever";
        supplierEmail = "Life is short but Now lasts for ever";
    }

    use server = Socket.dealer ()
    Socket.bind server "inproc://Types.test"

    use client = Socket.dealer ()
    Socket.connect client "inproc://Types.test"

    Example.send server msg

    let msg' = Example.recv client

    msg' |> should equal (Some msg)

[<Test>]
let ``Types size fits stream ``() =
    let types:Types = {
        sequence = 123us;
        clientForename = "Life is short but Now lasts for ever";
        clientSurname = "Life is short but Now lasts for ever";
        clientMobile = "Life is short but Now lasts for ever";
        clientEmail = "Life is short but Now lasts for ever";
        supplierForename = "Life is short but Now lasts for ever";
        supplierSurname = "Life is short but Now lasts for ever";
        supplierMobile = "Life is short but Now lasts for ever";
        supplierEmail = "Life is short but Now lasts for ever";
    }

    let messageSize = Types.getMessageSize types

    let stream =
        Stream.create messageSize
        |> Types.write types

    let offset = Stream.getOffset stream

    messageSize |> should equal offset

[<Test>]
let ``malformed message return None``() =
    use server = Socket.dealer ()
    Socket.bind server "inproc://Example.test"

    use client = Socket.dealer ()
    Socket.connect client "inproc://Example.test"

    Frame.send server "hello world"B

    let msg = Example.recv client
    msg |> should equal None
 