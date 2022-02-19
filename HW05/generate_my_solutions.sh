#!/bin/sh

HW=05
PROGRAM=./hw$HW-b0b36prp

if [ "$1" = "-osx" ]
then
	echo "Build for osx has been selected."
	PROGRAM=./b0b36prp-hw$HW-genref-osx
fi

mkdir -p files
for i in `seq 1 4`
do
   PROBLEM=files/hw$HW-$i
   echo "Solve '$PROBLEM.in' and store my solution to '$PROBLEM.myout'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

echo ""
echo "Solve optional assignments"

for i in `seq 1 4`
do
   PROBLEM=`printf "files/hw%sopt-$i" $HW`
   echo "Solve '$PROBLEM.in' and store my solution to '$PROBLEM.myout'"
   $PROGRAM -prp-optional < $PROBLEM.in > $PROBLEM.myout 2>$PROBLEM.myerr
done

return 0
