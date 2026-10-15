# Project 1: Simple Shell

## Overview

This project is a small interactive shell written in C. It reads one command at a time from standard input, parses it, and executes the resulting program with support for input redirection, output redirection, and background jobs.

The implementation is split across three key files:

- [shell/parse.h](shell/parse.h) and [shell/parse.c](shell/parse.c): parse user input into a `Command` structure
- [shell/exec.h](shell/exec.h) and [shell/exec.c](shell/exec.c): launch child processes with `fork()`, `dup2()`, and `execvp()`
- [shell/shell.c](shell/shell.c): the main interactive loop with a `myshell>` prompt and built-in `exit`

## Supported Features

The shell currently supports:

- command names and arguments
- input redirection using `<`
- output redirection using `>`
- background execution using `&`
- a built-in `exit` command to leave the shell
- periodic cleanup of finished background jobs

## Command Model

Each parsed command is stored in the `Command` struct defined in [shell/parse.h](shell/parse.h):

- `argv[]` contains the command and its arguments, terminated with `NULL`
- `infile` stores the filename from `<`, or `NULL` if none is present
- `outfile` stores the filename from `>`, or `NULL` if none is present
- `background` is `1` when `&` is included and `0` otherwise

The parser function `parse_line(line, cmd)` returns `0` on success and `-1` on malformed input.

## Execution Behavior

The executor in [shell/exec.c](shell/exec.c) does the following:

- forks a child process for the requested command
- redirects stdin/stdout as needed
- replaces the child with the target program using `execvp()`
- waits for foreground jobs with `waitpid()`
- prints the background PID and reaps completed jobs later with `reap_background_jobs()`

## Build and Run

From the project root, build the shell with:

```sh
make -C shell
```

Run it:

```sh
./shell/shell
```

Clean generated files:

```sh
make -C shell clean
```

## Example Usage

```text
myshell> ls -l
myshell> echo hello
myshell> cat < README.md > output.txt
myshell> sleep 5 &
myshell> exit
```

The shell prints a prompt, waits for input, runs the command, and exits cleanly on `exit` or EOF.

## Current Limitations

This shell is intentionally small and does not yet implement:

- pipelines (`|`)
- quoting and escaping rules
- environment variable expansion
- shell built-ins beyond `exit`
- job control or signal handling beyond basic background reaping

## Notes

The project builds cleanly with GCC using the warning flags in the Makefile and is intended to demonstrate a working minimal Unix-like shell for Project 1.
