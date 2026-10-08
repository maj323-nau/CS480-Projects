/*
exec.h
    - The shell's executor.

Takes a parsed Command and runs it as a child process, handling
redirection and foreground/background execution.
*/

#ifndef EXEC_H
#define EXEC_H

#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
run_command()

Runs the command described by cmd.

    - Foreground: the shell waits until the child finishes.
    - Background (cmd->background == 1): the shell returns immediately
      and the child is reaped later by reap_background_jobs().

Returns 0 if the child was started successfully, -1 if fork() failed.
*/
int run_command(Command *cmd);

/*
reap_background_jobs()

Collects any finished background children without blocking, so they don't
linger as zombies. Call once per shell loop iteration.
*/
void reap_background_jobs(void);

#endif