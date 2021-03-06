#include "FullChain.h"
  //this class

#include "../Configuration/TInputSample.h"
#include "../Configuration/TAllInputSamples.h"
#include "../Configuration/TConfiguration.h"
#include "../Selection/Selection.h"
#include "../DDBkgEtoGamma/TEtoGamma.h"
#include "../DDBkgTemplateMethod/TTemplates.h"
#include "../DDBkgTemplateMethod/AuxTemplates.C"
#include "../PrepareYields/TPrepareYields.h"
#include "../PrepareYields/AuxPrepareYields.C"
#include "../AcceptanceAndEfficiency/CalcAccAndEff.h"
#include "../CrossSection/CalcCrossSection.h"
#include "../CrossSection/AuxCrossSection.C"

#include <iostream>

FullChain::FullChain()
{
//  _INPUT=new TAllInputSamples(_channel,"../Configuration/config.txt");
//  _phoPtBinLimits = new float[_config.GetNPhoPtBins()];
//  _config.GetPhoPtBinsLimits(_phoPtBinLimits);
}

FullChain::~FullChain()
{
//  delete _INPUT;
//  delete _phoPtBinLimits;
}

void FullChain::SetDefaultFullChainParameters(TConfiguration::AnalysisParameters& anPars, TString varKin)
{
  TConfiguration conf;
  anPars.year=2012;//2012, 2011
  anPars.channel=conf.MUON;//MUON, ELECTRON
  anPars.vgamma=conf.W_GAMMA;//W_GAMMA, Z_GAMMA
  anPars.templFits=conf.TEMPL_CHISO;

  anPars.sampleMode=Selection::ALL;
  anPars.analyzedSamples="";
  anPars.configfile="../Configuration/config.txt";
  anPars.isNoPuReweight=0;
  anPars.isDebugMode=0;
  anPars.phoWP=TPhotonCuts::WP_MEDIUM;//WP_LOOSE,WP_MEDIUM,WP_TIGHT
  anPars.cutAdd="1";

  // anPars.blind[conf.MUON][conf.W_GAMMA]=conf.BLIND_COMBINED;
  anPars.blind[conf.MUON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;
  //  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.BLIND_COMBINED;
  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.blind[conf.ELECTRON][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.noDDBkgEtoGamma=1;

  for (int ich=0; ich<=1; ich++){
    for (int ivg=0; ivg<=1; ivg++){
      anPars.noPreSelection[ich][ivg]=1;
      anPars.noExtraSelection[ich][ivg]=1;
      anPars.noDDBkgComputation[ich][ivg][conf.TEMPL_CHISO]=1;
      anPars.noDDBkgComputation[ich][ivg][conf.TEMPL_SIHIH]=1;
      anPars.noDDBkgSystRandTempl[ich][ivg][conf.TEMPL_CHISO]=1;
      anPars.noDDBkgSystRandTempl[ich][ivg][conf.TEMPL_SIHIH]=1;
      anPars.noPrepareYields[ich][ivg]=1;
      anPars.noSubtractBackground[ich][ivg][conf.TEMPL_CHISO]=1;
      anPars.noSubtractBackground[ich][ivg][conf.TEMPL_SIHIH]=1;
      anPars.noSubtractBackground[ich][ivg][conf.TEMPL_OVERLAY]=1;
      anPars.noCalcAccAndEff[ich][ivg]=1;
      anPars.noCalcCrossSection[ich][ivg]=1;
      anPars.noSystDDBkgSidebandVariation[ich][ivg][conf.TEMPL_CHISO]=1;
      anPars.noSystDDBkgSidebandVariation[ich][ivg][conf.TEMPL_SIHIH]=1;
      anPars.noCalcAccAndEff[ich][ivg]=1;
      anPars.noCalcCrossSection[ich][ivg]=1;
    }//end of ivg
  }//end of ich

  if (varKin=="phoEt")
    SetAnalysisKinParameters(anPars);
  else
    SetDiffKinFullChainParameters(anPars,varKin);

}// end of FullChain::SetDefaultFullChainParameters

void FullChain::SetAnalysisKinParameters(TConfiguration::AnalysisParameters& anPars)
{
    anPars.varKin="phoEt";
//    anPars.nKinBins=26;
//    anPars.nKinBins=2;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    for (int ib=0; ib<anPars.nKinBins+1; ib++)
//      anPars.kinBinLims[ib]=15+2.5*ib;
//    anPars.nKinBins=3;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=15;
//    anPars.kinBinLims[1]=20;
//    anPars.kinBinLims[2]=25;
//    anPars.kinBinLims[3]=30;


  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);

}

void FullChain::SetDiffKinFullChainParameters(TConfiguration::AnalysisParameters& anPars, TString varKin)
{
  if (varKin=="phoEtNoPhosphor"){
    anPars.varKin="phoEtNoPhosphor";

  anPars.nKinBins=_config.GetNPhoPtBins();
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  _config.GetPhoPtBinsLimits(anPars.kinBinLims);

//    anPars.nKinBins=26;
//    anPars.nKinBins=2;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    for (int ib=0; ib<anPars.nKinBins+1; ib++)
//      anPars.kinBinLims[ib]=15+2.5*ib;

  }

  if (varKin=="phoSCEta" || varKin=="phoEta"){
    anPars.varKin=varKin;

//    anPars.nKinBins=3;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=-2.5;
//    anPars.kinBinLims[1]=-1.5;
//    anPars.kinBinLims[2]=1.5;
//    anPars.kinBinLims[3]=2.5;


//    anPars.nKinBins=4;
//    anPars.kinBinLims=new float[anPars.nKinBins+1];
//    anPars.kinBinLims[0]=-2.5;
//    anPars.kinBinLims[1]=-1.5;
//    anPars.kinBinLims[2]=0;
//    anPars.kinBinLims[3]=1.5;
//    anPars.kinBinLims[4]=2.5;

    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.5;
    anPars.kinBinLims[1]=-2.366;
    anPars.kinBinLims[2]=-2.166;
    anPars.kinBinLims[3]=-1.966;
    anPars.kinBinLims[4]=-1.766;
    anPars.kinBinLims[5]=-1.566;
    anPars.kinBinLims[6]=-1.4442;
    anPars.kinBinLims[7]=-1.08315;
    anPars.kinBinLims[8]=-0.7221;
    anPars.kinBinLims[9]=-0.36105;
    anPars.kinBinLims[10]=0;
    anPars.kinBinLims[11]=0.36105;
    anPars.kinBinLims[12]=0.7221;
    anPars.kinBinLims[13]=1.08315;
    anPars.kinBinLims[14]=1.4442;
    anPars.kinBinLims[15]=1.566;
    anPars.kinBinLims[16]=1.766;
    anPars.kinBinLims[17]=1.966;
    anPars.kinBinLims[18]=2.166;
    anPars.kinBinLims[19]=2.366;
    anPars.kinBinLims[20]=2.5;
  }
  if (varKin=="phoPhi"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
  }
  if (varKin=="lepton1Pt" || varKin=="lepton2Pt"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=26+10*ib;
  }
  if (varKin=="lepton1Phi" || varKin=="lepton2Phi"){
    anPars.varKin=varKin;
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
  }
  if (varKin=="lepton1Eta" || varKin=="lepton2Eta"){
    anPars.varKin=varKin;
    anPars.nKinBins=16;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    anPars.kinBinLims[0]=-2.1;
    anPars.kinBinLims[1]=-1.966;
    anPars.kinBinLims[2]=-1.766;
    anPars.kinBinLims[3]=-1.566;
    anPars.kinBinLims[4]=-1.4442;
    anPars.kinBinLims[5]=-1.08315;
    anPars.kinBinLims[6]=-0.7221;
    anPars.kinBinLims[7]=-0.36105;
    anPars.kinBinLims[8]=0;
    anPars.kinBinLims[9]=0.36105;
    anPars.kinBinLims[10]=0.7221;
    anPars.kinBinLims[11]=1.08315;
    anPars.kinBinLims[12]=1.4442;
    anPars.kinBinLims[13]=1.566;
    anPars.kinBinLims[14]=1.766;
    anPars.kinBinLims[15]=1.966;
    anPars.kinBinLims[16]=2.1;
  }
  if (varKin=="WMt"){
    anPars.varKin=varKin;
    anPars.nKinBins=20;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+5*ib;
  }

  if (varKin=="Mleplep"){
    anPars.varKin=varKin;
    anPars.nKinBins=30;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+2*ib;
  }

  if (varKin=="Mpholeplep"){
    anPars.varKin=varKin;
    anPars.nKinBins=50;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=70+2*ib;
  }

  if (varKin=="Mpholep1" || varKin=="Mpholep2"){
    anPars.varKin=varKin;
    anPars.nKinBins=30;
//    anPars.nKinBins=2;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=30+5*ib;
  }

  if (varKin=="lep1PhoDeltaR" || varKin=="lep2PhoDeltaR"){
    anPars.varKin=varKin;
    anPars.nKinBins=40;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0.4+0.1*ib;
  }
  if (varKin=="pfMET"){
    anPars.varKin="pfMET";
    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=50+5*ib;
  }
  if (varKin=="pfMETPhi"){
    anPars.varKin="pfMETPhi";
    anPars.nKinBins=8;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=-TMath::Pi()+0.25*TMath::Pi()*ib;
 }
  if (varKin=="phoPFChIsoCorr"){
    anPars.varKin=varKin;
    anPars.nKinBins=20;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0+1*ib;
  }

  if (varKin=="phoSigmaIEtaIEta"){
    anPars.varKin=varKin;
    anPars.nKinBins=32;
    anPars.kinBinLims=new float[anPars.nKinBins+1];
    for (int ib=0; ib<anPars.nKinBins+1; ib++)
      anPars.kinBinLims[ib]=0.005+0.002*ib;
  }
}// end of SetDefaultFullChainParameters

