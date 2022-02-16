#include <Rtypes.h>
#include <cstdlib>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

extern char* optarg;

void plotSpecific(const TString inputFiles, const std::string current, std::string legendEntries, std::string title, std::string outputFile, const bool debug)
{
  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Starting in DEBUG mode!\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }

  auto files = fileHandling::getFileNames(inputFiles);                                                                     if (debug) { printf("%s%s[DEBUG][Macro]%s%s Number of files to process: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, files.size(), text::RESET); }
  auto legend = fileHandling::splitString(legendEntries, delimiter::slash);                                                if (debug) { printf("%s%s[DEBUG][Macro]%s%s Number of legend entries given: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, legend.size(), text::RESET); }

  if (files.size() != legend.size()) {
    printf("\n%s%sGive the correct number of legend entries.\n%s", text::BOLD, text::RED, text::RESET);
    printf("number of files: %lu\nnumber of legend entries: %lu\n", files.size(), legend.size());
    return;
  }

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Checking observable...\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  auto observable = mapping::channelMap.at(current);

  if (debug) {
    switch(observable) {
      case current6ch::Suck:
        printf("%s%s[DEBUG][Macro]%s%s Observable is I_aperture\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET);
        break;
      case current6ch::Stop:
        printf("%s%s[DEBUG][Macro]%s%s Observable is I_stop\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET);
        break;
      case current6ch::Wire:
        printf("%s%s[DEBUG][Macro]%s%s Observable is I_wire\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET);
        break;
      case current6ch::GT:
        printf("%s%s[DEBUG][Macro]%s%s Observable is I_GT\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET);
        break;
      case current6ch::GB:
        printf("%s%s[DEBUG][Macro]%s%s Observable is I_GB\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET);
        break;
      case current6ch::Anode:
        printf("%s%s[DEBUG][Macro]%s%s Observable is I_anode\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET);
        break;
      default:
        exit(EXIT_FAILURE);
    }
  }

  auto output = fileHandling::splitString(outputFile, delimiter::dot).front();                                             if (debug) { printf("%s%s[DEBUG][Macro]%s%s Output file name: %s\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, output.data(), text::RESET); }

  TFile *fout = new TFile(output.append(".root").data(),"recreate");

  std::vector<TGraphErrors*> graphs;
  Drawer drawer;

  for (auto& file : files) {
    std::vector<std::vector<float>> data = readPAFile(file);
    graphs.emplace_back(drawer.getSpecificGraph(data, observable/*, true*/));   // can be called with bool for debug as third argument
  }

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Number of graphs filled: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, graphs.size(), text::RESET); }

  beautify::setStyle();

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Set marker\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  drawer.setDefaultMarker(graphs);
  drawer.setAxisLabels(graphs, "time [s]");

  TCanvas* c = drawer.drawGraphs(graphs, legend, title);

  fout->WriteObject(c, "canvas");
  for (int i = 0; i < graphs.size(); i++) {
    fout->WriteObject(graphs.at(i), legend.at(i).c_str());
  }
  fout->Close();

  output = fileHandling::splitString(output, delimiter::dot).front();
  c->Print(output.append(".pdf").c_str());

  return;
}

int main(int argc, char** argv)
{

  char inputFiles[512] = "";
  char outputFile[512] = "plotSpecific_output";
  char current[512]    = "I_GT";
  char legend[512]     = "";
  char title[512]      = "Default";
  bool debug     = false;

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:o:c:l:t:d:")) != -1) {
    switch (argsforloop) {
      case '?':
        ///TODO: write usage function
        exit(EXIT_FAILURE);
      case 'i':
        strncpy(inputFiles, optarg, 512);
        break;
      case 'o':
        strncpy(outputFile, optarg, 512);
        break;
      case 'c':
        strncpy(current, optarg, 512);
        break;
      case 'l':
        strncpy(legend, optarg, 512);
        break;
      case 't':
        strncpy(title, optarg, 512);
        break;
      case 'd':
        if (std::atoi(optarg) > 0) { debug = true; }
        else if (std::atoi(optarg) == 0) { debug = false; }
        else { printf("Valid debug options: '1', '0' (default)"); }
        break;
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning plotSpecific%s\n\n",text::BOLD,text::RESET);
  
  plotSpecific(inputFiles, current, legend, title, outputFile, debug);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}