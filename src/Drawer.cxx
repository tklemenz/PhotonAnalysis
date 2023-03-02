#include "Drawer.h"

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"

#include <cmath>

//ClassImp(Drawer);

//________________________________________________________________________________
Drawer::Drawer(const Drawer &drawer)
//: dummy(drawer.dummy)
{

}

//________________________________________________________________________________
//  The data has to be in in the following format
//
//  dataType 0: x-values  x-errors  y-values-1  y-values-1-errors  y-values-2  y-values-2-errors...
//  dataType 1: x-values  y-values-1  y-values-2  y-values-1-errors  y-values-2-errors...
std::vector<TGraphErrors*> Drawer::getGraphsAnyData(std::vector<std::vector<float>> &data, const int dataType, const bool debug)
{
  std::vector<TGraphErrors*> retVec;

  size_t nGraphs = 0;
  if (dataType == 0) {
    nGraphs = (data.size()-2)/2;   // dataType 0
  } else if (dataType == 1) {
    nGraphs = (data.size()-1)/2;   // dataType 1
  }

  for (size_t i=0; i<nGraphs; i++) {
    retVec.emplace_back(new TGraphErrors());
  }

  for (int i=0; i<nGraphs; i++) {
    int k = 0;
    if (dataType == 0) {
      k = 2*(i+1);   // dataType 0
    } else if (dataType == 1) {
      k = i+1;   // dataType 1
    }
    for (int j=0; j<data.at(0).size(); j++) {
      if (debug) { printf("%s%s[DEBUG][Drawer][getGraphsAnyData]%s%s Trying to access data column %i\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, k, text::RESET); }
      retVec.at(i)->SetPoint(j, data.at(0).at(j), data.at(k).at(j));
      if (debug) { printf("%s%s[DEBUG][Drawer][getGraphsAnyData]%s%s Trying to access data column %i\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, k+1, text::RESET); }
      if (dataType == 0) {
        retVec.at(i)->SetPointError(j, data.at(1).at(j), data.at(k+1).at(j));  // dataType 0
      } else if (dataType == 1) {
        retVec.at(i)->SetPointError(j, 0, data.at(k+nGraphs).at(j));  // dataType 1
      }
      if (debug) { printf("%s%s[DEBUG][Drawer][getGraphsAnyData]%s%s Trying to access data column %i worked!\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, k+1, text::RESET); }
    }
  }

  return retVec;
}

//________________________________________________________________________________
// obsolete
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


//________________________________________________________________________________
void Drawer::divideCanvas(TCanvas* can)
{
  gStyle->SetPadBorderMode(0);
  can->Divide(1,2);
  can->cd(1)->SetPad(0.005, 0.3525, 0.995, 0.995);
  can->cd(2)->SetPad(0.005, 0.005, 0.995, 0.3475);
  can->cd(1)->SetTopMargin(0.03);
  can->cd(1)->SetBottomMargin(0.001);
  can->cd(2)->SetTopMargin(0.001);
  can->cd(2)->SetBottomMargin(0.4);
}


//________________________________________________________________________________
TGraphErrors* Drawer::getRatioGraph(TGraphErrors* g1, TGraphErrors* g2, const bool debug)
{
  if(g1->GetN() != g2->GetN()) {
    printf("%s%s[ERROR][Drawer][getRatioGraph]%s%s Number of points of the two grpahs does not match. Cannot make ratio graph!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
    return nullptr;
  }

  TGraphErrors* output = new TGraphErrors();

  double x1, x2, y1, y2, ex1, ex2, ey1, ey2;

  if (debug) {
    printf("%s%s[DEBUG][Drawer][getRatioGraph]%s\n", text::BOLD, text::CYN, text::RESET);
  }

  for (int i = 0; i < g1->GetN(); i++) {
    g1->GetPoint(i, x1, y1);
    g2->GetPoint(i, x2, y2);

    if (x1 != x2) {
      printf("%s%s[ERROR][Drawer][getRatioGraph]%s%s X values do not match. Cannot make ratio graph!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
      return nullptr;
    }

    ex1 = g1->GetErrorX(i);
    ex2 = g2->GetErrorX(i);
    ey1 = g1->GetErrorY(i);
    ey2 = g2->GetErrorY(i);

    double ratioEX, ratioEY, ratio = 0;

    if (y2) {
      ratio = std::abs(y1/y2);
    } else if (y1 == 0 && y2 == 0) {
      ratio = 1;
    } else {
      ratio = 0;
    }
    ratioEX = std::max(ex1, ex2);
    ratioEY = std::sqrt(std::pow(ey1/y2,2) + std::pow(y1*ey2/pow(y2,2),2));

    if (debug) {
      printf("%s", text::CYN);
      printf(fmt::format("gr1 x: {}\t gr2 x: {}\t gr1 y: {}\t gr2 y: {}\t ratio: {}\n", x1,x2,y1,y2,ratio).data());
    }

    output->SetPoint(i, x1, ratio);
    output->SetPointError(i, ratioEX, ratioEY);
  }

  if (debug) {
    printf("\n%s", text::RESET);
  }

  return output;
}


//________________________________________________________________________________
TGraphErrors* Drawer::getSumGraph(std::vector<TGraphErrors*> gVec, const bool debug)
{
  unsigned int nPoints = gVec.at(0)->GetN();
  for (const auto& gr : gVec) {
    if (nPoints != gr->GetN()) {
      printf("%s%s[ERROR][Drawer][getSumGraph]%s%s Number of points of the graphs does not match. Cannot make sum graph!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
      return nullptr;
    }
  }

  TGraphErrors* output = new TGraphErrors();

  double sum = 0;
  double x = 0;
  for (unsigned int i=0; i<nPoints; i++) {
    for (const auto& gr : gVec) {
      sum += gr->GetPointY(i);
      x = gr->GetPointX(i);
    }
    output->SetPoint(i, x, sum);
    sum = 0;
    //TODO: calculate and set yErr
  }

  return output;
}


//________________________________________________________________________________
TGraphErrors* Drawer::getETEGraph(TGraphErrors* g1, TGraphErrors* g2, const bool normalize, const bool debug)
{
  if(g1->GetN() != g2->GetN()) {
    printf("%s%s[ERROR][Drawer][getRatioGraph]%s%s Number of points of the two graphs does not match. Cannot make ETE graph!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
    return nullptr;
  }

  TGraphErrors* output = new TGraphErrors();

  double maxY = TMath::MaxElement(g1->GetN(),g1->GetY());

  for (unsigned int i=0; i<g1->GetN(); i++) {
    if (normalize) {
      auto maxY2 = TMath::MinElement(g2->GetN(),g2->GetY()); // maxY2 is actually the min value since I_GB is negative
      output->SetPoint(i, g2->GetPointX(i), g2->GetPointY(i)/maxY2); // assume that the max I_GB value is 100% ETE
    } else {
      output->SetPoint(i, g2->GetPointX(i), std::abs(g2->GetPointY(i)/maxY));
    }
    //TODO: calculate and set yErr
  }

  return output;
}


//________________________________________________________________________________
TGraphErrors* Drawer::sumGraphs(TGraphErrors* g1, TGraphErrors* g2, const bool debug)
{
  if(g1->GetN() != g2->GetN()) {
    printf("%s%s[ERROR][Drawer][sumGraphs]%s%s Number of points of the two graphs does not match. Cannot sum graphs!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
    return nullptr;
  }

  TGraphErrors* output = new TGraphErrors();

  for (unsigned int i=0; i<g1->GetN(); i++) {
    output->SetPoint(i, g1->GetPointX(i), g1->GetPointY(i) + g2->GetPointY(i));
  }

  return output;
}