#!/usr/bin/env bash
# Download and extract the Cock et. al. FASTQ test set
# Please reference: Cock,P.J.A. *et al.* (2010) The Sanger FASTQ file format for sequences with quality scores, and the Solexa/Illumina FASTQ variants. *Nucleic Acids Research*, **38**, 1767–1771.

curl http://nar.oxfordjournals.org/content/suppl/2009/12/16/gkp1137.DC1/nar-02248-d-2009-File005.gz > testset.gz
mkdir cock-testset
tar -C./cock-testset -xvzf testset.gz
rm testset.gz
