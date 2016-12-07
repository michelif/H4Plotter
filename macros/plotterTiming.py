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


def crystalball(x,par):
    """
    Definition of crystal ball and constant names coherent with https://en.wikipedia.org/wiki/Crystal_Ball_function

    n:par[0]
    alpha:par[1]
    centroid:par[2]
    sigma:par[3]
    peak amplitude:par[4]
    """
    if (x[0]-par[2])/par[3] < -par[1]:
        #print x
        term1 = ((par[0]/abs(par[1]))**par[0])
        term2 = math.exp(-(par[1]**2)/2) 
        A = term1*term2
        B = par[0]/abs(par[1])-abs(par[1])
        term3 = B -(x[0]-par[2])/par[3]
        #print bgkd
        term4 = pow(float(term3),float(par[0]))
        y = (par[4]*A/term4)
    else:
        y = par[4]*math.exp(-(((x[0]-par[2])/par[3])**2)/2)
    return y



def crystal_ball(x,par):
    """ A Gaussian curve on one side and a power-law on the other side. Used in
    physics to model lossy processes.
    See http://en.wikipedia.org/wiki/Crystal_Ball_function
    Note that the definition used here differs slightly. At the time of this
    writing, the wiki article has some discrepancies in definitions/plots. This
    definition makes it easier to fit the function by using complex numbers
    and by negating any negative values for a and n.
    
    This version of the crystal ball is normalized by an additional parameter.
    params: N, a, n, xb, sig
    """
    x = x+0j # Prevent warnings...
    N, a, n, xb, sig = params
    if a < 0:
        a = -a
    if n < 0:
        n = -n
    aa = abs(a)
    A = (n/aa)**n * exp(- aa**2 / 2)
    B = n/aa - aa
    total = 0.*x
    total += ((x-xb)/sig  > -a) * N * exp(- (x-xb)**2/(2.*sig**2))
    total += ((x-xb)/sig <= -a) * N * A * (B - (x-xb)/sig)**(-n)
    try:
        return total.real
    except:
        return total
    return total

#----function to book histos
def bookHistos(histos,run,isAugustTB):
    if not isAugustTB:
        histos["h_normalized_amp_APD1"]=ROOT.TH1F("h_normalized_amp_APD1","h_normalized_amp_APD1",500,0,250);
        histos["h_normalized_amp_APD2"]=ROOT.TH1F("h_normalized_amp_APD2","h_normalized_amp_APD2",500,0,250);
        histos["h_normalized_amp_APD1andAPD2"]=ROOT.TH1F("h_normalized_amp_APD1andAPD2","h_normalized_amp_APD1andAPD2",1200,0,600);
        histos["h_normalized_amp_APD1vsAPD2"]=ROOT.TH1F("h_normalized_amp_APD1vsAPD2","h_normalized_amp_APD1vsAPD2",500,0,250);

        if(run == 5789):
            histos["h_res_APD1"]=ROOT.TH1F("h_res_APD1","h_res_APD1",150,3.0,5.5);
            histos["h_res_APD2"]=ROOT.TH1F("h_res_APD2","h_res_APD2",150,3.0,5.5);
            histos["h_res_APD1andAPD2"]=ROOT.TH1F("h_res_APD1andAPD2","h_res_APD1andAPD2",150,3.0,5.5);
            histos["h_res_APD1vsAPD2"]=ROOT.TH1F("h_res_APD1vsAPD2","h_res_APD1vsAPD2",150,-1.0,2.0);
        else:
            histos["h_res_APD1"]=ROOT.TH1F("h_res_APD1","h_res_APD1",150,3.5,5.);
            histos["h_res_APD2"]=ROOT.TH1F("h_res_APD2","h_res_APD2",150,3.5,5);
            histos["h_res_APD1andAPD2"]=ROOT.TH1F("h_res_APD1andAPD2","h_res_APD1andAPD2",150,3.5,5);
            histos["h_res_APD1vsAPD2"]=ROOT.TH1F("h_res_APD1vsAPD2","h_res_APD1vsAPD2",150,-0.5,1.5);
    else:
        histos["h_normalized_amp_APD1"]=ROOT.TH1F("h_normalized_amp_APD1","h_normalized_amp_APD1",600,0,600);
        histos["h_res_APD1"]=ROOT.TH1F("h_res_APD1","h_res_APD1",300,5,6.5);



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
    parser.add_argument("-a", "--august",dest='isAugustTB',action='store_true',
                       help="is a run from august TB")
    parser.set_defaults(isAugustTB=False)



    args = parser.parse_args()
    run = args.run
    prefix = args.prefix
    usefft = args.usefft
    isAugustTB = args.isAugustTB

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
    bookHistos(histos,run,isAugustTB)

#----loop over entries-----
    for entry in tree:
        #define here the xtals for a 3x3 matrixs, centered in two different xtals
        if entry.index % 1000 ==0:
            print "Analyzing event:"
            print entry.index

        if (entry.nFibresOnX[0]!=2 or entry.nFibresOnY[0]!=2):#showering
            continue
        if (ROOT.TMath.Abs(entry.X[0])>5 or ROOT.TMath.Abs(entry.Y[0])>5 ):
            continue
        if (ROOT.TMath.Abs(entry.X[0]-entry.X[1])>1.5 or ROOT.TMath.Abs(entry.Y[0]-entry.Y[1])>1.5):
            continue
        if (entry.amp_max[entry.MCP1]<250):
            continue
        if (entry.fit_chi2[entry.APD1]>2):
            continue
        if (entry.b_rms[entry.APD1]>8):
            continue

        if not isAugustTB:
            if (entry.fit_chi2[entry.APD2]>2):
                continue
            if (entry.b_rms[entry.APD2]>8):
                continue


        histos["h_res_APD1"].Fill(entry.fit_time[entry.APD1]-entry.time[entry.MCP1])
        amp1=entry.amp_max[entry.APD1]/entry.b_rms[entry.APD1]
        histos["h_normalized_amp_APD1"].Fill(amp1)
        if not isAugustTB:
            histos["h_res_APD2"].Fill(entry.fit_time[entry.APD2]-entry.time[entry.MCP1])
            histos["h_res_APD1vsAPD2"].Fill(entry.fit_time[entry.APD1]-entry.fit_time[entry.APD2])
            histos["h_res_APD1andAPD2"].Fill((entry.fit_time[entry.APD2]+entry.fit_time[entry.APD1])/2.-entry.time[entry.MCP1])
        #fill the normalized amplitudes
            amp2=entry.amp_max[entry.APD2]/entry.b_rms[entry.APD2]

            histos["h_normalized_amp_APD2"].Fill(amp2)
            eff_ampl_diff=amp1*amp2/(math.sqrt(amp1*amp1+amp2*amp2))
            
            histos["h_normalized_amp_APD1vsAPD2"].Fill(eff_ampl_diff)
            histos["h_normalized_amp_APD1andAPD2"].Fill(2*eff_ampl_diff)

#        histos["h_res_time_APD1"].Fill(entry.time[entry.APD1]-entry.time[entry.MCP1])
#        histos["h_res_time_APD2"].Fill(entry.time[entry.APD2]-entry.time[entry.MCP1])
#        histos["h_res_time_APD1vsAPD2"].Fill(entry.time[entry.APD1]-entry.time[entry.APD2])


#    f1=ROOT.TF1("f1","gaus")
#    f2=ROOT.TF1("f2","gaus")
#    f3=ROOT.TF1("f3","gaus")
#    f4=ROOT.TF1("f4","gaus")
#    f5=ROOT.TF1("f5","gaus")
#    f6=ROOT.TF1("f6","gaus")
#    f7=ROOT.TF1("f7","gaus")
#    f8=ROOT.TF1("f8","gaus")

    #in some runs there are some non gaussian tails, let's use a CB
    f1 = ROOT.TF1( 'f1', crystalball, histos["h_res_APD1"].GetBinLowEdge(1),histos["h_res_APD1"].GetBinLowEdge(histos["h_res_APD1"].GetNbinsX())+histos["h_res_APD1"].GetBinWidth(1), 5)
    f1.SetParameter(0,5)
    f1.SetParLimits(0,0.,7.)
    f1.SetParameter(1,1.5);
    f1.SetParLimits(1,0.,10.)
    f1.SetParameter(2,histos["h_res_APD1"].GetMean())
    f1.SetParameter(3,histos["h_res_APD1"].GetRMS())
    f1.SetParameter(4,histos["h_res_APD1"].Integral())


    f5 = ROOT.TF1( 'f5', crystalball, histos["h_normalized_amp_APD1"].GetBinLowEdge(1),histos["h_normalized_amp_APD1"].GetBinLowEdge(histos["h_normalized_amp_APD1"].GetNbinsX())+histos["h_normalized_amp_APD1"].GetBinWidth(1), 5)
    f5.SetParameter(0,5)
    f5.SetParLimits(0,0.,7.)
    f5.SetParameter(1,1.5)
    f5.SetParLimits(1,0.,10.)
    f5.SetParameter(2,histos["h_normalized_amp_APD1"].GetMean())
    f5.SetParameter(3,histos["h_normalized_amp_APD1"].GetRMS())
    f5.SetParameter(4,histos["h_normalized_amp_APD1"].Integral())


    if not isAugustTB:
        f2 = ROOT.TF1( 'f2', crystalball, histos["h_res_APD2"].GetBinLowEdge(1),histos["h_res_APD2"].GetBinLowEdge(histos["h_res_APD2"].GetNbinsX())+histos["h_res_APD2"].GetBinWidth(1), 5)
        f2.SetParameter(0,5)
        f2.SetParLimits(0,0.,7.)
        f2.SetParameter(1,1.5);
        f2.SetParLimits(1,0.,10.)
        f2.SetParameter(2,histos["h_res_APD2"].GetMean())
        f2.SetParameter(3,histos["h_res_APD2"].GetRMS())
        f2.SetParameter(4,histos["h_res_APD2"].Integral())

        f6 = ROOT.TF1( 'f6', crystalball, histos["h_normalized_amp_APD2"].GetBinLowEdge(1),histos["h_normalized_amp_APD2"].GetBinLowEdge(histos["h_normalized_amp_APD2"].GetNbinsX())+histos["h_normalized_amp_APD2"].GetBinWidth(1), 5)
        f6.SetParameter(0,5)
        f6.SetParLimits(0,0.,7.)
        f6.SetParameter(1,1.5);
        f6.SetParLimits(1,0.,10.)
        f6.SetParameter(2,histos["h_normalized_amp_APD2"].GetMean())
        f6.SetParameter(3,histos["h_normalized_amp_APD2"].GetRMS())
        f6.SetParameter(4,histos["h_normalized_amp_APD2"].Integral())



        f3 = ROOT.TF1( 'f3', crystalball, histos["h_res_APD1vsAPD2"].GetBinLowEdge(1),histos["h_res_APD1vsAPD2"].GetBinLowEdge(histos["h_res_APD1vsAPD2"].GetNbinsX())+histos["h_res_APD1vsAPD2"].GetBinWidth(1), 5)
        f3.SetParameter(0,5)
        f3.SetParLimits(0,0.,7.)
        f3.SetParameter(1,1.5);
        f3.SetParLimits(1,0.,10.)
        f3.SetParameter(2,histos["h_res_APD1vsAPD2"].GetMean())
        f3.SetParameter(3,histos["h_res_APD1vsAPD2"].GetRMS())
        f3.SetParameter(4,histos["h_res_APD1vsAPD2"].Integral())

        f7 = ROOT.TF1( 'f7', crystalball, histos["h_normalized_amp_APD1vsAPD2"].GetBinLowEdge(1),histos["h_normalized_amp_APD1vsAPD2"].GetBinLowEdge(histos["h_normalized_amp_APD1vsAPD2"].GetNbinsX())+histos["h_normalized_amp_APD1vsAPD2"].GetBinWidth(1), 5)
        f7.SetParameter(0,5)
        f7.SetParLimits(0,0.,7.)
        f7.SetParameter(1,1.5);
        f7.SetParLimits(1,0.,10.)
        f7.SetParameter(2,histos["h_normalized_amp_APD1vsAPD2"].GetMean())
        f7.SetParameter(3,histos["h_normalized_amp_APD1vsAPD2"].GetRMS())
        f7.SetParameter(4,histos["h_normalized_amp_APD1vsAPD2"].Integral())

        
        f4 = ROOT.TF1( 'f4', crystalball, histos["h_res_APD1andAPD2"].GetBinLowEdge(1),histos["h_res_APD1andAPD2"].GetBinLowEdge(histos["h_res_APD1andAPD2"].GetNbinsX())+histos["h_res_APD1andAPD2"].GetBinWidth(1), 5)
        f4.SetParameter(0,5)
        f4.SetParLimits(0,0.,7.)
        f4.SetParameter(1,1.5);
        f4.SetParLimits(1,0.,10.)
        f4.SetParameter(2,histos["h_res_APD1andAPD2"].GetMean())
        f4.SetParameter(3,histos["h_res_APD1andAPD2"].GetRMS())
        f4.SetParameter(4,histos["h_res_APD1andAPD2"].Integral())

        f8 = ROOT.TF1( 'f8', crystalball, histos["h_normalized_amp_APD1andAPD2"].GetBinLowEdge(1),histos["h_normalized_amp_APD1andAPD2"].GetBinLowEdge(histos["h_normalized_amp_APD1andAPD2"].GetNbinsX())+histos["h_normalized_amp_APD1andAPD2"].GetBinWidth(1), 5)
        f8.SetParameter(0,5)
        f8.SetParLimits(0,0.,7.)
        f8.SetParameter(1,1.5);
        f8.SetParLimits(1,0.,10.)
        f8.SetParameter(2,histos["h_normalized_amp_APD1andAPD2"].GetMean())
        f8.SetParameter(3,histos["h_normalized_amp_APD1andAPD2"].GetRMS())
        f8.SetParameter(4,histos["h_normalized_amp_APD1andAPD2"].Integral())





    histos["h_res_APD1"].Fit(f1)
    histos["h_normalized_amp_APD1"].Fit(f5)
    if not isAugustTB:
        histos["h_res_APD2"].Fit(f2)
        histos["h_res_APD1vsAPD2"].Fit(f3)
        histos["h_res_APD1andAPD2"].Fit(f4)


        histos["h_normalized_amp_APD2"].Fit(f6)
        histos["h_normalized_amp_APD1vsAPD2"].Fit(f7)
        histos["h_normalized_amp_APD1andAPD2"].Fit(f8)


    res = ROOT.TVectorD(4)
    resErr = ROOT.TVectorD(4)

    amp = ROOT.TVectorD(4)
    ampErr = ROOT.TVectorD(4)


    res[0] = math.sqrt(f1.GetParameter(3)*f1.GetParameter(3)-0.020*0.020) #subtracting mcp resolution
    resErr[0] = f1.GetParError(3)
    amp[0] = f5.GetParameter(2);
    if not isAugustTB:
        res[1] = math.sqrt(f2.GetParameter(3)*f2.GetParameter(3)-0.020*0.020) #subtracting mcp resolution
        res[2] = math.sqrt(f3.GetParameter(3)*f3.GetParameter(3)*(1-res[0]*res[0]/(res[0]*res[0]+res[1]*res[1]))) #resolution one single apd 
        res[3] = math.sqrt(f4.GetParameter(3)*f4.GetParameter(3)-0.020*0.020) #subtracting mcp resolution

        resErr[1] = f2.GetParError(3)
        resErr[2] = math.sqrt(f3.GetParError(3)*f3.GetParError(3)/2)
        resErr[3] = f4.GetParError(3)


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
