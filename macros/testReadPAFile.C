#include <Rtypes.h>
#include <cstdlib>

//#include "TApplication.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

extern char* optarg;

void testReadPAFile(std::string infile)
{
  TFile *fout = new TFile("testReadPAFile_output.root","recreate");
  std::vector<std::vector<float>> data = readPAFile(infile);

  Drawer drawer;

  std::vector<TGraphErrors*> graphs;

  for (int i=0; i<4; i++) { graphs.push_back(new TGraphErrors()); }

  drawer.getGraphsPAData(data, graphs);

  TCanvas* c = drawer.drawGraphs(graphs);
  fout->WriteObject(c, "canvas");
  fout->Close();
}

int main(int argc, char** argv)
{

  //TApplication* theApp = new TApplication("App", 0, 0);
  char    inputFile[512]="";

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:")) != -1) {
    switch (argsforloop) {
      case '?':
        ///TODO: write usage function
        exit(EXIT_FAILURE);
      case 'i':
        strncpy(inputFile, optarg, 512);
        break;
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning testReadPAFile%s\n\n",text::BOLD,text::RESET);
  
  testReadPAFile(inputFile);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}