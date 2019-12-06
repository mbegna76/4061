## Purpose of our program:

The purpose of our program to to return a count of all alphabetical characters that appear in text files within a hierarchy of folders. This is achieved by directory traversal, file i/o, and multi-process pipe communication.

## How to compile the program

To compile the program simply type 'make' into the command line and run it, this will produce an executable, 'mapreduce'. To run the program enter into the command line: './mapreduce folderName #mappers' where folderName is the name of the root folder to be traversed and #mappers is the number of mapper processes you would like to execute the program with.

## What exactly our program does

Our program takes a root folder and traverses through the directories within it, accessing text files. The program will then read the text files and store and return a count of all the alphabetical letters within the text files of the all directories within the root folder. 

## Team names
Moti Begna, begna002 and Wally Lu, luxx0460

## Contributions

Wally and Moti worked out the majority of the logic and wrote the code for phases 1-3 together. Moti took charge of debugging phases 1-2 of the project with Wally\'92s assistance and debugged and reviewed the code for phase 3 himself. Phase 4 was written, debugged, and reviewed by both team members. 

## Assumptions/Notes

We ran into an problem where the master process would end before the reducer process we created would end. To ammend this we tried to use a wait() call passing through the PID  of the reducer process however that did not work. We ended up using a sleep() call to get the parent master process to wait for the reducer process to complete.


