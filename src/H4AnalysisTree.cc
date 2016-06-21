#include "interface/H4AnalysisTree.h"


void H4AnalysisTree::SetBranches(){
   fChain->SetBranchAddress("index", &index, &b_index);
   fChain->SetBranchAddress("start_time", &start_time, &b_start_time);
   fChain->SetBranchAddress("time_stamp", &time_stamp, &b_time_stamp);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("spill", &spill, &b_spill);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("xtal1", &xtal1, &b_xtal1);
   fChain->SetBranchAddress("xtal2", &xtal2, &b_xtal2);
   fChain->SetBranchAddress("xtal3", &xtal3, &b_xtal3);
   fChain->SetBranchAddress("xtal4", &xtal4, &b_xtal4);
   fChain->SetBranchAddress("xtal5", &xtal5, &b_xtal5);
   fChain->SetBranchAddress("xtal6", &xtal6, &b_xtal6);
   fChain->SetBranchAddress("xtal4apd_1", &xtal4apd_1, &b_xtal4apd_1);
   fChain->SetBranchAddress("xtal4apd_2", &xtal4apd_2, &b_xtal4apd_2);
   fChain->SetBranchAddress("xtal4apd_3", &xtal4apd_3, &b_xtal4apd_3);
   fChain->SetBranchAddress("xtal4apd_4", &xtal4apd_4, &b_xtal4apd_4);
   fChain->SetBranchAddress("xtal11", &xtal11, &b_xtal11);
   fChain->SetBranchAddress("xtal12", &xtal12, &b_xtal12);
   fChain->SetBranchAddress("xtal13", &xtal13, &b_xtal13);
   fChain->SetBranchAddress("xtal14", &xtal14, &b_xtal14);
   fChain->SetBranchAddress("xtal15", &xtal15, &b_xtal15);
   fChain->SetBranchAddress("xtal16", &xtal16, &b_xtal16);
   fChain->SetBranchAddress("xtal17", &xtal17, &b_xtal17);
   fChain->SetBranchAddress("xtal18", &xtal18, &b_xtal18);
   fChain->SetBranchAddress("index", &index, &b_index);
   fChain->SetBranchAddress("n_channels", &n_channels, &b_n_channels);
   fChain->SetBranchAddress("b_charge", b_charge, &b_b_charge);
   fChain->SetBranchAddress("b_slope", b_slope, &b_b_slope);
   fChain->SetBranchAddress("b_rms", b_rms, &b_b_rms);
   fChain->SetBranchAddress("time", time, &b_time);
   fChain->SetBranchAddress("time_chi2", time_chi2, &b_time_chi2);
   fChain->SetBranchAddress("maximum", maximum, &b_maximum);
   fChain->SetBranchAddress("amp_max", amp_max, &b_amp_max);
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


}
