// parse.c
// This file implements the parser described in parse.h.
// Converts raw shell input into a Command struct

#include "parse.h"

#define DELIMS " \t"                      // Split input on spaces and tabs.
#define MAX_ARGS (MAX_COMMAND_ARGS - 1)   // Space for final NULL terminator in argv.
#define PARSE_OK 0                        // Successful parse value.
#define PARSE_ERROR (-1)                  // Parse error value.

int parse_line(char *line, Command *cmd)
{
    int argc = 0;         // Number of arguments currently stored in cmd->argv.
    int status = PARSE_OK; // Tracks whether parsing succeeded
    char *tok;            // Current token read from the user input.

    /*
    This means:
        - infile = NULL
        - outfile = NULL
        - background = 0
        - every argv entry = NULL
    */
    memset(cmd, 0, sizeof(*cmd));

    // Get the first token from the input line.
    tok = strtok(line, DELIMS); 

    // Loop through the rest of the tokens in the input line, processing each one.
    while (tok && status == PARSE_OK)
    {
        // Check for special tokens that indicate input/output redirection, 
        // or background execution.
        if (strcmp(tok, "<") == 0)
        {
            // If the user typed "<", the next token is the input filename.
            tok = strtok(NULL, DELIMS);

            // If there is no next token, 
            // it means the user did not provide a filename after "<".
            if (tok == NULL)
            {
                status = PARSE_ERROR; 
            }

            // A filename was present, so save it for input redirection.
            else
            {
                // Save the file name used for input redirection.
                cmd->infile = tok; 
            }
        }

        // If the user typed ">", it indicates output redirection.
        else if (strcmp(tok, ">") == 0)
        {
            // If the user typed ">", the next token is the output filename.
            tok = strtok(NULL, DELIMS);

            // If there is no next token,
            // it means the user did not provide a filename after ">".
            if (tok == NULL)
            {
                status = PARSE_ERROR; // Missing filename after ">".
            }
            // A filename was present, so save it for output redirection.
            else
            {
                // Save the file name used for output redirection.
                cmd->outfile = tok; 
            }
        }

        // If the user typed "&", it indicates the command should run in the background.
        else if (strcmp(tok, "&") == 0)
        {
            // Save the background execution flag.
            cmd->background = 1;
        }

        // If the token is not a special character, 
        // it is part of the command name or its arguments.
        else
        {
            // Check if we have space for another argument in the argv array.
            if (argc >= MAX_ARGS)
            {
                // Don't overflow the argv array.
                status = PARSE_ERROR; 
            }

            // If we have space, add the token to the argv array.
            else
            {
                cmd->argv[argc++] = tok; 
            }
        }

        // Move to the next token in the input string, if parsing has not failed.
        if (status == PARSE_OK)
        {
            tok = strtok(NULL, DELIMS); 
        }
    }

    // Add the required terminating NULL pointer for exec-style argv arrays.
    cmd->argv[argc] = NULL; 

    // If no command name or arguments were found, treat it as an error.
    if (argc == 0 && status == PARSE_OK)
    {
        status = PARSE_ERROR; 
    }

    // PARSE_OK (0) on success or PARSE_ERROR (-1) on failure.
    return status; 
}
