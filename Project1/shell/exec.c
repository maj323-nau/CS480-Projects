// exec.c
// Runs a parsed Command using fork(), dup2(), execvp(), and waitpid().

#include "exec.h"

int run_command(Command *cmd)
{
    pid_t pid;            // The process ID of the child process.
    int status;           // The exit status of the child process.
    int redirect_fd;      // File descriptor used for input/output redirection.

    // fork() clones the shell. Both processes continue from this line:
    //   pid == 0 -> we are the child
    //   pid  > 0 -> we are the parent (pid is the child's process ID)
    //   pid == -1 -> fork failed
    pid = fork();

    // If fork() failed, print an error and return -1 to indicate failure.
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        // ---------- CHILD PROCESS ----------

        // Input redirection ("<"): open the file for reading and make it
        // the child's stdin (fd 0). After dup2, the original fd is no
        // longer needed, so close it.
        if (cmd->infile != NULL)
        {
            redirect_fd = open(cmd->infile, O_RDONLY);
            if (redirect_fd < 0)
            {
                perror(cmd->infile);
                _exit(1);
            }
            dup2(redirect_fd, STDIN_FILENO);
            close(redirect_fd);
        }

        // Output redirection (">"): open (creating/truncating) the file for
        // writing and make it the child's stdout (fd 1).
        // O_CREAT = create if missing,
        // O_TRUNC = wipe existing contents,
        // 0644 = permissions (owner read/write, everyone else read).
        if (cmd->outfile != NULL)
        {
            redirect_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (redirect_fd < 0)
            {
                perror(cmd->outfile);
                _exit(1);
            }
            dup2(redirect_fd, STDOUT_FILENO);
            close(redirect_fd);
        }

        // Replace this child with the requested program. execvp searches
        // PATH for argv[0]. On success it NEVER returns. If we get past it,
        // the command wasn't found or couldn't run.
        execvp(cmd->argv[0], cmd->argv);
        perror(cmd->argv[0]);

        // _exit (not exit) so the child doesn't flush the shell's buffers twice
        _exit(127);
    }

    // ---------- PARENT PROCESS (the shell) ----------

    if (cmd->background)
    {
        // Background: don't wait. Print the PID so the user knows it started.
        printf("[background pid %d]\n", (int)pid);
    }
    else
    {
        // Foreground: block until THIS specific child finishes.
        status = 0;
        waitpid(pid, &status, 0);
    }

    return 0;
}

void reap_background_jobs(void)
{
    // WNOHANG = return immediately if no child has finished.
    // Loop because several background jobs may have finished since last check.
    // waitpid returns the PID of a reaped child, 0 if none are ready,
    // or -1 if there are no children at all. We stop on 0 or -1.
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        // Nothing to do; the call itself cleans up the zombie.
    }
}