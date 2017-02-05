# brainforkInterpreter

## Installation

Use the following commands to download, install and run:

```
git clone https://github.com/theTardigrade/brainforkInterpreter.git
cd brainforkInterpreter/
make
./bfi -f examples/hello.bf
```

## Usage

The `bfi` executable is called with either one of the following two option flags (in short-form or long-form) and then its corresponding argument:

| Short-form flag | Long-form flag | Argument |
| :-------------: | :------------: | -------- |
| `-f` | `--file` | The path to a file containing Brainfork sourcecode. |
| `-s` | `--string` | A string containing Brainfork sourcecode. |

It can also be called with any combination of the following argument-less option flags:

| Short-form flag | Long-form flag | Result |
| :-------------: | :------------: | ------ |
| `-l` | `--legacy` | Interprets only old-style Brainf*ck code (i.e. the character `Y` has no effect). |
| `-b` | `--byte-rollover` | If the value of the byte at the data pointer is 255, the `+` operator will cause it to rollover back to 0. Likewise, if the value is 0, the `-` operator will cause it to rollover to 255. |
| `-p` | `--pointer-rollover` | If the data pointer is at the last memory cell, the `>` operator will cause it to rollover back to the first. Likewise, if it is at the first, the `<` operator will cause it to rollover to the last. |
| `-r | `--rollover` | This is equivalent to both `--byte-rollover` and `--pointer-rollover` being set at once. |

## About Brainfork

Brainfork is a variant of and is backwards-compatible with Brainf*ck, the esoteric programming language. Both languages utilize an array of 30,000 memory cells, each one byte in size and initialized to zero, and a pointer to that array, allowing access and manipulation of data, as well as an input and output stream; they are Turing-complete (however they, arguably, also constitute [Turing tarpits](https://en.wikipedia.org/wiki/Turing_tarpit)). Brainfork differs only from its ancestor language in that it adds support for multithreading (through forking, hence the name).

### Syntax

Programs are written using only nine lexical tokens, each a single character. The `Y` token is the only one that is not also a part of Brainf*ck syntax (of which Brainfork's syntax is a superset), so this interpreter will also run programs written in the former language.

| Character | Meaning |
| :-------: | ------- |
| `>`       | Increment the data pointer by one, so that it points to the next cell. If the data pointer is already pointing at the last cell, it will do nothing, unless the `--pointer-rollover` (or more general `--rollover`) option is set, in which case it will wrap around to the first cell. |
| `<`       | Decrement the data pointer by one, so that it points to the previous cell. If the data pointer is already pointing at the first cell, it will do nothing, unless the `--pointer-rollover` (or more general `--rollover`) option  is set, in which case it will wrap around to the last cell. |
| `+`       | Increment the value of the byte at the data pointer. If the value is 255, it will do nothing, unless the `--byte-rollover` (or more general `--rollover`) option is set, in which case the value will wrap around to 0. |
| `-`       | Decrement the value of the byte at the data pointer. If the value is 0, it will do nothing, unless the `--byte-rollover` (or more general `--rollover`) option is set, in which case the value will wrap around to 255. |
| `.`       | Output the ASCII representation of the byte at the data pointer on `stdout`. |
| `,`       | Wait for one byte of input from `stdin`, storing its ASCII representation in the byte at the data pointer. |
| `[`       | If the byte at the data pointer is zero, then jump forward to the character after the matching `]` character, otherwise simply continue to the next character as usual. |
| `]`       | If the byte at the data pointer is non-zero, then jump back to the character after the matching `[` character, otherwise simply continue to the next character as usual. |
| `Y`       | Forks the current thread. In the parent thread, the byte at the data pointer is set to 0; in the child thread, the data pointer is incremented by one, moving to the next cell, and the byte there is set to 1. However, if the `--legacy` option is set, this character will have no effect whatsoever. |

