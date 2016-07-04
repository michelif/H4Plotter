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
#include "interface/H4AnalysisTree.h"
#include "TVectorD.h"
#include "TF1.h"

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
    std::cout << "./fitPedestal [runName] ((xtal11),(xtal_4fibres)) " << std::endl;
    exit(12345);

  }


  std::vector<int> chMap;
  chMap.push_back(0);//xtal1
  chMap.push_back(1);
  chMap.push_back(2);
  chMap.push_back(3);
  chMap.push_back(4);
  chMap.push_back(5);
  chMap.push_back(6);//4apd_1
  chMap.push_back(7);//4apd_2
  chMap.push_back(8);//4apd_3
  chMap.push_back(9);//4apd_4
  chMap.push_back(10);
  chMap.push_back(11);
  chMap.push_back(12);
  chMap.push_back(13);
  chMap.push_back(14);
  chMap.push_back(15);
  chMap.push_back(16);
  chMap.push_back(17);




  TFile*  file=TFile::Open("../H4Analysis_2016/ntuples/analysis_"+TString(runName)+".root");
  TTree* tree= (TTree*)file->Get("h4");
  H4AnalysisTree* inputTree= new H4AnalysisTree(tree);

  int nentries = inputTree->fChain->GetEntries();

  TFile*  outFile=TFile::Open("data/pedestals_"+TString(runName)+".root","recreate");
  float chXtal[chMap.size()];
  std::vector<TH1F*> chXtalHisto;
  TVectorD sigmaPedestal(chMap.size());
  
  for(int iEntry=0; iEntry<nentries; ++iEntry ) {
    
    inputTree->fChain->GetEntry( iEntry );
    if( iEntry %  1000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;
    
    float chTot=0;
    for(int i=0;i<chMap.size();++i){
      if(iEntry==0){
	TString istring="";
	istring+=(i+1);
	chXtalHisto.push_back(new TH1F("chXtalHisto_"+istring,"chXtalHisto_"+istring,100,-500,500));
      }
      chXtal[i]=inputTree->charge_sig[chMap[i]];
      chXtalHisto[i]->Fill(chXtal[i]);
    }
  }

  for(int i=0;i<chMap.size();++i){
     chXtalHisto[i]->Fit("gaus","","",chXtalHisto[i]->GetMean()-2*chXtalHisto[i]->GetRMS(),chXtalHisto[i]->GetMean()+2*chXtalHisto[i]->GetRMS());
     TF1* dummy= chXtalHisto[i]->GetFunction("gaus") ;
     sigmaPedestal[i]=dummy->GetParameter(2);
   //    chXtalHisto[i]->Write();
  }

  sigmaPedestal.Write("sigmaPedestal");

  outFile->Write();
  outFile->Close();


}
