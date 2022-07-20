#include "Drawer.h"

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"

//ClassImp(Drawer);

//________________________________________________________________________________
Drawer::Drawer(const Drawer &drawer)
//: dummy(drawer.dummy)
{

}

//________________________________________________________________________________
//  The data has to be in in the following format
//
//  x-values  x-errors  y-values-1  y-values-1-errors  y-values-2  y-values-2-errors...
std::vector<TGraphErrors*> Drawer::getGraphsAnyData(std::vector<std::vector<float>> &data, const bool debug)
{
  std::vector<TGraphErrors*> retVec;

  size_t nGraphs = (data.size()-2)/2;
  for (size_t i=0; i<nGraphs; i++) {
    retVec.emplace_back(new TGraphErrors());
  }

  for (int i=0; i<nGraphs; i++) {
    int k = 2*(i+1);
    for (int j=0; j<data.at(0).size(); j++) {
      if (debug) { printf("%s%s[DEBUG][Drawer][getGraphsAnyData]%s%s Trying to access data column %i\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, k, text::RESET); }
      retVec.at(i)->SetPoint(j, data.at(0).at(j), data.at(k).at(j));
      if (debug) { printf("%s%s[DEBUG][Drawer][getGraphsAnyData]%s%s Trying to access data column %i\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, k+1, text::RESET); }
      retVec.at(i)->SetPointError(j, data.at(1).at(j), data.at(k+1).at(j));
      if (debug) { printf("%s%s[DEBUG][Drawer][getGraphsAnyData]%s%s Trying to access data column %i worked!\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, k+1, text::RESET); }
    }
  }

  return retVec;
}

//________________________________________________________________________________
TGraphErrors* Drawer::getSpecificGraph(std::vector<std::vector<float>> &data, const current6ch channel, const bool debug)
{
  TGraphErrors* graph = new TGraphErrors();

  float refTime = data.at(0).at(0);

  if (debug) { printf("%s%s[DEBUG][Drawer][getSpecificGraph]%s%s data.size(): %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, data.size(), text::RESET); }
  if (debug) { printf("%s%s[DEBUG][Drawer][getSpecificGraph]%s%s Accessing data.at(%i)...\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, channel+1, text::RESET); }
  for (int i=0; i<data.at(0).size(); i++) {
    graph->SetPoint(i,(data.at(0).at(i)-refTime)/*/3600*/,data.at(channel+1).at(i));
  }
  if (debug) { printf("%s%s[DEBUG][Drawer][getSpecificGraph]%s%s done\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  return graph;
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
      graphs.at(j)->SetPoint(i,(data.at(0).at(i)-refTime)/*/3600*/,data.at(j+1).at(i));
    }
  }

  return;
}

//________________________________________________________________________________
TCanvas* Drawer::drawGraphs(std::vector<TGraphErrors*> &graphs, const std::vector<std::string> &nameVec, const std::string title)
{
  float maxY = TMath::MaxElement(graphs.at(0)->GetN(),graphs.at(0)->GetY());
  float minY = TMath::MinElement(graphs.at(0)->GetN(),graphs.at(0)->GetY());

  float maxX = TMath::MaxElement(graphs.at(0)->GetN(),graphs.at(0)->GetX());
  float minX = TMath::MinElement(graphs.at(0)->GetN(),graphs.at(0)->GetX());

  for (auto &graph : graphs) {
    float maxy = TMath::MaxElement(graph->GetN(),graph->GetY());
    float miny = TMath::MinElement(graph->GetN(),graph->GetY());
    if (maxy > maxY) { maxY = maxy; }
    if (miny < minY) { minY = miny; }

    float maxx = TMath::MaxElement(graph->GetN(),graph->GetX());
    float minx = TMath::MinElement(graph->GetN(),graph->GetX());
    if (maxx > maxX) { maxX = maxx; }
    if (minx < minX) { minX = minx; }
  }

  TCanvas* canvas = new TCanvas("c", "c", 800, 600);

  //canvas->SetGrid();
  //canvas->SetLogy();

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
    graphs.at(i)->SetMarkerColor(beautify::niceColors[i]);
  }

  return;
}


//________________________________________________________________________________
TLegend* Drawer::makeLegend (std::vector<TGraphErrors*> &graphs, const std::vector<std::string> &nameVec, const std::string title)
{
  if (graphs.size() != nameVec.size()) {
    printf("%s%s[ERROR][Drawer][makeLegend]%s%s Number of graphs and names need to be equal!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
    exit(1);
  }

  //TLegend* legend = new TLegend(0.8,0.775,0.95,0.925);
  // From ALICE style guide
  TLegend *legend = new TLegend( 0.46, 0.6, 0.72, 0.8);
  legend->SetTextSize(gStyle->GetTextSize()*0.6);

  if (title != std::string("")) { legend->SetHeader(title.c_str(),"C"); }

  for (int i = 0; i < graphs.size(); i++) {
    legend->AddEntry(graphs.at(i), nameVec.at(i).c_str(),"p");
  }

  return legend;
}