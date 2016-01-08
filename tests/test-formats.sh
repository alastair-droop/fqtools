#!/usr/bin/env bash

IN=../fastq
STEM=t
FQ_EXEC=../fqtools

# Clean up all temp FASTQ files:
rm ./*.fastq
rm ./*.fastq.gz

# Check that single files work OK:
echo "SINGLE FILES:"
for in_format in "fastq" "fastq.gz"
do
    [[ ${in_format} == "fastq" ]] && in_flag="F" || in_flag="f"
    for out_format in "fastq" "fastq.gz"
    do
        [[ ${out_format} == "fastq" ]] && out_flag="F" || out_flag="f"
        if [ ${in_format} == ${out_format} ]; then
            continue
        fi
        for pair in {1..2}
        do
            ${FQ_EXEC} -f${in_flag} -F${out_flag} view ${IN}/${STEM}${pair}.${in_format} > ./${STEM}${pair}.${out_format}
            ${FQ_EXEC} -f${out_flag} -F${in_flag} view ./${STEM}${pair}.${out_format} > ./${STEM}${pair}.${in_format}
            res=`./md5cmp ${IN}/${STEM}${pair}.fastq ./${STEM}${pair}.fastq`
            echo "(${in_format} -> ${out_format} -> ${in_format}) pair ${pair} result ${res}"
        done
    done
done
echo ""

# Check that paired files work OK:
echo "PAIRED FILES:"
for in_format in "fastq" "fastq.gz"
do
    [[ ${in_format} == "fastq" ]] && in_flag="F" || in_flag="f"
    for out_format in "fastq" "fastq.gz"
    do
        [[ ${out_format} == "fastq" ]] && out_flag="F" || out_flag="f"
        if [ ${in_format} == ${out_format} ]; then
            continue
        fi

        ${FQ_EXEC} -f${in_flag} -F${out_flag} view -o./temp${STEM}% ${IN}/${STEM}1.${in_format} ${IN}/${STEM}2.${in_format}
        ${FQ_EXEC} -f${out_flag} -F${in_flag} view -o./${STEM}% ./temp${STEM}1.${out_format} ./temp${STEM}2.${out_format}
        for pair in {1..2}
        do
            res=`./md5cmp ${IN}/${STEM}${pair}.fastq ./${STEM}${pair}.fastq`
            echo "(${in_format} -> ${out_format} -> ${in_format}) paired result ${pair} result ${res}"
        done
    done    
done

rm ./*.fastq
rm ./*.fastq.gz


