/*
parse.h
    - The shell's parser.

This file defines the interface used by the shell to turn one line of user input
into a structured command description. The parser reads text such as a command
name, arguments, input/output redirection, and background execution markers, and
stores that information in a Command object that the rest of the shell can use to
launch processes.

In other words, this file is responsible for translating raw shell input into a
format that is easier for execution code to consume.
*/

#ifndef PARSE_H
#define PARSE_H

#include <string.h>

// The maximum number of arguments that the shell can handle.
#define MAX_COMMAND_ARGS 64

/* 
The shell's parsed result container.
It holds everything the shell learned from one line of input,
in a form the rest of the code can use directly.
*/
typedef struct 
{
    char *argv[MAX_COMMAND_ARGS];   // The program name and its arguments
    char *infile;                   // The filename after <, or NULL if none
    char *outfile;                  // the filename after >, or NULL if none
    int   background;               // 1 if "&" is present, 0 otherwise
} Command;

/*
parse_line() 

This function parses a single line of shell input and fills in a Command
structure with the information it finds.

Parameters:
    line - A null-terminated string containing the user's input. This is the raw
           text the shell receives from the user.
    cmd  - A pointer to a Command structure that will be filled in by the parser.
           The parser writes the command name, arguments, redirection targets, and
           whether the command should run in the background into this structure.

Output:
    The function writes its results into the Command pointed to by cmd. It returns
    an integer status value: 0 indicates success, and -1 indicates an error.
*/
int parse_line(char *line, Command *cmd);

#endif