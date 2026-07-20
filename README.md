# ft_ls

A clean, modular starter template for rebuilding Linux `ls` in C.

This skeleton is aligned with the 42 School `ft_ls` spirit:
- strict compilation flags (`-Wall -Wextra -Werror`)
- separated modules (`cli`, `core`, `fs`, `util`)
- official `libft/` integration through root `Makefile`
- clear extension points for full feature implementation

## Project Layout

```text
.
├── include/
│   └── ft_ls.h
├── libft/
│   ├── Makefile
│   └── libft.a (generated)
├── src/
│   ├── cli/
│   │   ├── parse_args.c
│   │   └── print_help.c
│   ├── core/
│   │   └── run_ft_ls.c
│   ├── fs/
│   │   └── list_path.c
│   ├── util/
│   │   └── config.c
│   └── main.c
├── Makefile
└── README.md
```

## Build

```bash
make
```

## Run

```bash
./ft_ls
./ft_ls -a
./ft_ls -r /tmp
```

## Current State

Implemented baseline:
- argument parsing for `-a -l -r -R -t`
- `--help`
- listing files/directories
- alphabetical sorting + reverse order
- uses `libft` allocation/string helpers in parser/listing paths
- `-t` sorting by mtime
- `-R` recursive traversal
- `-l` long-format listing (permissions, links, owner, group, size, mtime)

Placeholders (to implement next):
- Linux `ls`-compatible error/ordering behavior

## Suggested Roadmap

1. Add rich file metadata model (`stat`, permissions, owner/group, time formatting).
2. Implement sorting strategies (`name`, `mtime`) with stable tie-breaking.
3. Add recursive traversal with cycle-safety (symlink handling).
4. Match GNU/BSD `ls` edge cases required by evaluator tests.
5. Add a small `tests/` harness with expected-output snapshots.
