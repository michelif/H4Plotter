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

    run = cfg.GetOpt(int)("calibration.runXtal11")
    print run
    file=ROOT.TFile("../H4Analysis_2016/ntuples/analysis_"+str(run)+".root")
#    cfgNtuple=file.Get("cfg")
#    xtalNames = cfgNtuple.GetOpt(std.vector(std.string))("DigiReco.channelsNames")
    tree=file.Get("h4")

#----histo definition---- FIXME move to a function
    outfile=ROOT.TFile("plots/plots_"+str(run)+".root","recreate")
    hmatrix_xtal11_hodosel_3x3=ROOT.TH1F("chint_xtal11_matrix_hodosel_3x3","chint_xtal11_matrix_hodosel_3x3",300,40000,60000);
    hmatrix_xtal11_hodosel_3x3.SetLineColor(ROOT.kBlack)

    hmatrix_xtal11_hodosel_3x3_calib=ROOT.TH1F("chint_xtal11_matrix_hodosel_3x3_calib","chint_xtal11_matrix_hodosel_3x3_calib",300,40000,60000);
    hmatrix_xtal11_hodosel_3x3_calib.SetLineColor(ROOT.kBlack)


#----loop over entries-----
    for entry in tree:
        if entry.event == 1:
        #define here the xtals for a 3x3 matrixs, centered in two different xtals
            xtalMatrix4APD = array.array('i',[tree.xtal1,tree.xtal2,tree.xtal3,tree.xtal6,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal11,tree.xtal14,tree.xtal15,tree.xtal16])
            xtalMatrixXtal11 = array.array('i',[tree.xtal11,tree.xtal2,tree.xtal3,tree.xtal4,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal12,tree.xtal15,tree.xtal16,tree.xtal17])
            xtalCalibXtal11  = array.array('d',[1,0.960394,1.00829,0.992018,0.987667,1.00295,0.999568,0.805411,1.41717,1.2068,0.982285,1.49918])

        matrixEnXtal11=0
        matrixEnXtal11_calib=0
        index=0
        for xtal in xtalMatrixXtal11:
            matrixEnXtal11+=entry.charge_sig[xtal]
            matrixEnXtal11_calib+=entry.charge_sig[xtal]*xtalCalibXtal11[index]
            index+=1
        hmatrix_xtal11_hodosel_3x3.Fill(matrixEnXtal11)
        hmatrix_xtal11_hodosel_3x3_calib.Fill(matrixEnXtal11_calib)


    outfile.Write()
    outfile.Close()


### MAIN ###
if __name__ == "__main__":
    main()
