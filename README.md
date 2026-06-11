# Histogram Overlay Macro
This repository contains a C++ macro (make_overlay.C) designed for the CERN ROOT Data Analysis Framework. 
It extracts, formats, and superimposes kinematic distributions from multiple .root files into a single canvas.


## Repository Contents
make_overlay.C: The primary C++ macro containing the extraction, decoration, and plotting logic.

root_inputs: This directory contains the sample .root files (DrellYan_20k_8TeV.root, ZZ_to_4L_20k_8TeV.root)
to create an overlay plot

output: This directory contains the output plots

## How to Run
You can run this macro directly from your terminal inside the root 
> [] .x make_overlay.C
