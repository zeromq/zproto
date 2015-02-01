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

				m.Sequence = 123;
				m.Level = 123;
				m.Event = 123;
				m.Node = 123;
				m.Peer = 123;
				m.Time = 123;
    			m.Host = "Life is short but Now lasts for ever";
    			m.Data = "Life is short but Now lasts for ever";
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Log));
				Assert.That(m.Sequence, Is.EqualTo(123));       
				Assert.That(m.Level, Is.EqualTo(123));          
				Assert.That(m.Event, Is.EqualTo(123));          
				Assert.That(m.Node, Is.EqualTo(123));           
				Assert.That(m.Peer, Is.EqualTo(123));           
				Assert.That(m.Time, Is.EqualTo(123));           
				Assert.That(m.Host, Is.EqualTo("Life is short but Now lasts for ever"));                   
				Assert.That(m.Data, Is.EqualTo("Life is short but Now lasts for ever"));                   
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

				clientMessage.Dispose();
				serverMessage.Dispose();
			}			
		}	
	
		[Test]
		public void StructuresTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Structures;

				m.Sequence = 123;
				m.Aliases = new List<string>();
				m.Aliases.Add("Name: Brutus");
				m.Aliases.Add("Age: 43");    
				m.Headers = new Dictionary<string,string>();
				m.Headers.Add("Name", "Brutus");
				m.Headers.Add("Age", "43");    
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Structures));
				Assert.That(m.Sequence, Is.EqualTo(123));       
				Assert.That(m.Aliases.Count, Is.EqualTo(2));
				Assert.That(m.Aliases[0], Is.EqualTo("Name: Brutus"));                
				Assert.That(m.Aliases[1], Is.EqualTo("Age: 43"));                                
				Assert.That(m.Headers.Count, Is.EqualTo(2));
				Assert.That(m.Headers["Name"], Is.EqualTo("Brutus"));                
				Assert.That(m.Headers["Age"], Is.EqualTo("43"));                                
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

				clientMessage.Dispose();
				serverMessage.Dispose();
			}			
		}	
	
		[Test]
		public void BinaryTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Binary;

				m.Sequence = 123;
				FillArray(m.Flags, 123);				      
				m.PublicKey = Encoding.ASCII.GetBytes("Captcha Diem");
				m.Identifier = Guid.Parse("DED9D22A-FCE1-488B-8761-84752ACC113E");
				m.Address = Encoding.ASCII.GetBytes("Captcha Diem");
				m.Content = new NetMQMessage();
				m.Content.Append("Hello, World");		
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Binary));
				Assert.That(m.Sequence, Is.EqualTo(123));       
				Assert.That(m.Flags[0], Is.EqualTo(123));          
				Assert.That(m.Flags[m.Flags.Length-1], Is.EqualTo(123));                    
				Assert.That(m.PublicKey, Is.EqualTo(Encoding.ASCII.GetBytes("Captcha Diem")));				
				Assert.That(m.Identifier, Is.EqualTo(Guid.Parse("DED9D22A-FCE1-488B-8761-84752ACC113E")));
				Assert.That(m.Address, Is.EqualTo(Encoding.ASCII.GetBytes("Captcha Diem")));				
				Assert.That(m.Content.FrameCount, Is.EqualTo(1));        
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

				clientMessage.Dispose();
				serverMessage.Dispose();
			}			
		}	
	
		[Test]
		public void TypesTest()
		{
			Action<Example> setMessage = m => 
			{
				m.Id = Example.MessageId.Types;

				m.Sequence = 123;
    			m.ClientForename = "Life is short but Now lasts for ever";
    			m.ClientSurname = "Life is short but Now lasts for ever";
    			m.ClientMobile = "Life is short but Now lasts for ever";
    			m.ClientEmail = "Life is short but Now lasts for ever";
    			m.SupplierForename = "Life is short but Now lasts for ever";
    			m.SupplierSurname = "Life is short but Now lasts for ever";
    			m.SupplierMobile = "Life is short but Now lasts for ever";
    			m.SupplierEmail = "Life is short but Now lasts for ever";
			};

			Action<Example> checkMessage = m=> 
			{
				Assert.That(m.Id, Is.EqualTo(Example.MessageId.Types));
				Assert.That(m.Sequence, Is.EqualTo(123));       
				Assert.That(m.ClientForename, Is.EqualTo("Life is short but Now lasts for ever"));         
				Assert.That(m.ClientSurname, Is.EqualTo("Life is short but Now lasts for ever"));          
				Assert.That(m.ClientMobile, Is.EqualTo("Life is short but Now lasts for ever"));           
				Assert.That(m.ClientEmail, Is.EqualTo("Life is short but Now lasts for ever"));            
				Assert.That(m.SupplierForename, Is.EqualTo("Life is short but Now lasts for ever"));       
				Assert.That(m.SupplierSurname, Is.EqualTo("Life is short but Now lasts for ever"));        
				Assert.That(m.SupplierMobile, Is.EqualTo("Life is short but Now lasts for ever"));         
				Assert.That(m.SupplierEmail, Is.EqualTo("Life is short but Now lasts for ever"));          
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

				clientMessage.Dispose();
				serverMessage.Dispose();
			}			
		}	
	}
}