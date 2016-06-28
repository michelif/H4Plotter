#!/bin/python

import sys
import re
import time
import argparse
import os
import subprocess
import array
import ROOT
import math

from ROOT import std

vstring = std.vector(std.string)
vfloat = std.vector(float)

#----function to get effSigma of TH1
def EffSigma( hist ):

    xaxis =  hist.GetXaxis()
    nb = xaxis.GetNbins()


    if nb < 10:
      print "effsigma: not enough bins. nbins = %i" % nb
      return 0.
  
    bwid = xaxis.GetBinWidth(1)
    if bwid == 0 :
      print   "effsigma: binwidth is zero. bwid = %f" %  bwid   
      return 0.
    
    #Double_t xmax = xaxis.GetXmax()#Not used
    xmin = xaxis.GetXmin()
    ave = hist.GetMean()
    rms = hist.GetRMS()
    
    total=0.
    for i in range(0, nb+2) :
        total+=hist.GetBinContent(i)

    #   if total < 100.) {
    #     print   "effsigma: Too few entries "   total   ;
    #     return 0.;
    #   }
    ierr=0 #flag for errors
    ismin=999
  
    rlim=0.683*total
    nrms=rms/(bwid)    # Set scan size to +/- rms
    if nrms > nb/10:
        nrms=nb/10 # Could be tuned...

    effSigma=9999999.#initial crazy value
#    print "-nrms: %f int= %i" % (-nrms,int(-nrms))
#    print "nrms+1: %f int= %i" % (nrms,int(nrms+1))
#    print "step %i " % int(nrms*2+1)
    for it in range(0, int(nrms*2+1) ): # Scan window centre
#        print "it %i" % it
        iscan=-nrms+it
#        print "effsigma: iscan %i " % iscan 
        ibm=int((ave-xmin)/bwid +1+iscan)
        x=(ibm-0.5)*bwid +xmin
        x_right=x
        x_left=x
        j_right=ibm
        j_left=ibm
        bin=hist.GetBinContent(ibm)#bin=nevents nel punto di scan
        total=bin
        for j in range(1,nb):#right direction
#            print "j %i" % j 
            if j_right < nb:
                j_right+=1
                x_right+=bwid
                bin=hist.GetBinContent(j_right)
                total+=bin
                if total > rlim:
                    break

            else:
                ierr=1
      

            if j_left > 0: #left direction
                j_left-=1
                x_left-=bwid
                bin=hist.GetBinContent(j_left)
                total+=bin
                if total > rlim:
                    break
                
            else:
                ierr=2

        dxf=(total-rlim)*bwid/bin #bin=#events nell'ultimo x toccato
#        print "x_right %f x_left %f" % (x_right,x_left)
#        print "dxf is %f" % dxf 
#        print "bwid is %f " % bwid 
        wid=(x_right-x_left+bwid-dxf)*0.5
#        print "wid is %f " %  wid 
        if wid < effSigma:
            effSigma=wid
            ismin=iscan
      
    if ismin == nrms or ismin == -nrms: 
        ierr=3
    if ierr != 0:
        print   "effsigma: Error of type %i " %   ierr   
  
    return effSigma
  




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
    calibC = cfg.GetOpt(vfloat)("calibration.calibConstants")


    print run
    file=ROOT.TFile("../H4Analysis_2016/ntuples/analysis_"+str(run)+".root")
#    cfgNtuple=file.Get("cfg")
#    xtalNames = cfgNtuple.GetOpt(std.vector(std.string))("DigiReco.channelsNames")
    tree=file.Get("h4")

#----histo definition---- FIXME move to a function
    outfile=ROOT.TFile("plots/plots_"+str(run)+".root","recreate")
    hmatrix_xtal11_hodosel_3x3=ROOT.TH1F("chint_xtal11_matrix_hodosel_3x3","chint_xtal11_matrix_hodosel_3x3",500,30000,70000);
    hmatrix_xtal11_hodosel_3x3.SetLineColor(ROOT.kBlack)

    hmatrix_xtal11_hodosel_3x3_calib=ROOT.TH1F("chint_xtal11_matrix_hodosel_3x3_calib","chint_xtal11_matrix_hodosel_3x3_calib",500,30000,70000);
    hmatrix_xtal11_hodosel_3x3_calib.SetLineColor(ROOT.kBlack)


#----loop over entries-----
    for entry in tree:
        if entry.event == 1:
        #define here the xtals for a 3x3 matrixs, centered in two different xtals
            xtalMatrix4APD = array.array('i',[tree.xtal1,tree.xtal2,tree.xtal3,tree.xtal6,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal11,tree.xtal14,tree.xtal15,tree.xtal16])
            xtalMatrixXtal11 = array.array('i',[tree.xtal11,tree.xtal2,tree.xtal3,tree.xtal4,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal12,tree.xtal15,tree.xtal16,tree.xtal17])
#            xtalCalibXtal11  = array.array('d',[1,0.521013,0.791507,0.500076,0.504625,0.506267,0.516959,0.502816,1.0016,0.513315,0.691472,0.899462])

        if (entry.nFibresOnX[0]!=2 or entry.nFibresOnX[1]!=2 or entry.nFibresOnY[0]!=2 or entry.nFibresOnY[1]!=2):#showering
            continue
        if (ROOT.TMath.Abs(entry.X[0])>3 or ROOT.TMath.Abs(entry.X[1])>3 or ROOT.TMath.Abs(entry.Y[0])>3 or ROOT.TMath.Abs(entry.Y[1])>3):
            continue


        matrixEnXtal11=0
        matrixEnXtal11_calib=0
        index=0
        for xtal in xtalMatrixXtal11:
            matrixEnXtal11+=entry.charge_sig[xtal]
            matrixEnXtal11_calib+=entry.charge_sig[xtal]*calibC[index]
            index+=1
        hmatrix_xtal11_hodosel_3x3.Fill(matrixEnXtal11)
        hmatrix_xtal11_hodosel_3x3_calib.Fill(matrixEnXtal11_calib)

    print "eff sigma of uncalib is: %f " % EffSigma(hmatrix_xtal11_hodosel_3x3)
    print "eff sigma of calib is: %f " % EffSigma(hmatrix_xtal11_hodosel_3x3_calib)
    outfile.Write()
    outfile.Close()


### MAIN ###
if __name__ == "__main__":
    main()
