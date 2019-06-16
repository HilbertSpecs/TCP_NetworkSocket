#include <stdio.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<stdlib.h>
#include <netdb.h>
#include "Practical.h"


float a, b, c, d, e, f, g, h;

struct dataSet{
    
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
    double g;
    double h;
    
};

struct dataSet SampleSet;

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int AcceptTCPConnection(int);
void DieWithUserMessage(const char *, const char *);
void DieWithSystemMessage(const char *);
void HandleTCPClient(int);
void TCPStructureStream(int);
int SetupTCPServerSocket(const char *);
void PrintSocketAddress(const struct sockaddr *, FILE *);

int main(int argc, char *argv[]) {
    
	 if (argc != 2) // Test for correct number of arguments
    		DieWithUserMessage("Parameter(s)", "<Server Port/Service>");
  
	char *service = argv[1]; // First arg:  local port
  							// Create socket for incoming connections

	int servSock = SetupTCPServerSocket(service);
  	if (servSock < 0)
    		DieWithUserMessage("SetupTCPServerSocket() failed", service);
  	for (;;) { // Run forever
   				 			// New connection creates a connected client socket
    		int clntSock = AcceptTCPConnection(servSock);
        /*
        FILE *instream = fdopen(clntSock, "r");
        if(instream == NULL)
            DieWithSystemMessage("fdopen() failed");
        if(fread(&SampleSet, sizeof(struct dataSet), 1, instream) != 1 ){
            DieWithSystemMessage("SockStream(): failed");
        }
        SampleSet.a = ntohs(SampleSet.a);
        SampleSet.b = ntohs(SampleSet.b);
        SampleSet.c = ntohl(SampleSet.c);
        
        //THE Data should exist in the Server instream need to print instream to std
        
        printf("The Data Recieved From Client: %.2f\t%.2f\t%.2f\n", SampleSet.a, SampleSet.b, SampleSet.c);
        */
        // fclose(instream);
        
        //HandleTCPClient(clntSock); 			// Process client
        TCPStructureStream(clntSock);
        
        //**SEND Data Structure HERE**
        
        close(clntSock);
       
  		}
  							// NOT REACHED
}


int AcceptTCPConnection(int servSock) {
struct sockaddr_storage clntAddr; // Client address
// Set length of client address structure (in-out parameter)
socklen_t clntAddrLen = sizeof(clntAddr);
// Wait for a client to connect
int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
if (clntSock < 0)
        DieWithSystemMessage("accept() failed");

// clntSock is connected to a client!
fputs("Handling client ", stdout);
PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
fputc('\n', stdout);

 return clntSock;
}


void DieWithUserMessage(const char *msg, const char *detail)
{
        fputs(msg, stderr);
        fputs(": ", stderr);
        fputs(detail, stderr);
        fputc('\n', stderr);

        exit(1);
}

void DieWithSystemMessage(const char *msg)
{
  perror(msg);
  exit(1);
}


void HandleTCPClient(int clntSocket) {
  char buffer[BUFSIZE]; // Buffer for echo string
  // Receive message from client
  ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithSystemMessage("recv() failed");
// Send received string and receive again until end of stream
  while (numBytesRcvd > 0) { // 0 indicates end of stream
    // Echo message back to client
    ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");
    else if (numBytesSent != numBytesRcvd)
      DieWithUserMessage("send()", "sent unexpected number of bytes");
    // See if there is more data to receive
    numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0)
      DieWithSystemMessage("recv() failed");
  }
    
  close(clntSocket); // Close client socket
}


int SetupTCPServerSocket(const char *service) {// Construct the server address structure
        struct addrinfo addrCriteria;
        memset(&addrCriteria, 0, sizeof(addrCriteria));
        addrCriteria.ai_family = AF_UNSPEC;
        addrCriteria.ai_flags = AI_PASSIVE;
        addrCriteria.ai_socktype = SOCK_STREAM;
        addrCriteria.ai_protocol = IPPROTO_TCP;

        struct addrinfo *servAddr; // List of server addresses
        int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
        if (rtnVal != 0)
          DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

        int servSock = -1;
        for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) { // Create a TCP socket
        servSock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
        if (servSock < 0)
          continue; // Socket creation failed; try next address

         // Bind to the local address and set socket to list
         if ((bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) == 0) &&
                (listen(servSock, MAXPENDING) == 0)) {
        // Print local address of socket
         struct sockaddr_storage localAddr;
         socklen_t addrSize = sizeof(localAddr);
         if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
            DieWithSystemMessage("getsockname() failed");
         fputs("Binding to ", stdout);
         PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
         fputc('\n', stdout);
         break; // Bind and list successful
        }

        close(servSock); // Close and try again
        servSock = -1;
   }
// Free address list allocated by getaddrinfo()
         freeaddrinfo(servAddr);

         return servSock;

}

void PrintSocketAddress(const struct sockaddr *address, FILE *stream) {
  // Test for address and stream
  if (address == NULL || stream == NULL)
return;
  void *numericAddress; // Pointer to binary address
  // Buffer to contain result (IPv6 sufficient to hold IPv4)
  char addrBuffer[INET6_ADDRSTRLEN];
  in_port_t port; // Port to print
  // Set pointer to address based on address family
  switch (address->sa_family) {
  case AF_INET:
    numericAddress = &((struct sockaddr_in *) address)->sin_addr;
    port = ntohs(((struct sockaddr_in *) address)->sin_port);
    break;
  case AF_INET6:
    numericAddress = &((struct sockaddr_in6 *) address)->sin6_addr;
    port = ntohs(((struct sockaddr_in6 *) address)->sin6_port);
    break;
default:
  fputs("[unknown type]", stream);
  return;
// Unhandled type
  }
  // Convert binary to printable address
  if (inet_ntop(address->sa_family, numericAddress, addrBuffer,
      sizeof(addrBuffer)) == NULL)
    fputs("[invalid address]", stream); // Unable to convert
  else {
    fprintf(stream, "%s", addrBuffer);
    if (port != 0)                // Zero not valid in any socket addr
      fprintf(stream, "-%u", port);
  }
}

void TCPStructureStream(int clntSocket){
    
    
        
     FILE *instream = fdopen(clntSocket, "r");
     if(instream == NULL)
     DieWithSystemMessage("fdopen() failed");
    
    for(int i = 0; i < 100;i++){
        if(fread(&SampleSet, sizeof(struct dataSet), 1, instream) != 1 ){
     DieWithSystemMessage("SockStream(): failed");
     }
    
    SampleSet.a = ntohs(SampleSet.a);
    SampleSet.b = ntohs(SampleSet.b);
    SampleSet.c = ntohs(SampleSet.c);
    SampleSet.d = ntohs(SampleSet.d);
    SampleSet.e = ntohs(SampleSet.e);
    SampleSet.f = ntohs(SampleSet.f);
    SampleSet.g = ntohs(SampleSet.g);
    SampleSet.h = ntohs(SampleSet.h);
    
    printf("The Data Recieved From Client: %f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", SampleSet.a, SampleSet.b, SampleSet.c, SampleSet.d, SampleSet.e, SampleSet.f, SampleSet.g, SampleSet.h);
    }
    
    fclose(instream);
    close(clntSocket);
}


