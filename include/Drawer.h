#ifndef DRAWER_H
#define DRAWER_H

#include <vector>
#include <string>

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

  void getGraphsPAData(std::vector<std::vector<float>> &data, std::vector<TGraphErrors*> &graphs);    // only works for 4 grsphs (picoLogic data)

  void setAxisLabels(std::vector<TGraphErrors*> &graphs, std::string xName = "time [s]", std::string yName = "current [nA]");

  void setBeautifulStyle(std::vector<TGraphErrors*> &graphs);

  void setDefaultMarker(std::vector<TGraphErrors*> &graphs);                                          // only works for 4 grsphs (picoLogic data)

  TLegend* makeLegend (std::vector<TGraphErrors*> &graphs, const std::vector<std::string> &nameVec);  // only works for 4 grsphs (picoLogic data)

  TCanvas* drawGraphs(std::vector<TGraphErrors*> &graphs, const std::vector<std::string> &nameVec = std::vector<std::string>());

  //ClassDef(Drawer,1);
};

#endif