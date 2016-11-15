#!/bin/python

import sys
import re
import time
import argparse
import os
import subprocess
import array
import string
import ROOT
import math

from ROOT import std
from optparse import OptionParser

vstring = std.vector(std.string)
vfloat = std.vector(float)


#----function to book histos
def bookHistos(histos):
    histos["h_res_APD1"]=ROOT.TH1F("h_res_APD1","h_res_APD1",150,3.5,5.);
    histos["h_res_APD2"]=ROOT.TH1F("h_res_APD2","h_res_APD2",150,3.5,5);
    histos["h_res_APD1vsAPD2"]=ROOT.TH1F("h_res_APD1vsAPD2","h_res_APD1vsAPD2",150,-0.5,1.5);
    
#    histos["h_res_time_APD1"]=ROOT.TH1F("h_res_time_APD1","h_res_time_APD1",100,4.,5.);
#    histos["h_res_time_APD2"]=ROOT.TH1F("h_res_time_APD2","h_res_time_APD2",100,3.5,4.5);
#    histos["h_res_time_APD1vsAPD2"]=ROOT.TH1F("h_res_time_APD1vsAPD2","h_res_time_APD1vsAPD2",100,0,1.);
    

#-----main function-----
def main():
    

    ROOT.gROOT.SetBatch(True)
    ROOT.gSystem.Load("CfgManagerDict.so")
    parser = argparse.ArgumentParser (description = 'Draw plots from ROOT files')
    parser.add_argument("-r", "--run", default='100',metavar='run',type=int,
                      help="run number")
    parser.add_argument("-p", "--prefix", default='',metavar='prefix',type=str,
                      help="prefix")


    args = parser.parse_args()
    run = args.run
    prefix = args.prefix



    file=ROOT.TFile("../H4Analysis_2016_2/ntuples/"+prefix+"_"+str(run)+".root")
#    cfgNtuple=file.Get("cfg")
#    xtalNames = cfgNtuple.GetOpt(std.vector(std.string))("DigiReco.channelsNames")
    tree=file.Get("h4")
    treeHodo=file.Get("hodo")

    tree.AddFriend(treeHodo)

    outfile=ROOT.TFile("plots/plotsTiming_"+prefix+"_"+str(run)+".root","recreate")
#----histo definition---- 
    histos={}
    bookHistos(histos)

#----loop over entries-----
    for entry in tree:
        #define here the xtals for a 3x3 matrixs, centered in two different xtals
        if entry.index % 1000 ==0:
            print "Analyzing event:"
            print entry.index

        if (entry.nFibresOnX[0]!=2 or entry.nFibresOnY[0]!=2):#showering
            continue
        if (ROOT.TMath.Abs(entry.X[0])>7 or ROOT.TMath.Abs(entry.Y[0])>7 ):
            continue
        if (ROOT.TMath.Abs(entry.X[0]-entry.X[1])>1.5 or ROOT.TMath.Abs(entry.Y[0]-entry.Y[1])>1.5):
            continue
        if (entry.amp_max[entry.MCP1]<250):
            continue
        if (entry.fit_chi2[entry.APD1]>2):
            continue
        if (entry.fit_chi2[entry.APD2]>2):
            continue
        if (entry.b_rms[entry.APD1]>8):
            continue
        if (entry.b_rms[entry.APD2]>8):
            continue

        histos["h_res_APD1"].Fill(entry.fit_time[entry.APD1]-entry.time[entry.MCP1])
        histos["h_res_APD2"].Fill(entry.fit_time[entry.APD2]-entry.time[entry.MCP1])
        histos["h_res_APD1vsAPD2"].Fill(entry.fit_time[entry.APD1]-entry.fit_time[entry.APD2])

#        histos["h_res_time_APD1"].Fill(entry.time[entry.APD1]-entry.time[entry.MCP1])
#        histos["h_res_time_APD2"].Fill(entry.time[entry.APD2]-entry.time[entry.MCP1])
#        histos["h_res_time_APD1vsAPD2"].Fill(entry.time[entry.APD1]-entry.time[entry.APD2])


    f1=ROOT.TF1("f1","gaus")
    f2=ROOT.TF1("f2","gaus")
    f3=ROOT.TF1("f3","gaus")
    histos["h_res_APD1"].Fit("f1")
    histos["h_res_APD2"].Fit("f2")
    histos["h_res_APD1vsAPD2"].Fit("f3")

    res = ROOT.TVectorD(3)
    resErr = ROOT.TVectorD(3)

    res[0] = math.sqrt(f1.GetParameter(2)*f1.GetParameter(2)-0.020*0.020) #subtracting mcp resolution
    res[1] = math.sqrt(f2.GetParameter(2)*f2.GetParameter(2)-0.020*0.020) #subtracting mcp resolution
    res[2] = math.sqrt(f3.GetParameter(2)*f3.GetParameter(2)*(1-res[0]*res[0]/(res[0]*res[0]+res[1]*res[1]))) #resolution one single apd 

    resErr[0] = f1.GetParError(2)
    resErr[1] = f2.GetParError(2)
    resErr[2] = math.sqrt(f3.GetParError(2)*f3.GetParError(2)/2)

    res.Write("timingResolution")
    resErr.Write("timingResolutionError")


    #not using template fit
#    f1_time=ROOT.TF1("f1_time","gaus")
#    f2_time=ROOT.TF1("f2_time","gaus")
#    f3_time=ROOT.TF1("f3_time","gaus")
#    histos["h_res_time_APD1"].Fit("f1_time")
#    histos["h_res_time_APD2"].Fit("f2_time")
#    histos["h_res_time_APD1vsAPD2"].Fit("f3_time")
#
#    res_time = ROOT.TVectorD(3)
#    resErr_time = ROOT.TVectorD(3)
#
#    res_time[0] = math.sqrt(f1_time.GetParameter(2)*f1_time.GetParameter(2)-0.020*0.020) #subtracting mcp resolution
#    res_time[1] = math.sqrt(f2_time.GetParameter(2)*f2_time.GetParameter(2)-0.020*0.020) #subtracting mcp resolution
#    res_time[2] = f3_time.GetParameter(2)/math.sqrt(2) #resolution one single apd do weighted
#
#    resErr[0] = f1_time.GetParError(2)
#    resErr[1] = f2_time.GetParError(2)
#    resErr[2] = f3_time.GetParError(2)/math.sqrt(2)
#
#    res_time.Write("timingResolution_time")
#    resErr_time.Write("timingResolutionError_time")


    outfile.Write()
    outfile.Close()


### MAIN ###
if __name__ == "__main__":
    main()
