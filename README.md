# Simple socket app

This is a socket app written in C language to be run on Linux and MacOS. The code in this repo was compiled with gcc.

## Project description
This app is a simple socket app that uses TCP as transport protocol. 
#### Server
Once the server is initialized, a random port will be chosen between 1100 and 1120. The server proccess will be blocked until it receives an incoming connection. When the connection is established, the server will send a welcome message asking for a string. The server will show the client's string reply. It will convert it to uppercase and send it. 
#### Client
Once the client is initialized, it will ask for the server IP address and will do a port scan between 1100 and 1120 until a connection is established. It will receive a message from the server, ask for a string and send it. It will then show the server reply.


## Compilation

In order to compile the code run:

```bash
gcc fileNameHere.c
```

## Why was this coded?
This app was an assignment for the subject "TCP/IP Communications Protocol" in the Computer Engineering degree from Universidad Nacional de Tucuman, Argentina.

## License
[MIT](https://choosealicense.com/licenses/mit/)

Nicolas Macian