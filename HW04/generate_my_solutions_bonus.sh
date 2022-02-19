#!/bin/sh

HW=04b
PROGRAM=./hw$HW-b0b36prp

for i in `seq 1 10`
do
   PROBLEM=files/hw$HW-$i
   echo "Solve '$PROBLEM.in' and store the solution to '$PROBLEM.myout'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

echo ""

return 0
