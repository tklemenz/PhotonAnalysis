#include "TCanvas.h"
#include "TH2.h"

#include <math.h>

const double pi = std::acos(-1);

void aperturePhYield()
{
  auto* hPhotonYield = new TH2F("hPhotonYield", "Photo current below aperture hole (vacuum);x [mm];y [mm]; I [nA]", 200,-10,10,200,-10,10);

  auto* hPhotonYieldGas = new TH2F("hPhotonYieldGas", "Photo current below aperture hole (ArCH4);x [mm];y [mm]; I [nA]", 200,-10,10,200,-10,10);

  hPhotonYield->SetStats(0);
  hPhotonYield->SetMinimum(5.4);
  hPhotonYield->SetMaximum(10.6);

  hPhotonYieldGas->SetStats(0);
  hPhotonYieldGas->SetMinimum(5.4);
  hPhotonYieldGas->SetMaximum(10.6);

  const double sinPi = std::sin(pi/4);

  std::vector<std::vector<double>> data = {
    {-1.1, -1.1, 10.4},   // center
    {-10, -1.1, 8.0},     // left
    {-1.1, 7.8, 6.9},     // top
    {7.8, -1.1, 10.39},   // right
    {-1.1, -10, 6.2},     // bottom
    {-5.5, -1.1, 7.3},    // left half way
    {3.3, -1.1, 6.4},     // right half way
    //{-1.1, 3.3, },      // top half way
    //{-5.5, -1.1, },     // bottom half way
    {-4.4*sinPi-1.1, -4.4*sinPi-1.1, 7.44},  // bottom left half way
    {-4.4*sinPi-1.1, 4.4*sinPi-1.1, 8.1},    // top left half way
    {4.4*sinPi-1.1, 4.4*sinPi-1.1, 7.49},    // top right half way
    {4.4*sinPi-1.1, -4.4*sinPi-1.1, 8.28},   // bottom right half way
    {-8.8*sinPi-1.1, -8.8*sinPi-1.1, 5.64},  // bottom left
    {-8.8*sinPi-1.1, 8.8*sinPi-1.1, 6.3},    // top left
    {8.8*sinPi-1.1, 8.8*sinPi-1.1, 6.3},     // top right
    {8.8*sinPi-1.1, -8.8*sinPi-1.1, 7.0}     // bottom right
  };

  for(const auto& point : data) {
    for(int dx=0; dx<22; dx++) {
      for(int dy=0; dy<22; dy++) {
        float x = point.at(0);
        float y = point.at(1);
        float value = point.at(2);
        hPhotonYield->Fill(x+(dx*0.1)+0.05, y+(dy*0.1)+0.05, value);
      }
    }
  }

  auto* canvas = new TCanvas();
  hPhotonYield->Draw("colz");


  std::vector<std::vector<double>> dataGas = {
    
  }
}