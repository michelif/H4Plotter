#ifndef CALIBRATION_MINIMIZER_H
#define CALIBRATION_MINIMIZER_H

#include "Math/Interpolator.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TMath.h"
#include "TH1F.h"
#include "assert.h"
#include <iostream>
#include <fstream>
#include "TCanvas.h"
#include "TTree.h"

#include "interface/H4AnalysisTree.h"

namespace calibrationMinimizer
{
  void setChInt(std::vector<float> ch);
  std::vector<float> getChInt();
  double sigma(const double *par );
  void fitConstants(ROOT::Math::Minimizer* minimizer);
  void InitHistos(H4AnalysisTree* tree,int nXtals);
  void FillHisto(int ch,float value);
  void setMatrix(std::string matrix);
  void fixVariable(ROOT::Math::Minimizer* minimizer,int ivar, float value);
  void limitVariable(ROOT::Math::Minimizer* minimizer,int ivar, float value, float low, float up);
  int getNXtals();
  void readConstants(TString filename);
  float getConstant(int iCh);
  Double_t EffSigma(TH1 * hist);
}
#endif
