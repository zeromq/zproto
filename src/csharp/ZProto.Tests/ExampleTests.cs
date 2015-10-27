using System;
using System.Text;
using System.Collections.Generic;
using NUnit.Framework;
using NetMQ;

namespace ZProto.Test
{
	[TestFixture]
	public class ExampleTests
	{
		private void FillArray(byte[] array, byte value)
		{
			for	(int i = 0; i < array.Length; i++)
			{
				array[i] = value;
			}
		}
	
		[Test]
		public void LogTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Log;

				m.Log.Sequence = 123;
				m.Log.Level = 123;
				m.Log.Event = 123;
				m.Log.Node = 123;
				m.Log.Peer = 123;
				m.Log.Time = 123;
    			m.Log.Host = "Life is short but Now lasts for ever";
    			m.Log.Data = "Life is short but Now lasts for ever";
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Log));
				Assert.That(m.Log.Sequence, Is.EqualTo(123));                   
				Assert.That(m.Log.Level, Is.EqualTo(123));                      
				Assert.That(m.Log.Event, Is.EqualTo(123));                      
				Assert.That(m.Log.Node, Is.EqualTo(123));                       
				Assert.That(m.Log.Peer, Is.EqualTo(123));                       
				Assert.That(m.Log.Time, Is.EqualTo(123));                       
				Assert.That(m.Log.Host, Is.EqualTo("Life is short but Now lasts for ever"));                               
				Assert.That(m.Log.Data, Is.EqualTo("Life is short but Now lasts for ever"));                               
			};

			using (NetMQContext context = NetMQContext.Create())
			using (var client = context.CreateDealerSocket())
			using (var server = context.CreateRouterSocket())
			{
				server.Bind("inproc://zprototest");
				client.Connect("inproc://zprototest");

				Example clientMessage = new Example();
				Example serverMessage = new Example();

				for (int i=0; i < 2; i++)
				{
					// client send message to server
					setMessage(clientMessage);				
					clientMessage.Send(client);				
												
					// server receive the message
					serverMessage.Receive(server);
				
					// check that message received ok
					Assert.That(serverMessage.RoutingId, Is.Not.Null);					
					checkMessage(serverMessage);

					// reply to client, no need to set the message, using client data
					serverMessage.Send(server);

					// client receive the message
					clientMessage.Receive(client);
				
					// check that message received ok
					Assert.That(clientMessage.RoutingId, Is.Null);					
					checkMessage(clientMessage);
				}				
			}			
		}	
	
		[Test]
		public void StructuresTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Structures;

				m.Structures.Sequence = 123;
				m.Structures.Aliases = new List<string>();
				m.Structures.Aliases.Add("Name: Brutus");
				m.Structures.Aliases.Add("Age: 43");         
				m.Structures.Headers = new Dictionary<string,string>();
				m.Structures.Headers.Add("Name", "Brutus");
				m.Structures.Headers.Add("Age", "43");         
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Structures));
				Assert.That(m.Structures.Sequence, Is.EqualTo(123));            
				Assert.That(m.Structures.Aliases.Count, Is.EqualTo(2));
				Assert.That(m.Structures.Aliases[0], Is.EqualTo("Name: Brutus"));                     
				Assert.That(m.Structures.Aliases[1], Is.EqualTo("Age: 43"));                                     
				Assert.That(m.Structures.Headers.Count, Is.EqualTo(2));
				Assert.That(m.Structures.Headers["Name"], Is.EqualTo("Brutus"));                     
				Assert.That(m.Structures.Headers["Age"], Is.EqualTo("43"));                                     
			};

			using (NetMQContext context = NetMQContext.Create())
			using (var client = context.CreateDealerSocket())
			using (var server = context.CreateRouterSocket())
			{
				server.Bind("inproc://zprototest");
				client.Connect("inproc://zprototest");

				Example clientMessage = new Example();
				Example serverMessage = new Example();

				for (int i=0; i < 2; i++)
				{
					// client send message to server
					setMessage(clientMessage);				
					clientMessage.Send(client);				
												
					// server receive the message
					serverMessage.Receive(server);
				
					// check that message received ok
					Assert.That(serverMessage.RoutingId, Is.Not.Null);					
					checkMessage(serverMessage);

					// reply to client, no need to set the message, using client data
					serverMessage.Send(server);

					// client receive the message
					clientMessage.Receive(client);
				
					// check that message received ok
					Assert.That(clientMessage.RoutingId, Is.Null);					
					checkMessage(clientMessage);
				}				
			}			
		}	
	
		[Test]
		public void BinaryTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Binary;

				m.Binary.Sequence = 123;
				FillArray(m.Binary.Flags, 123);				               
				m.Binary.PublicKey = Encoding.ASCII.GetBytes("Captcha Diem");
				m.Binary.Identifier = Guid.Parse("DED9D22A-FCE1-488B-8761-84752ACC113E");
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Binary));
				Assert.That(m.Binary.Sequence, Is.EqualTo(123));                
				Assert.That(m.Binary.Flags[0], Is.EqualTo(123));                   
				Assert.That(m.Binary.Flags[m.Binary.Flags.Length-1], Is.EqualTo(123));                                      
				Assert.That(m.Binary.PublicKey, Is.EqualTo(Encoding.ASCII.GetBytes("Captcha Diem")));				
				Assert.That(m.Binary.Identifier, Is.EqualTo(Guid.Parse("DED9D22A-FCE1-488B-8761-84752ACC113E")));
			};

			using (NetMQContext context = NetMQContext.Create())
			using (var client = context.CreateDealerSocket())
			using (var server = context.CreateRouterSocket())
			{
				server.Bind("inproc://zprototest");
				client.Connect("inproc://zprototest");

				Example clientMessage = new Example();
				Example serverMessage = new Example();

				for (int i=0; i < 2; i++)
				{
					// client send message to server
					setMessage(clientMessage);				
					clientMessage.Send(client);				
												
					// server receive the message
					serverMessage.Receive(server);
				
					// check that message received ok
					Assert.That(serverMessage.RoutingId, Is.Not.Null);					
					checkMessage(serverMessage);

					// reply to client, no need to set the message, using client data
					serverMessage.Send(server);

					// client receive the message
					clientMessage.Receive(client);
				
					// check that message received ok
					Assert.That(clientMessage.RoutingId, Is.Null);					
					checkMessage(clientMessage);
				}				
			}			
		}	
	
		[Test]
		public void TypesTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Types;

				m.Types.Sequence = 123;
    			m.Types.ClientForename = "Life is short but Now lasts for ever";
    			m.Types.ClientSurname = "Life is short but Now lasts for ever";
    			m.Types.ClientMobile = "Life is short but Now lasts for ever";
    			m.Types.ClientEmail = "Life is short but Now lasts for ever";
    			m.Types.SupplierForename = "Life is short but Now lasts for ever";
    			m.Types.SupplierSurname = "Life is short but Now lasts for ever";
    			m.Types.SupplierMobile = "Life is short but Now lasts for ever";
    			m.Types.SupplierEmail = "Life is short but Now lasts for ever";
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Types));
				Assert.That(m.Types.Sequence, Is.EqualTo(123));                 
				Assert.That(m.Types.ClientForename, Is.EqualTo("Life is short but Now lasts for ever"));                   
				Assert.That(m.Types.ClientSurname, Is.EqualTo("Life is short but Now lasts for ever"));                    
				Assert.That(m.Types.ClientMobile, Is.EqualTo("Life is short but Now lasts for ever"));                     
				Assert.That(m.Types.ClientEmail, Is.EqualTo("Life is short but Now lasts for ever"));                      
				Assert.That(m.Types.SupplierForename, Is.EqualTo("Life is short but Now lasts for ever"));                 
				Assert.That(m.Types.SupplierSurname, Is.EqualTo("Life is short but Now lasts for ever"));                  
				Assert.That(m.Types.SupplierMobile, Is.EqualTo("Life is short but Now lasts for ever"));                   
				Assert.That(m.Types.SupplierEmail, Is.EqualTo("Life is short but Now lasts for ever"));                    
			};

			using (NetMQContext context = NetMQContext.Create())
			using (var client = context.CreateDealerSocket())
			using (var server = context.CreateRouterSocket())
			{
				server.Bind("inproc://zprototest");
				client.Connect("inproc://zprototest");

				Example clientMessage = new Example();
				Example serverMessage = new Example();

				for (int i=0; i < 2; i++)
				{
					// client send message to server
					setMessage(clientMessage);				
					clientMessage.Send(client);				
												
					// server receive the message
					serverMessage.Receive(server);
				
					// check that message received ok
					Assert.That(serverMessage.RoutingId, Is.Not.Null);					
					checkMessage(serverMessage);

					// reply to client, no need to set the message, using client data
					serverMessage.Send(server);

					// client receive the message
					clientMessage.Receive(client);
				
					// check that message received ok
					Assert.That(clientMessage.RoutingId, Is.Null);					
					checkMessage(clientMessage);
				}				
			}			
		}	
	}
}