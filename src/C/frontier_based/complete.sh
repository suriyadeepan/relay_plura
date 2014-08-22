#!/bin/bash
x=$1
echo " " >> $2
while [ $x -le 2000 ]
do
  echo "$x 140" >> $2
  x=$(( $x + 1 ))
done
