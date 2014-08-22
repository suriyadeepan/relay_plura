#!/bin/bash

sum=$(ls -l *.txt | wc -l)
paste -d" " *.txt | nawk -v s="$sum" '{
    for(i=0;i<=s-1;i++)
    {
        t1 = 2+(i*5)
        temp1 = temp1 + $t1
        t2 = 3+(i*5)
        temp2 = temp2 + $t2
        t3 = 4+(i*5)
        temp3 = temp3 + $t3
    }
    print $1" "temp1" "$5
    temp1=0
    temp2=0
    temp3=0
}'
