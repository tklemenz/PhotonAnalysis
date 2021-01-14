#include "Drawer.h"
#include "Utility.h"

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"

//ClassImp(Drawer);

//________________________________________________________________________________
Drawer::Drawer(const Drawer &drawer)
//: dummy(drawer.dummy)
{
  
}

//________________________________________________________________________________
void Drawer::getGraphsPAData(std::vector<std::vector<float>> &data, std::vector<TGraphErrors*> &graphs)
{
  if (graphs.size() != 4) {
    printf("you need 4 graphs in the vector!\n");
    return;
  }

  float refTime = data.at(0).at(0);    /// time is mm/dd/hh/mm/ss.sss format --> convert to seconds!!

  for (int i=0; i<data.at(0).size(); i++) {
    graphs.at(0)->SetPoint(i,data.at(0).at(i)-refTime,data.at(1).at(i));
    graphs.at(1)->SetPoint(i,data.at(0).at(i)-refTime,data.at(2).at(i));
    graphs.at(2)->SetPoint(i,data.at(0).at(i)-refTime,data.at(3).at(i));
    graphs.at(3)->SetPoint(i,data.at(0).at(i)-refTime,data.at(4).at(i));
  }

  return;
}

//________________________________________________________________________________
TCanvas* Drawer::drawGraphs(std::vector<TGraphErrors*> &graphs)
{

  float maxY = TMath::MaxElement(graphs.at(0)->GetN(),graphs.at(0)->GetY());
  float minY = TMath::MinElement(graphs.at(0)->GetN(),graphs.at(0)->GetY());

  for (auto &graph : graphs) {
    float max = TMath::MaxElement(graph->GetN(),graph->GetY());
    float min = TMath::MinElement(graph->GetN(),graph->GetY());
    if (max > maxY) { maxY = max; }
    if (min < minY) { minY = min; }
  }

  TCanvas* canvas = new TCanvas();

  beautify::setStyle();

  beautify::setStyleGraph(graphs.at(0));
  beautify::setStyleGraph(graphs.at(1));
  beautify::setStyleGraph(graphs.at(2));
  beautify::setStyleGraph(graphs.at(3));

  graphs.at(0)->SetMarkerStyle(20);
  graphs.at(1)->SetMarkerStyle(20);
  graphs.at(2)->SetMarkerStyle(20);
  graphs.at(3)->SetMarkerStyle(20);

  graphs.at(0)->SetMarkerColor(kYellow+1);
  graphs.at(1)->SetMarkerColor(kBlue);
  graphs.at(2)->SetMarkerColor(kRed);
  graphs.at(3)->SetMarkerColor(kGreen);

  graphs.at(0)->GetYaxis()->SetRangeUser(beautify::getMinRange(minY), beautify::getMaxRange(maxY));

  graphs.at(0)->Draw("APEZ");
  graphs.at(1)->Draw("P same");
  graphs.at(2)->Draw("P same");
  graphs.at(3)->Draw("P same");

  return canvas;
}