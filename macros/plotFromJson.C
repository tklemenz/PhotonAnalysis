#include <Rtypes.h>
#include <cstdlib>
#include <TMath.h>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TStyle.h"

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
  printf("%s%s[DEBUG][Macro][printGraphInfo]%s%s\n", text::BOLD, text::CYN, text::RESET, text::CYN);
  std::cout << "name: " << info.name << std::endl;
  std::cout << "active: " << info.active << std::endl;
  std::cout << "column: " << info.column << std::endl;
  std::cout << "color: " << info.color << std::endl;
  std::cout << "title: " << info.title << std::endl;
  std::cout << "from file: " << info.fileNr << std::endl;
  std::cout << "include in sum plot: " << info.sum << std::endl;
  std::cout << "part of ete calc: " << info.ete << std::endl;
  std::cout << "graph address: " << info.graph << std::endl;
  std::cout << std::endl;
  printf("%s", text::RESET);
}

void printMiscInfo(plotting::miscInfo& info)
{
  printf("%s%s[DEBUG][Macro][printMiscInfo]%s%s\n", text::BOLD, text::CYN, text::RESET, text::CYN);
  std::cout << "x axis title: " << info.xTitle << std::endl;
  std::cout << "y axis title: " << info.yTitle << std::endl;
  std::cout << "legend header: " << info.legendHeader << std::endl;
  std::cout << "marker style: " << info.markerStyle << std::endl;
  std::cout << "marker size: " << info.markerSize << std::endl;
  std::cout << "make legend: " << info.makeLegend << std::endl;
  std::cout << "--ratio info--" << std::endl;
  std::cout << "make ratio: " << info.ratio.active << std::endl;
  std::cout << "numerator: " << info.ratio.numerator << std::endl;
  std::cout << "denominator: " << info.ratio.denominator << std::endl;
  std::cout << "y axis title: " << info.ratio.yTitle << std::endl;
  std::cout << "color: " << info.ratio.color << std::endl;
  std::cout << "--sum info--" << std::endl;
  std::cout << "make sum: " << info.sum.active << std::endl;
  std::cout << "color: " << info.sum.color << std::endl;
  std::cout << "--ETE info--" << std::endl;
  std::cout << "make ETE: " << info.ETE.active << std::endl;
  std::cout << "normalize: " << info.ETE.normalize << std::endl;
  std::cout << "color: " << info.ETE.color << std::endl;
  std::cout << std::endl;
  printf("%s", text::RESET);
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
        } else if (prop.first == "markerStyle") {
          grInfo.back().markerStyle = getFromConfig<short>(prop.second.data());
        } else if (prop.first == "fileNr") {
          grInfo.back().fileNr = getFromConfig<short>(prop.second.data());
        } else if (prop.first == "sum") {
          grInfo.back().sum = getFromConfig<bool>(prop.second.data());
        } else if (prop.first == "ete") {
          grInfo.back().ete = getFromConfig<bool>(prop.second.data());
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
  miscInfo.markerSize = pt.get<int>("marker_size",2);
  miscInfo.makeLegend = pt.get<bool>("make_legend", true);
  miscInfo.ratio.active = pt.get<bool>("make_ratio.active", false);
  miscInfo.ratio.numerator = pt.get<std::string>("make_ratio.numerator", "");
  miscInfo.ratio.denominator = pt.get<std::string>("make_ratio.denominator", "");
  miscInfo.ratio.color = pt.get<short>("make_ratio.color", 600);
  miscInfo.ratio.yTitle = pt.get<std::string>("make_ratio.y_axis_title", "");
  miscInfo.sum.active = pt.get<bool>("make_sum.active", false);
  miscInfo.sum.color = pt.get<short>("make_sum.color", 600);
  miscInfo.ETE.active = pt.get<bool>("make_ETE.active", false);
  miscInfo.ETE.normalize = pt.get<bool>("make_ETE.normalize", true);
  miscInfo.ETE.color = pt.get<short>("make_ETE.color", 600);

  if ((miscInfo.ratio.numerator == std::string("")) || (miscInfo.ratio.denominator == std::string(""))) {
    miscInfo.ratio.active = false;
    std::cout << "Ratio graph numerator or denominator was not set!" << std:: endl;
  }
}

int main(int argc, char* argv[])
{
  //-------------------------------------------------------------------------
  // Reading command line options
  std::string json_file_name = "";
  std::string input_file_name = "";
  std::string output_file_name = "";
  unsigned int dataType = 0;
  bool debug = false;

  namespace po = boost::program_options;
  po::options_description opts("options");
  opts.add_options()
    ("help", "Print help mesasge")
    ("config_file", po::value<std::string>(&json_file_name),"Plotting configuration json file. (required)\nCheck ~/PhotonAnalysis/data/testing/plotConfig.json for reference.")
    ("input_file", po::value<std::string>(&input_file_name),"Data file(s). Can be a list of comma separated file paths. (required)")
    ("output_file", po::value<std::string>(&output_file_name),"Output name")
    ("input_format", po::value<unsigned int>(&dataType), "Format of the data input.\n0: x-value tab x-error tab y-value of first graph (= column 0) tab y-error of first graph tab y-value of second graph (= column 1) etc\n\n1: x1 tab y1 tab y2 ... y1-err tab y2-err...")
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
    tmpGraphs = drawer.getGraphsAnyData(data, dataType, debug);
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
  for (auto& gr : grInfo) {
    if (!gr.active) continue;
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

  gStyle->SetLabelFont(43, "xy");
  gStyle->SetTitleFont(43, "xy");

  //-----------------------------------------------------------------------------------
  // make ratio plot in case miscInfos.ratio.active
  if (miscInfos.ratio.active) {
    drawer.divideCanvas(canvas);
    canvas->cd(2);
    TGraphErrors* numerator = nullptr;
    TGraphErrors* denominator = nullptr;

    for (auto& gr : grInfo) {
      if (gr.name == miscInfos.ratio.numerator) {
        numerator = gr.graph;
      }
      if (gr.name == miscInfos.ratio.denominator) {
        denominator = gr.graph;
      }
    }

    auto ratioGr = drawer.getRatioGraph(numerator, denominator, debug);
    beautify::setStyleGraph(ratioGr);
    ratioGr->SetMarkerColor(miscInfos.ratio.color);
    ratioGr->SetMarkerStyle(miscInfos.markerStyle);
    ratioGr->GetXaxis()->SetTitle(miscInfos.xTitle.c_str());
    ratioGr->GetYaxis()->SetTitle(miscInfos.ratio.yTitle.c_str());
    ratioGr->GetYaxis()->SetNdivisions(403, true);
    ratioGr->GetXaxis()->SetLabelSize(20);
    ratioGr->GetXaxis()->SetTitleSize(22);
    ratioGr->GetYaxis()->SetLabelSize(20);
    ratioGr->GetYaxis()->SetTitleSize(22);
    ratioGr->GetXaxis()->SetTitleOffset(1.25);
    ratioGr->GetYaxis()->SetTitleOffset(0.4);

    canvas->cd(2);
    ratioGr->Draw("APEZ");
    canvas->cd(1);

    fout->cd();
    gDirectory->mkdir("ratio_plot");
    fout->cd("ratio_plot");
    ratioGr->Write(fmt::format("ratio_{}/{}", miscInfos.ratio.numerator, miscInfos.ratio.denominator).data(), TObject::kOverwrite);
  }
  //-----------------------------------------------------------------------------------

  fout->cd();
  gDirectory->mkdir("graphs");
  fout->cd("graphs");

  grCounter = 0;
  for (auto& gr : grInfo) {
    if (!gr.active) continue;
    //gr.graph->SetMarkerStyle(miscInfos.markerStyle);
    gr.graph->SetMarkerStyle(gr.markerStyle);
    gr.graph->SetMarkerColor(gr.color);
    gr.graph->SetMarkerSize(miscInfos.markerSize);
    beautify::setStyleGraph(gr.graph);
    //gr.graph->SetMaximum(beautify::getMaxRange(maxY));
    //gr.graph->SetMinimum(beautify::getMinRange(minY));
    //gr.graph->SetMaximum(3000);
    //gr.graph->SetMinimum(0);
    gr.graph->GetXaxis()->SetTitle(miscInfos.xTitle.c_str());
    gr.graph->GetYaxis()->SetTitle(miscInfos.yTitle.c_str());
    gr.graph->GetXaxis()->SetLabelSize(0.03);
    gr.graph->GetXaxis()->SetTitleSize(0.04);
    gr.graph->GetXaxis()->SetTitleOffset(1.4);
    gr.graph->GetYaxis()->SetLabelSize(0.03);
    gr.graph->GetYaxis()->SetTitleSize(0.04);
    gr.graph->GetYaxis()->SetTitleOffset(1);
    if (grCounter == 0) {
      gr.graph->Draw("APEZ");
    } else {
      gr.graph->Draw("PE same");
    }
    gr.graph->GetXaxis()->SetRangeUser(-50,950);
    gr.graph->GetYaxis()->SetRangeUser(0.001,6000);
    canvas->Update();
    gr.graph->Write(gr.title.c_str(),TObject::kOverwrite);
    grCounter++;
  }

  // make the legend
  if (miscInfos.makeLegend) {
    TLegend *legend = new TLegend(0.8, 0.7, 0.89, 0.89);
    if (miscInfos.legendHeader != std::string("")) { legend->SetHeader(miscInfos.legendHeader.c_str(),"C"); }
    legend->SetTextSize(2);
    for (auto& gr: grInfo) {
      if (!gr.active) continue;
      //legend->AddEntry(gr.graph, gr.title.c_str(),"p");
    }
    legend->AddEntry(grInfo.at(1).graph, grInfo.at(1).title.c_str(),"p");
    legend->AddEntry(grInfo.at(2).graph, grInfo.at(2).title.c_str(),"p");
    legend->AddEntry(grInfo.at(3).graph, grInfo.at(3).title.c_str(),"p");
    legend->AddEntry(grInfo.at(4).graph, grInfo.at(4).title.c_str(),"p");
    legend->AddEntry(grInfo.at(0).graph, grInfo.at(0).title.c_str(),"p");
    legend->AddEntry(grInfo.at(5).graph, grInfo.at(5).title.c_str(),"p");
    legend->Draw("same");
  }
  fout->cd();
  canvas->Write("", TObject::kOverwrite);

  // make sum plot
  if (miscInfos.sum.active) {
    std::vector<TGraphErrors*> forSum;
    std::string grName;

    for (const auto& info : grInfo) {
      if (!info.active || !info.sum) continue;
      grName += fmt::format("_{}",info.name);
      forSum.emplace_back(info.graph);
    }

    auto grSum = drawer.getSumGraph(forSum, debug);
    beautify::setStyleGraph(grSum);
    grSum->GetXaxis()->SetTitle(miscInfos.xTitle.c_str());
    grSum->GetYaxis()->SetTitle(fmt::format("Sum_{} (nA)", grName).data());
    grSum->SetMarkerStyle(miscInfos.markerStyle);
    grSum->SetMarkerColor(miscInfos.sum.color);

    auto cSum = new TCanvas("c_sum",fmt::format("Sum of {}", grName).data(),1000,1000);
    cSum->cd();
    grSum->Draw("APEZ");
    fout->cd();
    gDirectory->mkdir("sum");
    fout->cd("sum");
    grSum->Write(fmt::format("gr_sum_{}", grName).data(), TObject::kOverwrite);
    cSum->Write("", TObject::kOverwrite);
  }

  //make ETE plot
  if (miscInfos.ETE.active) {
    TGraphErrors* gt = new TGraphErrors();
    TGraphErrors* gb = new TGraphErrors();
    for (const auto& gr : grInfo) {
      if (gr.ete && (gr.name == std::string("I_GT"))) {
        gt = gr.graph;
      }
      if (gr.ete && (gr.name == "I_GB")) {
        gb = gr.graph;
      }
    }

    auto grETE = drawer.getETEGraph(gt, gb, miscInfos.ETE.normalize, debug);
    beautify::setStyleGraph(grETE);
    grETE->GetXaxis()->SetTitle(miscInfos.xTitle.c_str());
    grETE->GetYaxis()->SetTitle("ETE");
    grETE->SetMarkerStyle(miscInfos.markerStyle);
    grETE->SetMarkerColor(miscInfos.ETE.color);

    auto cETE = new TCanvas("c_ETE","Electron Transfer Efficiency",1000,1000);
    cETE->cd();
    grETE->Draw("APEZ");
    fout->cd();
    gDirectory->mkdir("ETE");
    fout->cd("ETE");
    grETE->Write("gr_ETE", TObject::kOverwrite);
    cETE->Write("", TObject::kOverwrite);
  }

  fout->Close();
}
