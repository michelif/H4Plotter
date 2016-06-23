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
  std::string chosen3x3 = "xtal11";

  if( argc>1 ) {
    std::string runName_str(argv[1]);
    runName = runName_str;
    if(argc>2){
      std::string matrix(argv[2]);
      chosen3x3=matrix;
    }
  } else {

    std::cout << "Usage:" << std::endl;
    std::cout << "./intercalibrateChannels [runName] ((xtal11),(xtal_4fibres)) " << std::endl;
    exit(12345);

  }


  
  TFile*  file=TFile::Open("../H4Analysis_2016/ntuples/analysis_"+TString(runName)+".root");
  TTree* tree= (TTree*)file->Get("h4");
  H4AnalysisTree* inputTree= new H4AnalysisTree(tree);


  calibrationMinimizer::InitHistos(inputTree,12);
  calibrationMinimizer::setMatrix(chosen3x3);
  
  ROOT::Math::Minimizer*    minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  calibrationMinimizer::fitConstants(minimizer);


}
