#!/bin/bash

sudo cyclictest -l10000000 -m -S -p90 -i200 -h400 -q > output.txt

grep -v -e "^#" -e "^$" output.txt | tr " " "," | tr "\t" "," > histogram.csv
sed -i '1s/^/time,core1,core2,core3,core4\n /' histogram.csv
