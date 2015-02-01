//  =========================================================================
//    Example - zproto example protocol csharp
//
//    Codec class for Example.
//
//    ** WARNING *************************************************************
//    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
//    your changes at the next build cycle. This is great for temporary printf
//    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
//    for commits are:
//
//     * The XML model used for this code generation: zproto_example_cs.xml, or
//     * The code generation script that built this file: zproto_codec_cs
//    ************************************************************************
//    Copyright (C) 2014 the Authors                                         
//                                                                           
//    Permission is hereby granted, free of charge, to any person obtaining  
//    a copy of this software and associated documentation files (the        
//    "Software"), to deal in the Software without restriction, including    
//    without limitation the rights to use, copy, modify, merge, publish,    
//    distribute, sublicense, and/or sell copies of the Software, and to     
//    permit persons to whom the Software is furnished to do so, subject to  
//    the following conditions:                                              
//                                                                           
//    The above copyright notice and this permission notice shall be included
//    in all copies or substantial portions of the Software.                 
//                                                                           
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF             
//    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
//    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   
//    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   
//    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 
//    =========================================================================

using System;
using System.Collections.Generic;
using System.Text;
using NetMQ;
using NetMQ.Sockets;
using NetMQ.zmq;

namespace ZProto
{
	/// <summary>
	/// zproto example protocol csharp
	/// </summary>
	public class Example : IDisposable 
	{
		public class MessageException : Exception
		{
			public MessageException(string message) : base(message)
			{
			}
		}

		public enum MessageId
		{
			Log = 1,
			Structures = 2,
			Binary = 3,
			Types = 4,
		}

		public const int FlagsSize = 4;

		private byte[] m_buffer;    //  Read/write buffer for serialization    
		private int m_offset;

		/// <summary>
		/// Create a new Example
		/// </summary>
		public Example()
		{    
			Flags = new byte[FlagsSize];
		}

		/// <summary>
		/// Dispose the Example
		/// </summary>	
		public void Dispose()
		{
			// Free class properties
			RoutingId = null;       
			Aliases = null;
			Headers = null;
			PublicKey = null;
			Identifier = Guid.Empty;         
			Address = null;
			Content = null;
		}

		#region Message Properties

		/// <summary>
		/// Get/set the message RoutingId
		/// </summary>
		public byte[] RoutingId
		{
			get;set;
		}

		/// <summary>
		/// Get/Set the Example id
		/// </summary>
		public MessageId Id 
		{
			get;set;
		}

		/// <summary>
		/// Return a printable command string
		/// </summary>
		public string Command
		{
			get
			{	    
				switch (Id) 
				{
					case MessageId.Log:
						return "Log";
						break;					
					case MessageId.Structures:
						return "Structures";
						break;					
					case MessageId.Binary:
						return "Binary";
						break;					
					case MessageId.Types:
						return "Types";
						break;					
				}
				return "?";
			}
		}


		/// <summary>
		/// Get/Set the Sequence field
		/// </summary>
		public UInt16 Sequence
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Level field
		/// </summary>
		public byte Level
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Event field
		/// </summary>
		public byte Event
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Node field
		/// </summary>
		public UInt16 Node
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Peer field
		/// </summary>
		public UInt16 Peer
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Time field
		/// </summary>
		public UInt64 Time
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Host field
		/// </summary>
		public string Host
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Data field
		/// </summary>
		public string Data
		{
			get;set;
		}


		/// <summary>
		/// /// Get/Set the Aliases list
		/// </summary>
		public List<string> Aliases
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Headers dictionary
		/// </summary>
		public Dictionary<string, string> Headers
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Flags field
		/// </summary>
		public byte[] Flags 
		{
			get; private set;
		}


		/// <summary>
		/// Get/Set the PublicKey field
		/// </summary>
		public byte[] PublicKey 
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Identifier field
		/// </summary>
		public Guid Identifier 
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the Address frame
		/// </summary>
		public byte[] Address 
		{
			get;set;
		}


		/// <summary>
		/// /// Get/Set the Content NetMQMessage
		/// </summary>
		public NetMQMessage Content 
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the ClientForename field
		/// </summary>
		public string ClientForename
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the ClientSurname field
		/// </summary>
		public string ClientSurname
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the ClientMobile field
		/// </summary>
		public string ClientMobile
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the ClientEmail field
		/// </summary>
		public string ClientEmail
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the SupplierForename field
		/// </summary>
		public string SupplierForename
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the SupplierSurname field
		/// </summary>
		public string SupplierSurname
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the SupplierMobile field
		/// </summary>
		public string SupplierMobile
		{
			get;set;
		}


		/// <summary>
		/// Get/Set the SupplierEmail field
		/// </summary>
		public string SupplierEmail
		{
			get;set;
		}

	
		#endregion
		
		/// <summary>
		/// Receive a Example from the socket.               
		/// </summary>
		public void Receive(IReceivingSocket input)
		{	    
			bool more;     		
			   
			if (input is RouterSocket) 
			{   			
				RoutingId = input.Receive(out more);
				if (!more) 
				{
					throw new MessageException("No routing id");				
				}
			}
			else
			{
				RoutingId = null;
			}

			Msg msg = new Msg();
			msg.InitEmpty();

			try
			{
				input.Receive(ref msg, SendReceiveOptions.None);

				m_offset = 0;
				m_buffer = msg.Data;
				more = msg.HasMore;
        
				UInt16 signature = GetNumber2();
    
				if (signature != (0xAAA0 | 0)) 
				{
					throw new MessageException("Invalid signature");			
				}
		
				//  Get message id and parse per message type
				Id = (MessageId)GetNumber1();

				int listSize;
				int hashSize;
				int chunkSize;
				byte[] guidBytes;
				UInt16 version;
	
				switch (Id) 
				{
					case MessageId.Log:
						Sequence = GetNumber2();
						version = GetNumber2();
                
						if (version != 3) 
						{
							throw new MessageException("Version is invalid");						
						}
													
						Level = GetNumber1();
						Event = GetNumber1();
						Node = GetNumber2();
						Peer = GetNumber2();
						Time = GetNumber8();
						Host = GetString();
						Data = GetLongString();               
					break;
					case MessageId.Structures:
						Sequence = GetNumber2();
						
						listSize = (int)GetNumber4();                
						Aliases = new List<string>(listSize);                
						while (listSize-- > 0) 
						{
							string s = GetLongString();
							Aliases.Add(s);                   
						}		
										

						hashSize = (int)GetNumber4();                
						Headers = new Dictionary<string, string>();                
						while (hashSize-- > 0)  
						{
							string key = GetString();
							string value = GetLongString();
							Headers.Add(key, value);
						}						

					break;
					case MessageId.Binary:
						Sequence = GetNumber2();
						GetOctets(Flags, 4);
						
						chunkSize = (int)GetNumber4();                
						if (m_offset + chunkSize > m_buffer.Length) 
						{
							throw new MessageException("PublicKey is missing data");
						}
                
						PublicKey = new byte[chunkSize];
						GetOctets(PublicKey, chunkSize);              						


						if (m_offset + 16 > m_buffer.Length) 
						{
							throw new MessageException("Identifier is invalid");					
						}
            
						guidBytes = new byte[16];
						GetOctets(guidBytes, 16);
						Identifier = new Guid(guidBytes);         
						
						
						//  Get next frame off socket
						if (!more)
						{
							throw new MessageException("Address is missing");
						}
            
						Address = input.Receive(out more);


						//  Get zero or more remaining frames            
						if (more)
							Content = input.ReceiveMessage();
						else
							Content = new NetMQMessage();
					break;
					case MessageId.Types:
						Sequence = GetNumber2();
						ClientForename = GetString();
						ClientSurname = GetString();
						ClientMobile = GetString();
						ClientEmail = GetString();
						SupplierForename = GetString();
						SupplierSurname = GetString();
						SupplierMobile = GetString();
						SupplierEmail = GetString();
					break;
				default:
					throw new MessageException("Bad message id");            
					break;
				}        
			}
			finally
			{
				m_buffer = null;
				msg.Close();		
			}
		}

		/// <summary>
		/// Send the Example to the socket.
		/// </summary>
		public void Send(IOutgoingSocket output)
		{    
			if (output is RouterSocket)
				output.SendMore(RoutingId);

			int frameSize = 2 + 1;          //  Signature and message ID
			switch (Id) 
			{
				case MessageId.Log:
					frameSize += 2;            //  Sequence
					frameSize += 2;            //  Version
					frameSize += 1;            //  Level
					frameSize += 1;            //  Event
					frameSize += 2;            //  Node
					frameSize += 2;            //  Peer
					frameSize += 8;            //  Time
					frameSize += 1 + Host.Length;
					frameSize += 4;
					if (Data != null)
						frameSize += Data.Length;
					break;
				case MessageId.Structures:
					frameSize += 2;            //  Sequence
					frameSize += 4;            //  Size is 4 octets
					if (Aliases != null) 
					{
						foreach(string s in Aliases)
						{
							frameSize += 4 + s.Length;
						}                
					}
					frameSize += 4;            //  Size is 4 octets
					if (Headers != null) 
					{                
						int headersSize = 0;

						foreach (var pair in Headers)
						{
							headersSize += 1 + pair.Key.Length;
							headersSize += 4 + pair.Value.Length;
						}

						frameSize += headersSize;
					}
            
					break;
				case MessageId.Binary:
					frameSize += 2;            //  Sequence
					frameSize += 4;            //  Flags
					frameSize += 4;            //  Size is 4 octets
					if (PublicKey != null)
						frameSize += PublicKey.Length;
					frameSize  += 16;    //  Identifier
					break;
				case MessageId.Types:
					frameSize += 2;            //  Sequence
					frameSize += 1 + ClientForename.Length;
					frameSize += 1 + ClientSurname.Length;
					frameSize += 1 + ClientMobile.Length;
					frameSize += 1 + ClientEmail.Length;
					frameSize += 1 + SupplierForename.Length;
					frameSize += 1 + SupplierSurname.Length;
					frameSize += 1 + SupplierMobile.Length;
					frameSize += 1 + SupplierEmail.Length;
					break;
			}

			//  Now serialize message into the buffer    
			Msg msg = new Msg();
			msg.InitPool(frameSize);

			try
			{
				m_offset = 0;
				m_buffer = msg.Data;

				// put signature
				PutNumber2(0xAAA0 | 0);

				// put message id
				PutNumber1((byte)Id);
				bool sendContent = false;
				int frames = 1;              //  Total number of frames to send
    
				switch (Id) 
				{
					case MessageId.Log:
						PutNumber2(Sequence);
						PutNumber2(3); // Version
						PutNumber1(Level);
						PutNumber1(Event);
						PutNumber2(Node);
						PutNumber2(Peer);
						PutNumber8(Time);
						PutString(Host);
						if (Data != null) 						
							PutLongString(Data);                   						
						else
							PutNumber4(0);    //  Empty string
					break;
					case MessageId.Structures:
						PutNumber2(Sequence);
						
						if (Aliases != null) 
						{
							PutNumber4((UInt32)Aliases.Count);

							foreach (string s in Aliases)
							{
								PutLongString(s);
							}                
						}
						else
							PutNumber4(0);    //  Empty string array

						
						if (Headers != null) 
						{
							PutNumber4((UInt32)Headers.Count);
                
							foreach(var pair in Headers)
							{
								PutString(pair.Key);
								PutLongString(pair.Value);
							}				
						}
						else
							PutNumber4(0);    //  Empty dictionary

					break;
					case MessageId.Binary:
						PutNumber2(Sequence);
						PutOctets(Flags, 4);

						if (PublicKey != null)
						{
							PutNumber4((UInt32)PublicKey.Length);
							Buffer.BlockCopy(PublicKey, 0, m_buffer, m_offset, PublicKey.Length);
							m_offset += PublicKey.Length;
						}
						else
							PutNumber4(0);    //  Empty chunk


						// copy guid           
						Buffer.BlockCopy(Identifier.ToByteArray(), 0, m_buffer, m_offset, 16);
						m_offset += 16;
						frames++;
						frames += Content != null ? Content.FrameCount : 1;
						sendContent = true;
					break;
					case MessageId.Types:
						PutNumber2(Sequence);
						PutString(ClientForename);
						PutString(ClientSurname);
						PutString(ClientMobile);
						PutString(ClientEmail);
						PutString(SupplierForename);
						PutString(SupplierSurname);
						PutString(SupplierMobile);
						PutString(SupplierEmail);
					break;
				}

				//  Now send the data frame				
				output.Send(ref msg, --frames == 0 ? SendReceiveOptions.None : SendReceiveOptions.SendMore);
        
				//  Now send any frame fields, in order
				if (Id == MessageId.Binary) 
				{
					if (--frames == 0)
						output.Send(Address != null ? Address : new byte[0]);
					else 
						output.SendMore(Address != null ? Address : new byte[0]);                        		            
				}
				
				//  Now send the Content if necessary
				if (sendContent) 
				{
					if (Content != null) 
					{
						output.SendMessage(Content);                 
					}
					else
						output.Send(new byte[0]);            
				}
			}
			finally
			{
				m_buffer = null;
				msg.Close();
			}
		}	

		#region Network data encoding methods

		//  Put a block of octets to the frame
		private void PutOctets(byte[] host, int size) 
		{ 
			Buffer.BlockCopy(host, 0, m_buffer, m_offset, size);   
			m_offset += host.Length; 
		}

		//  Get a block of octets from the frame
		private void GetOctets(byte[] host, int size) 
		{
			if (m_offset + size > m_buffer.Length) 
			{ 
				throw new MessageException("Malformed message");            			
			} 
			
			Buffer.BlockCopy(m_buffer, m_offset, host, 0, size);
			m_offset += size; 			
		}

		//  Put a 1-byte number to the frame
		private void PutNumber1(byte host) 
		{ 
			m_buffer[m_offset] = host;
			m_offset++;
		}

		//  Put a 2-byte number to the frame
		private void PutNumber2(UInt16 host) 
		{ 
			m_buffer[m_offset] = (byte) (((host) >> 8)  & 255);
			m_buffer[m_offset+1] = (byte) (((host))       & 255); 

			m_offset+=2;
		}

		//  Put a 4-byte number to the frame
		private void PutNumber4(UInt32 host) 
		{
			m_buffer[m_offset] = (byte) (((host) >> 24) & 255);
			m_buffer[m_offset+1] = (byte) (((host) >> 16) & 255); 
			m_buffer[m_offset+2] = (byte) (((host) >> 8)  & 255); 
			m_buffer[m_offset+3] = (byte) (((host))       & 255);

			m_offset+=4;
		}

		//  Put a 8-byte number to the frame
		private void PutNumber8(UInt64 host) 
		{
			m_buffer[m_offset] = (byte) (((host) >> 56) & 255);
			m_buffer[m_offset+1] = (byte) (((host) >> 48) & 255);
			m_buffer[m_offset+2] = (byte) (((host) >> 40) & 255);
			m_buffer[m_offset+3] = (byte) (((host) >> 32) & 255);
			m_buffer[m_offset+4] = (byte) (((host) >> 24) & 255); 
			m_buffer[m_offset+5] = (byte) (((host) >> 16) & 255);
			m_buffer[m_offset+6] = (byte) (((host) >> 8)  & 255);
			m_buffer[m_offset+7] = (byte) (((host))       & 255);

			m_offset+=8;
		}

		//  Get a 1-byte number from the frame
		private byte GetNumber1() 
		{
			if (m_offset + 1 > m_buffer.Length) 
			{
				throw new MessageException("Malformed message.");
			} 
    
			byte b = m_buffer[m_offset];
		
			m_offset++;

			return b;
		}

		//  Get a 2-byte number from the frame
		private UInt16 GetNumber2() 
		{ 
			if (m_offset + 2 > m_buffer.Length) 
			{
				throw new MessageException("Malformed message.");
			} 
    
			UInt16 number = (UInt16)((m_buffer[m_offset] << 8) + 
							m_buffer[m_offset+1]);
		
			m_offset+=2;

			return number;
		}

		//  Get a 4-byte number from the frame
		private UInt32 GetNumber4() 
		{ 
			if (m_offset + 4 > m_buffer.Length) 
			{
				throw new MessageException("Malformed message.");
			} 
    
			UInt32 number = 
				(((UInt32)m_buffer[m_offset]) << 24) + 
				(((UInt32)m_buffer[m_offset+1]) << 16)  +
				(((UInt32)m_buffer[m_offset+2]) << 8) +
				(UInt32)m_buffer[m_offset+3];
		
			m_offset+=4;

			return number;
		}

		//  Get a 8byte number from the frame
		private UInt64 GetNumber8() 
		{ 
			if (m_offset + 8 > m_buffer.Length) 
			{
				throw new MessageException("Malformed message.");
			} 
    
			UInt64 number = 
				(((UInt64) m_buffer[m_offset]) << 56) + 
				(((UInt64) m_buffer[m_offset+1]) << 48)  +
				(((UInt64) m_buffer[m_offset+2]) << 40) +
				(((UInt64) m_buffer[m_offset+3]) << 32) +
				(((UInt64) m_buffer[m_offset+4]) << 24) +
				(((UInt64) m_buffer[m_offset+5]) << 16) +
				(((UInt64) m_buffer[m_offset+6]) << 8) +		
				(UInt64) m_buffer[m_offset+7];
		
			m_offset+=8;

			return number;
		}

		//  Put a string to the frame
		private void PutString(string host) 
		{   
			int length = Encoding.UTF8.GetByteCount(host); 
			
			if (length > 255)
				length = 255;
		
			PutNumber1((byte)length); 

			Encoding.UTF8.GetBytes(host, 0, length, m_buffer, m_offset);
    
			m_offset += length;
		}

		//  Get a string from the frame
		private string GetString() 
		{ 
			int length = GetNumber1();    
			if (m_offset + length > m_buffer.Length) 
			{ 
			  throw new MessageException("Malformed message.");
			} 

			string s = Encoding.UTF8.GetString(m_buffer, m_offset, length);

			m_offset += length;

			return s;
		}

		//  Put a long string to the frame
		private void PutLongString(string host) 
		{     
			PutNumber4((UInt32)Encoding.UTF8.GetByteCount(host));
	 
			Encoding.UTF8.GetBytes(host, 0, host.Length, m_buffer, m_offset);
    
			m_offset += host.Length;
		}

		//  Get a long string from the frame
		private string GetLongString() 
		{ 
			int length = (int)GetNumber4();    
			if (m_offset + length > m_buffer.Length) 
			{ 
			  throw new MessageException("Malformed message.");
			} 

			string s = Encoding.UTF8.GetString(m_buffer, m_offset, length);

			m_offset += length;

			return s;
		}

		#endregion
	}
}
