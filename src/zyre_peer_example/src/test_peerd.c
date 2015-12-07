#include <stdio.h>

#include "../include/test_peer.h"
int main(void)
{
	
	example_peer_t * peer = example_peer_new ();


    if (!peer)
        return -1;          //  Interrupted

   

    zpoller_t *poller = zpoller_new (stdin, example_peer_actor(peer), NULL);
    
    while (!zsys_interrupted) {
        void *which = zpoller_wait (poller, -1); // no timeout
         if(!which)
                break; //Inerruppted 
       
        if (which == stdin){
           
            char command [1024];
            if (!fgets( command, 1024, stdin))
                break;
            
            command[strlen(command)-1] = 0; // drop the trailing linefeed
            example_peer_send_chat(peer,command);
            
            
        }
        else
        {
            zmsg_t * msg = zmsg_recv(example_peer_actor(peer));
            zmsg_print(msg);
        
        }
    }
    zpoller_destroy(&poller);
    example_peer_destroy (&peer);
    zsys_shutdown();
    return 0;
}
