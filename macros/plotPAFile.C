#include <Rtypes.h>
#include <cstdlib>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

extern char* optarg;

const unsigned int NCHANNELS = 6;

void plotPAFile(std::string infiles, std::string output, const bool debug, const bool hourFormat)
{
  std::string outputTMP = fileHandling::splitString(output, delimiter::dot).front();
  TFile *fout = new TFile(outputTMP.append(".root").c_str(),"recreate");

  std::vector<std::vector<std::vector<float>>> dataVec;

  for (auto &infile : fileHandling::splitList(infiles)) {
    dataVec.emplace_back(readPAFile(infile, debug, hourFormat));
  }

  Drawer drawer;

  std::vector<std::vector<TGraphErrors*>> graphsVec;
  for (int i=0; i<dataVec.size(); i++) { 
    graphsVec.emplace_back(std::vector<TGraphErrors*>());
    for (int j=0; j<NCHANNELS; j++) { graphsVec.at(i).push_back(new TGraphErrors()); }
  }
  printf("Number of graphs: %lu\n", graphsVec.at(0).size());
  for (int i=0; i<dataVec.size(); i++) { drawer.getGraphsPAData(dataVec.at(i), graphsVec.at(i)); }
  beautify::setStyle();

  std::vector<TGraphErrors*> graphs;
     ///------------------------------------------------------------------------------------------------------------------
  /// pick the graphs you want to use here!!
  /// STANDARD!! Plot all 6 currents given within 2 files.
  //for (auto graph : graphsVec.at(0)) { graphs.emplace_back(std::move(graph)); }      // all of first file
                                                                                     // I_stop, I_wire, I_GT, I_anode
  //if (dataVec.size() > 1) {                                                          // 1 and 3 of second file
  //  graphs.emplace_back(std::move(graphsVec.at(1).at(current::suck)));               // I_suck
  //  graphs.emplace_back(std::move(graphsVec.at(1).at(current::GB)));                 // I_GB
  //}

  /*std::vector<std::string> nameVec = { std::string("I_stop"),
                                       std::string("I_wire"),
                                       std::string("I_GT"),
                                       std::string("I_anode"),
                                       std::string("I_suck"),
                                       std::string("I_GB") };*/

    graphs.emplace_back(std::move(graphsVec.at(0).at(channel::one)));
    graphs.emplace_back(std::move(graphsVec.at(0).at(channel::four)));

    std::vector<std::string> nameVec = { std::string("I_aperture"),
                                         std::string("I_diode")
                                         /*std::string("I_wire"),
                                         std::string("I_GT"),
                                         std::string("I_GB"),
                                         std::string("I_anode")*/ };

  drawer.setDefaultMarker(graphs);
  drawer.setAxisLabels(graphs);
  if (hourFormat) {
    for (auto& graph : graphs) {
      graph->GetXaxis()->SetTitle("time [h]");
    }
  }
  TCanvas* c = drawer.drawGraphs(graphs, nameVec);
  ///------------------------------------------------------------------------------------------------------------------*/

  ///--------------------------------------------------------
  /// plot I_GT from 5 different files
  /*graphs.emplace_back(std::move(graphsVec.at(0).at(channel::one)));
  graphs.emplace_back(std::move(graphsVec.at(0).at(channel::two)));
  graphs.emplace_back(std::move(graphsVec.at(1).at(channel::three)));
  graphs.emplace_back(std::move(graphsVec.at(1).at(channel::four)));
  graphs.emplace_back(std::move(graphsVec.at(1).at(channel::one)));
  graphs.emplace_back(std::move(graphsVec.at(1).at(channel::two)));

  std::vector<std::string> nameVec = { std::string("I_wire"),
                                       std::string("I_GT"),
                                       std::string("I_GB"),
                                       std::string("I_anode"),
                                       std::string("I_aperture"),
                                       std::string("I_stop") };

  drawer.setDefaultMarker(graphs);
  drawer.setAxisLabels(graphs, "time [s]", "I [nA]");
  TCanvas* c = drawer.drawGraphs(graphs, nameVec, std::string("#DeltaU_{THGEM}=190 V"));
  ///--------------------------------------------------------*/


  ///--------------------------------------------------------
  /*graphs.emplace_back(std::move(graphsVec.at(0).at(channel::one)));
  graphs.emplace_back(std::move(graphsVec.at(1).at(channel::two)));

  std::vector<std::string> nameVec = { std::string("BS 1: aperture"),
                                       std::string("wire plane only") };

  drawer.setDefaultMarker(graphs);
  drawer.setAxisLabels(graphs, "time [s]", "I_env [nA]", "Environmental current in different settings");
  TCanvas* c = drawer.drawGraphs(graphs, nameVec, std::string("U_electrode = -100V"));*/
  ///--------------------------------------------------------




  fout->WriteObject(c, "canvas");
  for (int i = 0; i < graphs.size(); i++) {
    fout->WriteObject(graphs.at(i), nameVec.at(i).c_str());
  }
  fout->Close();

  std::string pdf = fileHandling::splitString(output, delimiter::dot).front();
  c->Print(pdf.append(".pdf").c_str());


  return;
}

int main(int argc, char** argv)
{

  char inputFiles[512] = "";
  char output[512]     = "readPAFile_output.root";
  bool debug           = false;
  bool hourFormat      = false; // set true to change x axis title to 'time [h]' instead of 'time [s]'

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:o:d:f:")) != -1) {
    switch (argsforloop) {
      case '?':
        ///TODO: write usage function
        exit(EXIT_FAILURE);
      case 'i':
        strncpy(inputFiles, optarg, 512);
        break;
      case 'o':
        strncpy(output, optarg, 512);
        break;
      case 'd':
        if (std::atoi(optarg) > 0) { debug = true; }
        else if (std::atoi(optarg) == 0) { debug = false; }
        else { printf("Valid debug options: '1', '0' (default)"); }
        break;
      case 'f':
        if (std::atoi(optarg) > 0) { hourFormat = true; }
        else if (std::atoi(optarg) == 0) { hourFormat = false; }
        else { printf("Valid hourFormat options: '1', '0' (default)"); }
        break;
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning plotPAFile%s\n\n",text::BOLD,text::RESET);
  
  plotPAFile(inputFiles, output, debug, hourFormat);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}