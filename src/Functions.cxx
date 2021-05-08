#include "Functions.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#include <utility>

//____________________________________________________________________________________
std::vector<std::vector<float>> readFile(std::string infile, bool tab, char delimiter)
{
  using namespace std;

  vector<vector<float>> data;
  string line;
  ifstream inputFile;
  int columnCounter = -1;

  char Delimiter;
  if (tab) { Delimiter = '\t'; }
  else { Delimiter = delimiter; }

  inputFile.open(infile);

  if (inputFile.is_open()) {
    while(getline(inputFile, line)) {
      istringstream iss(line);
      string token;
      columnCounter = -1;
      while(getline(iss, token, Delimiter)) {
        columnCounter++;
        data.emplace_back(vector<float>());
        //printf("%s\n",token.c_str());
        if (columnCounter == 0) {
          data.at(columnCounter).push_back(convertTime(token));
        }
        else {
          data.at(columnCounter).push_back(atof(token.c_str()));
        }
      }
    }
  }

  return std::move(data);
}


//____________________________________________________________________________________
std::vector<std::vector<float>> readPAFile(std::string infile)
{
  using namespace std;

  vector<vector<float>> data;
  string line;
  ifstream inputFile;
  int columnCounter = -1;

  inputFile.open(infile);

  if (inputFile.is_open()) {
    while(getline(inputFile, line)) {
      istringstream iss(line);
      string token;
      columnCounter = -1;
      while(getline(iss, token, '\t')) {
        columnCounter++;
        data.emplace_back(vector<float>());
        //printf("%s\n",token.c_str());
        if (columnCounter == 0) {
          data.at(columnCounter).push_back(convertTime(token));
        }
        else {
          data.at(columnCounter).push_back(atof(token.c_str()));
        }
      }
    }
  }

  if (!(data.at(0).size()==data.at(1).size() && data.at(0).size()==data.at(2).size()
     && data.at(0).size()==data.at(3).size() && data.at(0).size()==data.at(4).size()))
  {
    printf("Something went wrong while reading the data!\n");
    exit(1);
  }

  return std::move(data);
}


//____________________________________________________________________________________
float convertTime(std::string inTime)
{
  std::string hour   = inTime.substr(4,2);
  std::string minute = inTime.substr(6,2);
  std::string second = inTime.substr(8);

  return std::stod(hour)*3600 + std::stod(minute)*60 + std::stod(second);
}