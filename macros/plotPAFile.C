#include <Rtypes.h>
#include <cstdlib>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

extern char* optarg;

void plotPAFile(std::string infiles, std::string output = "readPAFile_output")
{
  std::string outputTMP = output;
  TFile *fout = new TFile(outputTMP.append(".root").c_str(),"recreate");

  std::vector<std::vector<std::vector<float>>> dataVec;

  for (auto &infile : splitList(infiles)) {
    dataVec.emplace_back(readPAFile(infile));
  }

  Drawer drawer;

  std::vector<std::vector<TGraphErrors*>> graphsVec;
  for (int i=0; i<dataVec.size(); i++) { 
    graphsVec.emplace_back(std::vector<TGraphErrors*>());
    for (int j=0; j<4; j++) { graphsVec.at(i).push_back(new TGraphErrors()); }
  }
  for (int i=0; i<dataVec.size(); i++) { drawer.getGraphsPAData(dataVec.at(i), graphsVec.at(i)); }
  beautify::setStyle();

  std::vector<TGraphErrors*> graphs;
     ///------------------------------------------------------------------------------------------------------------------
  /// pick the graphs you want to use here!!
  /// STANDARD!! Plot all 6 currents given within 2 files.
  for (auto graph : graphsVec.at(0)) { graphs.emplace_back(std::move(graph)); }      // all of first file
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

    std::vector<std::string> nameVec = { std::string("I_wire"),
                                         std::string("I_GT"),
                                         std::string("I_GB"),
                                         std::string("I_anode") };

  drawer.setDefaultMarker(graphs);
  drawer.setAxisLabels(graphs);
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

  std::string pdf = output;
  c->Print(pdf.append(".pdf").c_str());


  return;
}

int main(int argc, char** argv)
{

  char inputFiles[512] = "";
  char output[512]    = "";

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:o:")) != -1) {
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
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning plotPAFile%s\n\n",text::BOLD,text::RESET);
  
  plotPAFile(inputFiles, output);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}