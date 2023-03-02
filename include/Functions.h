#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

/// The readFile function reads a text file containing columns of data
/// and returns a vector of vectors containing the data.
/// @todo This is not implemented properly!
std::vector<std::vector<float>> readFile(std::string infile, bool tab = true, char delimiter = ' ');


/// The readPAFile function reads a file from the PicoLogic picoampere meter
/// and returns a vector of vectors containing the data.
std::vector<std::vector<float>> readPAFile(std::string infile, bool debug = false, bool hourFormat = false);

std::vector<std::vector<float>> readCSVFile(std::string infile, bool debug = false);


/// Time converter from picologic format monthdayhhmmss to seconds e.g. 1209184110.795..... = 9 Dec 18:41 and 10.xy seconds
float convertTime(std::string inTime, bool hours = false);

#endif