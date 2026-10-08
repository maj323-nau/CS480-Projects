/* 
A simple shell that reads and executes commands from standard input.

Supported features:
    - Command name and arguments
    - Input redirection ("<")
    - Output redirection (">")
    - Background execution ("&")
    - Built-in "exit" command to terminate the shell

Each loop iteration:
    1. Displays a prompt and reads a line of input.
    2. Parses the input into a Command structure.
    3. Executes the command, handling redirection and background execution.
    4. Reaps any finished background jobs to prevent zombies.
*/

#include "parse.h"
#include "exec.h"

// Maximum length of one input line, including its terminating null byte. 
#define BUFFER 1024

int main(int argc, char *argv[])
{
    // Keep the conventional main signature; this test shell uses no options. 
    (void)argc;
    (void)argv;

    // Buffer to hold the user's input line, Command struct to hold the parsed command.
    char line[BUFFER];
    Command cmd;
    int should_exit = 0;

    // Continue reading and executing commands until exit or end-of-file.
    while (!should_exit)
    {
        // Reap any finished background jobs to prevent zombies.
        reap_background_jobs();

        // Display the prompt
        printf("myshell> ");

        // Display the prompt before fgets() waits for input.
        fflush(stdout);

        // EOF (Ctrl-D) or an input error ends the loop.
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("\n");
            should_exit = 1;
        }

        else
        {
            // Remove the newline so the exit check compares the command text.
            line[strcspn(line, "\n")] = '\0';

            // Ignore an empty line; handle the shell's exit built-in locally.
            if (line[0] != '\0')
            {
                // If the user typed "exit", set the flag to exit the shell.
                if (strcmp(line, "exit") == 0)
                {
                    should_exit = 1;
                }

                else
                {
                    // parse_line() returns 0 on success and -1 on malformed input.
                    if (parse_line(line, &cmd) == -1)
                    {
                        fprintf(stderr, "myshell: syntax error\n");
                    }

                    // If parsing succeeded, run the command.
                    else
                    {
                        run_command(&cmd);
                    }
                }
            }
        }
    }

    // Exit the shell with a success status.
    return 0;
}