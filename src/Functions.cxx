#include "Functions.h"
#include "Utility.h"

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
std::vector<std::vector<float>> readPAFile(std::string infile, bool debug, bool hourFormat)
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
        columnCounter++;                                          if (debug) { printf("%s%s[DEBUG][Functions][readPAFile]%s%s Reading column: %i\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, columnCounter, text::RESET); }
        data.emplace_back(vector<float>());                       if (debug) { printf("%s%s[DEBUG][Functions][readPAFile]%s%s Token: %s\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, token.data(), text::RESET); }
        //printf("%s\n",token.c_str());
        if (columnCounter == 0) {
          data.at(columnCounter).push_back(convertTime(token, hourFormat));
        }
        else {
          data.at(columnCounter).push_back(atof(token.c_str()));
        }
      }
    }
  }

  for (int i=0; i<columnCounter; i++) {
    if (!data.at(0).size()==data.at(i).size()) {
      printf("Something went wrong while reading the data!\n");
      if (debug) { printf("%s%s[DEBUG][Functions][readPAFile]%s%s number of time stamps: %lu\t number of data points in column %i: %lu\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, data.at(0).size(), i, data.at(i).size(), text::RESET); }
      exit(1);
    }
  }

  return std::move(data);
}


//____________________________________________________________________________________
std::vector<std::vector<float>> readCSVFile(std::string infile, bool debug)
{
  using namespace std;

  vector<vector<float>> data;
  string line;
  ifstream inputFile;
  int columnCounter = -1;

  inputFile.open(infile);

  if (inputFile.is_open()) {
    int lineCounter = 0;
    while(getline(inputFile, line)) {
      istringstream iss(line);
      string token;
      columnCounter = -1;
      while(getline(iss, token, '\t')) {
        columnCounter++;                                          if (debug) { printf("%s%s[DEBUG][Functions][readCSVFile]%s%s Reading column: %i\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, columnCounter, text::RESET); }
        if (lineCounter == 0) {
          data.emplace_back(vector<float>());                       if (debug) { printf("%s%s[DEBUG][Functions][readCSVFile]%s%s Token: %s\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, token.data(), text::RESET); }
        }
        data.at(columnCounter).push_back(atof(token.c_str()));
      }
      lineCounter++;
    }
  }

  if (debug) { 
    int counter = 0;
    for (auto& vector : data) {
      printf("%s%s[DEBUG][Functions][readCSVFile]%s%s Column %i has size %lu.\n%s", text::BOLD, text::CYN, text::RESET, text::CYN, counter, vector.size(), text::RESET);
      counter++;
    }
  }

  for (int i=0; i<data.size(); i++) { // should data.size not rather be columnCounter?
    if (!data.at(0).size()==data.at(i).size()) {
      printf("%s%s[ERROR][Functions][readCSVFile]%s%s Something went wrong while reading the data!%s\n", text::BOLD, text::RED, text::RESET, text::RED, text::RESET);
      printf("%s%s[ERROR][Functions][readCSVFile]%s%s Data length in column %i should be %lu but is %lu.%s\n", text::BOLD, text::RED, text::RESET, text::RED, i, data.at(0).size(), data.at(i).size(), text::RESET);
      printf("%s%s[ERROR][Functions][readCSVFile]%s%s Number of columns in the data file is %lu.%s\n", text::BOLD, text::RED, text::RESET, text::RED, data.size(), text::RESET);
      exit(1);
    }
  }

  return std::move(data);
}


//____________________________________________________________________________________
float convertTime(std::string inTime, bool hours)
{
  std::string hour   = inTime.substr(4,2);
  std::string minute = inTime.substr(6,2);
  std::string second = inTime.substr(8);

  if (hours) { return (std::stod(hour)*3600 + std::stod(minute)*60 + std::stod(second))/float(3600); }
  else { return std::stod(hour)*3600 + std::stod(minute)*60 + std::stod(second); }
}