#ifndef UTILITY_H
#define UTILITY_H

#include "TColor.h"

class TH1F;
class TGraphErrors;

std::vector<std::string> splitList(std::string& inString);

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

namespace beautify
{

  static const std::vector<short> colors {kBlue+2,
                                          kRed+1,
                                          kYellow+1,
                                          kGreen+2,
                                          kRed+3,
                                          kCyan+1,
                                          kMagenta,
                                          kOrange,
                                          kSpring,
                                          kTeal,
                                          kAzure,
                                          kViolet,
                                          kPink};

  void setStyle();

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

#endif