/*

##################################################
Overlay Script
Written on 11th June 2026

To run: Type the following command in the ROOT prompt
        .x make_overlay.C
##################################################

*/

#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TF1.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <math.h>

using std::cout;
using std::endl;

// Declare all the other functions needed. See the function implementations for more
void decorate(TH1*h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill);
void decorate(TLegend *g, float textSize, TString legendheader);
float get_nevents(TH1F *hst, float bin_lo, float bin_hi);
float get_nevents_err(TH1F *hst, float bin_lo, float bin_hi);

//Function Implementation
/*
This function decorates the histogram, by setting the titles of the X and Y axes,
by setting the line and marker colors, the marker style, and by deciding whether
to fill in the histogram with a color or not.
 */
void decorate(TH1*h,const char* xtitle, const char* ytitle, const char* title,
	      int linecolor, int linewidth, int markercolor, int markerstyle, int tofill) {

  h->GetXaxis()->SetTitle(xtitle);
  h->GetYaxis()->SetTitle(ytitle);

  h->SetLineColor(linecolor); 
  h->SetLineWidth(linewidth);
  h->SetMarkerColor(markercolor);
  h->SetMarkerStyle(markerstyle);
  if(tofill==1) h->SetFillColor(markercolor);

  h->SetMarkerSize(1.0);
  h->SetTitle(title);
  
}
//This function decorates the legend, by setting the textsize, and filling in the legend with a pure white color.
void decorate(TLegend *g, float textSize, TString legendheader){
  // g->SetTextSize(textSize);
  g->SetTextSize(0.);
  g->SetFillStyle(1);
  g->SetFillColor(10);
  g->SetBorderSize(0);
  g->SetLineColor(10);
  //Usually legends should not have headers but if you want one, uncomment the next line.
  //g->SetHeader(legendheader);
}


// Here are a couple of other utility functions

// For a given histogram hst, return the number of entries between bin_lo and bin_hi
float get_nevents(TH1F *hst, float bin_lo, float bin_hi)
{
  int bin_width = hst->GetBinWidth(1);
  int ibin_begin = 1;
  float nevents = 0;
  while ( hst->GetBinCenter(ibin_begin) < bin_lo )
    ibin_begin++;
  int ibin_end = ibin_begin;
  while ( hst->GetBinCenter(ibin_end) < bin_hi )
    ibin_end++;
  for(int i=ibin_begin; i<ibin_end; i++)
    nevents += hst->GetBinContent(i);

  return nevents;
}
// Partner function for above, returning the error for the above nevents
float get_nevents_err(TH1F *hst, float bin_lo, float bin_hi)
{
  int bin_width = hst->GetBinWidth(1);
  int ibin_begin = 1;
  float nevents_err = 0;
  while ( hst->GetBinCenter(ibin_begin) < bin_lo )
    ibin_begin++;
  int ibin_end = ibin_begin;
  while ( hst->GetBinCenter(ibin_end) < bin_hi )
    ibin_end++;
  for(int i=ibin_begin; i<ibin_end; i++)
    nevents_err += pow(hst->GetBinError(i),2);
  nevents_err = sqrt(nevents_err);
  return nevents_err;
}

// This is the primary function.It opens two histogram files, extracts two histograms from them, and plots them on top of each other (i.e. overlays them)
TH1F *hfinal;
void make_overlay()
{
  // 1. Declare and open files

  //First declare the file names (just strings)
  TString filename_DY                   = "root_inputs/DrellYan_20k_8TeV.root" ;
  TString filename_ZZ_4L                = "root_inputs/ZZ_to_4L_20k_8TeV.root" ;
  
  //Now let us open the files
  TFile *file_DY                        = new TFile(filename_DY);
  TFile *file_ZZ4L                      = new TFile(filename_ZZ_4L);
  
  //---------------------------------------------------------------------------------------
  
  // 2. Define plotting parameters

  const int nPlots = 10; // Set this variable to the no of hists we want to overlay
  TString plotnames[nPlots] = {"lep_pT" , "nlep"};
  TString xtitle[nPlots]    = {"Transverse Momentum (p_{T}) of Leptons", "No of Leptons"};
  TString ytitle            = "Events";
  
  TString plot;
  TH1F *h1, *h2;
  
  
  for(int i=0; i<25; i++){
    // for(int i=0; i< sizeof(plotnames); i++){
    plot= plotnames[i]; 

    //---------------------------------------------------------------------------------------
    
    // 3. Extract histograms
    h1 = (TH1F*)file_DY->Get(plot);
    h2 = (TH1F*)file_ZZ4L->Get(plot);
    

    //---------------------------------------------------------------------------------------
    
    // 4. Decorate histograms
    // the 1 or 0 here means- 1=fill the color and 0=doesn't fill the color
    decorate(h1,xtitle[i],ytitle,"",kBlue,2,kBlue,20,0); 
    decorate(h2,xtitle[i],ytitle,"",kRed,2,kRed,22,0);

    //---------------------------------------------------------------------------------------

    // 5. Setup Canvas
    TCanvas *canvas = new TCanvas(plot, plot, 800, 600);
    canvas->SetTickx(1);
    canvas->SetTicky(1);
    canvas->SetLogy(1); // Setting Log Y scale

    //---------------------------------------------------------------------------------------

    
    // 6. Manage Y-Axis Maximums 
    float max1 = h1->GetMaximum();
    float max2 = h2->GetMaximum();
    float globalMax = (max1 > max2) ? max1 : max2;
    
    // Scale maximum up to leave room for the legend
    h1->SetMaximum(globalMax * 5.0); // Multiply by larger factor because of LogY
    h1->SetMinimum(10); // Minimum set to 10 based on your original script

    //---------------------------------------------------------------------------------------
    
    // 7. Draw Histograms directly

    //Explicitly turn off stat boxes
    h1->SetStats(0);
    h2->SetStats(0);
    
    h1->Draw("HIST");
    h2->Draw("HIST SAME"); // "SAME" tells ROOT to overlay on the active canvas

    //---------------------------------------------------------------------------------------
    
    // 8. Build Legend
    TLegend *lg1 = new TLegend(0.65, 0.70, 0.88, 0.88, NULL, "NDC");
    decorate(lg1, 0.04, ""); 
    
    TString name1 = "DY [" + std::to_string((int)h1->Integral()) + "]";
    lg1->AddEntry(h1, name1, "l"); // "l" for line, "f" for fill
    
    TString name2 = "ZZ to 4L [" + std::to_string((int)h2->Integral()) + "]";
    lg1->AddEntry(h2, name2, "l");
    
    lg1->Draw(); 

    //---------------------------------------------------------------------------------------

    
    // 9. Save Canvas
    canvas->SaveAs("/home/tashvi/Uttsavir/Work/overlay/output/" + plot + ".png");

    //---------------------------------------------------------------------------------------
    
  }//for loop
  
  cout << "_____________________________  END _______________________________" << endl;




}//overlay_SingleTop



