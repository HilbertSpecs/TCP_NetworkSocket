//This Simple Client_Server architecture can be used to Transfer Data Structures over a TCP/IP 
	SOCK_STREAM socket.  Any function can be written in Server.c to deal with a Client in a
		particular unique response.
//While any Data Structure can be loaded into the Client.c int main() to transfer the data of
	unique desire.



Mac OSX:

	
	Application Code:
	TCPDataStructureServer.c
	TCPDataStructureClient.c

	Compiling Executables:
	gcc -o dataServer TCPDataStructureServer.c
	gcc -o dataClient TCPDataStructureClient.c

	Executables Created:
	
	dataServer
	dataClient

	Run Server:
	
	./dataServer <port Number>

	RunClient:
	
	./dataClient <server hostname> <echo String> <port Number>

*As of Now the HandleTCPClient() was re-written into A new TCPStructureStream() where the Server
  instead of Echoing the Client String in the System Call Parameters. The Client Defines
     an Abstract Data Structure within its Main Function and this DataStructure is Streamed
	through the Network at which point it is printed to standard Output on the Server Output.

In the Client Running the echo String is still required in the command-line although it does nothing
	effectively.






******************************************************************************************
				
1/16
******************************************************************************************
DataStructureTransmit.cpp	generates Output DataFiles (Length) Lines
Output Files: “location_i.txt”

AuxillaryCode: 
Used to Read in Data From Output “location_i.txt” into DataTranmissionStructure struct dataSet SampleSet_i Each Line in “location_i.txt” represents a SampleSet_i state data will be loaded and sent Line by line in the file until EOF is reached.

DataStructureTransmission_InterfaceClient.c
DataStructureTransmission_InterfaceServer.c	      

-Used to attempt reading from file loading into dataStructure
GenerateRandomTransmissionStructure.cpp
RandomMeasurements.txt
Application Code:

FileStreamTCP_Client.c
FileStreamTCP_Server.c


Executables:
<Server_IP> —>localhost::127.0.0.1

fileTcp_dataClient <Server IP> < ‘echo’ > <ServerPort>
fileTcp_dataServer <ServerPort Number>

-Client sends over N number of Pre-Loaded DataStructures

-DataStructureTransmit.cpp is used to generate arbitrary Number of DataFiles loaded with Parameters. (i.e. “location.txt”)
-An additional Program is used to read-in from these .txt or .dat files.  This Program will load in the information to struct SampleSet dataSet_i using C file fgets(char *, int, FILE *)

<<The Number of States in each Data Output file (Length) must match the Number of Lines read in and sent in FileStreamTCP_client/server.c (N) >>





