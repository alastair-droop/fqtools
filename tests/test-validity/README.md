`test-validity`
===============

This script tests the validity of multiple FASTQ manipulation toolkits against the Cock et. al. consensus standards.

The testset comes as supplemental information from:

Cock,P.J.A. *et al.* (2010) The Sanger FASTQ file format for sequences with quality scores, and the Solexa/Illumina FASTQ variants. *Nucleic Acids Research*, **38**, 1767–1771.

Before running, you must:

* Make sure that all of the tools to be tested are installed and executable; and
* Get the testset.

Input Data
==========

The `get-testset.sh` will download the testset from [Nucleic Acids Research](http://nar.oxfordjournals.org/content/suppl/2009/12/16/gkp1137.DC1/nar-02248-d-2009-File005.gz).

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

    ./validity-test > validity-test.log

The resulting log contains the results of running each tool against each input file, as well as a summary of the tool's performance.
