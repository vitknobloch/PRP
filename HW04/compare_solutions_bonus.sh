#!/bin/sh

HW=04b
PROGRAM=./hw$HW-b0b36prp

for i in `seq 1 10`
do
   PROBLEM=files/hw$HW-$i
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
   echo ""
done

return 0
