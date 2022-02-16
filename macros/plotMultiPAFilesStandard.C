#include <Rtypes.h>
#include <cstdlib>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

extern char* optarg;

void plotPAFile6chReadout(const TString inputFiles)
{
  auto files = fileHandling::getFileNames(inputFiles);
  printf("\n%sFiles to be processed: %lu%s\n", text::BOLD, files.size(), text::RESET);
  for(auto& file : files) {
    printf("%s\n", file.data());
  }
  printf("\n");

  for (auto& file : files) {
    printf("Processing file %s...\n", file.data());
    std::string output = fileHandling::splitString(fileHandling::splitString(file, delimiter::slash).back(), delimiter::dot).front();

    TFile *fout = new TFile(output.append(".root").c_str(),"recreate");
    output = fileHandling::splitString(output, delimiter::dot).front();

    std::vector<std::vector<float>> data = readPAFile(file);

    Drawer drawer;
    std::vector<TGraphErrors*> graphsVec;

    for (int j=0; j<24; j++) { graphsVec.emplace_back(new TGraphErrors()); }
    drawer.getGraphsPAData(data, graphsVec);
    beautify::setStyle();

    std::vector<TGraphErrors*> graphs;
    ///------------------------------------------------------------------------------------------------------------------
    graphs.emplace_back(std::move(graphsVec.at(current6ch::Suck)));
    graphs.emplace_back(std::move(graphsVec.at(current6ch::Stop)));
    graphs.emplace_back(std::move(graphsVec.at(current6ch::Wire)));
    graphs.emplace_back(std::move(graphsVec.at(current6ch::GT)));
    graphs.emplace_back(std::move(graphsVec.at(current6ch::GB)));
    graphs.emplace_back(std::move(graphsVec.at(current6ch::Anode)));

    std::vector<std::string> nameVec = { std::string("I_aperture"),
                                         std::string("I_stop"),
                                         std::string("I_wire"),
                                         std::string("I_GT"),
                                         std::string("I_GB"),
                                         std::string("I_anode") };
    ///------------------------------------------------------------------------------------------------------------------

    drawer.setDefaultMarker(graphs);
    drawer.setAxisLabels(graphs, "time [s]");

    TCanvas* c = drawer.drawGraphs(graphs, nameVec);
    fout->WriteObject(c, "canvas");
    for (int i = 0; i < graphs.size(); i++) {
      fout->WriteObject(graphs.at(i), nameVec.at(i).c_str());
    }
    fout->Close();

    c->Print(output.append(".pdf").c_str());
    output = fileHandling::splitString(output, delimiter::dot).front();
  }
  return;
}

int main(int argc, char** argv)
{

  char inputFiles[512] = "";
  char outputFile[512] = "readPAFile_output";

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:o:")) != -1) {
    switch (argsforloop) {
      case '?':
        ///TODO: write usage function
        exit(EXIT_FAILURE);
      case 'i':
        strncpy(inputFiles, optarg, 512);
        break;
      /*case 'o':
        strncpy(output, optarg, 512);
        break;*/
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning plotPAFile6chReadout%s\n\n",text::BOLD,text::RESET);
  
  plotPAFile6chReadout(inputFiles);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}