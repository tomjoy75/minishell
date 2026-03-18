# Minishell

**Minimal interactive Unix shell in C** — parses user input, builds an execution tree, runs commands with pipes, redirections, and logical operators.

## Project context

School project from **42** (curriculum shell assignment). Goal: implement a usable subset of bash-like behavior from scratch in C, without relying on a shell library for parsing or execution.

## What this project demonstrates

- **Process management:** external commands via `fork`, `execve`, and `waitpid`; correct exit status propagation.
- **I/O and pipelines:** pipes and file redirections using `pipe` / `dup2`; here-documents with a dedicated subprocess and signal discipline.
- **Parsing and structure:** tokenization and syntax checks feed an **AST** (four node kinds: simple command, pipe, `&&`, `||`) before execution.
- **Robustness:** signal handlers differ for interactive REPL, running commands, and here-doc input so the shell stays usable under Ctrl-C and related cases.
- **Word expansion:** parameter expansion (`$VAR`, `$?`), pathname globbing (`*`), quote removal, and IFS-aware field splitting before command execution.

## Features / scope

| Supported | Notes |
|-----------|--------|
| Pipes `\|` | Multi-stage pipelines |
| Redirections `<` `>` `>>` | Per-command stdin/stdout |
| Here-doc `<<` | Quoted vs unquoted delimiter behavior |
| Logical ops `&&` `\|\|` | Short-circuit on exit status |
| Parentheses | Command grouping |
| Builtins (7) | `echo`, `cd`, `pwd`, `env`, `export`, `unset`, `exit` |
| Word expansion | `$VAR`, `$?`, pathname globbing (`*`), quote removal, IFS-aware splitting |
| Environment | Inherited env, mutable; rebuilt for `execve` |

**Out of scope:** job control (`&`), command separator `;`, non-interactive one-shot mode (`-c`). Tokens such as lone `&` or `\` are rejected by the lexer/parser.

## Technical stack

- **Language:** C (POSIX/Linux-oriented).
- **Build:** `gcc`, GNU Make; static helper library **libft** (42 standard library recreation).
- **Line editing:** **GNU Readline** (and history) — required at compile/link time.

## Architecture overview

1. **REPL** — `readline` loop; each non-empty line is evaluated once.
2. **Evaluate** — tokenize → syntax check → attach redirections / collect here-docs → build AST.
3. **Execute** — walk the tree: run pipes and logical nodes recursively; for leaf commands, expand words then either run a builtin or `fork` + `execve` with the right FDs.

Environment lives in a dedicated structure (hash buckets + shell-local vars such as `$?` and IFS).

## Repository structure

```
minishell/
├── Makefile
├── include/          # minishell, parse/exec, env, expansion, signals headers
├── srcs/
│   ├── parsing/      # lexer, syntax, tree, here-doc collection
│   ├── execution/    # fork/exec, pipes, apply redirections
│   ├── expansion/    # $var, globs, quotes, redir targets
│   ├── env_variables/
│   └── builtins/
├── libft/            # linked static library
├── doc/              # optional reference links
└── test*/            # legacy fixtures / scripts (not wired to `make test`)
```

## Challenges and technical decisions

- **Pipe layout:** allocate and connect all pipe FDs for a pipeline before forking children, then close unused ends in each process to avoid hangs.
- **Builtins vs externals:** builtins run in the shell process when not in a pipeline; in a pipe they run in a child like any other command so stdin/stdout chain correctly.
- **Signals:** separate disposition for interactive prompt, child execution, and here-doc reader so SIGINT clears the line or stops the right process without corrupting global state.
- **TTY-only entrypoint:** the binary requires a real terminal on stdin and stdout (`isatty`); piping or redirecting the shell itself causes immediate exit. There is no `-c` flag to run a single command string.

## Results / validation

Build verified manually (`make`). The following behaviors were checked by hand inside `./minishell`:

- Builtins: `echo`, `pwd`
- `export` / `unset`
- Pipe, redirection
- `&&` / `||`
- Here-doc
- Exit status: after `false`, `$?` is **1**; after a non-existent command, `$?` is **127**

There is **no** automated test suite aligned with the current TTY-only binary; older shell scripts in the repo assume a different invocation model and are not used as proof of correctness here.

## Installation / usage

**Dependency (Debian/Ubuntu):**

```bash
sudo apt install build-essential libreadline-dev
```

(Fedora/RHEL: `readline-devel`; macOS: often via Xcode CLT / Homebrew `readline`.)

**Build and run:**

```bash
make
./minishell
```

Use `exit` or Ctrl-D to leave. Run only from a terminal (TTY).

## Possible improvements

- Optional **`-c 'command'`** or stdin batch mode for scripting and CI-friendly checks.
- Centralized **integration tests** driven by a PTY (`expect` or similar) so behavior is reproducible without manual steps.
- Narrow **README** for external testers if legacy `test_*.sh` assets are kept or retired.

## Contribution

This project was developed as part of the 42 curriculum in a team setting.
This repository is my portfolio version of the project, prepared to document it clearly for recruiters and technical reviewers.

Original teammate reference:
- [Nicolas Rea](https://github.com/NicolasReaItalo)
