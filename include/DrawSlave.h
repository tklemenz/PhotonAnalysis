#ifndef DRAWSLAVE_H
#define DRAWSLAVE_H

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#include <vector>
#include <string>

#define DEFAULTMARKERSTYLE 20

///
/// This class is used to draw stuff
///

class DrawSlave
{
 public:
  DrawSlave() = default;
  ~DrawSlave() = default;
  DrawSlave(const DrawSlave &drawSlave);

  void addData(std::vector<std::vector<float>> data) { mDataVec.emplace_back(data); }
  void makeGraphs();

  /// @todo implement method to choose which graph should be plotted
  TCanvas* drawGraphs(); 

  std::vector<std::vector<std::vector<float>>> mDataVec{};
  std::vector<std::vector<TGraphErrors*>> mGraphsVec{};
  std::vector<std::string> mNameVec{};
  TLegend* mLegend = nullptr;
  TCanvas* mCanvas = nullptr;


  //ClassDef(DrawSlave,1);
};

#endif