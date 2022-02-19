#!/bin/sh

HW=04
PROGRAM=./hw$HW-b0b36prp

for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
done

echo ""

return 0
