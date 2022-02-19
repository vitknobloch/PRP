#!/bin/sh

HW=08

mkdir -p files

echo ""
echo "Compare non-optional assignments"

for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
done

echo ""

return 0
