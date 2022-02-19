#!/bin/sh

HW=04
PROGRAM=./hw$HW-b0b36prp

for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Solve '$PROBLEM.in' and store the solution to '$PROBLEM.myout'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

echo ""

return 0
