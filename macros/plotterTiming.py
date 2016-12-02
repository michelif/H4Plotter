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
def bookHistos(histos,run):
    histos["h_normalized_amp_APD1"]=ROOT.TH1F("h_normalized_amp_APD1","h_normalized_amp_APD1",500,0,250);
    histos["h_normalized_amp_APD2"]=ROOT.TH1F("h_normalized_amp_APD2","h_normalized_amp_APD2",500,0,250);
    histos["h_normalized_amp_APD1andAPD2"]=ROOT.TH1F("h_normalized_amp_APD1andAPD2","h_normalized_amp_APD1andAPD2",1200,0,600);
    histos["h_normalized_amp_APD1vsAPD2"]=ROOT.TH1F("h_normalized_amp_APD1vsAPD2","h_normalized_amp_APD1vsAPD2",500,0,250);
    if(run != 5789):
        histos["h_res_APD1"]=ROOT.TH1F("h_res_APD1","h_res_APD1",150,3.5,5.);
        histos["h_res_APD2"]=ROOT.TH1F("h_res_APD2","h_res_APD2",150,3.5,5);
        histos["h_res_APD1andAPD2"]=ROOT.TH1F("h_res_APD1andAPD2","h_res_APD1andAPD2",150,3.5,5);
        histos["h_res_APD1vsAPD2"]=ROOT.TH1F("h_res_APD1vsAPD2","h_res_APD1vsAPD2",150,-0.5,1.5);
    else:
        histos["h_res_APD1"]=ROOT.TH1F("h_res_APD1","h_res_APD1",150,3.0,5.5);
        histos["h_res_APD2"]=ROOT.TH1F("h_res_APD2","h_res_APD2",150,3.0,5.5);
        histos["h_res_APD1andAPD2"]=ROOT.TH1F("h_res_APD1andAPD2","h_res_APD1andAPD2",150,3.0,5.5);
        histos["h_res_APD1vsAPD2"]=ROOT.TH1F("h_res_APD1vsAPD2","h_res_APD1vsAPD2",150,-1.0,2.0);


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
    parser.add_argument("-f", "--fft",dest='usefft',action='store_true',
                       help="use fft tree")
    parser.set_defaults(usefft=False)



    args = parser.parse_args()
    run = args.run
    prefix = args.prefix
    usefft = args.usefft

    file=ROOT.TFile("../H4Analysis_2016_2/ntuples/"+prefix+"_"+str(run)+".root")
#    cfgNtuple=file.Get("cfg")
#    xtalNames = cfgNtuple.GetOpt(std.vector(std.string))("DigiReco.channelsNames")
    tree=file.Get("h4")
    if (usefft):
        print "Using fft tree"
        tree=file.Get("fft_digi")
        

    treeHodo=file.Get("hodo")

    tree.AddFriend(treeHodo)

    outfile=ROOT.TFile("plots/plotsTiming_"+prefix+"_"+str(run)+".root","recreate")
#----histo definition---- 
    histos={}
    bookHistos(histos,run)

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
        histos["h_res_APD1andAPD2"].Fill((entry.fit_time[entry.APD2]+entry.fit_time[entry.APD1])/2.-entry.time[entry.MCP1])
        #fill the normalized amplitudes
        amp1=entry.amp_max[entry.APD1]/entry.b_rms[entry.APD1]
        amp2=entry.amp_max[entry.APD2]/entry.b_rms[entry.APD2]
        histos["h_normalized_amp_APD1"].Fill(amp1)
        histos["h_normalized_amp_APD2"].Fill(amp2)
        eff_ampl_diff=amp1*amp2/(math.sqrt(amp1*amp1+amp2*amp2))

        histos["h_normalized_amp_APD1vsAPD2"].Fill(eff_ampl_diff)
        histos["h_normalized_amp_APD1andAPD2"].Fill(2*eff_ampl_diff)

#        histos["h_res_time_APD1"].Fill(entry.time[entry.APD1]-entry.time[entry.MCP1])
#        histos["h_res_time_APD2"].Fill(entry.time[entry.APD2]-entry.time[entry.MCP1])
#        histos["h_res_time_APD1vsAPD2"].Fill(entry.time[entry.APD1]-entry.time[entry.APD2])


    f1=ROOT.TF1("f1","gaus")
    f2=ROOT.TF1("f2","gaus")
    f3=ROOT.TF1("f3","gaus")
    f4=ROOT.TF1("f4","gaus")
    f5=ROOT.TF1("f5","gaus")
    f6=ROOT.TF1("f6","gaus")
    f7=ROOT.TF1("f7","gaus")
    f8=ROOT.TF1("f8","gaus")

    histos["h_res_APD1"].Fit("f1")
    histos["h_res_APD2"].Fit("f2")
    histos["h_res_APD1vsAPD2"].Fit("f3")
    histos["h_res_APD1andAPD2"].Fit("f4")

    histos["h_normalized_amp_APD1"].Fit("f5")
    histos["h_normalized_amp_APD2"].Fit("f6")
    histos["h_normalized_amp_APD1vsAPD2"].Fit("f7")
    histos["h_normalized_amp_APD1andAPD2"].Fit("f8")


    res = ROOT.TVectorD(4)
    resErr = ROOT.TVectorD(4)

    amp = ROOT.TVectorD(4)
    ampErr = ROOT.TVectorD(4)


    res[0] = math.sqrt(f1.GetParameter(2)*f1.GetParameter(2)-0.020*0.020) #subtracting mcp resolution
    res[1] = math.sqrt(f2.GetParameter(2)*f2.GetParameter(2)-0.020*0.020) #subtracting mcp resolution
    res[2] = math.sqrt(f3.GetParameter(2)*f3.GetParameter(2)*(1-res[0]*res[0]/(res[0]*res[0]+res[1]*res[1]))) #resolution one single apd 
    res[3] = math.sqrt(f4.GetParameter(2)*f4.GetParameter(2)-0.020*0.020) #subtracting mcp resolution

    resErr[0] = f1.GetParError(2)
    resErr[1] = f2.GetParError(2)
    resErr[2] = math.sqrt(f3.GetParError(2)*f3.GetParError(2)/2)
    resErr[3] = f4.GetParError(2)

    amp[0] = f5.GetParameter(1);
    amp[1] = f6.GetParameter(1);
    amp[2] = f7.GetParameter(1);
    amp[3] = f8.GetParameter(1);

    res.Write("timingResolution")
    resErr.Write("timingResolutionError")
    amp.Write("effAmplitude")

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
