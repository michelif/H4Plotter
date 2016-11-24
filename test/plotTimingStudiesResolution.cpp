#include <cmath>
#include <cstdlib>
#include <sstream>
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TVectorD.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include <TROOT.h> 

int main( int argc, char* argv[] ) {

  gROOT->ProcessLine(".x ~/rootlogon.C");

  if( argc<1 ) {
    std::cout << "Usage: ./bin/plotResolution [matrix] ([addMC])"<<std::endl;
    std::cout << "ERROR. You need to specify the matrix you want to process" << std::endl;
    exit(1);
  }
  
  std::vector<int> runs;
  std::vector<float> energies;
  runs.push_back(5789);
  energies.push_back(20);
  runs.push_back(5796);
  energies.push_back(50);
  runs.push_back(5780);
  energies.push_back(100);
  runs.push_back(5903);
  energies.push_back(150);
  runs.push_back(5882);
  energies.push_back(200);


  TGraphErrors* resVsEnergy_MCP = new TGraphErrors(0);
  TGraphErrors* resVsEnergy_APD2_MCP = new TGraphErrors(0);
  TGraphErrors* resVsEnergy_APD = new TGraphErrors(0);
  TGraphErrors* resVsEnergy_APD1andAPD2 = new TGraphErrors(0);


  for (int i=0;i<runs.size();++i){
    TString run;
    run.Form("%d",runs[i]);

    TFile* file = TFile::Open("plots/plotsTiming_TimingStudies_"+run+".root");

    if( file==0 ) {
      std::cout << "ERROR! Din't find file for run" << runs[i] << std::endl;
      std::cout << "Exiting." << std::endl;
      exit(11);
    }

    
    TVectorD* resValue=(TVectorD*)file->Get("timingResolution");
    TVectorD* resValueErr=(TVectorD*)file->Get("timingResolutionError");

    TVectorD* resValue_time=(TVectorD*)file->Get("timingResolution_time");
    TVectorD* resValueErr_time=(TVectorD*)file->Get("timingResolutionError_time");

    std::cout<<i<<" "<<(*resValue)[0]<<" "<<(*resValue)[3]<<" "<<(*resValueErr)[0]<<" "<<energies[i]<<std::endl;
    
    resVsEnergy_MCP->SetPoint( i, energies[i], (*resValue)[0]*1000);
    resVsEnergy_MCP->SetPointError( i, 0,(*resValueErr)[0]*1000);

    resVsEnergy_APD2_MCP->SetPoint( i, energies[i], (*resValue)[1]*1000);
    resVsEnergy_APD2_MCP->SetPointError( i, 0,(*resValueErr)[1]*1000);

    resVsEnergy_APD->SetPoint( i, energies[i], (*resValue)[2]*1000);
    resVsEnergy_APD->SetPointError( i, 0,(*resValueErr)[2]*1000);

    resVsEnergy_APD1andAPD2->SetPoint( i, energies[i], (*resValue)[3]*1000);
    resVsEnergy_APD1andAPD2->SetPointError( i, 0,(*resValueErr)[3]*1000);


  }


  TCanvas* c1 = new TCanvas();
  //  TH2D* h2_axes_3 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 1.1*((resVsEnergy_xtal->GetY())[0]+(resVsEnergy_xtal->GetErrorY(0))));
  TH2D* h2_axes_3 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 650.);
  h2_axes_3->SetXTitle("Beam Energy [GeV]");
  h2_axes_3->GetYaxis()->SetTitleOffset(1.3);
  h2_axes_3->SetYTitle("Timing Resolution [ps]");


  //MCP 1
  TF1* f_MCP= new TF1("fun_MCP","sqrt([0]*[0]/(x*x)+[1]*[1])",(resVsEnergy_MCP->GetX())[0]-10,(resVsEnergy_MCP->GetX())[resVsEnergy_MCP->GetN()-1] +10);
  h2_axes_3->Draw("");

  resVsEnergy_MCP->SetMarkerStyle(20);
  resVsEnergy_MCP->SetMarkerSize(1.6);
  resVsEnergy_MCP->SetMarkerColor(kBlue);

  resVsEnergy_MCP->Draw("p same");


  f_MCP->SetParameter(0, 0);
  //  f_MCP->SetParLimits(0, 5, 20.);
  f_MCP->SetParameter(1, 10);
  f_MCP->SetParameter(2, 50);
  //  f_MCP->SetParLimits(1, 0.,3);
  //  f_MCP->FixParameter(2, 803.);

  resVsEnergy_MCP->Fit("fun_MCP","RNE");



  f_MCP->SetLineWidth(1.);
  f_MCP->SetLineColor(kBlue+2);
  f_MCP->Draw("L same");


  //MCP 2
  TF1* f_APD2_MCP= new TF1("fun_APD2_MCP","sqrt([0]*[0]/(x*x)+[1]*[1])",(resVsEnergy_APD2_MCP->GetX())[0]-10,(resVsEnergy_APD2_MCP->GetX())[resVsEnergy_APD2_MCP->GetN()-1] +10);

  resVsEnergy_APD2_MCP->SetMarkerStyle(20);
  resVsEnergy_APD2_MCP->SetMarkerSize(1.6);
  resVsEnergy_APD2_MCP->SetMarkerColor(kMagenta);

  resVsEnergy_APD2_MCP->Draw("p same");


  f_APD2_MCP->SetParameter(0, 0);
  //  f_APD2_MCP->SetParLimits(0, 5, 20.);
  f_APD2_MCP->SetParameter(1, 10);
  f_APD2_MCP->SetParameter(2, 50);
  //  f_APD2_MCP->SetParLimits(1, 0.,3);
  //  f_APD2_MCP->FixParameter(2, 803.);

  resVsEnergy_APD2_MCP->Fit("fun_APD2_MCP","RNE");



  f_APD2_MCP->SetLineWidth(1.);
  f_APD2_MCP->SetLineColor(kMagenta);
  f_APD2_MCP->Draw("L same");



  //APD
  TF1* f_APD= new TF1("fun_APD","sqrt([0]*[0]/(x*x)+[1]*[1])",(resVsEnergy_APD->GetX())[0]-10,(resVsEnergy_APD->GetX())[resVsEnergy_APD->GetN()-1] +10);

  resVsEnergy_APD->SetMarkerStyle(20);
  resVsEnergy_APD->SetMarkerSize(1.6);
  resVsEnergy_APD->SetMarkerColor(kRed);


  resVsEnergy_APD->Draw("p same");


  f_APD->SetParameter(0, 0);
  //  f_APD->SetParLimits(0, 5, 20.);
  f_APD->SetParameter(1, 10);
  f_APD->SetParameter(2, 50);
  //  f_ene->SetParLimits(1, 0.,3);
  //  f_ene->FixParameter(2, 803.);

  resVsEnergy_APD->Fit("fun_APD","RNE");


  f_APD->SetLineWidth(1.);
  f_APD->SetLineColor(kRed);
  f_APD->Draw("L same");


  //APD1 and APD2 average
  TF1* f_APD1andAPD2= new TF1("fun_APD1andAPD2","sqrt([0]*[0]/(x*x)+[1]*[1])",(resVsEnergy_APD1andAPD2->GetX())[0]-10,(resVsEnergy_APD1andAPD2->GetX())[resVsEnergy_APD1andAPD2->GetN()-1] +10);

  resVsEnergy_APD1andAPD2->SetMarkerStyle(20);
  resVsEnergy_APD1andAPD2->SetMarkerSize(1.6);
  resVsEnergy_APD1andAPD2->SetMarkerColor(kGreen+3);

  resVsEnergy_APD1andAPD2->Draw("p same");


  f_APD1andAPD2->SetParameter(0, 0);
  f_APD1andAPD2->SetParameter(1, 10);
  f_APD1andAPD2->SetParameter(2, 50);
  
  resVsEnergy_APD1andAPD2->Fit("fun_APD1andAPD2","RNE");


  f_APD1andAPD2->SetLineWidth(1.);
  f_APD1andAPD2->SetLineColor(kGreen+3);
  f_APD1andAPD2->Draw("L same");



  TLegend* leg_neat = new TLegend(0.4, 0.92-0.06*7 , 0.75, 0.92);
  leg_neat->SetTextSize(0.038);
  std::string ene="Electron Beam";
  leg_neat->AddEntry(resVsEnergy_MCP,"APD1 vs MCP","p");
  leg_neat->AddEntry((TObject*)0 ,Form("N =  %2.0f #pm %2.0f",f_MCP->GetParameter(0),f_MCP->GetParError(0) ),"");
  leg_neat->AddEntry( (TObject*)0 ,Form("C =  %1.0f  #pm %1.0f",(f_MCP->GetParameter(1)),f_MCP->GetParError(1) ),"");
  leg_neat->AddEntry(resVsEnergy_APD2_MCP,"APD2 vs MCP","p");
  leg_neat->AddEntry((TObject*)0 ,Form("N =  %2.0f #pm %2.0f",f_APD2_MCP->GetParameter(0),f_APD2_MCP->GetParError(0) ),"");
  leg_neat->AddEntry( (TObject*)0 ,Form("C =  %1.0f  #pm %1.0f",(f_APD2_MCP->GetParameter(1)),f_APD2_MCP->GetParError(1) ),"");
  leg_neat->AddEntry(resVsEnergy_APD,"APD1 vs APD2","p");
  leg_neat->AddEntry((TObject*)0 ,Form("N =  %2.0f #pm %2.0f",f_APD->GetParameter(0),f_APD->GetParError(0) ),"");
  leg_neat->AddEntry( (TObject*)0 ,Form("C =  %1.0f  #pm %1.0f",(f_APD->GetParameter(1)) ,f_APD->GetParError(1) ),"");
  leg_neat->AddEntry(resVsEnergy_APD1andAPD2,"APD average vs MCP","p");
  leg_neat->AddEntry((TObject*)0 ,Form("N =  %2.0f #pm %2.0f",f_APD1andAPD2->GetParameter(0),f_APD1andAPD2->GetParError(0) ),"");
  leg_neat->AddEntry( (TObject*)0 ,Form("C =  %1.0f  #pm %1.0f",(f_APD1andAPD2->GetParameter(1)) ,f_APD1andAPD2->GetParError(1) ),"");
  leg_neat->SetFillColor(0);
  leg_neat->Draw("same");


  c1->SaveAs("plots/reso_TimingStudies.png");
  c1->SaveAs("plots/reso_TimingStudies.pdf");


}
