#ifndef DRAWER_H
#define DRAWER_H

#include <vector>
#include <string>

#include "Utility.h"

#define DEFAULTMARKERSTYLE 20

class TCanvas;
class TGraphErrors;
class TLegend;

///
/// This class is used to draw stuff
///

class Drawer
{
 public:
  Drawer() = default;
  ~Drawer() = default;
  Drawer(const Drawer &drawer);

  TGraphErrors* getSpecificGraph(std::vector<std::vector<float>> &data, const current6ch channel, const bool debug = false);

  void getGraphsPAData(std::vector<std::vector<float>> &data, std::vector<TGraphErrors*> &graphs);    // only works for picoLogic data format

  void setAxisLabels(std::vector<TGraphErrors*> &graphs,
                     std::string                 xName = "time [s]",
                     std::string                 yName = "current [nA]",
                     std::string                 title = std::string(""));

  void setBeautifulStyle(std::vector<TGraphErrors*> &graphs);

  void setDefaultMarker(std::vector<TGraphErrors*> &graphs);

  TLegend* makeLegend (std::vector<TGraphErrors*> &graphs,
                 const std::vector<std::string>   &nameVec,
                 const std::string                 title = std::string(""));

  TCanvas* drawGraphs(std::vector<TGraphErrors*> &graphs,
                const std::vector<std::string>   &nameVec = std::vector<std::string>(),
                const std::string                 title = std::string(""));

  //ClassDef(Drawer,1);
};

#endif