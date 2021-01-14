#include <Rtypes.h>
#include <cstdlib>

#include "Functions.h"
#include "Utility.h"

extern char* optarg;

void testReadFile(std::string infile, bool tab, char delimiter)
{
  std::vector<std::vector<float>> data = readFile(infile, tab, delimiter);

  for (auto &vector : data) {
    printf("length: %lu\n", vector.size());
  }
}

int main(int argc, char** argv)
{
  char    inputFile[512]="";
  char    delimiter;
  bool    tab;
  ULong_t procNr=-1;

  int argsforloop;
  while ((argsforloop = getopt(argc, argv, "hi:t:d:")) != -1) {
    switch (argsforloop) {
      case '?':
        ///TODO: write usage function
        exit(EXIT_FAILURE);
      case 'i':
        strncpy(inputFile, optarg, 512);
        break;
      case 'd':
        delimiter = optarg[0];
        break;
      case 't':
        if (optarg[0] == '1') {
          tab = true;
        }
        else if (optarg[0] == '0') {
          tab = false;
        }
        else {
          printf("tab needs to be 0 or 1!\n");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        printf("\n\n%s%sdefault case%s\n\n",text::BOLD,text::RED,text::RESET);
        exit(EXIT_FAILURE);
    }
  }

  printf("\n\n%sRunning testReadFile%s\n\n",text::BOLD,text::RESET);
  
  testReadFile(inputFile, tab, delimiter);

  printf("\n\n%s%sDONE!%s\n\n",text::BOLD,text::GRN,text::RESET);
}