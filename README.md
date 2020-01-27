[![Build Status](https://travis-ci.org/vaofford/fqtools.svg?branch=master)](https://travis-ci.org/vaofford/fqtools)
[![Docker Repository on Quay.io](https://quay.io/repository/vaofford/fqtools/status "Docker Repository on Quay.io")](https://quay.io/repository/vaofford/fqtools)
[![Install with Bioconda](https://img.shields.io/badge/install%20with-bioconda-brightgreen.svg?style=flat)](http://bioconda.github.io/recipes/fqtools/README.html)


Introduction
============

`fqtools` is a software suite for fast processing of `FASTQ` files. Various file manipulations are supported. See below for a full list of the subcommands available and a brief description of their purpose. Most of the individual subcommands will take either a single file or a pair of files as input. If no input file is specified, fqtools will attempt to read data from `stdin`. In this case, it is advisabe to specify the format of the data provided. For subcommands that generate FASTQ data, either a single file or a pair of files will be generated. If no `-o` argument is provided, single files will be writted to `stdout`.

Citation
========

If you use `fqtools` in pblished work, please can you include a reference to my [Bioinformatics paper](https://doi.org/10.1093/bioinformatics/btw088):

*   Droop, A. P. (2016). fqtools: An efficient software suite for modern FASTQ file manipulation. *Bioinformatics* (Oxford, England). [**DOI**:[10.1093/bioinformatics/btw088](https://doi.org/10.1093/bioinformatics/btw088)]

Installation
============

`fqtools` requires building against both the [zlib](http://zlib.net) and [htslib](https://github.com/samtools/htslib) libraries:

* `zlib` is required for processing compressed (`.gz`) data. The code relies on several recent zlib file IO functions, so must be a version >= 1.2.3.5.
* `htslib` is required for reading BAM files. If htslib is not installed, download and compile `htslib`. Then, alter the `HTSDIR` path in the `fqtools` Makefile to point to the htslib source directory.

If ZLib is already installed, building can be performed similar to the following:

    git clone https://github.com/alastair-droop/fqtools
    cd fqtools/
    git clone https://github.com/samtools/htslib
    cd htslib/
    autoheader
    autoconf 
    ./configure
    make
    make install
    cd ..
    make

You might need to run the `make install` as `sudo make install`. The `htslib` library must be installed into a location that the built `fqtools` program can find (as `fqtools` executable is [dynamically linked](https://en.wikipedia.org/wiki/Dynamic_linker) to the `htslib` library). So, if you can not (or do not want to) install HTSlib, you must add the location of the `libhts.so` file to your `LD_LIBRARY_PATH` variable.
        
Licence
=======

`fqtools` is released under the [GNU General Public License version 3](http://www.gnu.org/licenses/gpl.html).

Subcommands
===========

The `fqtools` suite contains the following subcommands:

* **`view`**      View FASTQ files
* **`head`**      View the first reads in FASTQ files
* **`count`**     Count FASTQ file reads
* **`header`**    View FASTQ file header data
* **`sequence`**  View FASTQ file sequence data
* **`quality`**   View FASTQ file quality data
* **`header2`**   View FASTQ file secondary header data
* **`fasta`**     Convert FASTQ files to FASTA format
* **`basetab`**   Tabulate FASTQ base frequencies
* **`qualtab`**   Tabulate FASTQ quality character frequencies
* **`type`**      Attempt to guess the FASTQ quality encoding type
* **`validate`**  Validate FASTQ files
* **`find`**      Find FASTQ reads containing specific sequences
* **`trim`**      Trim reads in a FASTQ file
* **`qualmap`**   Translate quality values using a mapping file


Each subcommand has its own set of arguments. The global arguments are:

* **`-h`**               Show this help message and exit.
* **`-v`**               Show the program version and exit.
* **`-d`**               Allow DNA sequence bases       (ACGTN)
* **`-r`**               Allow RNA sequence bases       (ACGUN)
* **`-a`**               Allow ambiguous sequence bases (RYKMSWBDHV)
* **`-m`**               Allow mask sequence base       (X)
* **`-u`**               Allow uppercase sequence bases
* **`-l`**               Allow lowercase sequence bases
* **`-p CHR`**           Set the pair replacement character (default "%")
* **`-b BUFSIZE`**       Set the input buffer size
* **`-B BUFSIZE`**       Set the output buffer size
* **`-q QUALTYPE`**      Set the quality score encoding
* **`-f FORMAT`**        Set the input file format
* **`-F FORMAT`**        Set the output file format
* **`-i`**               Read interleaved input file pairs
* **`-I`**               Write interleaved output file pairs

**`CHR`**

This character will be replaced by the pair value when writing paired files.

**`BUFSIZE`**

Possible suffixes are [bkMG]. If no suffix is given, value is in bytes.

**`QUALTYPE`**

* **`u`** Do not assume specifc quality score encoding
* **`s`** Interpret quality scores as Sanger encoded
* **`o`** Interpret quality scores as Solexa encoded
* **`i`** Interpret quality scores as Illumina encoded

**`FORMAT`**

* **`F`** uncompressed FASTQ format (.fastq)
* **`f`** compressed FASTQ format (.fastq.gz)
* **`b`** unaligned BAM format (.bam)
* **`u`** attempt to infer format from file extension, (default .fastq.gz)
