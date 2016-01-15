`test-speed`
============

This script tests the speed of multiple FASTQ manipulation toolkits against both plain and compressed FASTQ files.

Before running, you must:

* Make sure that all of the tools to be tested are installed and executable; and
* Generate the input data.

Input Data
==========

The `generate-testset.sh` will use [`art_illumina`](http://www.niehs.nih.gov/research/resources/software/biostatistics/art/) to generate a pair of test files containing 100,000 reads.

Tools
=====

The tools used by default are:

* [fqtools](https://github.com/alastair-droop/fqtools)
* [fastx-toolkit](http://hannonlab.cshl.edu/fastx_toolkit/)
* [bio-awk](https://github.com/lh3/bioawk)
* [fastq-tools](http://homes.cs.washington.edu/~dcjones/fastq-tools/)
* [fast](https://github.com/tlawrence3/FAST)
* [seqmagick](https://github.com/fhcrc/seqmagick)
* [seq-tk](https://github.com/lh3/seqtk)

Output
======

Once the test data is generated and the tools installed, run

    ./speed-test > speed-test.log

The resulting log contains the number of reads processed by each tool per second.
