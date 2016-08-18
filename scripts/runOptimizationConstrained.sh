#!/bin/bash

for iStep in `seq 0 10`;
do
    echo "starting step "$iStep
    iStepPlusOne=$((iStep+1))
    rm data/optStepConstrained$iStepPlusOne.txt
    for i in `seq 1 3`;
    do
	echo $i
#	./bin/intercalibrateChannels 4683 xtal11 $i $iStep &> data/$i.dat
	./bin/intercalibrateChannels 4683 xtal11_2 $i $iStep &> data/constrained$i.dat
	cat data/constrained$i.dat |grep minimizer|awk '{print $6}'>>data/optStepConstrained$iStepPlusOne.txt
    done    
    echo "done step "$iStep


done        
