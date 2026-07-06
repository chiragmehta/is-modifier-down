# is-modifier-down

A tiny, fast macOS CLI for reading the current modifier state and testing modifier combinations.

By Chirag Mehta — https://chir.ag

## Features

- Reads the current modifier mask
- Checks `cmd`, `opt`, `ctrl`, `shift`, and `fn`
- Supports `!mod` for required-off modifiers
- Supports `+mod` for exclusive-on mode
- Prints help and version
- Apple Silicon only (`arm64`)

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

## Usage

No args prints the normalized modifier mask and exits 0:

```sh
./is-modifier-down
```

`--mask` exits with the normalized mask and no stdout:

```sh
./is-modifier-down --mask
```

Help:

```sh
./is-modifier-down -h
./is-modifier-down --help
```

Version:

```sh
./is-modifier-down -V
./is-modifier-down --version
```

Mask bits:
- `cmd` = `1`
- `opt` = `2`
- `ctrl` = `4`
- `shift` = `8`
- `fn` = `16`

Examples:

```sh
./is-modifier-down cmd opt
./is-modifier-down ctrl shift
./is-modifier-down fn
./is-modifier-down cmd shift !ctrl !fn !opt
./is-modifier-down +cmd shift
```

Supported tokens:
- `cmd` / `command`
- `opt` / `option` / `alt`
- `ctrl` / `control`
- `shift`
- `fn` / `function` / `function-key` / `globe`

Prefixes:
- `!mod` = that modifier must be off
- `+mod` = exclusive-on mode; all other modifiers must be off unless explicitly allowed

Return codes:
- `0` = success
- `1` = condition failed
- `2` = invalid argument

License: MIT
