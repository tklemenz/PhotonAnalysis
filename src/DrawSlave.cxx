#include "DrawSlave.h"

/*DrawSlave::~DrawSlave()
{
  delete mLegend;
  delete mCanvas;
}*/

void DrawSlave::makeGraphs()
{
  for (int k=0; k<mDataVec.size(); k++) {
    mGraphsVec.emplace_back(std::vector<TGraphErrors*>());
    for (int j=0; j<4; j++) { mGraphsVec.at(k).push_back(new TGraphErrors()); }

    float refTime = mDataVec.at(k).at(0).at(0);    /// time is mm/dd/hh/mm/ss.sss format --> convert to seconds!!

    for (int i=0; i<mDataVec.at(k).at(0).size(); i++) {
      mGraphsVec.at(k).at(0)->SetPoint(i,mDataVec.at(k).at(0).at(i)-refTime,mDataVec.at(k).at(1).at(i));
      mGraphsVec.at(k).at(1)->SetPoint(i,mDataVec.at(k).at(0).at(i)-refTime,mDataVec.at(k).at(2).at(i));
      mGraphsVec.at(k).at(2)->SetPoint(i,mDataVec.at(k).at(0).at(i)-refTime,mDataVec.at(k).at(3).at(i));
      mGraphsVec.at(k).at(3)->SetPoint(i,mDataVec.at(k).at(0).at(i)-refTime,mDataVec.at(k).at(4).at(i));
    }
  }
}
