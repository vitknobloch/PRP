#!/bin/sh

HW=03
PROGRAM=./hw$HW-b0b36prp

echo ""

for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
done

echo ""

for i in `seq 1 4`
do
   PROBLEM=`printf "files/hw%sopt-$i" $HW`
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
done

return 0
