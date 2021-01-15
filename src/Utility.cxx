#include "Utility.h"

#include "TH1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TROOT.h"

namespace beautify
{

//________________________________________________________________________________
void setStyle()
{
  const Int_t NCont=255;
  TStyle *st = new TStyle("goodstyle","goodstyle");
  gROOT->GetStyle("Plain")->Copy((*st));
  st->SetStatX(.9);
  st->SetStatY(.9);
  st->SetNumberContours(NCont);
  st->SetPalette(1,0);
  st->SetOptFit(0);
  st->SetOptStat(0);
  st->SetGridColor(kGray+1);
  st->SetPadTickX(kTRUE);
  st->SetPadTickY(kTRUE);
  st->SetPadLeftMargin(0.12);   //mine alternative
  st->SetPadBottomMargin(0.12); //mine alternative
  st->SetPadRightMargin(0.04);
  st->SetPadTopMargin(0.06);
  st->SetHistLineColor(kBlue+2);
  st->cd();

  Int_t nimTPCFont=42;          // 42 for a non-bold font
  st->SetTextFont(nimTPCFont);
  st->SetTitleFont(nimTPCFont, "T");
  st->SetTitleFont(nimTPCFont, "XYZ");
  st->SetLabelFont(nimTPCFont,"XYZ");
  st->SetLabelSize(0.05,"XYZ");
  st->SetTitleSize(0.055,"XYZ");
  st->SetTitleOffset(1.1,"XZ");
  st->SetTitleOffset(0.9,"Y");   //mine
  //st->SetNdivisions(505, "Y");
  st->SetStatFont(nimTPCFont);
  st->SetOptTitle(1);
  st->SetTitleAlign(13);
  st->SetTitleBorderSize(0);
  st->SetPalette(1,0);
  st->SetStatBorderSize(1);
  new TColor(2001,1,1,1);
  st->SetFillColor(2001);
  st->SetTickLength(gStyle->GetTickLength()/696.*472.,"y");
  const Int_t NRGBs = 5;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };  
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  st->cd();
  gROOT->SetStyle("goodstyle");
  gStyle->SetOptStat(0);
}


//________________________________________________________________________________
void setStyleHisto(TH1F* histo)
{
  histo->GetXaxis()->SetLabelSize(0.045);
  histo->GetXaxis()->SetTitleSize(0.05);
  histo->GetXaxis()->SetLabelOffset(0.01);
  histo->GetXaxis()->SetTitleOffset(1.2);
  histo->GetXaxis()->SetLabelFont(42);
  histo->GetYaxis()->SetLabelSize(0.045);
  histo->GetYaxis()->SetTitleSize(0.05);
  histo->GetYaxis()->SetLabelOffset(0.01);
  histo->GetYaxis()->SetTitleOffset(1.25);
}

//________________________________________________________________________________
void setStyleGraph(TGraphErrors* gr)
{
  gr->GetXaxis()->SetLabelSize(0.045);
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->GetXaxis()->SetLabelOffset(0.01);
  gr->GetXaxis()->SetTitleOffset(1.2);
  gr->GetXaxis()->SetLabelFont(42);
  gr->GetYaxis()->SetLabelSize(0.045);
  gr->GetYaxis()->SetTitleSize(0.05);
  gr->GetYaxis()->SetLabelOffset(0.01);
  gr->GetYaxis()->SetTitleOffset(1.25);
}

} // namespace beautify