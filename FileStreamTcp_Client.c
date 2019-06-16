#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <limits.h>
#include "Practical.h"

float a, b, c, d, e, f, g, h;
float one, two, three;

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
struct dataSet SampleSet_2;

static const int MAXPENDING = 5; // Maximum outstanding connection requests



void DieWithUserMessage(const char *, const char *);
void DieWithSystemMessage(const char *);
int SetupTCPClientSocket(const char *, const char *);
void PrintSocketAddress(const struct sockaddr *, FILE *);


int main(int argc, char *argv[]) { 

    
	 if (argc < 3 || argc > 4) // Test for correct number of arguments
		 DieWithUserMessage("Parameter(s)", "<Server Address/Name> <Echo Word> [<Server Port/Service>]");

	 char *server = argv[1]; // First arg: server address/name
	 char *echoString = argv[2]; // Second arg: string to echo
    
    // Third arg (optional): server port/service

	char *service = (argc == 4) ? argv[3] : "echo";
    
    // Create a connected TCP socket
	
	int sock = SetupTCPClientSocket(server, service);
	if (sock < 0)
		DieWithUserMessage("SetupTCPClientSocket() failed", "unable to connect");
	//size_t echoStringLen = strlen(echoString); // Determine input length

	 // Send the string to the server
	//ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
    
    
    
    FILE *str =fdopen(sock, "w");
    if(str == NULL)
        DieWithSystemMessage("fdopen() failed");
    
    unsigned seed = time(0);
    srand(seed);
    
    for(int i = 0; i < 100;i++){
    
        SampleSet.a = 0;
        SampleSet.b = 0;
        SampleSet.c = 0;
        SampleSet.d = 0;
        SampleSet.e = 0;
        SampleSet.f = 0;
        SampleSet.g = 0;
        SampleSet.h = 0;
        
        SampleSet.a = (double)rand();
        SampleSet.b = (double)rand();
        SampleSet.c = (double)rand();
        SampleSet.d = (double)rand();
        SampleSet.e = (double)rand();
        SampleSet.f = (double)rand();
        SampleSet.g = (double)rand();
        SampleSet.h = (double)rand();
        
        SampleSet.a = htons(SampleSet.a);
        SampleSet.b = htons(SampleSet.b);
        SampleSet.c = htons(SampleSet.c);
        SampleSet.d = htons(SampleSet.d);
        SampleSet.e = htons(SampleSet.e);
        SampleSet.f = htons(SampleSet.f);
        SampleSet.g = htons(SampleSet.g);
        SampleSet.h = htons(SampleSet.h);
    
    
        if(send(sock, &SampleSet, sizeof(SampleSet), 0) != sizeof(SampleSet)){
            DieWithSystemMessage("SockStream(): failed");
    }
}
    
    // struct dataSet SampleSet;
    /*
    if (numBytes < 0)
	  DieWithSystemMessage("send() failed");
	else if (numBytes != echoStringLen)
	  DieWithUserMessage("send()", "sent unexpected number of bytes");
    
	// Receive the same string back from the server
	unsigned int totalBytesRcvd = 0; // Count of total bytes received
	fputs("Received: ", stdout); // Setup to print the echoed string
	 while (totalBytesRcvd < echoStringLen) {
	   char buffer[BUFSIZE]; // I/O buffer
	// Receive up to the buffer size (minus 1 to leave space for
	// a null terminator) bytes from the sender
	numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
	 if (numBytes < 0)
	   DieWithSystemMessage("recv() failed");
	else if (numBytes == 0)
	   DieWithUserMessage("recv()", "connection closed prematurely");
	totalBytesRcvd += numBytes;
	buffer[numBytes] = '\0';
	fputs(buffer, stdout);
	}
     
	fputc('\n', stdout); // Print 53
    */

// Keep tally of total bytes
// Terminate the string!
// Print the buffer a final linefeed
    
    
	close(sock);
    
    fclose(str);
	
    exit(0);

}


// Warning:  Untested preconditions (e.g., 0 <= size <= 8)
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



int SetupTCPClientSocket(const char *host, const char *service) {
// Tell the system what kind(s) of address info
	struct addrinfo addrCriteria;
	memset(&addrCriteria, 0, sizeof(addrCriteria));
	addrCriteria.ai_family = AF_UNSPEC;
	addrCriteria.ai_socktype = SOCK_STREAM;
	addrCriteria.ai_protocol = IPPROTO_TCP;

	struct addrinfo *servAddr; // Holder for returned list of server addrs
	int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
	if (rtnVal != 0)
        	DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));


	int sock = -1;
	for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
            sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
            if (sock < 0)
                continue; 	// Socket creation failed; try next address
 				// Establish the connection to the echo server

        if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)
                break; 			// Socket connection succeeded; break and return socket

        close(sock); // Socket connection failed; try next address
        sock = -1;
 										   }

 freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo()
  return sock;
 
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
  if (inet_ntop(address->sa_family, numericAddress, addrBuffer, sizeof(addrBuffer)) == NULL)
    fputs("[invalid address]", stream); // Unable to convert
  else {
    fprintf(stream, "%s", addrBuffer);
    if (port != 0)                // Zero not valid in any socket addr
      fprintf(stream, "-%u", port);
  }
}



