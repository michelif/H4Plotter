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

  std::string matrix = "xtal11";
  bool addMC=false;

  if( argc>1 ) {
    std::string matrix_str(argv[1]);
    matrix = matrix_str;
    if( argc>2 ) {
      addMC=true;
    }
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
  }else if (matrix == "xtal4apd") {
    runs.push_back(4772);
    energies.push_back(20);
    runs.push_back(4669);
    energies.push_back(50);
    runs.push_back(4682);
    energies.push_back(100);
    runs.push_back(4694);
    energies.push_back(200);
  }





  TGraphErrors* resVsEnergy_xtal = new TGraphErrors(0);
  TGraphErrors* resVsEnergy_matrix = new TGraphErrors(0);

  TGraphErrors* resVsEnergy_xtal_MC = new TGraphErrors(0);
  TGraphErrors* resVsEnergy_matrix_MC = new TGraphErrors(0);


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

    std::cout<<i<<" "<<(*resValue)[0]<<" "<<(*resValueErr)[0]<<" "<<energies[i]<<std::endl;

    resVsEnergy_xtal->SetPoint( i, energies[i], (*resValue)[0]);
    resVsEnergy_xtal->SetPointError( i, 0,(*resValueErr)[0]);

    resVsEnergy_matrix->SetPoint( i, energies[i], (*resValue)[1]);
    resVsEnergy_matrix->SetPointError( i, 0,(*resValueErr)[1]);

    if(addMC) {
      

      TString energy;
      energy.Form("%d",(int)energies[i]);
      energy+="GeV";
      TFile* file_MC = TFile::Open("plots/resolution_Ideal2016_smeared_"+energy+".root");


      TVectorD* resValue_MC=(TVectorD*)file_MC->Get("resolution");
      TVectorD* resValueErr_MC=(TVectorD*)file_MC->Get("resolutionErr");
      
      resVsEnergy_xtal_MC->SetPoint( i, energies[i], (*resValue_MC)[0]);
      resVsEnergy_xtal_MC->SetPointError( i, 0,(*resValueErr_MC)[0]);
      
      resVsEnergy_matrix_MC->SetPoint( i, energies[i], (*resValue_MC)[1]);
      resVsEnergy_matrix_MC->SetPointError( i, 0,(*resValueErr_MC)[1]);
      
      
    }


  }


  TCanvas* c1 = new TCanvas();
  //  TH2D* h2_axes_3 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 1.1*((resVsEnergy_xtal->GetY())[0]+(resVsEnergy_xtal->GetErrorY(0))));
  TH2D* h2_axes_3 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 7.);
  h2_axes_3->SetXTitle("Beam Energy [GeV]");
  h2_axes_3->SetYTitle("Energy Resolution [%]");

  //  TF1 *fun= new TF1("fun",  "sqrt([0]*[0]/x+[1]*[1]+ [2]*[2]/(x*x))",1, 250+5.);

  TF1* f_ene= new TF1("fun_ene","sqrt([0]*[0]/x+[1]*[1])",(resVsEnergy_xtal->GetX())[0]-10,(resVsEnergy_xtal->GetX())[resVsEnergy_xtal->GetN()-1] +10);

  h2_axes_3->Draw("");

  resVsEnergy_xtal->SetMarkerStyle(20);
  resVsEnergy_xtal->SetMarkerSize(1.6);
  resVsEnergy_xtal->SetMarkerColor(kBlue);

  resVsEnergy_xtal->Draw("p same");

  if(addMC){
    resVsEnergy_xtal_MC->SetMarkerStyle(20);
    resVsEnergy_xtal_MC->SetMarkerSize(1.6);
    resVsEnergy_xtal_MC->SetMarkerColor(kRed);
    
    resVsEnergy_xtal_MC->Draw("p same");
  }



  f_ene->SetParameter(0, 0);
  //  f_ene->SetParLimits(0, 5, 20.);
  f_ene->SetParameter(1, 0);
  f_ene->SetParLimits(1, 0.,3);
  //  f_ene->FixParameter(2, 803.);

  resVsEnergy_xtal->Fit("fun_ene","RNE");



  f_ene->SetLineWidth(1.);
  f_ene->SetLineColor(kBlue+2);
  f_ene->Draw("L same");



  TLegend* leg_neat = new TLegend(0.4, 0.92-0.06*5 , 0.8, 0.92);
  leg_neat->SetTextSize(0.038);
  std::string ene="Electron Beam";
  leg_neat->AddEntry(resVsEnergy_xtal,"Data CeF_{3}","p");
  //      leg_neat->AddEntry((TObject*)0 ,Form("S =  %.2f\n%s #pm %.2f / #sqrt{E [GeV]}",f_ene->GetParameter(0),"%",f_ene->GetParError(0) ),"");                                                                                       

  if(addMC) leg_neat->AddEntry(resVsEnergy_xtal_MC,"Simulation","p");
    
  leg_neat->AddEntry((TObject*)0 ,Form("S =  %.2f\n%s #pm %.2f",f_ene->GetParameter(0),"%",f_ene->GetParError(0) ),"");
  leg_neat->AddEntry( (TObject*)0 ,Form("C =  %.2f\n%s  #pm %.2f",(f_ene->GetParameter(1)) ,"%",f_ene->GetParError(1) ),"");
  leg_neat->SetFillColor(0);
  leg_neat->Draw("same");


  c1->SaveAs("plots/reso_"+TString(matrix)+".png");
  c1->SaveAs("plots/reso_"+TString(matrix)+".pdf");

  //matrix
  c1->Clear();
  //  TH2D* h2_axes_4 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 1.1*((resVsEnergy_matrix->GetY())[0]+(resVsEnergy_matrix->GetErrorY(0))));
  TH2D* h2_axes_4 = new TH2D( "axes_1", "", 100, -0.0, 250. , 110, 0., 7);
  h2_axes_4->SetXTitle("Beam Energy [GeV]");
  h2_axes_4->SetYTitle("Energy Resolution [%]");

  //  TF1 *fun= new TF1("fun",  "sqrt([0]*[0]/x+[1]*[1]+ [2]*[2]/(x*x))",1, 250+5.);

  TF1* f_ene_2= new TF1("fun_ene_2","sqrt([0]*[0]/x+[1]*[1])",(resVsEnergy_matrix->GetX())[0]-10,(resVsEnergy_matrix->GetX())[resVsEnergy_matrix->GetN()-1] +10);

  h2_axes_4->Draw("");

  resVsEnergy_matrix->SetMarkerStyle(20);
  resVsEnergy_matrix->SetMarkerSize(1.6);
  resVsEnergy_matrix->SetMarkerColor(kBlue);

  resVsEnergy_matrix->Draw("p same");


  if(addMC){
    resVsEnergy_matrix_MC->SetMarkerStyle(20);
    resVsEnergy_matrix_MC->SetMarkerSize(1.6);
    resVsEnergy_matrix_MC->SetMarkerColor(kRed);
    
    resVsEnergy_matrix_MC->Draw("p same");
  }


  f_ene_2->SetParameter(0, 0);
  //  f_ene_2->SetParLimits(0, 5, 20.);
  f_ene_2->SetParameter(1, 90);
  f_ene_2->SetParLimits(1, 0,2);
  //  f_ene_2->FixParameter(2, 803.);

  resVsEnergy_matrix->Fit("fun_ene_2","RN");



  f_ene_2->SetLineWidth(1.);
  f_ene_2->SetLineColor(kBlue+2);
  f_ene_2->Draw("L same");



  TLegend* leg_neat_2 = new TLegend(0.4, 0.92-0.06*5 , 0.8, 0.92);
  leg_neat_2->SetTextSize(0.038);
  leg_neat_2->AddEntry(resVsEnergy_matrix,"Data CeF_{3}","p");
  //      leg_neat_2->AddEntry((TObject*)0 ,Form("S =  %.2f\n%s #pm %.2f / #sqrt{E [GeV]}",f_ene_2->GetParameter(0),"%",f_ene_2->GetParError(0) ),"");                                                                                 

  if(addMC) leg_neat_2->AddEntry(resVsEnergy_matrix_MC,"Simulation","p");
          
  leg_neat_2->AddEntry((TObject*)0 ,Form("S =  %.2f\n%s #pm %.2f",f_ene_2->GetParameter(0),"%",f_ene_2->GetParError(0) ),"");
  leg_neat_2->AddEntry( (TObject*)0 ,Form("C =  %.2f\n%s  #pm %.2f",(f_ene_2->GetParameter(1)) ,"%",f_ene_2->GetParError(1) ),"");
  leg_neat_2->SetFillColor(0);
  leg_neat_2->Draw("same");


  c1->SaveAs("plots/reso_matrix_"+TString(matrix)+".png");
  c1->SaveAs("plots/reso_matrix_"+TString(matrix)+".pdf");

  c1->Clear();
  h2_axes_3->Draw("");
  resVsEnergy_matrix->Draw("p same");
  f_ene_2->Draw("L same");
  resVsEnergy_xtal->SetMarkerColor(kRed);
  resVsEnergy_xtal->Draw("p same");
  f_ene->SetLineColor(kRed);
  f_ene->Draw("L same");

  c1->SaveAs("plots/reso_matrix_xtal_comparison_"+TString(matrix)+".png");
  c1->SaveAs("plots/reso_matrix_xtal_comparison_"+TString(matrix)+".pdf");


}
