//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep  7 10:30:07 2016 by ROOT version 6.06/06
// from TTree /main_h4reco_tree
// found on file: ../H4Analysis_2016_2/ntuples/FFT_noiseCh18_4683.root
//////////////////////////////////////////////////////////

#ifndef h4tree_h
#define h4tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TH2.h"

// Header file for the classes stored in the TTree if any.

class h4tree {
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
   Int_t           xtal2;
   Int_t           xtal3;
   Int_t           xtal4;
   Int_t           xtal4apd_1;
   Int_t           xtal4apd_2;
   Int_t           xtal4apd_3;
   Int_t           xtal4apd_4;
   Int_t           xtal11;
   Int_t           xtal12;
   Int_t           xtal15;
   Int_t           xtal16;
   Int_t           xtal17;
   Int_t           xtal18;
   Int_t           CFD;
   UInt_t          n_channels;
   UInt_t          n_timetypes;
   Float_t         b_charge[13];   //[n_channels]
   Float_t         b_slope[13];   //[n_channels]
   Float_t         b_rms[13];   //[n_channels]
   Float_t         time[13];   //[n_timetypes]
   Float_t         time_chi2[13];   //[n_timetypes]
   Float_t         maximum[13];   //[n_channels]
   Float_t         time_maximum[13];   //[n_channels]
   Float_t         amp_max[13];   //[n_channels]
   Float_t         time_max[13];   //[n_channels]
   Float_t         chi2_max[13];   //[n_channels]
   Float_t         charge_tot[13];   //[n_channels]
   Float_t         charge_sig[13];   //[n_channels]
   Float_t         fit_ampl[13];   //[n_channels]
   Float_t         fit_time[13];   //[n_channels]
   Float_t         fit_chi2[13];   //[n_channels]
   Float_t         calibration[13];   //[n_channels]
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
   TBranch        *b_xtal2;   //!
   TBranch        *b_xtal3;   //!
   TBranch        *b_xtal4;   //!
   TBranch        *b_xtal4apd_1;   //!
   TBranch        *b_xtal4apd_2;   //!
   TBranch        *b_xtal4apd_3;   //!
   TBranch        *b_xtal4apd_4;   //!
   TBranch        *b_xtal11;   //!
   TBranch        *b_xtal12;   //!
   TBranch        *b_xtal15;   //!
   TBranch        *b_xtal16;   //!
   TBranch        *b_xtal17;   //!
   TBranch        *b_xtal18;   //!
   TBranch        *b_CFD;   //!
   TBranch        *b_n_channels;   //!
   TBranch        *b_n_timetypes;   //!
   TBranch        *b_b_charge;   //!
   TBranch        *b_b_slope;   //!
   TBranch        *b_b_rms;   //!
   TBranch        *b_time;   //!
   TBranch        *b_time_chi2;   //!
   TBranch        *b_maximum;   //!
   TBranch        *b_time_maximum;   //!
   TBranch        *b_amp_max;   //!
   TBranch        *b_time_max;   //!
   TBranch        *b_chi2_max;   //!
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

   //IC vector
   std::vector<float> IC;
   std::vector<float> IC_2;
   //histos
   TH2F* responseVsX_uncalib;
   TH2F* responseVsY_uncalib;

   TH2F* responseVsX_calib;
   TH2F* responseVsY_calib;

   TH2F* responseVsX_calib_2;
   TH2F* responseVsY_calib_2;


   h4tree(TTree *tree=0);
   virtual ~h4tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef h4tree_cxx
h4tree::h4tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../H4Analysis_2016_2/ntuples/FFT_noiseCh18_4683.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../H4Analysis_2016_2/ntuples/FFT_noiseCh18_4683.root");
      }
      f->GetObject("",tree);

   }
   Init(tree);
}

h4tree::~h4tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t h4tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t h4tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void h4tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("index", &index, &b_index);
   fChain->SetBranchAddress("start_time", &start_time, &b_start_time);
   fChain->SetBranchAddress("time_stamp", &time_stamp, &b_time_stamp);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("spill", &spill, &b_spill);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("xtal2", &xtal2, &b_xtal2);
   fChain->SetBranchAddress("xtal3", &xtal3, &b_xtal3);
   fChain->SetBranchAddress("xtal4", &xtal4, &b_xtal4);
   fChain->SetBranchAddress("xtal4apd_1", &xtal4apd_1, &b_xtal4apd_1);
   fChain->SetBranchAddress("xtal4apd_2", &xtal4apd_2, &b_xtal4apd_2);
   fChain->SetBranchAddress("xtal4apd_3", &xtal4apd_3, &b_xtal4apd_3);
   fChain->SetBranchAddress("xtal4apd_4", &xtal4apd_4, &b_xtal4apd_4);
   fChain->SetBranchAddress("xtal11", &xtal11, &b_xtal11);
   fChain->SetBranchAddress("xtal12", &xtal12, &b_xtal12);
   fChain->SetBranchAddress("xtal15", &xtal15, &b_xtal15);
   fChain->SetBranchAddress("xtal16", &xtal16, &b_xtal16);
   fChain->SetBranchAddress("xtal17", &xtal17, &b_xtal17);
   fChain->SetBranchAddress("xtal18", &xtal18, &b_xtal18);
   fChain->SetBranchAddress("CFD", &CFD, &b_CFD);
   fChain->SetBranchAddress("index", &index, &b_index);
   fChain->SetBranchAddress("n_channels", &n_channels, &b_n_channels);
   fChain->SetBranchAddress("n_timetypes", &n_timetypes, &b_n_timetypes);
   fChain->SetBranchAddress("b_charge", b_charge, &b_b_charge);
   fChain->SetBranchAddress("b_slope", b_slope, &b_b_slope);
   fChain->SetBranchAddress("b_rms", b_rms, &b_b_rms);
   fChain->SetBranchAddress("time", time, &b_time);
   fChain->SetBranchAddress("time_chi2", time_chi2, &b_time_chi2);
   fChain->SetBranchAddress("maximum", maximum, &b_maximum);
   fChain->SetBranchAddress("time_maximum", time_maximum, &b_time_maximum);
   fChain->SetBranchAddress("amp_max", amp_max, &b_amp_max);
   fChain->SetBranchAddress("time_max", time_max, &b_time_max);
   fChain->SetBranchAddress("chi2_max", chi2_max, &b_chi2_max);
   fChain->SetBranchAddress("charge_tot", charge_tot, &b_charge_tot);
   fChain->SetBranchAddress("charge_sig", charge_sig, &b_charge_sig);
   fChain->SetBranchAddress("fit_ampl", fit_ampl, &b_fit_ampl);
   fChain->SetBranchAddress("fit_time", fit_time, &b_fit_time);
   fChain->SetBranchAddress("fit_chi2", fit_chi2, &b_fit_chi2);
   fChain->SetBranchAddress("calibration", calibration, &b_calibration);
   fChain->SetBranchAddress("index", &index, &b_index);
   fChain->SetBranchAddress("n_planes", &n_planes, &b_n_planes);
   fChain->SetBranchAddress("X", X, &b_X);
   fChain->SetBranchAddress("Y", Y, &b_Y);
   fChain->SetBranchAddress("nFibresOnX", nFibresOnX, &b_nFibresOnX);
   fChain->SetBranchAddress("nFibresOnY", nFibresOnY, &b_nFibresOnY);



   Notify();
}

Bool_t h4tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void h4tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t h4tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef h4tree_cxx
