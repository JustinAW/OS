#!/bin/sh

BRANCHLIST=($(git branch -a --contains tags/totest))

for i in ${BRANCHLIST[@]}
do
    git checkout $i
    make
    RESULTS= test
    echo $RESULTS
done
