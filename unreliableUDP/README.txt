
-------------------------------------------------------------------README----------------------------------------------------------------------------

The Makefiles are included in the correspoding Debug folder. 
Run makefile to excute the program.

------------------------------------------------------------------Running the Client-----------------------------------------------------------------

->execute the makefile in the debug folder
->perform makeclean if necessary
->the minimum number of arguments to be given are 4 and they are
1) host address
2) port number
3) filename &
4) window size
-> the client will send the filename from the data part of the header file initially. It then waits for the segmented packets to arrive and will continue to recieve til the window size. After receiving the packets it processes the data, sends back the corresponding Ack's back to the server and the data that is processed will be written into a file 'dum.txt' which is hardcoded.

----------------------------------------------------------------Running the Server-------------------------------------------------------------------

->execute the makefile in the debug folder
->perform makeclean if necessary
->the minimum number of arguments to be given are 3 and they are
1)portnumber 
2)filename
3) window size
->the server after accepting the parameters will extract the filename from the data field of the header and looks it up and opens the file for filetransfer in read mode. It will then compute the length of the file, if it is less than or equal to MSS, it will send directly else, it will segment the file into packets and send them accordingly depending on the window size. It will also send the sequence number along with the packet data and then will wait to recieve the Ack's accordingly.

*****************************************************************************************************************************************************
NOTE:
1) Depending on the size of the input file, you might want to change the size of the MAXSIZE parameter in the Header files "header.h"
2) If program needs to be run again, please remove the textfile dum.txt from the Debug folder in the Udp_Client, because of the append mode that i have implemented on the Client Side.
3) MD5Checksum has been computed to check the integrity of the source file as well as the target file.
4) The source file has to be in the Debug Folder on the Server Side.
5) implementation has been done using static window size.
6) In case any special characters are being appended into the target file, please add the last bit as null or use the one in comment.
********************************************************************************************************************************************************
