`test-commands`
===============

This script tests fqtools subcommands against another method (either a simple Python script or an equivalent bash command) for random FASTQ files. No attempt is made to test file conversions, or paired files. These functions are tested elsewhere.

Before running, you must:

* Make sure that `bin/fqtools` is available, i.e. that you have created the fqtools executable; and
* Make sure you have Python3 installed.

Extra Scripts
=============

*   The `random-fastq` Python script generates random FASTQ files. No attempt is made to correctly model the quality scores; they are randomly selected from the full range of valid characters.
*   The `process-fastq` Python script performs basic analysis of a single FASTQ file

Output
======

To perform the tests, run

    ./test-commands

Each command will be tested for a random input file multiple times (by default 10,000 times).