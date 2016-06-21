#!/bin/python

import sys
import re
import time
import argparse
import os
import subprocess
import array
import ROOT


#-----main function-----
def main():
    
    ROOT.gROOT.SetBatch(True)
    ROOT.gSystem.Load("CfgManagerDict.so")

    parser = argparse.ArgumentParser (description = 'Draw plots from ROOT files')
    parser.add_argument('-c', '--cfg', default='', help='cfg file')
    args = parser.parse_args()

    cfg = ROOT.CfgManager()
    if args.cfg != "":
        cfg.ParseConfigFile(args.cfg)

    cfg.Print()

    run = cfg.GetOpt(int)("calibration.run4APD")
    print run
    file=ROOT.TFile("../H4Analysis_2016/ntuples/analysis_"+str(run)+".root")
#    cfgNtuple=file.Get("cfg")
#    xtalNames = cfgNtuple.GetOpt(std.vector(std.string))("DigiReco.channelsNames")
    tree=file.Get("h4")

#----loop over entries-----
    for entry in tree:
        if entry.event == 1:
        #define here the xtals for a 3x3 matrixs, centered in two different xtals
            xtalMatrix4APD = array.array('i',[tree.xtal1,tree.xtal2,tree.xtal3,tree.xtal6,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal11,tree.xtal14,tree.xtal15,tree.xtal16])
            xtalMatrixXtal11 = array.array('i',[tree.xtal11,tree.xtal2,tree.xtal3,tree.xtal4,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal12,tree.xtal15,tree.xtal16,tree.xtal17])

        for xtal in xtalMatrix4APD:
            print entry.charge_sig[xtal]



    print "tu madre"


    print "tu madre"


### MAIN ###
if __name__ == "__main__":
    main()
