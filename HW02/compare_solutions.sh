#!/bin/sh

HW=02
PROGRAM=./hw$HW-b0b36prp

mkdir -p files
for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout' and print the difference"
   diff $PROBLEM.out $PROBLEM.myout
done

return 0
