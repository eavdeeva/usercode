#include "Selection.h" 
  //this class
#include "../Include/TMetTools.h" 
#include "../Include/TMuonCuts.h" 
#include "../Include/TElectronCuts.h" 
#include "../Include/TPhotonCuts.h" 
#include "../Include/TFullCuts.h" 
#include "../Include/TPuReweight.h" 
#include "../Configuration/TConfiguration.h" 
#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
  //this package
//#include "../Include/PhosphorCorrectorFunctor.hh"
#include "../PHOSPHOR_CORRECTION/PhosphorCorrectorFunctor.hh"
  //taken from https://github.com/cmorgoth/PHOSPHOR_CORRECTION
  //currently in this package
#include "TMath.h"
#include "TRandom.h"  
  // ROOT class
#include <iostream> 
#include <string>
#include <sstream>  
  //standard C++ class

Selection::Selection()
{
}

Selection::Selection(TConfiguration::AnalysisParameters &anPars)
{

  _INPUT = new TAllInputSamples(anPars.channel, anPars.vgamma, anPars.configfile);
  _vgamma=anPars.vgamma;
  _channel=anPars.channel;
   std::cout<<"Selection constructor: channel="<<_config.StrChannel(anPars.channel)<<", vgamma="<<_config.StrVgType(anPars.vgamma)<<", sampleMode="<<StrSampleMode(anPars.sampleMode)<<std::endl;
  _isNoPuReweight=anPars.isNoPuReweight;
  _isDebugMode=anPars.isDebugMode;
  _photonCorrector = new zgamma::PhosphorCorrectionFunctor(_config.GetPhosphorConstantFileName(),1);
    //field of this class, Selection
    //in PhosphorCorrectionFunctor class: PhosphorCorrectionFunctor(const char* filename, bool R9Cat);//Bool just used to overload constructor and allow R9 categories inplementation

  _sampleMode=anPars.sampleMode;
  if (_sampleMode==NOTSPECIFIED){
    stringstream ss(anPars.analyzedSamples);
    vector <string> names;
    string name;
    while (ss >> name) 
      names.push_back(name);
    int nNames = names.size();
    std::cout<<"1 - will do, 0 - will not do"<<std::endl;
    for (int i=0; i<_INPUT->nSources_; i++)
      {
        _doAnalizeSample.push_back(0);  
        for (int j=0; j<nNames; j++)
          {
            if (names[j]==_INPUT->allInputs_[i].sourceName_)
              _doAnalizeSample.back()=1; 
          }
        std::cout<<_INPUT->allInputs_[i].sourceName_<<": "<<_doAnalizeSample[i]<<std::endl;
      }  
    if ((int)_doAnalizeSample.size()!=_INPUT->nSources_)
      std::cout<<"ERROR in Selection::Selection: wrong _doAnalizeSample.size()"<<std::endl;
      std::cout<<"ERROR in Selection::Selection: _sampleMode is NOTSPECIFIED"<<std::endl;
      return;
    }
  else if (_sampleMode>NOTSPECIFIED){
    std::cout<<"ERROR in Selection::Selection: wrong _sampleMode "<<std::endl;
    return;    
  }
  else 
    for (int i=0; i<_INPUT->nSources_; i++)
      _doAnalizeSample.push_back(1);      
}// end of Selection::Selection()

Selection::~Selection()
{
   _eventTree.fChain = 0;
     //field of TEventTree 
   //delete _photonCorrector;
}

TString Selection::StrSampleMode(int sampleMode)
{
  if (sampleMode==DATA) return "DATA";
  if (sampleMode==SIGMC) return "SIGMC";
  if (sampleMode==BKGMC) return "BKGMC";
  if (sampleMode==MC) return "MC";
  if (sampleMode==NOBKG) return "NOBKG";
  if (sampleMode==ALL) return "ALL";
  if (sampleMode==NOTSPECIFIED) return "NOTSPECIFIED";
  return "sampleMode="+TString(sampleMode)+"is unknown";
}

void Selection::LoopOverInputFiles()
{
  int nSources = _INPUT->nSources_; 
  for (int iSource=0; iSource<nSources; iSource++)
    {
       //sources are data, sigmc, DYjets, multibosons etc.
       //each source may consist of several input files
       if (_INPUT->allInputs_[iSource].sourceName_=="DYjets_to_ll" ||
              _INPUT->allInputs_[iSource].sourceName_=="Wjets_to_lnu" || 
              _INPUT->allInputs_[iSource].sourceName_=="ttbarjets")
         _isVJets=1;
       else _isVJets=0;
       if (!_doAnalizeSample[iSource])
         continue;
       _sample = _INPUT->allInputs_[iSource].sample_;
       if (_sample==_config.SIGMC_UNSKIMMED){
           continue; //SIGMC_UNSKIMMED is for accXeff, not for selection
       }
       if (_sample==_config.DATA){
           _lumiData=_INPUT->allInputs_[iSource].lumiTotal_;
           if (_sampleMode==ALL || _sampleMode==DATA || _sampleMode==NOBKG || _sampleMode==NOTSPECIFIED);
           else continue;
       }
       if (_sample==_config.SIGMC){
           if (_sampleMode==ALL || _sampleMode==SIGMC || _sampleMode==MC || _sampleMode==NOBKG || _sampleMode==NOTSPECIFIED);
           else continue;
       }
       if (_sample==_config.BKGMC){
           if (_sampleMode==ALL || _sampleMode==BKGMC || _sampleMode==MC || _sampleMode==NOTSPECIFIED);
           else continue;
       }
       std::cout<<"_channel="<<_channel<<"("<<_config.StrChannel(_channel)<<")";
       std::cout<<", _vgamma="<<_vgamma<<"("<<_config.StrVgType(_vgamma)<<")"<<std::endl;
       std::cout<<"_sample="<<_sample<<"("<<_config.StrSample(_sample)<<")";
       std::cout<<", _sampleMode="<<_sampleMode<<"("<<StrSampleMode(_sampleMode)<<")"<<std::endl;
       std::cout<<"sourceName="<<_INPUT->allInputs_[iSource].sourceName_<<", _isVJets="<<_isVJets<<std::endl;
       TTree* tree;
       int inputFileNMax = _INPUT->allInputs_[iSource].nFiles_;
       TString fileOutName;
       fileOutName=_config.GetSelectedName(_config.VERY_PRELIMINARY,_channel,_vgamma,_config.UNBLIND,_INPUT->allInputs_[iSource].sample_,_INPUT->allInputs_[iSource].sourceName_,_isDebugMode,_isNoPuReweight);
       _fileOut = new TFile(fileOutName,"recreate");
       _outTree = new TTree("selectedEvents","selected Events");
       _selEvTree.SetAsOutputTree(_outTree);

       TFullCuts fullCuts;
       fullCuts.SetPassedToZeros(_passed);

       for (_inputFileN=0; _inputFileN< inputFileNMax; _inputFileN++){

         TFile f(_INPUT->allInputs_[iSource].fileNames_[_inputFileN]);
         if (f.IsOpen()) 
           std::cout<<std::endl<<"processing file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<std::endl;
         else{
            std::cout<<"ERROR detected in Selection::LoopOverInputFiles: file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<" was not found"<<std::endl;
            return;
         } 
         f.cd("ggNtuplizer");
         tree = (TTree*)gDirectory->Get("EventTree");
         if (tree){
           _eventTree.Init(tree);
           //if (_sample!=_config.DATA) SetMCSpecificAddresses();
         }   
         else{
           std::cout<<"ERROR detected in Selection::LoopOverInputFiles: tree in the file "<<_INPUT->allInputs_[iSource].fileNames_[_inputFileN]<<" does not exist"<<std::endl;
           return;
         }  
         //determine _lumiWeight
         if (_sample==_config.DATA) _lumiWeight=1;
         else _lumiWeight=_INPUT->allInputs_[iSource].lumiWeights_[_inputFileN];
         std::cout<<"_lumiWeight="<<_lumiWeight<<std::endl;
         _puWeight=new TPuReweight(_config.GetPileupDataFileName(),_INPUT->allInputs_[iSource].fileNames_[_inputFileN]);      

         LoopOverTreeEvents();//method of this class (Selection)

         _eventTree.fChain=0;//field of TEventTree
         delete _puWeight;
       }//loop over _inputFileN ends
       std::cout<<"the output will be saved to "<<_fileOut->GetName()<<std::endl<<std::endl;
      _fileOut->cd();
      _outTree->Write();
      _fileOut->Close();
      //delete outTree;
      _outTree = 0;
    } //loop over iSource ends
}


void Selection::LoopOverTreeEvents()
{
   if (_eventTree.fChain == 0) return;
   Long64_t nentries = _eventTree.fChain->GetEntries();
   if (_isDebugMode) 
     {
       if (_eventTree.fChain->GetEntries()<_debugModeNEntries) nentries=_eventTree.fChain->GetEntries();
       else nentries=_debugModeNEntries;
     }

   int nTotal=0;
   int nPassed=0;
//   int nBlind=0;
   float nTotalW=0;//weighted
   float nPassedW=0;//weighted
//   float nBlindW=0;

   //goodLeptonPhotonPairs(two-dimentional array of bool-s)
   //memory allocation for some variables: 
   int nLeptonMax;
   if (_channel==_config.MUON) nLeptonMax=_eventTree.kMaxnMu;
   else if (_channel==_config.ELECTRON) nLeptonMax=_eventTree.kMaxnEle;
     //kMaxnMu - field of TEventTree
   else{
       std::cout<<"Error detected in Selection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
       return;
   }
   int nCands=_eventTree.kMaxnPho*nLeptonMax;
   TFullCuts::Candidate cands[nCands];

   CheckMaxNumbersInTree();

   TPhotonCuts emptyPhoton;
   std::cout<<"nentries="<<nentries<<std::endl;

   for (Long64_t ientry=0; ientry<nentries; ientry++){

   //loop over events in the tree
     _eventTree.GetEntryNeededBranchesOnly(_channel,_sample,ientry);

     if (_channel==_config.MUON) _nLe=_eventTree.treeLeaf.nMu;
     else if (_channel==_config.ELECTRON) _nLe=_eventTree.treeLeaf.nEle;
     else{
       std::cout<<"Error detected in  Selection::LoopOverTreeEvents: channel must be either MUON or ELECTRON."<<std::cout;
       return;
     }
     _totalWeight = _lumiWeight;
     if (!_eventTree.treeLeaf.isData && !_isNoPuReweight)
       _totalWeight*=_puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
     nTotalW+=_totalWeight;
     nTotal+=1;

     TFullCuts fullCuts;
     //bool selPassed=1;
     bool selPassed = fullCuts.VeryPreliminaryCut(_eventTree.treeLeaf,_photonCorrector,
       _channel,_vgamma,_isVJets,nCands,cands,_passed);        
     
     if (!selPassed) continue;
       //added blinding prescaling into the beginning

     for (int icand=0; icand<nCands; icand++){

       nPassedW+=_totalWeight;
       nPassed+=1;
       float puWeightVal=1;
       float puTrueVal=1;

       if (_sample!=_config.DATA){
         puWeightVal=_puWeight->GetPuWeightMc(_eventTree.treeLeaf.puTrue->at(1));
         puTrueVal=_eventTree.treeLeaf.puTrue->at(1);
       }//end of if (_sample!=_config.DATA)     
       
       _selEvTree.SetValues(_channel, _sample, _eventTree.treeLeaf,  cands[icand],_inputFileN, 
               _totalWeight, puWeightVal, puTrueVal, _photonCorrector);
       _outTree->Fill();

     }//end of loop over icand

  } //end of loop over ientry in the tree

  std::cout<<"Summary:"<<std::endl;
  std::cout<<"nEntries="<<nentries<<", nTotal="<<nTotal;
  if (nTotal==0) return;
  std::cout<<", nPassed="<<nPassed<<", eff="<<(float)nPassed/nTotal<<std::endl;
  std::cout<<"Weighted:"<<"nTotal="<<nTotalW<<", nPassed="<<nPassedW<<", eff="<<(float)nPassedW/nTotalW<<std::endl;
  TFullCuts fullCut;
  fullCut.Print(_passed);

}

bool Selection::CheckMaxNumbersInTree()
{
   if ((_channel==_config.MUON) 
       && (_eventTree.fChain->GetMaximum("nMu")>_eventTree.kMaxnMu))
     //kMaxnMu - field of TEventTree
     {
       PrintErrorMessageMaxNumberOf(_MUON);
          //methof of this class (Selection)
       return 0;
     }
   if (_channel==_config.ELECTRON 
       && _eventTree.fChain->GetMaximum("nEle")>_eventTree.kMaxnEle)
     {
       PrintErrorMessageMaxNumberOf(_ELECTRON);
          //methof of this class (Selection)
       return 0;
     }
   if (_eventTree.fChain->GetMaximum("nPho")>_eventTree.kMaxnPho)
     {
       PrintErrorMessageMaxNumberOf(_PHOTON);
          //methof of this class (Selection)
       return 0;
     }
//   if (fChain->GetMaximum("nJet")>kMaxnJet)
//     {
//       PrintErrorMessageMaxNumberOf(_JET);
//          //methof of this class (Selection)
//       return 0;
//     }
//   if (fChain->GetMaximum("nLowPtJet")>kMaxnLowPtJet)
//     {
//       PrintErrorMessageMaxNumberOf(LOWPT_JET);
//          //methof of this class (Selection)
//       return 0;
//     }
//   if (!treeLeaf.isData && fChain->GetMaximum("nMC")>kMaxnMC)
//     {
//       PrintErrorMessageMaxNumberOf(_MC);
//          //methof of this class (Selection)
//       return 0;
//     }
   return 1;
}

void Selection::PrintErrorMessageMaxNumberOf(int particle)
{
       std::cout<<std::endl;
       std::cout<<"Error detected in Selection::PrintErrorMessageMaxNumberOf:"<<std::endl;
       if (particle==_MUON)
         std::cout<<"maximum number of muons in the file nMu="<<_eventTree.fChain->GetMaximum("nMu")<<", when kMaxnMu="<<_eventTree.kMaxnMu<<" only"<<std::endl;
       else if (particle==_ELECTRON)
         std::cout<<"maximum number of electrons in the file nEle="<<_eventTree.fChain->GetMaximum("nEle")<<", when kMaxnEle="<<_eventTree.kMaxnEle<<" only"<<std::endl;
       else if (particle==_PHOTON)
         std::cout<<"maximum number of photons in the file nPho="<<_eventTree.fChain->GetMaximum("nPho")<<", when kMaxnPho="<<_eventTree.kMaxnPho<<" only"<<std::endl;
//       else if (particle==_JET)
//         std::cout<<"maximum number of jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnJet="<<kMaxnJet<<" only"<<std::endl;
//      else if (particle==LOWPT_JET)
//         std::cout<<"maximum number of low Pt jets in the file nJet="<<fChain->GetMaximum("nJet")<<", when kMaxnLowPtJet="<<kMaxnLowPtJet<<" only"<<std::endl;
//      else if (particle==_MC)
//         std::cout<<"maximum number of mc particles in the file nMC="<<fChain->GetMaximum("nMC")<<", when kMaxnMC="<<kMaxnMC<<" only"<<std::endl;
       std::cout<<"please, go to TEventTree.h to increase this number up to proper value"<<std::endl;
       std::cout<<std::endl;
}

void Selection::ExtraSelection(TConfiguration::AnalysisParameters &anPars)
{
  TFullCuts fullCut;
  TConfiguration config;
  TAllInputSamples INPUT(anPars.channel,anPars.vgamma,anPars.configfile);
  _tr=0;
  _trReduced=0;
  _tr1=0;
  _tr2=0;
  _tr3=0;

  for (int i=0; i<INPUT.nSources_; i++){

    int sample=INPUT.allInputs_[i].sample_;
    if(anPars.sampleMode==ALL);
    else if(anPars.sampleMode==DATA && sample!=config.DATA) continue;
    else if(anPars.sampleMode==SIGMC && sample!=config.SIGMC) continue;
    else if(anPars.sampleMode==BKGMC && sample!=config.BKGMC) continue;
    else if(anPars.sampleMode==MC && sample==config.DATA) continue;
    else if(anPars.sampleMode==NOBKG && sample==config.BKGMC) continue;
    
    std::cout<<"ExtraSelection: sample="<<_config.StrSample(sample)<<std::endl;
    ExtraSelectionOne(INPUT,i,config,fullCut,anPars.year,anPars.channel,anPars.vgamma,anPars.phoWP,config.UNBLIND);
    if (sample==config.DATA){
      ExtraSelectionOne(INPUT,i, config, fullCut,anPars.year,anPars.channel,anPars.vgamma,anPars.phoWP,config.BLIND_PRESCALE);
      ExtraSelectionOne(INPUT,i, config, fullCut,anPars.year,anPars.channel,anPars.vgamma,anPars.phoWP,config.BLIND_COMBINED);
    }
  }//end of loop over i (for (int i=0; i<INPUT.nSources_; i++))

}// end of ExtraSelection

void Selection::ExtraSelectionOne(TAllInputSamples &INPUT, int iSource, TConfiguration& config, TFullCuts &fullCut, int year, int channel, int vgamma, int wp, int blind)
{

    std::cout<<"ExtraSelection; year="<<year<<", channel="<<_config.StrChannel(channel)<<", vgamma="<<_config.StrVgType(vgamma)<<", wp="<<wp<<",blind="<<_config.StrBlindType(blind)<<std::endl;

    TString fInName = config.GetSelectedName(config.VERY_PRELIMINARY,channel,vgamma,_config.UNBLIND,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);

    TFile fIn(fInName);
    if (!fIn.IsOpen()){
      std::cout<<"file "<<fInName<<" can't be open"<<std::endl;
      return;
    }

    std::cout<<"file "<<fInName<<" was open"<<std::endl;
    _tr = (TTree*)fIn.Get("selectedEvents");


    // Preliminary for template method selected
      TString fOutName2=config.GetSelectedName(config.PRELIMINARY_FOR_TEMPLATE_METHOD,channel,vgamma,blind,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut2(fOutName2,"recreate");
      std::cout<<"For TemplateMethodCut Selection cut:      "<<fullCut.RangeForTemplateMethodCut(year,channel,vgamma,blind,wp).GetTitle()<<std::endl;
      _tr2 = new TTree("selectedEvents","selected events, one candidate per event");
      _trReduced = _tr->CopyTree(fullCut.RangeForTemplateMethodCut(year,channel,vgamma,blind,wp));

      PickHardestPhotonInEvent(_tr2, _trReduced, blind);

      _tr2->Write();
      std::cout<<"Will be saved to file:      "<<fOutName2<<std::endl;

    // Preliminary for e->gamma selected
    if (channel==config.ELECTRON && vgamma==config.W_GAMMA && blind==config.UNBLIND){
      
      TString fOutName2_1=config.GetSelectedName(config.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_NO_WMT_CUT,config.ELECTRON,config.W_GAMMA,config.UNBLIND,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut2_1(fOutName2_1,"recreate");
      std::cout<<"For EtoGamma with PSV Selection cut:      "<<fullCut.RangeForEtoGamma(wp,1,0).GetTitle()<<std::endl;
      _tr2_1 = new TTree("selectedEvents","selected events, one candidate per event");
      _trReduced = _tr->CopyTree(fullCut.RangeForEtoGamma(wp,1,0));
      PickHardestPhotonInEvent(_tr2_1, _trReduced, blind);
      _tr2_1->Write();
      std::cout<<"Will be saved to file:      "<<fOutName2_1<<std::endl;

      TString fOutName2_2=config.GetSelectedName(config.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT,config.ELECTRON,config.W_GAMMA,config.UNBLIND,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut2_2(fOutName2_2,"recreate");
      std::cout<<"For EtoGamma with PSV Selection cut:      "<<fullCut.RangeForEtoGamma(wp,1,0).GetTitle()<<std::endl;
      _tr2_2 = new TTree("selectedEvents","selected events, one candidate per event");
      _trReduced = _tr->CopyTree(fullCut.RangeForEtoGamma(wp,1,1));
      PickHardestPhotonInEvent(_tr2_2, _trReduced, blind);
      _tr2_2->Write();
      std::cout<<"Will be saved to file:      "<<fOutName2_2<<std::endl;

      TString fOutName2_3=config.GetSelectedName(config.PRELIMINARY_FOR_E_TO_GAMMA_NO_PSV_CUT,config.ELECTRON,config.W_GAMMA,config.UNBLIND,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut2_3(fOutName2_3,"recreate");
      std::cout<<"For EtoGamma inversted PSV Selection cut:      "<<fullCut.RangeForEtoGamma(wp,0,0).GetTitle()<<std::endl;
      _tr2_3 = new TTree("selectedEvents","selected events, one candidate per event");
      _trReduced = _tr->CopyTree(fullCut.RangeForEtoGamma(wp,0,1));
      PickHardestPhotonInEvent(_tr2_3, _trReduced, blind);
      _tr2_3->Write();
      std::cout<<"Will be saved to file:      "<<fOutName2_3<<std::endl;
      
    }//end of if (channel==config.ELECTRON && vgamma==config.W_GAMMA)

    
    // Fully selected
      TString fOutName3=config.GetSelectedName(config.FULLY,channel,vgamma,blind,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut3(fOutName3,"recreate");
      _tr3 = new TTree("selectedEvents","selected events, one candidate per event");
      _trReduced = _tr->CopyTree(fullCut.RangeFullCut(year,channel,vgamma,blind,wp));

      std::cout<<std::endl;
      std::cout<<"full Selection cut:      "<<fullCut.RangeFullCut(year,channel,vgamma,blind,wp).GetTitle()<<std::endl;
      std::cout<<std::endl;
      
      PickHardestPhotonInEvent(_tr3, _trReduced, blind);

      _tr3->Write();
      if (INPUT.allInputs_[iSource].sample_==_config.DATA && blind==_config.UNBLIND);
      else{
        std::cout<<"before hardest photon selection: "<<_trReduced->GetEntries()<<std::endl;
        std::cout<<"after  hardest photon selection: "<<_tr3->GetEntries()<<std::endl;
      }


    if (vgamma==config.W_GAMMA) return;

    // FSR selected, Z_GAMMA only
      TString fOutName4=config.GetSelectedName(config.FSR,channel,vgamma,config.UNBLIND,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut4(fOutName4,"recreate");
      _tr4 = new TTree("selectedEvents","selected events, one candidate per event");
      _tr4 = _tr->CopyTree(fullCut.RangeFsrCut());
      std::cout<<std::endl;
      std::cout<<"full Selection cut:      "<<fullCut.RangeFsrCut().GetTitle()<<std::endl;
      std::cout<<std::endl;
//      PickHardestPhotonInEvent(_tr4, _trReduced);
      _tr4->Write();

    // FSR_EXCLUDED selected, Z_GAMMA only
      TString fOutName5=config.GetSelectedName(config.FSR_EXCLUDED,channel,vgamma,config.UNBLIND,INPUT.allInputs_[iSource].sample_,INPUT.allInputs_[iSource].sourceName_);
      TFile fOut5(fOutName5,"recreate");
      _tr5 = new TTree("selectedEvents","selected events, one candidate per event");
      _tr5 = _tr->CopyTree(fullCut.RangeFsrExcludedCut());
      std::cout<<std::endl;
      std::cout<<"full Selection cut:      "<<fullCut.RangeFsrExcludedCut().GetTitle()<<std::endl;
      std::cout<<std::endl;
//      PickHardestPhotonInEvent(_tr5, _trReduced);
      _tr5->Write();



}// end of ExtraSelectionOne

void Selection::PickHardestPhotonInEvent(TTree* trToBeWritten, TTree* trReduced, int blind)
{
      TSelectedEventsTree selEvTree;
      selEvTree.SetAsInputTree(trReduced);
      selEvTree.SetAsOutputTree(trToBeWritten);
      long nEntries = trReduced->GetEntries();
      long eventCurr=0;
      long eventPrev=-1;
      for (long ientry=0; ientry<nEntries; ientry++){
        trReduced->GetEntry(ientry);
        eventPrev=eventCurr;
        eventCurr = selEvTree._event;
        if (eventCurr==eventPrev) continue;
        if (blind==_config.BLIND_COMBINED){
          if (selEvTree._phoEt>_config.GetPhoPtBlindThreshold()) selEvTree._weight=_config.GetBlindPrescale();
        }
        trToBeWritten->Fill();
      }//end of loop over ientry
}//end of PickHardestPhotonInEvent
