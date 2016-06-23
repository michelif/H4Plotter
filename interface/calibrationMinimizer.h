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
}
#endif
