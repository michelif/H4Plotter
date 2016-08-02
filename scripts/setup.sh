#!/bin/sh

source /afs/cern.ch/sw/lcg/external/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
#source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.05.02/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
#source /afs/cern.ch/sw/lcg/releases/LCG_84/Python/2.7.10/x86_64-slc6-gcc49-opt/Python-env.sh 
#source /afs/cern.ch/sw/lcg/releases/LCG_84/ROOT/6.06.02/x86_64-slc6-gcc49-opt/bin/thisroot.sh 
source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.06/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh 

#export PATH=t/bin/:$PATH
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.06/x86_64-slc6-gcc49-opt/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/opt/rh/python27/root/usr/lib64/:$LD_LIBRARY_PATH

#export PYTHON_PATH=/afs/cern.ch/user/m/micheli/scratch1/H4Analysis_2016_2/lib/:$PYTHON_PATH
export PYTHON_PATH=/afs/cern.ch/user/m/micheli/scratch1/H4Analysis_2016_2/lib/:$PYTHON_PATH
export PYTHON_PATH=/opt/rh/python27/root/usr/lib64/:$PYTHON_PATH

export LD_LIBRARY_PATH=/afs/cern.ch/user/m/micheli/scratch1/H4Analysis_2016_2/DynamicTTree/lib/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/afs/cern.ch/user/m/micheli/scratch1/H4Analysis_2016_2/CfgManager/lib/:$LD_LIBRARY_PATH