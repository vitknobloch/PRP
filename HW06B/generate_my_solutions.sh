#!/bin/sh

HW=06b
PROGRAM=./hw$HW-b0b36prp

echo ""

mkdir -p files
for i in `seq 1 10`
do
   PROBLEM=files/hw$HW-$i
   echo "Solve '$PROBLEM.in' and store the my solution to '$PROBLEM.out'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

echo ""


return 0
