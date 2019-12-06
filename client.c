#include<stdio.h> 
#include<string.h>
#include<stdlib.h>
#include<time.h> 
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h> 
   

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void txt_white();		//set text to normal
void txt_red();		//set red font
void txt_green();		//set green font
int createSocket();		
int controlAddr(char *ipAddr);		//function to control input address
int connectServer(char *ipAddr, int socket_creation);		//function to connect to server


int main(int argc , char *argv[])
{
	int socket_creation,control=0,msg_rcv,msg_sent;
	char server_address[16];
	char buffer_rcv[100] = {0}; 
	char buffer_send[100] = {0}; 


		//Welcome message
	system("clear");
	printf("\nWelcome to the TCP/IP 2019 socket app");
	printf("\nNicolas Macian - Universidad Nacional de Tucuman");
	printf("\n=================================================\n");
	printf("\nPress any key to run the client...");
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

	//Get server address
	while(!control){
		printf("\nType the server IP address: ");
		scanf("%s",server_address);
		control = controlAddr(server_address);
	}

	//Try server connection
	control = connectServer(server_address, socket_creation);
	if(control<0){
		txt_red();
        printf("\nWe're sorry. We couldn't establish a connection with the server."); 
        txt_white();
		return -1;
	}
	
	//Read server message
	msg_rcv = read(socket_creation , buffer_rcv , 100);
	if(msg_rcv >= 0){
		printf("\n\n\nServer sent:");
		printf("\n=======================");
		printf("\n%s",buffer_rcv);
		printf("\n=======================");
	}else{
		txt_red();
		printf("\nCouldn't receive server message");
		txt_white();
	}

	//Flush input buffer
	fgetc(stdin);

	//Send string to server
	printf("\nEnter a string: ");
	fgets(buffer_send,100,stdin);
	buffer_send[strlen(buffer_send)-1]='\0';	//remove line break char
	msg_sent = send(socket_creation,buffer_send,strlen(buffer_send),0);
	if(msg_sent >= 0){
		txt_green();
		printf("\nMessage sent to server.");
		txt_white();
	}else{
		txt_red();
		printf("\n\nCouldn't send message to server.");
		txt_white();
	}

	//Read server message
	memset(buffer_rcv,'\0',sizeof(buffer_rcv));
	msg_rcv = read(socket_creation , buffer_rcv , 100);
	if(msg_rcv >= 0){
		printf("\n\nServer answered:");
		printf("\n=======================");
		printf("\n%s",buffer_rcv);
		printf("\n======================");
	}else{
		txt_red();
		printf("\nCouldn't receive server message");
		txt_white();
	}

	//Exit message
	txt_green();
	printf("\n\nClient execution ended.");
	printf("\nGood bye!\n");
	txt_white();

	//Close connections
	shutdown(socket_creation,2);
	close(socket_creation);

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

int controlAddr(char *ipAddr){
	int n1, n2, n3, n4;
	if( sscanf(ipAddr, "%d.%d.%d.%d", &n1, &n2, &n3, &n4) == 4 ){
		if( n1 > 0 && n1 < 255){
			if( n2 >= 0 && n2 < 256){
				if( n3 >= 0 && n3 < 256){
					if( n4 >= 0 && n4 < 256){
						printf("\nIP address is: %d.%d.%d.%d",n1,n2,n3,n4);	
						return 1;
					}
				}
			}
		}
	}
	printf("\nInvalid IP address, try again.");
	return 0;
}

int connectServer(char *ipAddr, int socket_creation){
	int port=1099, con_result=-1;
	struct sockaddr_in s_address = {0};

	printf("\nClient will try to connect to server.");	//port scan between 1100 and 1120
	s_address.sin_addr.s_addr = inet_addr(ipAddr);
	s_address.sin_family = AF_INET; 

	do{
		port++;
		s_address.sin_port = htons(port);
		con_result = connect(socket_creation, (struct sockaddr *)&s_address, sizeof(s_address));
		if(con_result==0){
			txt_green();
			printf("\nConnection has been established at port %d.",port);
			txt_white();		
		}else{
			txt_red();
		    printf("\nCouldn't establish a connection at port %d.",port);
		    txt_white();
		}
	}while(con_result<0 && port<1120);

	return con_result;
}
