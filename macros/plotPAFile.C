#include <Rtypes.h>
#include <cstdlib>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

extern char* optarg;

void plotPAFile(std::string infile, std::string output = "readPAFile_output")
{
  std::string outputTMP = output;
  TFile *fout = new TFile(outputTMP.append(".root").c_str(),"recreate");
  std::vector<std::vector<float>> data = readPAFile(infile);

  Drawer drawer;

  std::vector<TGraphErrors*> graphs;

  for (int i=0; i<4; i++) { graphs.push_back(new TGraphErrors()); }
  beautify::setStyle();
  drawer.setDefaultMarker(graphs);
  drawer.setAxisLabels(graphs);

  drawer.getGraphsPAData(data, graphs);

  std::vector<std::string> nameVec = { std::string("I_stop"), std::string("I_wire"), std::string("I_GT"), std::string("I_anode") };
  TCanvas* c = drawer.drawGraphs(graphs, nameVec);

  fout->WriteObject(c, "canvas");
  for (int i = 0; i < graphs.size(); i++) {
    fout->WriteObject(graphs.at(i), nameVec.at(i).c_str());
  }
  fout->Close();

  c->Print(output.append(".pdf").c_str());

  return;
}

int main(int argc, char** argv)
{

  char inputFile[512] = "";
  char output[512]    = "";

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:o:")) != -1) {
    switch (argsforloop) {
      case '?':
        ///TODO: write usage function
        exit(EXIT_FAILURE);
      case 'i':
        strncpy(inputFile, optarg, 512);
        break;
      case 'o':
        strncpy(output, optarg, 512);
        break;
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning testReadPAFile%s\n\n",text::BOLD,text::RESET);
  
  plotPAFile(inputFile, output);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}