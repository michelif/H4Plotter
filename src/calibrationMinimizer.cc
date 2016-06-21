#include "interface/calibrationMinimizer.h"

using namespace std;


namespace calibrationMinimizer{


  std::vector<float> chInt_;
  std::vector<TH1F*>  histoChInt_;
  TH1F*  histoTot_;

  void InitHistos(int nXtals){
     for(int i=0;i<nXtals;++i){
      TString istring;
      istring+=i;
      std::cout<<istring<<std::endl;
      histoChInt_.push_back(new TH1F("chint_xtal"+istring,"chint_xtal"+istring,300,0,60000));
    }
    histoTot_= new TH1F("chint_xtal_total","chint_xtal_total",300,0,60000*4);
  }

  void FillHisto(int ch,float value){
    histoChInt_[ch]->Fill(value);
  }


  void setChInt(std::vector<float> ch)
  {
    for(int i=0;i<ch.size();++i)
      chInt_.push_back(ch.at(i));
   
  }


  std::vector<float> getChInt(){
    return chInt_;
  }





  double sigma(const double *par)
  {
    int nXtal=histoChInt_.size();
    float chTot=0;
    float chXtal;
    for (int i=0;i<nXtal;++i)
      {
	for(int j=0;j<histoChInt_[i]->GetNbinsX();++j){
	  chXtal=histoChInt_[i]->GetBinContent(j);
	  histoTot_->Fill(chXtal*par[i]);
	}
      }

    return histoTot_->GetRMS();
  }





  void fitConstants(ROOT::Math::Minimizer* minimizer)
  {
    //    ROOT::Math::Minimizer*    minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
    ROOT::Math::Functor f(&sigma,1);

    minimizer->SetMaxFunctionCalls(100000);
    minimizer->SetMaxIterations(100);
    minimizer->SetTolerance(1e-3);
    minimizer->SetPrintLevel(0);


    minimizer->SetFunction(f);
    minimizer->SetLimitedVariable(0,"ic_0",1,1e-2,0,10);
    minimizer->SetLimitedVariable(1,"ic_1",1,1e-2,0,10);

    minimizer->Minimize();

    const double* par=minimizer->X();
    std::cout << "+++++ FIT RESULT: " << par[0] << "," << par[1] << std::endl;


  }

};

