#!/bin/sh

HW=06
PROGRAM=./hw$HW-b0b36prp

mkdir -p files
for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Solve '$PROBLEM.in' and store the my solution to '$PROBLEM.out'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

echo ""
echo "Solve optional assignments"

for i in `seq 1 4`
do
   PROBLEM=`printf "files/hw%sopt-$i" $HW`
   echo "Solve '$PROBLEM.in' and store the my solution to '$PROBLEM.out'"
   $PROGRAM -prp-optional < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

return 0
