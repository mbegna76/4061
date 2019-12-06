* Team names and x500 *

Moti Begna, begna002 and Wally Lu, luxx0460

* How to compile the program *

To compile the program navigate to the PA4_Server and PA4_Client directories in two
different terminal windows  and type 'make' and hit enter into the command line.
This will compile the program with all necessary C and header files. To run the program on a
single machine, use two terminal windows and in one, type in and run './server port#' in one where port#
is the server port number. In the second terminal window type in and run './client filepath #ofmappers serverip port#'
where filepath is the file path of the directory you would like to traverse through, #ofmappers is the
number of mapper processes the user would like the consumer code to use, serverip is the
host server ip address, and port# is the same server port number you used to start the server.

* Contributions *

Wally worked on the socket creation and connection for the server and client code while Moti reviewed and
debugged it. Both Wally and Moti worked together on the request and response sending portion to the sockets
and debugged/reviewed it together. The communication protocol and the rest of the heavy lifting of the project was
worked on by both team members and debugged/reviewed together. Moti took charge of the log printout while Wally reviewed
the code for it.

* Assumptions *

n/a

* What exactly our program does *

Our program reads text files and returns a text file with a histogram counting the of an alphabetical character's
appearance in the file using multiprocessing, multithreading, and a TCP client/server model. The server
function of the code takes in a single argument which is the server port number to be listening on while the client
function takes in four arguments, the root directory to traverse, number of mappers, server ip address, and server port.
When the user passes through the correct arguments to the client function, it will spawn an x amount of mapper processes,
which will go and traverse through text files and return a histogram of the alphabetical characters appearances in the file.
At the end of this, the mapper processes will then send a request to the server to update a master list of alphabetical characters
counts and also log mapper process requests and server responses in a text file.

* Extra Credit ? *

We did not attempt.
