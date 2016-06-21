#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream> 
#include <TROOT.h> 
#include "TSystem.h"

#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TChain.h"
#include "TVectorD.h"
#include "TH2.h"
#include "TGraph.h"
#include "TPaveText.h"
#include "interface/calibrationMinimizer.h"
#include "interface/H4AnalysisTree.h"


int main( int argc, char* argv[] ) {


  if( argc<2 ) {
    std::cout << "ERROR. You need to specify the name of the run you want to process." << std::endl;
    exit(1);
  }

  std::string runName = "";
  if( argc>1 ) {
    std::string runName_str(argv[1]);
    runName = runName_str;
  }

  
  TFile*  file=TFile::Open("../H4Analysis_2016/ntuples/analysis_"+TString(runName)+".root");
  TTree* tree= (TTree*)file->Get("h4");
  int nentries = tree->GetEntries();
  nentries=10;
  H4AnalysisTree* inputTree= new H4AnalysisTree(tree);


  calibrationMinimizer::InitHistos(2);

  for(int iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry( iEntry );
    if( iEntry %  1000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    calibrationMinimizer::FillHisto(0,inputTree->charge_sig[inputTree->xtal11]);
    calibrationMinimizer::FillHisto(1,inputTree->charge_sig[inputTree->xtal12]);

  }

  //    calibrationMinimizer::setChInt(chargeInt);

    ROOT::Math::Minimizer*    minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
    calibrationMinimizer::fitConstants(minimizer);


}
