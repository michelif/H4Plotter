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
#include "interface/h4tree.h"

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

  TString fileName;
  fileName = "../H4Analysis_2016_2/ntuples/"+prefix+"_"+runName+".root";

  TFile* file = TFile::Open(fileName);
  if( file==0 ) {
    std::cout << "ERROR! Din't find file " << fileName << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(11);
  }

  TTree* tree = (TTree*) file->Get("h4");
  h4tree* h4 = new h4tree(tree);


  TFile* outFile = TFile::Open(TString("plots/checkInterCalib_"+prefix+"_"+runName+".root"),"recreate");
  h4->responseVsX_uncalib = new TH2F("responseVsX_uncalib","responseVsX_uncalib",20,-5,5,100,40000,60000);
  h4->responseVsY_uncalib = new TH2F("responseVsY_uncalib","responseVsY_uncalib",20,-5,5,100,40000,60000);

  h4->responseVsX_calib = new TH2F("responseVsX_calib","responseVsX_calib",20,-5,5,100,40000,60000);
  h4->responseVsY_calib = new TH2F("responseVsY_calib","responseVsY_calib",20,-5,5,100,40000,60000);

  h4->responseVsX_calib_2 = new TH2F("responseVsX_calib_2","responseVsX_calib_2",20,-5,5,100,40000,60000);
  h4->responseVsY_calib_2 = new TH2F("responseVsY_calib_2","responseVsY_calib_2",20,-5,5,100,40000,60000);

  //old const derived with iterative process
  h4->IC.push_back(1);
  h4->IC.push_back(0.521416);
  h4->IC.push_back(0.791515);
  h4->IC.push_back(0.524998);
  h4->IC.push_back(0.504333);
  h4->IC.push_back(0.506726);
  h4->IC.push_back(0.517119);
  h4->IC.push_back(0.50302);
  h4->IC.push_back(1.00179);
  h4->IC.push_back(0.512666);
  h4->IC.push_back(0.622622);
  h4->IC.push_back(0.899577);

  //new const with constraints
  h4->IC_2.push_back(1);
  h4->IC_2.push_back(0.932688);
  h4->IC_2.push_back(1.56220);
  h4->IC_2.push_back(0.850238);
  h4->IC_2.push_back(0.492765);
  h4->IC_2.push_back(0.482391);
  h4->IC_2.push_back(0.606879);
  h4->IC_2.push_back(0.508326);
  h4->IC_2.push_back(1.37240);
  h4->IC_2.push_back(0.963999);
  h4->IC_2.push_back(1.44540);
  h4->IC_2.push_back(0.862974);
  
  h4->Loop();

  h4->responseVsX_uncalib->Write();
  h4->responseVsY_uncalib->Write();

  h4->responseVsX_calib->Write();
  h4->responseVsY_calib->Write();

  h4->responseVsX_calib_2->Write();
  h4->responseVsY_calib_2->Write();


  return 0;

}
