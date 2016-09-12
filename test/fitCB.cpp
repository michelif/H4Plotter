#include <cmath>
#include <cstdlib>
#include <sstream>


#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "RooCBShape.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TVectorD.h"
#include "TF1.h"

#include <TROOT.h> 

float getRatioError( float num, float denom, float numErr, float denomErr ) {

  return sqrt( numErr*numErr/(denom*denom) + denomErr*denomErr*num*num/(denom*denom*denom*denom) );

}


std::pair<float,float> fitCB(TH1F* histo, TString runNumberString, float noiseTerm, std::string prefix){
  


  double peakpos = histo->GetBinCenter(histo->GetMaximumBin());
  double sigma = histo->GetRMS();

  double fitmin;
  double fitmax;


  fitmin = peakpos-4*sigma;
  fitmax = peakpos+4*sigma;




  RooRealVar x("x","deltaT", fitmin, fitmax);
  RooDataHist data("data","dataset with x",x,RooFit::Import(*histo) );

  RooRealVar meanr("meanr","Mean",peakpos-sigma,peakpos-3*sigma, peakpos+3*sigma);
  RooRealVar width("width","#sigma",sigma , 0, 5.*sigma);
  RooRealVar A("A","Dist",1.5, 0.0, 7.0);
  RooRealVar N("N","Deg",5, 0.0, 10);

  RooCBShape fit_fct("fit_fct","fit_fct",x,meanr,width,A,N); 
  fit_fct.fitTo(data);



 RooPlot* frame;

 frame = x.frame("Title");
 frame->SetXTitle("Charge Integrated");
 std::string ytitle = Form("Events");
 frame->SetYTitle(ytitle.c_str());

 data.plotOn(frame);  //this will show histogram data points on canvas                                                                                                                                                                     
 fit_fct.plotOn(frame);//this will show fit overlay on canvas                                                                                                                                                                              

 double rms,rmsErr;

 rms = sqrt(width.getVal()*width.getVal()-noiseTerm*noiseTerm);
 rmsErr = width.getError();
 TCanvas* cans = new TCanvas();
 cans->cd();
 frame->Draw();
 TLegend* lego = new TLegend(0.55, 0.8, 0.8, 0.9);
 lego->SetTextSize(0.031);
 lego->AddEntry(  (TObject*)0 ,Form("#sigma = %.1f #pm %.1f", rms, rmsErr), "");
 lego->AddEntry(  (TObject*)0 ,Form("mean = %.1f #pm %.1f", meanr.getVal(), meanr.getError()), "");
 float mean = meanr.getVal();
 float error=getRatioError(rms,meanr.getVal(),rmsErr,meanr.getError());
 lego->AddEntry(  (TObject*)0 ,Form("res = (%.1f  #pm %.1f)%% ",rms/mean*100 ,error*100), "");

 lego->SetFillColor(0);
 lego->Draw("same");

 TString name = "plots/reso_"+TString(histo->GetName())+"_"+runNumberString;
 if (prefix!=""){
   name+="_";
   name+=prefix;
 }
 if(noiseTerm>0) name+="_noiseSub";

 cans->SaveAs(name+".png");
 cans->SaveAs(name+".pdf");

 return std::make_pair(rms/mean*100.,error*100);

}


std::pair<float,float> fitGaus(TH1F* histo, TString runNumberString, float noiseTerm, std::string prefix){
  


  double peakpos = histo->GetBinCenter(histo->GetMaximumBin());
  double sigma = histo->GetRMS();

  double fitmin;
  double fitmax;


  fitmin = peakpos-1.5*sigma;
  fitmax = peakpos+1.5*sigma;

  RooRealVar x("x","deltaT", fitmin, fitmax);
  RooDataHist data("data","dataset with x",x,RooFit::Import(*histo) );

  RooRealVar meanr("meanr","Mean",peakpos+sigma,peakpos-3*sigma, peakpos+3*sigma);
  RooRealVar width("width","#sigma",sigma/3. , 0., sigma*0.7);

  RooGaussian fit_fct("fit_fct","fit_fct",x,meanr,width); 
  fit_fct.fitTo(data);



  RooPlot* frame;
  
  frame = x.frame("Title");
  frame->SetXTitle("Charge Integrated");
  std::string ytitle = Form("Events");
  frame->SetYTitle(ytitle.c_str());
  
  data.plotOn(frame);  //this will show histogram data points on canvas                                                                                                                                                                     
  fit_fct.plotOn(frame);//this will show fit overlay on canvas                                                                                                                                                                              
  
  double rms,rmsErr;
  rms = width.getVal();
  std::cout<<"rms:"<<rms<<" "<<sigma<<std::endl;
  //  rms = sqrt(width.getVal()*width.getVal()-noiseTerm*noiseTerm);
  rmsErr = width.getError();
  TCanvas* cans = new TCanvas();
  cans->cd();
  frame->Draw();
  TLegend* lego = new TLegend(0.55, 0.8, 0.8, 0.9);
  lego->SetTextSize(0.031);
  lego->AddEntry(  (TObject*)0 ,Form("#sigma = %.1f #pm %.1f", rms, rmsErr), "");
  lego->AddEntry(  (TObject*)0 ,Form("mean = %.1f #pm %.1f", meanr.getVal(), meanr.getError()), "");
  float mean = meanr.getVal();
  float error=getRatioError(rms,meanr.getVal(),rmsErr,meanr.getError());
  lego->AddEntry(  (TObject*)0 ,Form("res = (%.1f  #pm %.1f)%% ",rms/mean*100 ,error*100), "");
  
  lego->SetFillColor(0);
  lego->Draw("same");
  
  TString name = "plots/reso_"+TString(histo->GetName())+"_"+runNumberString;
  if (prefix!=""){
   name+="_";
   name+=prefix;
  }
  if(noiseTerm>0) name+="_noiseSub";
  
  cans->SaveAs(name+".png");
  cans->SaveAs(name+".pdf");
 
  return std::make_pair(rms/mean*100.,error*100);

}


std::pair<float,float> fitGausROOT(TH1F* histo, TString runNumberString, float noiseTerm, std::string prefix){
  
  //  histo->Rebin(2);


  double peakpos = histo->GetBinCenter(histo->GetMaximumBin());
  double sigma = histo->GetRMS();

  double fitmin;
  double fitmax;


  fitmin = peakpos-2*sigma;
  fitmax = peakpos+2*sigma;

  TF1 f1("f1","gaus",fitmin,fitmax);
  f1.SetParameter(0,peakpos);
  f1.SetParameter(1,sigma);
  f1.SetLineColor(kBlue);
  f1.SetLineWidth(2);
  histo->GetXaxis()->SetRangeUser(fitmin,fitmax);
  histo->Fit("f1","","",fitmin,fitmax);
  histo->SetStats(kFALSE);
  //  RooPlot* frame;
  
  double rms,rmsErr;
  rms = f1.GetParameter(2);
//  std::cout<<"rms:"<<rms<<" "<<sigma<<std::endl;
  rms = sqrt(f1.GetParameter(2)*f1.GetParameter(2)-noiseTerm*noiseTerm);
  rmsErr = f1.GetParError(2);
  TCanvas* cans = new TCanvas();
  cans->cd();
  //  frame->Draw();
  TLegend* lego = new TLegend(0.55, 0.8, 0.8, 0.9);
  lego->SetTextSize(0.031);
  lego->AddEntry(  (TObject*)0 ,Form("#sigma = %.1f #pm %.1f", rms, rmsErr), "");
  lego->AddEntry(  (TObject*)0 ,Form("mean = %.1f #pm %.1f", f1.GetParameter(1),f1.GetParError(1)), "");
  float mean = f1.GetParameter(1);
  float error=getRatioError(rms,f1.GetParameter(1),rmsErr,f1.GetParError(1));
  lego->AddEntry(  (TObject*)0 ,Form("res = (%.1f  #pm %.1f)%% ",rms/mean*100 ,error*100), "");
  
  lego->SetFillColor(0);


  histo->Draw("EP");
  lego->Draw("same");
  
  TString name = "plots/reso_"+TString(histo->GetName())+"_"+runNumberString;
  if (prefix!=""){
    name+="_";
    name+=prefix;
  }
  if(noiseTerm>0) name+="_noiseSub";
  
  cans->SaveAs(name+".png");
  cans->SaveAs(name+".pdf");
 
  return std::make_pair(rms/mean*100.,error*100);
  //return std::make_pair(100.,100);

}



int main( int argc, char* argv[] ) {

  gROOT->ProcessLine(".x ~/rootlogon.C");

  if( argc<2 ) {
    std::cout << "ERROR. You need to specify the name of the run you want to process." << std::endl;
    exit(1);
  }

  std::string runName = "";
  std::string prefix = "";

  if( argc>1 ) {
    std::string runName_str(argv[1]);
    runName = runName_str;
    if( argc>2 ) {
    std::string prefix_str(argv[2]);
    prefix = prefix_str;
    }
  }
  TString runNumberString(runName);

  std::string fileName;
  fileName = "plots/plots_"+prefix+"_"+runName+".root";

  TFile* file = TFile::Open(fileName.c_str());
  if( file==0 ) {
    std::cout << "ERROR! Din't find file " << fileName << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(11);
  }


  TH1F* histo_xtal11=(TH1F*)file->Get("h_xtal11_ch");
  TH1F* histo_maxAmpl_xtal11=(TH1F*)file->Get("h_xtal11_ampl");
  TH1F* histo_maxAmpl_xtal11_fit=(TH1F*)file->Get("h_xtal11_ampl_fit");

  TH1F* histo_xtal11_fft=(TH1F*)file->Get("h_xtal11_ch_FFT");
  TH1F* histo_maxAmpl_xtal11_fft=(TH1F*)file->Get("h_xtal11_ampl_FFT");
  TH1F* histo_maxAmpl_xtal11_fit_fft=(TH1F*)file->Get("h_xtal11_ampl_fit_FFT");

  TH1F* histo_matrix_uncalib_xtal11=(TH1F*)file->Get("h_matrix_xtal11_ch");
  TH1F* histo_matrix_uncalib_xtal11_FFT=(TH1F*)file->Get("h_matrix_xtal11_ch_FFT");

  TH1F* histo_matrix_xtal11=(TH1F*)file->Get("h_matrix_xtal11_ch_calib");
  TH1F* histo_matrix_xtal11_FFT=(TH1F*)file->Get("h_matrix_xtal11_ch_calib_FFT");

  TH1F* histo_maxAmpl_matrix_xtal11=(TH1F*)file->Get("h_matrix_xtal11_ampl_calib");
  TH1F* histo_maxAmpl_matrix_xtal11_fft=(TH1F*)file->Get("h_matrix_xtal11_ampl_calib_FFT");


  TFile* noiseFile = TFile::Open("data/pedestals_4642.root");


  std::vector<TH1F*> chXtalHisto;
  TVectorD* sigmaPedestal=(TVectorD*)noiseFile->Get("sigmaPedestal");

  TFile* outFile = TFile::Open("plots/resolution_"+runNumberString+".root","recreate");
  TVectorD resolution(2);
  TVectorD resolutionErr(2);


  fitCB(histo_xtal11, runNumberString,0,prefix);
  fitCB(histo_maxAmpl_xtal11, runNumberString,0,prefix);
  fitCB(histo_maxAmpl_xtal11_fit, runNumberString,0,prefix);

  fitCB(histo_xtal11_fft, runNumberString,0,prefix);
  fitCB(histo_maxAmpl_xtal11_fft, runNumberString,0,prefix);
  fitCB(histo_maxAmpl_xtal11_fit_fft, runNumberString,0,prefix);

  std::pair<float,float> reso_xtal=  fitCB(histo_xtal11, runNumberString,(*sigmaPedestal)[10],prefix);
  
  resolution [0] = reso_xtal.first;
  resolutionErr [0] = reso_xtal.second;


  float noiseMatrix_xtal11=0;
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[10],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[1],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[2],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[3],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[6],2);//4apd
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[7],2);//4apd
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[8],2);//4apd
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[9],2);//4apd
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[11],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[14],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[15],2);
  noiseMatrix_xtal11+=pow((*sigmaPedestal)[16],2);


  noiseMatrix_xtal11=sqrt(noiseMatrix_xtal11);
  noiseMatrix_xtal11=8.03337e+02;

  TH1F* cloneHisto=(TH1F*)histo_matrix_xtal11->Clone();
  fitGausROOT(histo_matrix_xtal11, runNumberString,0,prefix);
  std::pair<float,float> reso_matrix  =  fitGausROOT(cloneHisto, runNumberString,noiseMatrix_xtal11,prefix);
  fitGausROOT(histo_matrix_xtal11_FFT, runNumberString,0,prefix);

  fitGausROOT(histo_matrix_uncalib_xtal11, runNumberString,0,prefix);
  fitGausROOT(histo_matrix_uncalib_xtal11_FFT, runNumberString,0,prefix);



  //  fitGaus(histo_matrix_xtal11_FFT, runNumberString,0);
//  std::pair<float,float> reso_matrix  =  fitGaus(histo_matrix_xtal11, runNumberString,noiseMatrix_xtal11);
//
//  resolution [1] = reso_matrix.first;
//  resolutionErr [1] = reso_matrix.second;
//
//
//  resolution.Write("resolution");
//  resolutionErr.Write("resolutionErr");

  outFile->Write();
  outFile->Close();
}
