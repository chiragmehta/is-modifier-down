# is-modifier-down

A tiny, fast macOS CLI for reading the current keyboard modifier (CMD⌘, Option⌥, Control⌃, Shift⇧, Function🌐) state and checking modifier combinations.

By Chirag Mehta — https://chir.ag

## Features

- Reads the current modifier mask
- Checks `cmd`, `opt`, `ctrl`, `shift`, and `fn`
- Supports `!mod` for required-off modifiers
- Supports `+mod` for exclusive-on mode
- Prints help and version
- Apple Silicon only (`arm64`)

## Examples:

Use these in scripts and conditionals:

```sh
if is-modifier-down cmd >/dev/null 2>&1; then
  echo "cmd is down"
fi

if is-modifier-down cmd shift !ctrl !fn !opt >/dev/null 2>&1; then
  echo "only cmd+shift is down"
fi
```

If you need a specific modifier pattern, `is-modifier-down` is designed for `if` checks:

```sh
if is-modifier-down +cmd shift !ctrl !fn !opt >/dev/null 2>&1; then
  do_something
fi
```

Loop examples (press Ctrl+C to exit):

This prints `yes` when Cmd is held, otherwise `no`, once per second. Useful for debugging scripts that branch on `cmd` state:

```sh
while true; do if is-modifier-down cmd >/dev/null 2>&1; then echo yes; else echo no; fi; sleep 1; done
```

This prints the current normalized mask once per second so you can watch it change as you press/release modifiers:

```sh
while true; do is-modifier-down; sleep 1; done
```

## Usage

No args prints the normalized modifier mask and exits 0:

```sh
./is-modifier-down
```

`--mask` exits with the normalized mask and no stdout:

```sh
./is-modifier-down --mask
```

Mask bits (shows which modifiers are pressed):
- `cmd` = `1`
- `opt` = `2`
- `ctrl` = `4`
- `shift` = `8`
- `fn` = `16`

Supported modifier args (any combination, any order):
- `cmd` / `command`
- `opt` / `option` / `alt`
- `ctrl` / `control`
- `shift`
- `fn` / `function` / `function-key` / `globe`

Prefixes:
- `!mod` = that modifier must be off (e.g. `!ctrl !shift`)
- `+mod` = exclusive-on mode; all other modifiers must be off unless explicitly allowed (e.g. `+cmd shift`)

Exit codes:
- `0` = success
- `1` = condition failed
- `2` = invalid argument
- `0-255` = if `--mask` is specified with no other args


## Build

```sh
make
```

Or directly:

```sh
clang -arch arm64 -Oz -DNDEBUG -Wall -Wextra -Wpedantic -Wl,-dead_strip is-modifier-down.c -framework CoreGraphics -o is-modifier-down && strip -x is-modifier-down
```

## Install

Copy or symlink the binary wherever you want it on your PATH:

```sh
ln -sf /path/to/is-modifier-down /usr/local/bin/is-modifier-down
```

License: MIT
