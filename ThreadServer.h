/*
* ThreadServer will handle the necessary communicaionts for a server to
* to communicate with another device using threads to manage sending and 
* receiving communications.  
*/

/*
* Error allows error handeling, which will throw an error given a message
* of *char[]
*/
void error(char *msg);

/*
* Once communcation with the client has been established, receiveMessage will
* handle receiving the message(s) from the client in order to process and output
* the message in a logical and simple way. It will also detect when the client
* exits the program, and notify the server user of such event
*/
void *receiveMessage(void *socket);

/*
* Once communcation with the client has been established, sendMessage will
* handle sending the message(s) to the client from the server. The messages will
* simply be inputs from the terminal, and the message will be sent to the client
* for the client to handle. 
*/
void *sendMessage(void *socket);
