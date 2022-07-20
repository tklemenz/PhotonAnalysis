#include <Rtypes.h>
#include <cstdlib>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

#include "Functions.h"
#include "Utility.h"
#include "Drawer.h"

#include "fmt/format.h"
#include <boost/property_tree/info_parser.hpp>
#include <boost/program_options.hpp>

extern char* optarg;

void plotFromCSV(const TString inputFiles, std::string xLabel, std::string yLabel, std::string legendEntries, std::string title, std::string outputFile, const bool debug)
{
  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Starting in DEBUG mode!\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }

  auto files = fileHandling::getFileNames(inputFiles, debug);                                                              if (debug) { printf("%s%s[DEBUG][Macro]%s%s Number of files to process: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, files.size(), text::RESET); }
  auto legend = fileHandling::splitString(legendEntries, delimiter::slash);                                                if (debug) { printf("%s%s[DEBUG][Macro]%s%s Number of legend entries given: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, legend.size(), text::RESET); }
  auto output = fileHandling::splitString(outputFile, delimiter::dot).front();                                             if (debug) { printf("%s%s[DEBUG][Macro]%s%s Output file name: %s\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, output.data(), text::RESET); }

  TFile *fout = new TFile(output.append(".root").data(),"recreate");

  std::vector<TGraphErrors*> graphs;
  Drawer drawer;

  if (debug) { 
    printf("%s%s[DEBUG][Macro]%s%s Reading data from %lu files...\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, files.size(), text::RESET);
    for (auto& file : files) {
      printf("%s%s[DEBUG][Macro]%s%s %s\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, file.data(), text::RESET);
    }
  }
  for (auto& file : files) {
    std::vector<std::vector<float>> data = readCSVFile(file, debug);
    std::vector<TGraphErrors*> tmpGraphs;
    tmpGraphs = drawer.getGraphsAnyData(data, debug);
    graphs.insert(std::end(graphs), std::begin(tmpGraphs), std::end(tmpGraphs));
  }

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Number of graphs filled: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, graphs.size(), text::RESET); }

  beautify::setStyle(false);

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Set marker\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  drawer.setDefaultMarker(graphs);

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Set axis label\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  drawer.setAxisLabels(graphs, xLabel, yLabel);

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Call drawer.drawGraphs\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  TCanvas* c = drawer.drawGraphs(graphs, legend, title);
  //c->SetLogy();

  fout->WriteObject(c, "canvas");

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Write canvas to file\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  for (int i = 0; i < graphs.size(); i++) {
    if (legend.size() > 1) {
      fout->WriteObject(graphs.at(i), legend.at(i).c_str());
    }
    else {
      fout->WriteObject(graphs.at(i), graphs.at(i)->GetName());
    }
  }
  fout->Close();

  if (debug) { printf("%s%s[DEBUG][Macro]%s%s Create pdf\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, text::RESET); }
  output = fileHandling::splitString(output, delimiter::dot).front();
  c->Print(output.append(".pdf").c_str());

  return;
}

int main(int argc, char** argv)
{

  char inputFiles[512] = "";
  char xLabel[512]     = "x";
  char yLabel[512]     = "y";
  char outputFile[512] = "plotSpecific_output";
  char legend[512]     = "";
  char title[512]      = "Default";
  bool debug     = false;

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:o:x:y:l:t:d:")) != -1) {
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
      case 'x':
        strncpy(xLabel, optarg, 512);
        break;
      case 'y':
        strncpy(yLabel, optarg, 512);
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

  printf("\n\n%sRunning plotFromCSV%s\n\n",text::BOLD,text::RESET);
  
  plotFromCSV(inputFiles, xLabel, yLabel, legend, title, outputFile, debug);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}