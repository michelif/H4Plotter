#include "interface/calibrationMinimizer.h"


using namespace std;


namespace calibrationMinimizer{


  std::vector<float> chInt_;
  TH1F*  histoTot_;
  H4AnalysisTree* inputT_;
  int nXtals_;
  int nParameters_;
  std::vector<int> chMap;
  std::vector<int> fixedVar;
  std::vector<int> limitedVar;
  std::vector<float> calibConstant;

  void InitHistos(H4AnalysisTree* tree,int nXtals){
    //100 GeV
    //    histoTot_= new TH1F("chint_xtal_total","chint_xtal_total",600,35000,60000);
    //200 GeV
    //    histoTot_= new TH1F("chint_xtal_total","chint_xtal_total",600,35000*2,60000*2);
    //100 GeV - 4apd
    histoTot_= new TH1F("chint_xtal_total","chint_xtal_total",600,100000,150000);
    inputT_=tree;
    nXtals_=nXtals;
    nParameters_=nXtals_;
  }

  void InitHistosConstrained(H4AnalysisTree* tree,int nXtals,int nParameters){
    histoTot_= new TH1F("chint_xtal_total","chint_xtal_total",600,35000,60000);
    inputT_=tree;
    nXtals_=nXtals;
    nParameters_=nParameters;
  }


  int getNXtals(){
    int n=nXtals_;
    if(nParameters_ != nXtals_) n=nParameters_;
    return n;
  }

  void readConstants(TString filename){
    std::cout<<"reading calib constants from file: "<<filename<<std::endl;
    std::fstream myCfgFile(filename.Data(), std::ios_base::in);
    calibConstant.push_back(1);//central xtal always at 1
    float a;
    while (myCfgFile >> a)
      {
	calibConstant.push_back(a);
	std::cout<<a<<std::endl;
      }

  }

  float getConstant(int iCh){
    return calibConstant[iCh];
  }

  double sigma(const double *par)
  {

    float chXtal[nXtals_];

    int nentries = inputT_->fChain->GetEntries();
    nentries = 25000;

    for(int iEntry=0; iEntry<nentries; ++iEntry ) {

      inputT_->fChain->GetEntry( iEntry );
      //      if( iEntry %  1000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

      if (TMath::Abs(inputT_->X[0])>3 or TMath::Abs(inputT_->X[1])>3 or TMath::Abs(inputT_->Y[0])>3 or TMath::Abs(inputT_->Y[1])>3)continue;
      //      if (TMath::Abs(inputT_->X[0])>2 or TMath::Abs(inputT_->X[1])>2 or TMath::Abs(inputT_->Y[0])>2 or TMath::Abs(inputT_->Y[1])>2)continue;

      if (inputT_->nFibresOnX[0]!=2 || inputT_->nFibresOnY[0]!=2) continue;


      float chTot=0;
      for(int i=0;i<nXtals_;++i){
	chXtal[i]=inputT_->charge_sig[chMap[i]];
	//	if(iEntry%2==0)std::cout<<"par:"<<i<<" "<<par[i]<<" "<<chXtal[i]<<std::endl;

	chTot+=chXtal[i]*par[i];
	//	if(i==10)std::cout<<chXtal[i]<<std::endl;
      }
      histoTot_->Fill(chTot);
    }

    //   float rms_1=histoTot_->GetRMS();
    //    float rms=EffSigma(histoTot_)/histoTot_->GetMean();
    float rms_1=EffSigma(histoTot_);

    float    rms = sqrt(rms_1*rms_1-8.03337e+02*8.03337e+02)/histoTot_->GetMean();

    //    float rms=histoTot_->GetRMS();
//       std::cout<<rms<<" "<<par[8]<<std::endl;
//
//       TCanvas dummy;
//       histoTot_->Draw();
//       dummy.SaveAs("dummy.png");


    histoTot_->Reset(); 

    return rms;
  }


  double sigmaConstrained(const double *par)
  {

    float chXtal[12];//FIXME

    int nentries = inputT_->fChain->GetEntries();
    nentries = 40000;

    for(int iEntry=0; iEntry<nentries; ++iEntry ) {

      inputT_->fChain->GetEntry( iEntry );
      //      if( iEntry %  1000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

      if (TMath::Abs(inputT_->X[0])>3 or TMath::Abs(inputT_->X[1])>3 or TMath::Abs(inputT_->Y[0])>3 or TMath::Abs(inputT_->Y[1])>3)continue;

      if (inputT_->nFibresOnX[0]!=2 || inputT_->nFibresOnY[0]!=2) continue;


      float chTot=0;
      for(int i=0;i<12;++i){//FIXME
	if(iEntry==1 && i<4)	std::cout<<"par:"<<i<<" "<<par[i]<<" "<<std::endl;
	chXtal[i]=inputT_->charge_sig[chMap[i]];
      }

      //      chTot = chXtal[0]*par[0]+chXtal[1]*par[1]+chXtal[2]*1.07*par[2]+chXtal[3]*par[3]+chXtal[4]*par[4]*0.95+chXtal[5]*par[4]*0.93+chXtal[6]*par[4]*1.17+chXtal[7]*par[4]*0.98+chXtal[8]*0.95*par[2]+chXtal[9]*par[5]+chXtal[10]*0.99*par[2]+chXtal[11]*par[6];

      chTot = chXtal[0]*par[0]+chXtal[1]*par[1]*1.03632+chXtal[2]*1.07*par[2]+chXtal[3]*par[1]*0.944709+chXtal[4]*par[3]*0.95+chXtal[5]*par[3]*0.93+chXtal[6]*par[3]*1.17+chXtal[7]*par[3]*0.98+chXtal[8]*0.94*par[2]+chXtal[9]*par[1]*1.07111+chXtal[10]*0.99*par[2]+chXtal[11]*par[1]*0.958860;
      histoTot_->Fill(chTot);
      //      std::cout<<chTot<<std::endl;
    }

    //    float rms=histoTot_->GetRMS()/histoTot_->GetMean();
    float rms=EffSigma(histoTot_)/histoTot_->GetMean();
    //float rms=histoTot_->GetRMS();
    //    std::cout<<rms<<" "<<std::endl;
    TCanvas dummy;
    histoTot_->Draw();
    dummy.SaveAs("dummy.png");
    histoTot_->Reset(); 
    return rms;
  }



  Double_t EffSigma(TH1 * hist)
  {

    TAxis *xaxis = hist->GetXaxis();
    Int_t nb = xaxis->GetNbins();
    if(nb < 10) {
      cout << "effsigma: not enough bins. nbins = " << nb << endl;
      return 0.;
    }
  
    Double_t bwid = xaxis->GetBinWidth(1);
    if(bwid == 0) {
      cout << "effsigma: binwidth is zero. bwid = " << bwid << endl;
      return 0.;
    }
    //Double_t xmax = xaxis->GetXmax();//Not used
    Double_t xmin = xaxis->GetXmin();
    Double_t ave = hist->GetMean();
    Double_t rms = hist->GetRMS();

    Double_t total=0.;
    for(Int_t i=0; i<nb+2; i++) {
      total+=hist->GetBinContent(i);
    }
    //   if(total < 100.) {
    //     cout << "effsigma: Too few entries " << total << endl;
    //     return 0.;
    //   }
    Int_t ierr=0;//flag for errors
    Int_t ismin=999;
  
    Double_t rlim=0.683*total;
    Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
    if(nrms > nb/10) nrms=nb/10; // Could be tuned...

    Double_t effSigma=9999999.;//initial crazy value
    for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
      Int_t ibm=(ave-xmin)/bwid +1+iscan;
      Double_t x=(ibm-0.5)*bwid +xmin;
      Double_t x_right=x;
      Double_t x_left=x;
      Int_t j_right=ibm;
      Int_t j_left=ibm;
      Double_t bin=hist->GetBinContent(ibm);//bin=#events nel punto di scan
      total=bin;
      for(Int_t j=1;j<nb;j++){//right direction
	if(j_right < nb) {
	  j_right++;
	  x_right+=bwid;
	  bin=hist->GetBinContent(j_right);
	  total+=bin;
	  if(total > rlim) break;
	}
	else ierr=1;
      
	if(j_left > 0) {//left direction
	  j_left--;
	  x_left-=bwid;
	  bin=hist->GetBinContent(j_left);
	  total+=bin;
	  if(total > rlim) break;
	}
	else ierr=2;

      }

      Double_t dxf=(total-rlim)*bwid/bin;//bin=#events nell'ultimo x toccato
      //cout<<"x_right "<<x_right<<" x_left "<<x_left<<endl;
      //cout<<"dxf is "<<dxf<<endl;
      //cout<<"bwid is "<<bwid<<endl;
      Double_t wid=(x_right-x_left+bwid-dxf)*0.5;
      //cout<<"wid is "<<wid<<endl;
      if(wid < effSigma) {
	effSigma=wid;
	ismin=iscan;
      }
    }//chiude lo scan su iscan
    if(ismin == nrms || ismin == -nrms) ierr=3;
    if(ierr != 0) cout << "effsigma: Error of type " << ierr << endl;
  
    return effSigma;
  
  }


  void setMatrix(std::string matrix){

    //define here the xtals for a 3x3 matrixs, centered in two different xtals
    //            xtalMatrix4APD = array.array('i',[tree.xtal1,tree.xtal2,tree.xtal3,tree.xtal6,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal11,tree.xtal14,tree.xtal15,tree.xtal16])
    // xtal11      xtalMatrixXtal11 = array.array('i',[tree.xtal11,tree.xtal2,tree.xtal3,tree.xtal4,tree.xtal4apd_1,tree.xtal4apd_2,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal12,tree.xtal15,tree.xtal16,tree.xtal17])
    // xtal11_2    xtalMatrixXtal11 = array.array('i',[tree.xtal11,tree.xtal2,tree.xtal15,tree.xtal3,tree.xtal16,tree.xtal4apd_1,tree.xtal12,tree.xtal4apd_2,tree.xtal17,tree.xtal4apd_3,tree.xtal4apd_4,tree.xtal4])

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

    if(matrix=="xtal11_FFT"){//FIXME do dynamic assignement, fft files have different ordering since some channels not reconstructed to save time
      chMap.push_back(7);
      chMap.push_back(0);
      chMap.push_back(1);
      chMap.push_back(2);
      chMap.push_back(3);
      chMap.push_back(4);
      chMap.push_back(5);
      chMap.push_back(6);
      chMap.push_back(8);
      chMap.push_back(9);
      chMap.push_back(10);
      chMap.push_back(11);
    }

    if(matrix=="xtal11_2"){//FIXME do dynamic assignement, new files have different ordering since some channels not reconstructed to save time
      chMap.push_back(7);
      chMap.push_back(0);
      chMap.push_back(1);
      chMap.push_back(2);
      chMap.push_back(3);
      chMap.push_back(4);
      chMap.push_back(5);
      chMap.push_back(6);
      chMap.push_back(8);
      chMap.push_back(9);
      chMap.push_back(10);
      chMap.push_back(11);
    }

    if(matrix=="xtal4apd"){//FIXME do dynamic assignement
      chMap.push_back(3);
      chMap.push_back(4);
      chMap.push_back(5);
      chMap.push_back(6);
      chMap.push_back(7);
      chMap.push_back(0);
      chMap.push_back(1);
      chMap.push_back(9);
      chMap.push_back(10);
      chMap.push_back(13);
      chMap.push_back(14);
      chMap.push_back(15);
    }



  }

  void fixVariable(ROOT::Math::Minimizer* minimizer,int ivar, float value){
      TString istring="ic_";
      istring+=ivar;
      minimizer->SetFixedVariable(ivar,istring.Data(),value);
      fixedVar.push_back(ivar);
  }

  void limitVariable(ROOT::Math::Minimizer* minimizer,int ivar, float value, float low, float up){
      TString istring="ic_";
      istring+=ivar;
      minimizer->SetLimitedVariable(ivar,istring.Data(),value,0.005,low,up);
      limitedVar.push_back(ivar);
  }


  void fitConstants(ROOT::Math::Minimizer* minimizer)
  {

    //if(nParameters_==nXtals_)
    ROOT::Math::Functor f(&sigma,nParameters_);
    //       ROOT::Math::Functor f(&sigmaConstrained,nParameters_);

    minimizer->SetMaxFunctionCalls(100);
    minimizer->SetMaxIterations(100);
    minimizer->SetTolerance(1e-3);
    minimizer->SetPrintLevel(0);


    minimizer->SetFunction(f);


    minimizer->Minimize();

    const double* par=minimizer->X();
    std::cout << "+++++ FIT RESULT: " <<std::endl;
    for (int i=0;i<nParameters_;++i){
      if(std::find(limitedVar.begin(), limitedVar.end(), i) != limitedVar.end())std::cout<<"----> minimizer result ----> ";
      std::cout<<"c_"<<i<<" "<<par[i]<<std::endl;
      }

  }

};


