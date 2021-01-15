#!/bin/bash

fileList=$(ls $1)
fileArr=($fileList)

for fileInd in "${!fileArr[@]}"; do
  tmpFileName=${fileArr[$fileInd]};
  ~/PhotonAnalysis/bin/plotPAFile -i $1/$tmpFileName -o ${tmpFileName%.txt}
done;