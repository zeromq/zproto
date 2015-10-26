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
	public class Example
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
		
		#region Log

		public class LogMessage
		{
			public LogMessage()
			{
			}			

			/// <summary>
			/// Get/Set the Sequence field
			/// </summary>
			public UInt16 Sequence
			{
				get;set;
			}

			/// <summary>
			/// Get/Set the Version field
			/// </summary>
			public UInt16 Version
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


			internal int GetFrameSize()
			{
				int frameSize = 0;

				//  Sequence
				frameSize += 2;          

				//  Version
				frameSize += 2;          

				//  Level
				frameSize += 1;          

				//  Event
				frameSize += 1;          

				//  Node
				frameSize += 2;          

				//  Peer
				frameSize += 2;          

				//  Time
				frameSize += 8;          

				//  Host
				frameSize += 1 + Host.Length;

				//  Data
				frameSize += 4;
				if (Data != null)
					frameSize += Data.Length;

				return frameSize;
			}		

			internal void Write(Example m)
			{
				// Sequence
				m.PutNumber2(Sequence);

				// Version
				m.PutNumber2(3); // Version

				// Level
				m.PutNumber1(Level);

				// Event
				m.PutNumber1(Event);

				// Node
				m.PutNumber2(Node);

				// Peer
				m.PutNumber2(Peer);

				// Time
				m.PutNumber8(Time);

				// Host
				m.PutString(Host);

				// Data
				if (Data != null) 						
					m.PutLongString(Data);                   						
				else
					m.PutNumber4(0);    //  Empty string

			}

			internal void Read(Example m)
			{
				int listSize;
				int hashSize;
				int chunkSize;
				byte[] guidBytes;
				UInt16 version;

				// Sequence
				Sequence = m.GetNumber2();

				// Version
				version = m.GetNumber2();                          
				if (version != 3) 
				{
					throw new MessageException("Version is invalid");						
				}													

				// Level
				Level = m.GetNumber1();

				// Event
				Event = m.GetNumber1();

				// Node
				Node = m.GetNumber2();

				// Peer
				Peer = m.GetNumber2();

				// Time
				Time = m.GetNumber8();

				// Host
				Host = m.GetString();

				// Data
				Data = m.GetLongString();               

			}
		}

		#endregion

		#region Structures

		public class StructuresMessage
		{
			public StructuresMessage()
			{
			}			

			/// <summary>
			/// Get/Set the Sequence field
			/// </summary>
			public UInt16 Sequence
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


			internal int GetFrameSize()
			{
				int frameSize = 0;

				//  Sequence
				frameSize += 2;          

				//  Aliases
				frameSize += 4;            //  Size is 4 octets
				if (Aliases != null) 
				{
					foreach(string s in Aliases)
					{
						frameSize += 4 + s.Length;
					}                
				}

				//  Headers
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
            

				return frameSize;
			}		

			internal void Write(Example m)
			{
				// Sequence
				m.PutNumber2(Sequence);

				// Aliases
				if (Aliases != null) 
				{
					m.PutNumber4((UInt32)Aliases.Count);

					foreach (string s in Aliases)
					{
						m.PutLongString(s);
					}                
				}
				else
					m.PutNumber4(0);    //  Empty string array

				// Headers
				if (Headers != null) 
				{
					m.PutNumber4((UInt32)Headers.Count);
               
					foreach(var pair in Headers)
					{
						m.PutString(pair.Key);
						m.PutLongString(pair.Value);
					}				
				}
				else
					m.PutNumber4(0);    //  Empty dictionary

			}

			internal void Read(Example m)
			{
				int listSize;
				int hashSize;
				int chunkSize;
				byte[] guidBytes;

				// Sequence
				Sequence = m.GetNumber2();

				// Aliases
				listSize = (int)m.GetNumber4();                
				Aliases = new List<string>(listSize);                
				while (listSize-- > 0) 
				{
					string s = m.GetLongString();
					Aliases.Add(s);                   
				}												

				// Headers
				hashSize = (int)m.GetNumber4();                
				Headers = new Dictionary<string, string>();                
				while (hashSize-- > 0)  
				{
					string key = m.GetString();
					string value = m.GetLongString();
					Headers.Add(key, value);
				}						

			}
		}

		#endregion

		#region Binary

		public class BinaryMessage
		{
			public const int FlagsSize = 4;

			public BinaryMessage()
			{
				Flags = new byte[FlagsSize];
			}			

			/// <summary>
			/// Get/Set the Sequence field
			/// </summary>
			public UInt16 Sequence
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


			internal int GetFrameSize()
			{
				int frameSize = 0;

				//  Sequence
				frameSize += 2;          

				//  Flags
				frameSize += 4;

				//  PublicKey
				frameSize += 4;            //  Size is 4 octets
				if (PublicKey != null)
					frameSize += PublicKey.Length;

				//  Identifier
				frameSize  += 16;

				return frameSize;
			}		

			internal void Write(Example m)
			{
				// Sequence
				m.PutNumber2(Sequence);

				// Flags
				m.PutOctets(Flags, 4);

				// PublicKey
				if (PublicKey != null)
				{
					m.PutNumber4((UInt32)PublicKey.Length);
					m.PutOctets(PublicKey, PublicKey.Length);				
				}
				else
					m.PutNumber4(0);    //  Empty chunk

				// Identifier
				// copy guid           
				m.PutOctets(Identifier.ToByteArray(), 16);			

			}

			internal void Read(Example m)
			{
				int listSize;
				int hashSize;
				int chunkSize;
				byte[] guidBytes;

				// Sequence
				Sequence = m.GetNumber2();

				// Flags
				m.GetOctets(Flags, 4);

				// PublicKey
				chunkSize = (int)m.GetNumber4();                
				if (m.m_offset + chunkSize > m.m_buffer.Length) 
				{
					throw new MessageException("PublicKey is missing data");
				}
                
				PublicKey = new byte[chunkSize];
				m.GetOctets(PublicKey, chunkSize);              						

				// Identifier
				if (m.m_offset + 16 > m.m_buffer.Length) 
				{
					throw new MessageException("Identifier is invalid");					
				}
            
				guidBytes = new byte[16];
				m.GetOctets(guidBytes, 16);
				Identifier = new Guid(guidBytes);         						

			}
		}

		#endregion

		#region Types

		public class TypesMessage
		{
			public TypesMessage()
			{
			}			

			/// <summary>
			/// Get/Set the Sequence field
			/// </summary>
			public UInt16 Sequence
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


			internal int GetFrameSize()
			{
				int frameSize = 0;

				//  Sequence
				frameSize += 2;          

				//  ClientForename
				frameSize += 1 + ClientForename.Length;

				//  ClientSurname
				frameSize += 1 + ClientSurname.Length;

				//  ClientMobile
				frameSize += 1 + ClientMobile.Length;

				//  ClientEmail
				frameSize += 1 + ClientEmail.Length;

				//  SupplierForename
				frameSize += 1 + SupplierForename.Length;

				//  SupplierSurname
				frameSize += 1 + SupplierSurname.Length;

				//  SupplierMobile
				frameSize += 1 + SupplierMobile.Length;

				//  SupplierEmail
				frameSize += 1 + SupplierEmail.Length;

				return frameSize;
			}		

			internal void Write(Example m)
			{
				// Sequence
				m.PutNumber2(Sequence);

				// ClientForename
				m.PutString(ClientForename);

				// ClientSurname
				m.PutString(ClientSurname);

				// ClientMobile
				m.PutString(ClientMobile);

				// ClientEmail
				m.PutString(ClientEmail);

				// SupplierForename
				m.PutString(SupplierForename);

				// SupplierSurname
				m.PutString(SupplierSurname);

				// SupplierMobile
				m.PutString(SupplierMobile);

				// SupplierEmail
				m.PutString(SupplierEmail);

			}

			internal void Read(Example m)
			{
				int listSize;
				int hashSize;
				int chunkSize;
				byte[] guidBytes;

				// Sequence
				Sequence = m.GetNumber2();

				// ClientForename
				ClientForename = m.GetString();

				// ClientSurname
				ClientSurname = m.GetString();

				// ClientMobile
				ClientMobile = m.GetString();

				// ClientEmail
				ClientEmail = m.GetString();

				// SupplierForename
				SupplierForename = m.GetString();

				// SupplierSurname
				SupplierSurname = m.GetString();

				// SupplierMobile
				SupplierMobile = m.GetString();

				// SupplierEmail
				SupplierEmail = m.GetString();

			}
		}

		#endregion


		private byte[] m_buffer;    //  Read/write buffer for serialization    
		private int m_offset;
		private byte[] m_routingId;

		/// <summary>
		/// Create a new Example
		/// </summary>
		public Example()
		{    
			Log = new LogMessage();
			Structures = new StructuresMessage();
			Binary = new BinaryMessage();
			Types = new TypesMessage();
		}			

		public LogMessage Log {get;private set;}

		public StructuresMessage Structures {get;private set;}

		public BinaryMessage Binary {get;private set;}

		public TypesMessage Types {get;private set;}

	
		/// <summary>
		/// Get/set the message RoutingId.
		/// </summary>
		public byte[] RoutingId
		{
			get
			{
				return m_routingId;
			}
			set 
			{
				if (value == null)
			        m_routingId = null;
			    else
			    {       
					if (m_routingId == null || m_routingId.Length != value.Length)
						m_routingId = new byte[value.Length];

					Buffer.BlockCopy(value, 0, m_routingId, 0, value.Length);
				}
			}
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
					case MessageId.Structures:
						return "Structures";										
					case MessageId.Binary:
						return "Binary";										
					case MessageId.Types:
						return "Types";										
				}
				return "?";
			}
		}
			
		/// <summary>
		/// Receive a Example from the socket.               
		/// </summary>
		public void Receive(IReceivingSocket input)
		{	    
			bool more;     		
			   
			if (input is RouterSocket) 
			{   			
				Msg routingIdMsg = new Msg();
				routingIdMsg.InitEmpty();

				try
				{
					input.Receive(ref routingIdMsg);

					if (!routingIdMsg.HasMore) 
					{
						throw new MessageException("No routing id");				
					}

					if (m_routingId == null || m_routingId.Length == routingIdMsg.Size)					
						m_routingId = new byte[routingIdMsg.Size];					

					Buffer.BlockCopy(routingIdMsg.Data, 0, m_routingId, 0, m_routingId.Length);
				}
				finally
				{
					routingIdMsg.Close();
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
				input.Receive(ref msg);

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
				
				switch (Id) 
				{
					case MessageId.Log:
						Log.Read(this);
					break;
					case MessageId.Structures:
						Structures.Read(this);
					break;
					case MessageId.Binary:
						Binary.Read(this);
					break;
					case MessageId.Types:
						Types.Read(this);
					break;
				default:
					throw new MessageException("Bad message id");            					
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
				output.SendMoreFrame(RoutingId);

			int frameSize = 2 + 1;          //  Signature and message ID
			switch (Id) 
			{
				case MessageId.Log:
					frameSize += Log.GetFrameSize();
					break;
				case MessageId.Structures:
					frameSize += Structures.GetFrameSize();
					break;
				case MessageId.Binary:
					frameSize += Binary.GetFrameSize();
					break;
				case MessageId.Types:
					frameSize += Types.GetFrameSize();
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
	
				switch (Id) 
				{
					case MessageId.Log:
						Log.Write(this);
					break;
					case MessageId.Structures:
						Structures.Write(this);
					break;
					case MessageId.Binary:
						Binary.Write(this);
					break;
					case MessageId.Types:
						Types.Write(this);
					break;
				}

				//  Send the data frame				
				output.Send(ref msg, false);       
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
			m_offset += size; 
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
