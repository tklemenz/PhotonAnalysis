#ifndef DRAWER_H
#define DRAWER_H

#include <vector>

class TCanvas;
class TGraphErrors;

///
/// This class is used to draw stuff
///

class Drawer
{
 public:
  Drawer() = default;
  ~Drawer() = default;
  Drawer(const Drawer &drawer);

  void getGraphsPAData(std::vector<std::vector<float>> &data, std::vector<TGraphErrors*> &graphs);

  TCanvas* drawGraphs(std::vector<TGraphErrors*> &graphs);

  //ClassDef(Drawer,1);
};

#endif