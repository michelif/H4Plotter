#ifndef H4AnalysisTree_h
#define H4AnalysisTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class H4AnalysisTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong64_t       index;
   ULong64_t       start_time;
   ULong64_t       time_stamp;
   UInt_t          run;
   UInt_t          spill;
   UInt_t          event;
   Int_t           xtal1;
   Int_t           xtal2;
   Int_t           xtal3;
   Int_t           xtal4;
   Int_t           xtal5;
   Int_t           xtal6;
   Int_t           xtal4apd_1;
   Int_t           xtal4apd_2;
   Int_t           xtal4apd_3;
   Int_t           xtal4apd_4;
   Int_t           xtal11;
   Int_t           xtal12;
   Int_t           xtal13;
   Int_t           xtal14;
   Int_t           xtal15;
   Int_t           xtal16;
   Int_t           xtal17;
   Int_t           xtal18;
   UInt_t          n_channels;
   Float_t         b_charge[18];   //[n_channels]
   Float_t         b_slope[18];   //[n_channels]
   Float_t         b_rms[18];   //[n_channels]
   Float_t         time[18];   //[n_channels]
   Float_t         time_chi2[18];   //[n_channels]
   Float_t         maximum[18];   //[n_channels]
   Float_t         amp_max[18];   //[n_channels]
   Float_t         charge_tot[18];   //[n_channels]
   Float_t         charge_sig[18];   //[n_channels]
   Float_t         fit_ampl[18];   //[n_channels]
   Float_t         fit_time[18];   //[n_channels]
   Float_t         fit_chi2[18];   //[n_channels]
   Float_t         calibration[18];   //[n_channels]
   Int_t           n_planes;
   Float_t         X[2];   //[n_planes]
   Float_t         Y[2];   //[n_planes]
   Int_t           nFibresOnX[2];   //[n_planes]
   Int_t           nFibresOnY[2];   //[n_planes]


   // List of branches
   TBranch        *b_index;   //!
   TBranch        *b_start_time;   //!
   TBranch        *b_time_stamp;   //!
   TBranch        *b_run;   //!
   TBranch        *b_spill;   //!
   TBranch        *b_event;   //!
   TBranch        *b_xtal1;   //!
   TBranch        *b_xtal2;   //!
   TBranch        *b_xtal3;   //!
   TBranch        *b_xtal4;   //!
   TBranch        *b_xtal5;   //!
   TBranch        *b_xtal6;   //!
   TBranch        *b_xtal4apd_1;   //!
   TBranch        *b_xtal4apd_2;   //!
   TBranch        *b_xtal4apd_3;   //!
   TBranch        *b_xtal4apd_4;   //!
   TBranch        *b_xtal11;   //!
   TBranch        *b_xtal12;   //!
   TBranch        *b_xtal13;   //!
   TBranch        *b_xtal14;   //!
   TBranch        *b_xtal15;   //!
   TBranch        *b_xtal16;   //!
   TBranch        *b_xtal17;   //!
   TBranch        *b_xtal18;   //!
   TBranch        *b_n_channels;   //!
   TBranch        *b_b_charge;   //!
   TBranch        *b_b_slope;   //!
   TBranch        *b_b_rms;   //!
   TBranch        *b_time;   //!
   TBranch        *b_time_chi2;   //!
   TBranch        *b_maximum;   //!
   TBranch        *b_amp_max;   //!
   TBranch        *b_charge_tot;   //!
   TBranch        *b_charge_sig;   //!
   TBranch        *b_fit_ampl;   //!
   TBranch        *b_fit_time;   //!
   TBranch        *b_fit_chi2;   //!
   TBranch        *b_calibration;   //!
   TBranch        *b_n_planes;   //!
   TBranch        *b_X;   //!
   TBranch        *b_Y;   //!
   TBranch        *b_nFibresOnX;   //!
   TBranch        *b_nFibresOnY;   //!


   void SetBranches();

   H4AnalysisTree(TTree* tree){
     
     fChain = tree;
     fChain->SetMakeClass(1);
     SetBranches();
   }

};

#endif
