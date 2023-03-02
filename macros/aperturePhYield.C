#include "TCanvas.h"
#include "TH2.h"

#include <math.h>

const double pi = std::acos(-1);

void aperturePhYield()
{
  auto* hPhotonYield = new TH2F("hPhotonYield", "Photo current below aperture hole (vacuum);x [mm];y [mm]; I [nA]", 200,-10,10,200,-10,10);

  auto* hPhotonYieldGas = new TH2F("hPhotonYieldGas", "Photo current below aperture hole (ArCH4);x [mm];y [mm]; I [nA]", 200,-10,10,200,-10,10);

  hPhotonYield->SetStats(0);
  hPhotonYield->SetMinimum(2.03);
  hPhotonYield->SetMaximum(2.54);

  hPhotonYieldGas->SetStats(0);
  hPhotonYieldGas->SetMinimum(5.4);
  hPhotonYieldGas->SetMaximum(10.6);

  const double sinPi = std::sin(pi/4);

  std::vector<std::vector<double>> data = {
    /*
    // Not normalized to first diode current measurement
    {-1.1, -1.1, 2.05},   // center
    {-10, -1.1, 2.04},     // left
    {-1.1, 7.8, 2.06},     // top
    {7.8, -1.1, 1.92},   // right
    {-1.1, -10, 2.06},     // bottom
    {-5.5, -1.1, 1.65},    // left half way
    {3.3, -1.1, 1.8},     // right half way
    //{-1.1, 3.3, },      // top half way
    //{-5.5, -1.1, },     // bottom half way
    {-4.4*sinPi-1.1, -4.4*sinPi-1.1, 1.79},  // bottom left half way
    {-4.4*sinPi-1.1, 4.4*sinPi-1.1, 1.74},    // top left half way
    {4.4*sinPi-1.1, 4.4*sinPi-1.1, 1.88},    // top right half way
    {4.4*sinPi-1.1, -4.4*sinPi-1.1, 1.82},   // bottom right half way
    {-8.8*sinPi-1.1, -8.8*sinPi-1.1, 2.02},  // bottom left
    {-8.8*sinPi-1.1, 8.8*sinPi-1.1, 2.12},    // top left
    {8.8*sinPi-1.1, 8.8*sinPi-1.1, 1.99},     // top right
    {8.8*sinPi-1.1, -8.8*sinPi-1.1, 1.76}     // bottom right*/

    {-1.1, -1.1, 2.05},   // center
    {-10, -1.1, 2.18},     // left
    {-1.1, 7.8, 2.15},     // top
    {7.8, -1.1, 2.09},   // right
    {-1.1, -10, 2.1},     // bottom
    {-5.5, -1.1, 2.24},    // left half way
    {3.3, -1.1, 2.44},     // right half way
    //{-1.1, 3.3, },      // top half way
    //{-5.5, -1.1, },     // bottom half way
    {-4.4*sinPi-1.1, -4.4*sinPi-1.1, 2.29},  // bottom left half way
    {-4.4*sinPi-1.1, 4.4*sinPi-1.1, 2.26},    // top left half way
    {4.4*sinPi-1.1, 4.4*sinPi-1.1, 2.34},    // top right half way
    {4.4*sinPi-1.1, -4.4*sinPi-1.1, 2.39},   // bottom right half way
    {-8.8*sinPi-1.1, -8.8*sinPi-1.1, 2.46},  // bottom left
    {-8.8*sinPi-1.1, 8.8*sinPi-1.1, 2.23},    // top left
    {8.8*sinPi-1.1, 8.8*sinPi-1.1, 2.31},     // top right
    {8.8*sinPi-1.1, -8.8*sinPi-1.1, 2.1}     // bottom right
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


//  std::vector<std::vector<double>> dataGas = {
//    
//  }
}