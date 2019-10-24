{\rtf1\ansi\ansicpg1252\cocoartf1671\cocoasubrtf500
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 \
## Purpose of our program\
\
The purpose of our program is to implement a function similar to the GNU "Make" function. The end user is to input a Makefile along with another argument which could be a flag or a specified target. Running the program will read in and parse the Makefile and read the targets, dependencies, and recipes and will print and or execute depending on the second argument given. \
\
## How to compile the program\
\
To compile the program have all of the code and header files in the "src" folder and the Makefiles (named in this format Makefile#) in the "test" folder. Open a terminal and go into the correct file directory (ex. "user/.../Assignment1/pa1/src") and enter "gcc main.c" into the terminal to compile the program. To run the program enter "./myMake [put flag here] [put Makefile path here]" (ex. "./myMake -p test/Makefile2), additionally you can also run the program using this format for specifying targets "./myMake [put Makefile path here] [specify target here]"\
\
## What Exactly our Program Does\
\
Our program takes a Makefile and reads the lines to be printed out and executed depending on the flags passed through the command line when runing the prorgam.\
\
## Team names\
\
Moti Begna, begna002 and Wally Lu, luxx0460\
\
## Contributions\
\
Moti contributed towards working on the parsing helper function and the -r and -p flags functionalities/cases. Wally took care of debugging and reviewing for the -r flag conditions and wrote the execution (execute function) code (forking and execv) for the condition of running the program without the flags. \
\
Other helper functions such as removeSpaces and removeColon were written by Moti and debugged/reviewed by Wally. The trimTrailing helpfer function logic was worked out by both team members and the writing was done by Moti while the testing and implementing was done by Wally. \
\
## Assumptions/Notes\
\
Went to office hours on (10/9) and worked with Aditya to solve an error with finding the renderer.c and myprog.c files when running Makefile3 and Makefile4, he confirmed our execvp call is successful however we still ocassionally run into 'no input file found' errors. Making note of this as he told us to do so in our project submission.}