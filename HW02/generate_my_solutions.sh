#!/bin/sh

HW=02
PROGRAM=./hw$HW-b0b36prp

mkdir -p files
for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Solve '$PROBLEM.in' and store the reference solution to '$PROBLEM.myout'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.myout
done

return 0
