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
    std::cout << "ERROR. You need to specify the matrix you want to process" << std::endl;
    exit(1);
  }

  std::string matrix = "xtal11";

  if( argc>1 ) {
    std::string matrix_str(argv[1]);
    matrix = matrix_str;
  }

  std::vector<int> runs;
  std::vector<float> energies;
  if (matrix == "xtal11") {
    runs.push_back(4774);
    energies.push_back(20);
    runs.push_back(4668);
    energies.push_back(50);
    runs.push_back(4683);
    energies.push_back(100);
    runs.push_back(4695);
    energies.push_back(200);
  }

  TGraphErrors* resVsEnergy_xtal = new TGraphErrors(0);
  TGraphErrors* resVsEnergy_matrix = new TGraphErrors(0);

  for (int i=0;i<runs.size();++i){
    TString run;
    run.Form("%d",runs[i]);

    TFile* file = TFile::Open("plots/resolution_"+run+".root");

    if( file==0 ) {
      std::cout << "ERROR! Din't find file for run" << runs[i] << std::endl;
      std::cout << "Exiting." << std::endl;
      exit(11);
    }
    
    TVectorD* resValue=(TVectorD*)file->Get("resolution");
    TVectorD* resValueErr=(TVectorD*)file->Get("resolutionErr");

    std::cout<<i<<" "<<(*resValue)[0]<<" "<<(*resValueErr)[0]<<std::endl;

    resVsEnergy_xtal->SetPoint( i, energies[i], (*resValue)[0]);
    resVsEnergy_xtal->SetPointError( i, 0,(*resValueErr)[0]*1.10);

  }


  TCanvas* c1 = new TCanvas();
  TH2D* h2_axes_3 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 1.1*((resVsEnergy_xtal->GetY())[0]+(resVsEnergy_xtal->GetErrorY(0))));
  h2_axes_3->SetXTitle("Beam Energy [GeV]");
  h2_axes_3->SetYTitle("Energy Resolution [%]");

  //  TF1 *fun= new TF1("fun",  "sqrt([0]*[0]/x+[1]*[1]+ [2]*[2]/(x*x))",1, 250+5.);

  TF1* f_ene= new TF1("fun_ene","sqrt([0]*[0]/x+[1]*[1])",(resVsEnergy_xtal->GetX())[0]-10,(resVsEnergy_xtal->GetX())[resVsEnergy_xtal->GetN()-1] +10);

  h2_axes_3->Draw("");

  resVsEnergy_xtal->SetMarkerStyle(20);
  resVsEnergy_xtal->SetMarkerSize(1.6);
  resVsEnergy_xtal->SetMarkerColor(kBlue);

  resVsEnergy_xtal->Draw("p same");

  f_ene->SetParameter(0, 12);
  f_ene->SetParameter(1, 10);
  f_ene->SetParLimits(1, 0.,10);


  resVsEnergy_xtal->Fit("fun_ene","RN");



  f_ene->SetLineWidth(1.);
  f_ene->SetLineColor(kBlue+2);
  f_ene->Draw("L same");



  TLegend* leg_neat = new TLegend(0.4, 0.92-0.06*5 , 0.8, 0.92);
  leg_neat->SetTextSize(0.038);
  std::string ene="Electron Beam";
  leg_neat->AddEntry(resVsEnergy_xtal,"Data CeF_{3}","p");
  //      leg_neat->AddEntry((TObject*)0 ,Form("S =  %.2f\n%s #pm %.2f / #sqrt{E [GeV]}",f_ene->GetParameter(0),"%",f_ene->GetParError(0) ),"");                                                                                           
  leg_neat->AddEntry((TObject*)0 ,Form("S =  %.2f\n%s #pm %.2f",f_ene->GetParameter(0),"%",f_ene->GetParError(0) ),"");
  leg_neat->AddEntry( (TObject*)0 ,Form("C =  %.2f\n%s  #pm %.2f",(f_ene->GetParameter(1)) ,"%",f_ene->GetParError(1) ),"");
  leg_neat->SetFillColor(0);
  leg_neat->Draw("same");


  c1->SaveAs("plots/reso_"+TString(matrix)+".png");
  c1->SaveAs("plots/reso_"+TString(matrix)+".pdf");




}
