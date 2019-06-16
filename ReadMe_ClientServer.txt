				Mac OSX:
				
-->This Simple Client_Server architecture can be used to Transfer Data Structures over a TCP/IP 
	SOCK_STREAM socket.  Any function can be written in Server.c to deal with a Client in a
		particular unique response.
-->While any Data Structure can be loaded into the Client.c int main() to transfer the data of
	unique desire.

*As of Now the HandleTCPClient() was re-written into A new TCPStructureStream() where the Server
  instead of Echoing the Client String in the System Call Parameters. The Client Defines
     an Abstract Data Structure within its Main Function and this DataStructure is Streamed
	through the Network at which point it is printed to standard Output on the Server Output.

In the Client Running the echo String is still required in the command-line although it does nothing
	effectively.

ApplicationCode:
FileStreamTCP_Client.c
FileStreamTCP_Server.c

Compiling Application Code:

gcc -o fileTcp_dataClient FileStreamTCP_Client.c
gcc -o fileTcp_dataServer FileStreamTCP_Server.c


Executables Created:

fileTcp_dataClient
fileTcp_dataServer


<ServerPort> -> Choose Arbitrary Open Port of Choice
<Server_IP> —>localhost::127.0.0.1

RunningExecutables: 
./fileTcp_dataServer <ServerPort Number>
./fileTcp_dataClient <Server IP> < ‘echo’ > <ServerPort>

-Server must be up and Running for Client to form a connection!
-Client sends over N number of Pre-Loaded DataStructures
-An additional Program is used to read-in from these .txt or .dat files.  This Program will load in the information to struct SampleSet dataSet_i using C file fgets(char *, int, FILE *)
<<The Number of States in each Data Output file (Length) must match the Number of Lines read in and sent in FileStreamTCP_client/server.c (N) >>





