# Brainfuck Compiler From Scratch!

## Functionality

This project is actually a non-optimizing (for now) compiler for the esoteric programming language Brainfuck.
The input brainfuck file is compiled to a real, little-endian x64 ELF, without using external programs such as an assembler.
It should in theory work on any unix-like little endian machine, which includes Linux, *BSD, and maybe even Apple Macs, but was only tested on my Arch Linux machine. 

### Usage

In order to compile a brainfuck file, run:

```
bfc <source_file> <output_binary>
```

## Story

Originally, this language was created in order to demonstrate making a simple C compiler, but the original compiler was written in assembly for some the Commodore Amiga machine running AmigaOS, and did not have to deal with any annoying x86_64 machine code weirdness.
The idea behind this came to me when I was bored in a computer architecture lecture. I thought: "hell, I'm sure writing code byte by byte would be more interesting than this lecture, so I might as well try, no?". Well, this took me about 2 weeks to get to the non-optimizing version, and while it was incredibly difficult trying to decipher the weird bullshit happening with this instruction set, it definitely was a lot more fun than any lecture in that course. 


## Build instructions

In order to compile the compiler in Linux:

```bash
make
```

And in order to run the compiler on a static `brainfuck.bf` and output the ELF `output`:

```
make run
```

In order to run a simple test which compiles `brainfuck.bf`, reads the raw hex data and the header of the output ELF, then runs it, use:

```
make test
```

Very simple.

## Optimizations
Soon, I will be adding optimizations to this compiler. 
Now, the unoptimized compiler compiling [this program](https://github.com/Dougaak/Brainfuck-Calculator/blob/master/Calculator.bf), and using it to calculate the expression `1000*200=` takes 0m9.765s on my machine. As I optimize this, I will note down the times it took to calculate this expression.

### Implemented Levels
- None: Unoptimized.
- Folding: Takes identical operations and folds them to one instruction

### Unimplemented Levels
- Pattern Recognition: Will recognize patterns such as `[-]`
- Search Optimizations (extreme): Will optimize blocks such as `[>]` to use a faster search for a zero. Is pretty extreme so I probably will never implement it

In order to test this I simply ran the following line:
```
time echo 1000*200= | ./output
```

| Optimization level    | Time      | Speedup   |
|-----------------------|-----------|-----------|
| None                  | 0m9.765s  |   x1      |
| Folding               | 0m7.882s  |   x1.238  |

## Code Architecture

The code is actually not all that complicated once you try to read it.\
Source files are located in the `src` directory, headers in the `include` directory.

- `headers.c/.h` are mostly for ELF header-related functions and definitions
- `instructions.c/.h` are for machine code instructions
- `opcodes.h` and `registers.h` contains all the opcodes, instructions, and register names and types supported by this compiler
- `operations.c/.h` are for brainfuck operation-related functions and definitions
- `stack.c/.h` is a simple stack implementation for the address stack used in the `[` and `]` operations

## Notes and Thanks

Contrary to most of my projects, I decided to not use an LLM to write this documentation. Not that it matters too much, but if you do see some error or funky phrasing then it is all human funky phrasing and not some computer error.

As for thanks, I want to thank Felix, who hosts [This very useful website](https://www.felixcloutier.com/x86/).\
I would also like to thank my friends for making sure I don't ignore my university work to work on this project. GG guys.