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

#define OPT_ALL 0 //to optimize all constants all together
#define OPT_CONSTRAINED 0 //to optimize constants with constraints

int main( int argc, char* argv[] ) {


  if( argc<2 ) {
    std::cout << "ERROR. You need to specify the name of the run you want to process." << std::endl;
    exit(1);
  }

  std::string runName = "";
  std::string chosen3x3 = "xtal11";
  int floatVariable=-1;
  std::string optimizationStep="0";

  if( argc>1 ) {
    std::string runName_str(argv[1]);
    runName = runName_str;
    if(argc>2){
      std::string matrix(argv[2]);
      chosen3x3=matrix;
      if(argc>3){
	floatVariable=atoi(argv[3]);
	if(argc>4){
	  std::string opt(argv[4]);
	  optimizationStep=opt;
	}
      }
    }
  } else {

    std::cout << "Usage:" << std::endl;
    std::cout << "./intercalibrateChannels [runName] ((xtal11),(xtal_4fibres)) (floatVariable) (optimizationStep)" << std::endl;
    exit(12345);

  }


  
  //  TFile*  file=TFile::Open("../H4Analysis_2016/ntuples/analysis_"+TString(runName)+".root");//FIXME do better handling of files and trees
  //  TFile*  file=TFile::Open("../H4Analysis_2016_2/ntuples/FFT_noiseCh18_"+TString(runName)+".root");
  TFile*  file=TFile::Open("../H4Analysis_2016_2/ntuples/FFT_noiseCh18_test_"+TString(runName)+".root");
  TTree* tree= (TTree*)file->Get("h4");
  //  TTree* tree= (TTree*)file->Get("fft_digi");
  TTree* hodoTree= (TTree*)file->Get("hodo");
  //  tree->AddFriend(hodoTree);

  H4AnalysisTree* inputTree= new H4AnalysisTree(tree);


  if(!OPT_CONSTRAINED)  calibrationMinimizer::InitHistos(inputTree,12);
  else calibrationMinimizer::InitHistos(inputTree,4);

  calibrationMinimizer::setMatrix(chosen3x3);
  
  ROOT::Math::Minimizer*    minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  //  ROOT::Math::Minimizer*    minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Simplex");
  if(chosen3x3!="xtal4apd"){//one fixed parameter with xtal11
    minimizer->SetFixedVariable(0,"c_0",1.);//intercalib of central channel is fixed to 1
  }else{
    minimizer->SetFixedVariable(0,"c_0",0.95);//intercalib of 4 apd taken from single channel response
    minimizer->SetFixedVariable(1,"c_1",0.93);//intercalib of 4 apd taken from single channel response
    minimizer->SetFixedVariable(2,"c_2",1.17);//intercalib of 4 apd taken from single channel response
    minimizer->SetFixedVariable(3,"c_3",0.98);//intercalib of 4 apd taken from single channel response
  }
    
  if(OPT_CONSTRAINED) calibrationMinimizer::readConstants("data/optStepConstrained"+optimizationStep+".txt");
  else if(!OPT_ALL){
    if(chosen3x3!="xtal4apd") calibrationMinimizer::readConstants("data/optStep"+optimizationStep+".txt");
    else calibrationMinimizer::readConstants("data/opt4apdStep"+optimizationStep+".txt");
  }
  else calibrationMinimizer::readConstants("data/opt_all"+optimizationStep+".txt");

  int index=1;
    for(int i =1;i<calibrationMinimizer::getNXtals();++i){
      if(chosen3x3=="xtal4apd" && i<4)continue;
      if(!OPT_CONSTRAINED){
	if(i==floatVariable || OPT_ALL){
	  calibrationMinimizer::limitVariable(minimizer,i,calibrationMinimizer::getConstant(index),0.5,3);
	}else{
	  calibrationMinimizer::fixVariable(minimizer,i,calibrationMinimizer::getConstant(index));
	}
      }else{
	if(i==floatVariable || OPT_ALL){
	  if(i!=3)	  calibrationMinimizer::limitVariable(minimizer,i,calibrationMinimizer::getConstant(index),0.5,1.5);  
	  else calibrationMinimizer::limitVariable(minimizer,i,calibrationMinimizer::getConstant(index),0.4,1.5);  //with 4apd try to go lower
	}else{
	  calibrationMinimizer::fixVariable(minimizer,i,calibrationMinimizer::getConstant(index));
	}
    }
      index++;
    }
  calibrationMinimizer::fitConstants(minimizer);


}
