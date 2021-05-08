#include "Drawer.h"
#include "Utility.h"

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TLegend.h"

//ClassImp(Drawer);

//________________________________________________________________________________
Drawer::Drawer(const Drawer &drawer)
//: dummy(drawer.dummy)
{
  
}

//________________________________________________________________________________
void Drawer::getGraphsPAData(std::vector<std::vector<float>> &data, std::vector<TGraphErrors*> &graphs)
{
  /*if (graphs.size() != 4) {
    printf("you need 4 graphs in the vector!\n");
    return;
  }*/

  float refTime = data.at(0).at(0);    /// time is mm/dd/hh/mm/ss.sss format --> convert to seconds!!

  /*for (int i=0; i<data.at(0).size(); i++) {
    graphs.at(0)->SetPoint(i,data.at(0).at(i)-refTime,data.at(1).at(i));
    graphs.at(1)->SetPoint(i,data.at(0).at(i)-refTime,data.at(2).at(i));
    graphs.at(2)->SetPoint(i,data.at(0).at(i)-refTime,data.at(3).at(i));
    graphs.at(3)->SetPoint(i,data.at(0).at(i)-refTime,data.at(4).at(i));
  }*/

  for (int i=0; i<data.at(0).size(); i++) {
    for (int j=0; j<graphs.size(); j++) {
      graphs.at(j)->SetPoint(i,(data.at(0).at(i)-refTime)/3600,data.at(j+1).at(i));
    }
  }

  return;
}

//________________________________________________________________________________
TCanvas* Drawer::drawGraphs(std::vector<TGraphErrors*> &graphs, const std::vector<std::string> &nameVec, const std::string title)
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

  canvas->SetGrid();

  graphs.at(0)->GetYaxis()->SetRangeUser(beautify::getMinRange(minY), beautify::getMaxRange(maxY));

  graphs.at(0)->Draw("APEZ");

  for (auto &graph : graphs) {
    graph->Draw("P same");
  }

  if (nameVec.size() > 0) { this->makeLegend(graphs, nameVec, title)->Draw("same"); }

  return canvas;
}


//________________________________________________________________________________
void Drawer::setAxisLabels(std::vector<TGraphErrors*> &graphs, std::string xName, std::string yName, std::string title)
{
  for (auto &graph : graphs) {
    graph->GetXaxis()->SetTitle(xName.c_str());
    graph->GetYaxis()->SetTitle(yName.c_str());
    if(title != std::string("")) { graph->SetTitle(title.c_str()); }
  }

  return;
}


//________________________________________________________________________________
void Drawer::setBeautifulStyle(std::vector<TGraphErrors*> &graphs)
{
  for (auto &graph : graphs) {
    beautify::setStyleGraph(graph);
  }

  return;
}


//________________________________________________________________________________
void Drawer::setDefaultMarker(std::vector<TGraphErrors*> &graphs)
{
  for (int i = 0; i < graphs.size(); i++) {
    graphs.at(i)->SetMarkerStyle(DEFAULTMARKERSTYLE);
    graphs.at(i)->SetMarkerColor(beautify::colors[i]);
  }

  return;
}


//________________________________________________________________________________
TLegend* Drawer::makeLegend (std::vector<TGraphErrors*> &graphs, const std::vector<std::string> &nameVec, const std::string title)
{
  if (graphs.size() != nameVec.size()) {
    printf("Number of graphs and names need to be equal!\n");
    exit(1);
  }

  TLegend* legend = new TLegend(0.8,0.775,0.95,0.925);

  if (title != std::string("")) { legend->SetHeader(title.c_str(),"C"); }

  for (int i = 0; i < graphs.size(); i++) {
    legend->AddEntry(graphs.at(i), nameVec.at(i).c_str(),"p");
  }

  return legend;
}