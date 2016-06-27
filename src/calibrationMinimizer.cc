#include "interface/calibrationMinimizer.h"


using namespace std;


namespace calibrationMinimizer{


  std::vector<float> chInt_;
  TH1F*  histoTot_;
  H4AnalysisTree* inputT_;
  int nXtals_;
  std::vector<int> chMap;
  std::vector<int> fixedVar;
  std::vector<int> limitedVar;

  void InitHistos(H4AnalysisTree* tree,int nXtals){
    histoTot_= new TH1F("chint_xtal_total","chint_xtal_total",300,40000,60000);
    inputT_=tree;
    nXtals_=nXtals;
  }


  int getNXtals(){
    return nXtals_;
  }

  double sigma(const double *par)
  {

    float chXtal[nXtals_];

    int nentries = inputT_->fChain->GetEntries();;
    nentries = 1000;

    for(int iEntry=0; iEntry<nentries; ++iEntry ) {

      inputT_->fChain->GetEntry( iEntry );
      //      if( iEntry %  1000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

      if (inputT_->nFibresOnX[0]!=2 || inputT_->nFibresOnY[0]!=2) continue;

      float chTot=0;
      for(int i=0;i<nXtals_;++i){
	//	if(iEntry==1)	std::cout<<"par:"<<i<<" "<<par[i]<<" "<<std::endl;
	chXtal[i]=inputT_->charge_sig[chMap[i]];
	chTot+=chXtal[i]*par[i];
      }
      histoTot_->Fill(chTot);
    }

    float rms=histoTot_->GetRMS()/histoTot_->GetMean();
    //    float rms=histoTot_->GetRMS();
    //    std::cout<<rms<<std::endl;

    histoTot_->Reset(); 
    return rms;
  }



  void setMatrix(std::string matrix){

    //define here the xtals for a 3x3 matrixs, centered in two different xtals
    //            xtalMatrix4APD = array.array('i',[tree.xtal1,tree.xtal2,tree.xtal3,tree.xtal6,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal11,tree.xtal14,tree.xtal15,tree.xtal16])
    //            xtalMatrixXtal11 = array.array('i',[tree.xtal11,tree.xtal2,tree.xtal3,tree.xtal4,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal12,tree.xtal15,tree.xtal16,tree.xtal17])

    if(matrix=="xtal11"){//FIXME do dynamic assignement
      chMap.push_back(10);
      chMap.push_back(1);
      chMap.push_back(2);
      chMap.push_back(3);
      chMap.push_back(6);
      chMap.push_back(7);
      chMap.push_back(8);
      chMap.push_back(9);
      chMap.push_back(11);
      chMap.push_back(14);
      chMap.push_back(15);
      chMap.push_back(16);
    }

  }

  void fixVariable(ROOT::Math::Minimizer* minimizer,int ivar, float value){
      TString istring="ic_";
      istring+=ivar;
      minimizer->SetFixedVariable(ivar,istring.Data(),value);
  }

  void limitVariable(ROOT::Math::Minimizer* minimizer,int ivar, float value, float low, float up){
      TString istring="ic_";
      istring+=ivar;
      minimizer->SetLimitedVariable(ivar,istring.Data(),value,1e-2,low,up);
  }


  void fitConstants(ROOT::Math::Minimizer* minimizer)
  {
    ROOT::Math::Functor f(&sigma,nXtals_);

    minimizer->SetMaxFunctionCalls(100000);
    minimizer->SetMaxIterations(10000);
    minimizer->SetTolerance(1e-3);
    minimizer->SetPrintLevel(0);


    minimizer->SetFunction(f);




    minimizer->Minimize();

    const double* par=minimizer->X();
    std::cout << "+++++ FIT RESULT: " <<std::endl;
    for (int i=0;i<nXtals_;++i){
	std::cout<<"c_"<<i<<" "<<par[i]<<std::endl;
      }

  }

};


