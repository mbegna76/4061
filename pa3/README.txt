* Team names and x500 *

Moti Begna, begna002 and Wally Lu, luxx0460

* How to compile the program *

To compile the program navigate the directory to the pa3 directory and type
'make' and hit enter into the command line. This will compile the program with all
necessary C and header files. To run the program enter './wcs #consumers textfile.txt'
alternatively to run the program and print a log file enter './wcs #consumers textfile.txt -p'

* Contributions *

Wally and Moti worked on both the producer and consumer files together, discussing most of the logic
and code together for 4.0-4.3. Wally worked on the code for step 4.4 of printing out the Histogram in a
text file and Moti debugged and revised it. Moti worked on the code for step 4.5 of creating and
printing the log file and Wally reviewed and debugged it.

* Assumptions *



* What exactly our program does *

Our program reads text files and returns a text file with a histogram counting the of an alphabetical character's appearance in
the file using multi-threading and a singular process. It takes up to 3 arguments, a integer representing the
number of consumer threads to create and use, a text file to read in, and an optional flag. The integer passed
through will decide the number of consumers the program uses and the optional flag determines whether or not a
log file will be printed documenting the actions of each of the thread.

* Extra Credit ? *

We did not attempt.
