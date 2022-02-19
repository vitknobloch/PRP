#!/bin/sh

HW=06b

mkdir -p files
for i in `seq 1 10`
do
   PROBLEM=files/hw$HW-$i
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
done

echo ""

return 0
