#define h4tree_cxx
#include "h4tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void h4tree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L h4tree.C
//      root> h4tree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(jentry%1000==0) std::cout<<"Entry:"<<jentry<<std::endl;

      float charge = charge_sig[xtal11]+charge_sig[xtal2]+charge_sig[xtal3]+charge_sig[xtal4]+charge_sig[xtal4apd_1]+charge_sig[xtal4apd_2]+charge_sig[xtal4apd_3]+charge_sig[xtal4apd_4]+charge_sig[xtal12]+charge_sig[xtal15]+charge_sig[xtal16]+charge_sig[xtal17];

      float  charge_calib = IC[0]*charge_sig[xtal11]+IC[1]*charge_sig[xtal2]+IC[2]*charge_sig[xtal3]+IC[3]*charge_sig[xtal4]+IC[4]*charge_sig[xtal4apd_1]+IC[5]*charge_sig[xtal4apd_2]+IC[6]*charge_sig[xtal4apd_3]+IC[7]*charge_sig[xtal4apd_4]+IC[8]*charge_sig[xtal12]+IC[9]*charge_sig[xtal15]+IC[10]*charge_sig[xtal16]+IC[11]*charge_sig[xtal17];

      float  charge_calib_2 = IC_2[0]*charge_sig[xtal11]+IC_2[1]*charge_sig[xtal2]+IC_2[2]*charge_sig[xtal3]+IC_2[3]*charge_sig[xtal4]+IC_2[4]*charge_sig[xtal4apd_1]+IC_2[5]*charge_sig[xtal4apd_2]+IC_2[6]*charge_sig[xtal4apd_3]+IC_2[7]*charge_sig[xtal4apd_4]+IC_2[8]*charge_sig[xtal12]+IC_2[9]*charge_sig[xtal15]+IC_2[10]*charge_sig[xtal16]+IC_2[11]*charge_sig[xtal17];


      if(TMath::Abs(Y[0])<3){
	responseVsX_uncalib->Fill(X[0],charge);
	responseVsX_calib->Fill(X[0],charge_calib);
	responseVsX_calib_2->Fill(X[0],charge_calib_2);
      }
      if(TMath::Abs(X[0])<3) {
	responseVsY_uncalib->Fill(Y[0],charge);
	responseVsY_calib->Fill(Y[0],charge_calib);
	responseVsY_calib_2->Fill(Y[0],charge_calib_2);
      }

   }
}
