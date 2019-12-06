#include<stdio.h> 
#include<string.h>
#include<stdlib.h>
#include<time.h> 
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h> 
#include<ctype.h>
   

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void txt_white();		//set text to normal
void txt_red();		//set red font
void txt_green();		//set green font
int createSocket();		
int choosePort();		//function to choose which port will be opened
int bindSocket(int socket_creation, int port);		
void msgToUppercase(char *msg_rcv, char *msg_send);	

int main(int argc , char *argv[])
{
	int socket_creation,port,con_result, msg_sent, msg_rcv;
	struct sockaddr_in client_addr;
	int client_addr_len = sizeof(struct sockaddr_in);
	char *hello = "Welcome to the server. \nType a string no longer than 100 characters "; 
	char buffer_rcv[100] = {0}; 
	char buffer_send[100] = {0}; 


	//Welcome message
	system("clear");
	printf("\nWelcome to the TCP/IP 2019 socket app");
	printf("\nNicolas Macian - Universidad Nacional de Tucuman");
	printf("\n=================================================\n");
	printf("\nPress any key to run the server...");
	fgetc(stdin);
	
	//Socket creation
	socket_creation = createSocket();
	if (socket_creation < 0) 
    { 
		txt_red();
		printf("\nError. Server couldn't create the socket.");
		printf("\nGood bye!");
		txt_white();
		return -1;
    } 

	//Choose random port
	port = choosePort();

	//Bind socket
	if(bindSocket(socket_creation, port) < 0)
	{
		txt_red();
		printf("\nError. Server couldn't bind socket.");
		printf("\nGood bye!");
		txt_white();
		return -1;
	}

	
	//Set server to listen, convert socket to pasive
	if(listen(socket_creation,3) < 0){	//up to 3 connections in the queu
		txt_red();
		printf("\nServer can't listen to new connections.");
		txt_white();
		return -1;
	}else{
		txt_green();
		printf("\nServer went passive.");
		txt_white();
	}
	
	printf("\n\n\nPress any key to start accepting incoming connections...");
	fgetc(stdin);
	
	//Accept incoming connection
	con_result = accept(socket_creation, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr_len);
	if (con_result < 0)
	{
		txt_red();
		printf("\nServer coulnt't accept new connections.");
		txt_white();
		return -1;	
	}else{
		txt_green();
		printf("\n\nServer accepted a connection.");
		txt_white();
		printf("\nClient's connection data:");
		printf("\n=================================");
		printf("\nClient IP address: %s", inet_ntoa(client_addr.sin_addr));
		printf("\nPort: %d", (int) ntohs(client_addr.sin_port));
	}
	
	//Send welcome message to client
	msg_sent = send(con_result,hello,strlen(hello),0);
	if(msg_sent >= 0){
		txt_green();
		printf("\n\nServer has sent welcome message to client.");
		txt_white();
	}else{
		txt_red();
		printf("\n\nServer couldn't send welcome message to client.");
		txt_white();
	}
	//Read client reply
	msg_rcv = read(con_result , buffer_rcv , 100);
	if(msg_rcv >= 0){
		printf("\nClient has sent the string:");
		printf("\n======================");
		printf("\n%s",buffer_rcv);
		printf("\n======================");
	}else{
		txt_red();
		printf("\nServer couldn't receive client's reply message. ");
		txt_white();
	}
	//Convert client string to uppercase and send
	msgToUppercase(buffer_rcv,buffer_send);
	msg_sent = send(con_result,buffer_send,strlen(buffer_send),0);
	if(msg_sent >= 0){
		txt_green();
		printf("\nServer has sent the converted string:");
		txt_white();
		printf("\n%s",buffer_send);
	}else{
		txt_red();
		printf("\nServer couldn't send the converted string.");
		txt_white();
	}

	//Exit message
	txt_green();
	printf("\n\nServer execution ended.");
	printf("\nGood bye!\n");
	txt_white();

	//Close socket connections
	shutdown(socket_creation,2);
	close(con_result);

	return 0;
}

void txt_white(){
	printf(WHT RESET);
}
void txt_red(){
	printf(RED);
}
void txt_green(){
	printf(GRN);
}

int createSocket(){
	int socket_creation;
	socket_creation = socket(AF_INET, SOCK_STREAM, 0); 	//use TCP as transport protocol
    if (socket_creation < 0) 
    { 
		txt_red();
        printf("\nFailed to create socket."); 
        txt_white();
    }else{
		txt_green();
		printf("\nSocket succesfully created.");
		txt_white();
	}
	return socket_creation;
}

int choosePort(){
	int port;
	srand(time(0));		//random seed
	port = rand() % 21 + 1100; 		//select port between 1100 and 1120
	printf("\nPort %d has been randomly selected.",port);
	return port;
}

int bindSocket(int socket_creation, int port){
	int ret_value=-1;
	struct sockaddr_in s_address = {0};
	s_address.sin_family = AF_INET;		//select IPv4 as address family
	s_address.sin_addr.s_addr = htonl(INADDR_ANY);  //bind socket to all host interfaces
	s_address.sin_port = htons(port);
	ret_value = bind(socket_creation,(struct sockaddr *)&s_address,sizeof(s_address));

	if (ret_value < 0) 
    { 
		txt_red();
        printf("\nFailed to bind socket."); 
        txt_white();
    } 
	else{
		txt_green();
		printf("\nSocket bind was succesful.");
		txt_white();
	}
    return ret_value;
}

void msgToUppercase(char *msg_rcv, char *msg_send){
	int i;
	for(i=0;i<strlen(msg_rcv);i++){
		msg_send[i]=toupper(msg_rcv[i]);
	}
}


