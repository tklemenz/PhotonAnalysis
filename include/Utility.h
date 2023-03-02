#ifndef UTILITY_H
#define UTILITY_H

#include <Rtypes.h>
#include "TColor.h"

#include <map>

#include <fmt/format.h>
#include <boost/lexical_cast.hpp>

class TH1F;
class TGraphErrors;

namespace fileHandling
{
  std::vector<std::string> splitList(std::string& inString);

  std::vector<std::string> splitString(std::string inString, const char* delimiter);

  std::vector<std::string> getFileNames(const TString& input, const bool debug = false);

  void stripString(std::string& inString, const char pattern);
}

namespace delimiter
{
  static constexpr const char* slash = "/";
  static constexpr const char* dot = ".";
  static constexpr const char* minus = "-";
  static constexpr const char* underscore = "_";
}

enum current {
  suck  = 0,
  stop  = 0,
  wire  = 1,
  gt    = 2,
  gb    = 2,
  anode = 3
};

enum current6ch {
  Suck  = 0,
  Stop  = 1,
  Wire  = 2,
  GT    = 3,
  GB    = 4,
  Anode = 5
};

enum channel {
  one   = 0,
  two   = 1,
  three = 2,
  four  = 3,
  five  = 4,
  six   = 5
};

namespace mapping {

  const std::map<std::string, current6ch> channelMap{
    {"I_aperture", current6ch::Suck},
    {"I_stop", current6ch::Stop},
    {"I_wire", current6ch::Wire},
    {"I_GT", current6ch::GT},
    {"I_GB", current6ch::GB},
    {"I_anode", current6ch::Anode}
  };

}

namespace plotting {

  struct graphInfo
  {
    TGraphErrors* graph = nullptr;
    bool active;
    int column;
    std::string name;
    std::string title;
    short color;
    short markerStyle;
    short fileNr;
    bool sum;
    bool ete;
  };

  struct ratioInfo
  {
    bool active;
    std::string numerator;
    std::string denominator;
    std::string yTitle;
    short color;
  };

  struct sumInfo
  {
    bool active;
    short color;
  };

  struct eteInfo
  {
    bool active;
    bool normalize;
    short color;
  };

  struct miscInfo
  {
    std::string xTitle;
    std::string yTitle;
    std::string legendHeader;
    bool makeLegend;
    int markerStyle;
    int markerSize;
    ratioInfo ratio;
    sumInfo sum;
    eteInfo ETE;
  };

}

namespace beautify
{

  static const std::vector<short> excelColors {kBlue+2,
                                               kRed+1,
                                               kGray+2,
                                               kYellow+1,
                                               kCyan+1,
                                               kGreen+2,
                                               kRed+3,
                                               kMagenta,
                                               kOrange,
                                               kSpring,
                                               kTeal,
                                               kAzure,
                                               kViolet,
                                               kPink};

  static const std::vector<short> niceColors {kGreen+2,
                                              kRed+1,
                                              kBlue+2,
                                              kCyan+1,
                                              kTeal+1,
                                              kGreen+2,
                                              kRed+3,
                                              kMagenta,
                                              kOrange,
                                              kSpring,
                                              kTeal,
                                              kAzure,
                                              kViolet,
                                              kPink};

/*
From TColor class reference:
kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
*/

  void setStyle(bool graypalette = false);

  void setStyleHisto(TH1F* histo);

  void setStyleGraph(TGraphErrors* gr);

  inline float getMaxRange(float val) { return val > 0 ? 1.2*val : 0.8*val; }

  inline float getMinRange(float val) { return val > 0 ? 0.8*val : 1.2*val; }
}

/// This namespace holds some useless text modifications for terminal output.
namespace text
{

  const char* const BLK   = "\e[30m";
  const char* const RED   = "\e[31m";
  const char* const GRN   = "\e[32m";
  const char* const YEL   = "\e[33m";
  const char* const BLU   = "\e[34m";
  const char* const MAG   = "\e[35m";
  const char* const CYN   = "\e[36m";
  const char* const GRY   = "\e[37m";
  const char* const LBLK  = "\e[90m";
  const char* const LRED  = "\e[91m";
  const char* const LGRN  = "\e[92m";
  const char* const LYEL  = "\e[93m";
  const char* const LBLU  = "\e[94m";
  const char* const LMAG  = "\e[95m";
  const char* const LCYN  = "\e[96m";
  const char* const WHT   = "\e[97m";
  const char* const RESET = "\e[0m";

  const char* const BOLD  = "\e[1m";
  const char* const ULINE = "\e[4m";
  const char* const BLINK = "\e[5m";

} // namespace text

template <typename T>
T getFromConfig(const std::string& param)
{
  T retVal{};

  if constexpr (std::is_same<int, T>::value) {
    retVal = std::stoi(param);
  } else if constexpr (std::is_same<float, T>::value) {
    retVal = std::stof(param);
  } else if constexpr (std::is_same<double, T>::value) {
    retVal = std::stod(param);
  } else if constexpr (std::is_same<short, T>::value) {
    retVal = boost::lexical_cast<short>(param);
  } else if constexpr (std::is_same<bool, T>::value) {
    if ((param == "true") || (param == "True") || (param == "TRUE") || (param == "1")) {
      retVal = true;
    } else if ((param == "false") || (param == "False") || (param == "FALSE") || (param == "0")) {
      retVal = false;
    } else {
    }
  } else if constexpr (std::is_same<std::string, T>::value) {
    retVal = param;
  } else {
  }
return retVal;
}

#endif