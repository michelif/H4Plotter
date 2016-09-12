#!/bin/bash

for iStep in `seq 0 10`;
do
    echo "starting step "$iStep
    iStepPlusOne=$((iStep+1))
    rm data/optStep$iStepPlusOne.txt
    for i in `seq 1 11`;
    do
	echo $i
#	./bin/intercalibrateChannels 4683 xtal11 $i $iStep &> data/$i.dat
#	./bin/intercalibrateChannels 4695 xtal11_2 $i $iStep &> data/$i.dat
	./bin/intercalibrateChannels 4682 xtal4apd $i $iStep &> data/$i.dat
#	cat data/$i.dat |grep minimizer|awk '{print $6}'>>data/optStep$iStepPlusOne.txt
	cat data/$i.dat |grep minimizer|awk '{print $6}'>>data/opt4apdStep$iStepPlusOne.txt
    done    
    echo "done step "$iStep


done        
