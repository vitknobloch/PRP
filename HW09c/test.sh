#!/bin/sh

N=10000000
SEED=13

echo "Create graph with $N nodes"
./graph_creator $N $SEED g


echo "Load text graph g and save it to g.bin"
time ./txt2bin g g.bin

echo "Load g.bin and save it to g.txt"
time ./bin2txt g.bin g.txt

echo "Compare original txt file and the txt->bin->txt file"
md5sum g
md5sum g.txt
