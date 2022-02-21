# The Great Firewall of Santa Cruz

NOTE: This program is adapted from a Computer Systems and C Programming assignment.\
(Header files, parser.c, and speck.c provided by Professor.)

This C program is a word filtering program that filters out
and reports bad words parsed from stdin.

## Formatting

Format the code with:

```
make format
```

## Building

Build the program with:

```
make all
```

## Running

Run the program with:

```
$ ./banhammer
```
and add any of the following options to specify what you want to run.
```
OPTIONS
  -h           Program usage and help.
  -s           Print program statistics.
  -t size      Specify hash table size (default: 2^16).
  -f size      Specify Bloom filter size (default: 2^20).
```

## Cleaning

Remove all files that are compiler generated with:

```
make clean
```
