#!/bin/sh

HW=05
PROGRAM=./hw$HW-b0b36prp

if [ "$1" = "-osx" ]
then
	echo "Build for osx has been selected."
	PROGRAM=./b0b36prp-hw$HW-genref-osx
fi

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
echo "Compare optional assignments"

for i in `seq 1 4`
do
   PROBLEM=`printf "files/hw%sopt-$i" $HW`
   echo "Compare '$PROBLEM.out' and '$PROBLEM.myout'"
   diff $PROBLEM.out $PROBLEM.myout
   diff $PROBLEM.err $PROBLEM.myerr
done

return 0
