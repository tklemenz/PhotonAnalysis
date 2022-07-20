#include <Rtypes.h>
#include <cstdlib>
#include <TMath.h>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TAxis.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

#include "fmt/format.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/program_options.hpp>
#include <iostream>

using boost::property_tree::ptree;

void printGraphInfo(plotting::graphInfo& info)
{
  std::cout << "name: " << info.name << std::endl;
  std::cout << "active: " << info.active << std::endl;
  std::cout << "column: " << info.column << std::endl;
  std::cout << "color: " << info.color << std::endl;
  std::cout << "title: " << info.title << std::endl;
  std::cout << "from file: " << info.fileNr << std::endl;
  std::cout << "graph address: " << info.graph << std::endl;
  std::cout << std::endl;
}

void printMiscInfo(plotting::miscInfo& info)
{
  std::cout << "x axis title: " << info.xTitle << std::endl;
  std::cout << "y axis title: " << info.yTitle << std::endl;
  std::cout << "legend header: " << info.legendHeader << std::endl;
  std::cout << "marker style: " << info.markerStyle << std::endl;
  std::cout << "make legend: " << info.makeLegend << std::endl;
  std::cout << std::endl;
}

void getInfoFromConfig(boost::property_tree::ptree& pt, std::vector<plotting::graphInfo>& grInfo, plotting::miscInfo& miscInfo)
{
  for (const auto& graph : pt.get_child("graphs")) {
    for (const auto& gr : graph.second) {
      grInfo.emplace_back(plotting::graphInfo());
      for (const auto& prop : gr.second) {
        if (prop.first == "active") {
          grInfo.back().active = getFromConfig<bool>(prop.second.data());
        } else if (prop.first == "column") {
          grInfo.back().column = getFromConfig<int>(prop.second.data());
        } else if (prop.first == "name") {
          grInfo.back().name = getFromConfig<std::string>(prop.second.data());
        } else if (prop.first == "color") {
          grInfo.back().color = getFromConfig<short>(prop.second.data());
        } else if (prop.first == "title") {
          grInfo.back().title = getFromConfig<std::string>(prop.second.data());
        } else if (prop.first == "fileNr") {
          grInfo.back().fileNr = getFromConfig<short>(prop.second.data());
        } else {
          std::cout << "Unexpected graph porperty found: " << prop.first << std::endl;
        }
      }
    }
  }

  miscInfo.xTitle = pt.get<std::string>("x_axis_title", "");
  miscInfo.yTitle = pt.get<std::string>("y_axis_title", "");
  miscInfo.legendHeader = pt.get<std::string>("legend_title", "");
  miscInfo.markerStyle = pt.get<int>("marker_style",20);
  miscInfo.makeLegend = pt.get<bool>("make_legend", true);
}

void makeGraphs(std::vector<plotting::graphInfo> info)
{

}

int main(int argc, char* argv[])
{
  //-------------------------------------------------------------------------
  // Reading command line options
  std::string json_file_name = "";
  std::string input_file_name = "";
  std::string output_file_name = "";
  bool debug = false;

  namespace po = boost::program_options;
  po::options_description opts("options");
  opts.add_options()
    ("help", "Print help mesasge")
    ("config_file", po::value<std::string>(&json_file_name),"Plotting configuration json file. (required)\nCheck ~/PhotonAnalysis/data/testing/plotConfig.json for reference.")
    ("input_file", po::value<std::string>(&input_file_name),"Data file(s). Can be a list of comma separated file paths. (required)\nThe files need to be in the following format:\nx-value tab x-error tab y-value of first graph (= column 0) tab y-error of first graph tab y-value of second graph (= column 1) etc")
    ("output_file", po::value<std::string>(&output_file_name),"Output name")
    ("debug", "Debug mode (verbosity)");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, opts), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << opts << std::endl;
    return 1;
  }

  if (vm.count("debug")) {
    debug = true;
  }

  if (!vm.count("output_file")) {
    output_file_name = input_file_name;
  }

  boost::property_tree::ptree pt;
  read_json(json_file_name, pt);
  //-------------------------------------------------------------------------

  beautify::setStyle();

  std::vector<plotting::graphInfo> grInfo;
  plotting::miscInfo miscInfos;

  // read the info from the json file
  // now we know which graphs to plot
  // this info is stored in 'grInfo'
  // the axis titles etc are stored in 'miscInfos'
  getInfoFromConfig(pt, grInfo, miscInfos);

  auto files = fileHandling::getFileNames(input_file_name, debug);
  auto output = fileHandling::splitString(output_file_name, delimiter::dot).front();

  TFile *fout = new TFile(output.append(".root").data(),"recreate");

  std::vector<std::vector<TGraphErrors*>> graphs;
  for (int i = 0; i<files.size(); i++) {
    graphs.emplace_back(std::vector<TGraphErrors*>{});
  }
  Drawer drawer;

  // create all graphs from all files
  // now we have all graphs available in 'graphs'
  int fileCounter = 0;
  for (const auto& file : files) {
     if (debug) std::cout << file << std::endl;
    std::vector<std::vector<float>> data = readCSVFile(file, debug);
    std::vector<TGraphErrors*> tmpGraphs;
    tmpGraphs = drawer.getGraphsAnyData(data, debug);
    graphs[fileCounter].insert(std::end(graphs[fileCounter]), std::begin(tmpGraphs), std::end(tmpGraphs));
    fileCounter++;
  }

  if (debug) {
    int nGraphs = 0;
    for (const auto& fileGr : graphs) {
      nGraphs += fileGr.size();
    }
      std::cout << "number of graphs found: " << nGraphs << std::endl;
  }

  // now we need to use the 'grInfo' to pick specific graphs
  std::map<TGraphErrors*, plotting::graphInfo> drawThese;

  for (auto& gr : grInfo) {
    if (!gr.active) continue;
    drawThese.insert(std::pair<TGraphErrors*, plotting::graphInfo>(graphs[gr.fileNr-1][gr.column], gr));
    gr.graph = graphs[gr.fileNr-1][gr.column];
  }

  if (debug) {
    for (auto& gr : grInfo) {
      printGraphInfo(gr);
    }
    printMiscInfo(miscInfos);
  }


  // set all options, prepare canvas and draw
  float maxY;
  float minY;
  float maxX;
  float minX;

  TCanvas* canvas = new TCanvas("c", "c", 1000, 1000);

  int grCounter = 0;
  for (auto& gr : grInfo) {
    if (!gr.active) continue;
    if (grCounter == 0) {
      maxY = TMath::MaxElement(gr.graph->GetN(),gr.graph->GetY());
      minY = TMath::MinElement(gr.graph->GetN(),gr.graph->GetY());
      maxX = TMath::MaxElement(gr.graph->GetN(),gr.graph->GetX());
      minX = TMath::MinElement(gr.graph->GetN(),gr.graph->GetX());
    }

    float maxy = TMath::MaxElement(gr.graph->GetN(),gr.graph->GetY());
    float miny = TMath::MinElement(gr.graph->GetN(),gr.graph->GetY());
    if (maxy > maxY) { maxY = maxy; }
    if (miny < minY) { minY = miny; }

    float maxx = TMath::MaxElement(gr.graph->GetN(),gr.graph->GetX());
    float minx = TMath::MinElement(gr.graph->GetN(),gr.graph->GetX());
    if (maxx > maxX) { maxX = maxx; }
    if (minx < minX) { minX = minx; }

    grCounter++;
  }

  fout->cd();
  gDirectory->mkdir("graphs");
  fout->cd("graphs");

  grCounter = 0;
  for (auto& gr : grInfo) {
    if (!gr.active) continue;
    gr.graph->SetMarkerStyle(miscInfos.markerStyle);
    gr.graph->SetMarkerColor(gr.color);
    beautify::setStyleGraph(gr.graph);
    gr.graph->GetYaxis()->SetLimits(beautify::getMinRange(minY), beautify::getMaxRange(maxY));
    gr.graph->GetYaxis()->SetRange(beautify::getMinRange(minY), beautify::getMaxRange(maxY));
    gr.graph->GetYaxis()->SetRangeUser(beautify::getMinRange(minY), beautify::getMaxRange(maxY));
    gr.graph->GetXaxis()->SetTitle(miscInfos.xTitle.c_str());
    gr.graph->GetYaxis()->SetTitle(miscInfos.yTitle.c_str());
    if (grCounter == 0) {
      gr.graph->Draw("APEZ");
    } else {
      gr.graph->Draw("PE same");
    }
    gr.graph->Write(gr.title.c_str(),TObject::kOverwrite);
    grCounter++;
  }

  // make the legend
  if (miscInfos.makeLegend) {
    TLegend *legend = new TLegend( 0.8, 0.7, 0.89, 0.89);
    if (miscInfos.legendHeader != std::string("")) { legend->SetHeader(miscInfos.legendHeader.c_str(),"C"); }
    for (auto& pair: drawThese) {
      legend->AddEntry(pair.first, pair.second.title.c_str(),"p");
    }

    legend->Draw("same");
  }

  fout->cd();
  canvas->Write("", TObject::kOverwrite);
  fout->Close();
}
