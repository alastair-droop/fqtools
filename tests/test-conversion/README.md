`test-conversion`
===============

This script tests the validity of data converted from one format to another using `fqtools`.

Input Data
==========

The `reference` directory contains a set of files all derived from a single SAM file. The original file contained 48 paired reads.

Output
======

The tests are run as:

    ./conversion-test

The script runs through multiple options, and tests the resulting data (stored in `temp`) against the original. File comparisons are done by opening the file (decompressing if necessary) and generating a MD5 checksum of the data. Identical checksums from input and output result in a test pass.
