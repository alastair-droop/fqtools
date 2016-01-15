#!/usr/bin/env bash
# This script uses ART to generate a 100,000 read test set for speed testing of the fqtools suite.
# For this test, the human reference GRCh38 patch 2 was used. The value of REF should be set to point
# at a suitable reference genome FASTA file before running the script.

REF=~/reference/GRCh38p2/GRCh38p2.fa
TOTAL=100000
N_AMP=`grep ">" $REF | wc -l`
N_READS=`echo "($N_AMP + $TOTAL - 1)/$N_AMP" | bc`
art_illumina -q -na -nf 0 -i $REF -l 150 -c $N_READS -o ./test-100k
fqtools -fF -FF head -n$TOTAL test-100k.fq > test-100k.fastq
gzip -k ./test-100k.fastq
rm test-100k.fq
