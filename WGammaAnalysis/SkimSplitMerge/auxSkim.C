#include "SkimLeptonPhoton.h"

#include "TString.h"
#include "TBenchmark.h"

#include <iostream>

void auxSkim(TString fileToSkimName)
{
  TBenchmark time;
  time.Start("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl;  

  TString fileToSkimFullName = "/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/eavdeeva2/WGammaMC/" + fileToSkimName;
  SkimLeptonPhoton skimmer(fileToSkimFullName, "ggNtuplizer", "EventTree", "./", SkimLeptonPhoton::MUON);
  skimmer.LoopOverInputTree();

  std::cout<<"file "<<fileToSkimFullName<<" was skimmed"<<std::endl;

  time.Stop("time");
  std::cout<<"CPU time = "<<time.GetCpuTime("time")<<", Real time = "<<time.GetRealTime("time")<<std::endl; 
}