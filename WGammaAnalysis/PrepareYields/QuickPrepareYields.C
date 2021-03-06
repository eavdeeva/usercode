#include "../Configuration/TConfiguration.h"
#include "../Include/TFullCuts.h"
#include "../Include/TPhotonCuts.h"
#include "../FullChain/FullChain.h"
#include "AuxPrepareYields.C"

void QuickPrepareYields_FSRandISR_Ich(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;

  fch.SetDefaultFullChainParameters(anPars,"phoPFChIsoCorr");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.cutAdd="1";
  int selStage=conf.FSR_EXCLUDED;
  AuxPrepareYields(anPars,0,selStage,1,1);

  anPars.cutAdd="1";
  anPars.nKinBins=6;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
  anPars.kinBinLims[ib]=0+1*ib;
  selStage=conf.FSR;
  AuxPrepareYields(anPars,0,selStage,1,1);

}//end of QuickPrepareYields_FSRandISR_Ich

void QuickPrepareYields_FSRandISR_TemplatesIch(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;

  fch.SetDefaultFullChainParameters(anPars,"phoPFChIsoCorr");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;

  TCut cutSihihNomB="phoSigmaIEtaIEta<0.011 && phoSCEta>-1.4442 && phoSCEta<1.4442";
  TCut cutSihihNomE="phoSigmaIEtaIEta<0.033 && ((phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566))";
  TCut cutSihihNom = cutSihihNomB || cutSihihNomE;
  
  anPars.cutAdd=cutSihihNom && TCut("phoEt>10 && phoEt<15");
  int selStage=conf.FSR_EXCLUDED;
  
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to15_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>15 && phoEt<20");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to20_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>20 && phoEt<25");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt20to25_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>25 && phoEt<30");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt25to30_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>30 && phoEt<35");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt30to35_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>35 && phoEt<45");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt35to45_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>45 && phoEt<55");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt45to55_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>55 && phoEt<65");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt55to65_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>65 && phoEt<75");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt65to75_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>75 && phoEt<85");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt75to85_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>85 && phoEt<95");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt85to95_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>95 && phoEt<120");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt95to120_");

  anPars.cutAdd=cutSihihNom && TCut("phoEt>120 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt120to500_");
  
  /*
  anPars.cutAdd=cutSihihNom && TCut("phoEt>15 && phoEt<500");
  anPars.nKinBins=6;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
  anPars.kinBinLims[ib]=0+1*ib;
  selStage=conf.FSR;
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_ FSR");
  selStage=conf.FSR_EXCLUDED;
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_ ISR");
  selStage=conf.PRELIMINARY_FOR_TEMPLATE_METHOD;
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_ FSR+ISR");
  */
}//end of QuickPrepareYields_FSRandISR_TemplatesIch

void QuickPrepareYields_FSRandISR_TemplatesSihih(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;

  TCut cutIsoNomB="phoPFChIsoCorr<1.5 && phoSCEta>-1.4442 && phoSCEta<1.4442";
  TCut cutIsoNomE="phoPFChIsoCorr<1.2 && ((phoSCEta>-2.5 && phoSCEta<-1.566)||(phoSCEta<2.5 && phoSCEta>1.566))";
  TCut cutIsoNom = cutIsoNomB || cutIsoNomE;

  fch.SetDefaultFullChainParameters(anPars,"phoSigmaIEtaIEta");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.cutAdd=cutIsoNom && TCut("phoEt>10 && phoEt<15");
  int selStage=conf.FSR_EXCLUDED;
  
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to15_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>15 && phoEt<20");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to20_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>20 && phoEt<25");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt20to25_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>25 && phoEt<30");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt25to30_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>30 && phoEt<35");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt30to35_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>35 && phoEt<45");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt35to45_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>45 && phoEt<55");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt45to55_ ISR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>55 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt55to500_ ISR");


  selStage=conf.FSR;

  anPars.cutAdd=cutIsoNom && TCut("phoEt>10 && phoEt<15");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to15_  FSR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>15 && phoEt<20");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to20_ FSR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>20 && phoEt<25");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt20to25_ FSR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>25 && phoEt<30");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt25to30_ FSR");

  anPars.cutAdd=cutIsoNom && TCut("phoEt>30 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt30to500_ FSR");
  
  anPars.cutAdd=cutIsoNom && TCut("phoEt>15 && phoEt<500");
  selStage=conf.FSR;
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_ FSR");
  selStage=conf.FSR_EXCLUDED;
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_ ISR");
  selStage=conf.PRELIMINARY_FOR_TEMPLATE_METHOD;
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_ FSR+ISR");

}//end of QuickPrepareYields_FSRandISR_TemplatesSihih

void QuickPrepareYields_FSRandISR(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon;
  TCut cut = photon.RangePhoton(conf.MUON, conf.Z_GAMMA, 2012, photon.WP_MEDIUM, 1, 1, 1, 1, 1, 1) ;
  anPars.cutAdd=cut;//"phoEt<40";
//  anPars.cutAdd=photon.RangeSigmaIEtaIEta(2012,photon.WP_MEDIUM,conf.COMMON);

  anPars.blind[conf.MUON][conf.Z_GAMMA]=conf.UNBLIND;

  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;

  
  int selStage=conf.VERY_PRELIMINARY;
  
  fch.SetDefaultFullChainParameters(anPars,"Mpholeplep");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  //  anPars.cutAdd=cut && TCut("phoEt>10 && phoEt<500");
  //  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to500_");
  anPars.cutAdd=cut && TCut("phoEt>15 && phoEt<500");
  //AuxPrepareYields(anPars,0,selStage,1,1);
   
  
  fch.SetDefaultFullChainParameters(anPars,"Mleplep");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  //  anPars.cutAdd=cut && TCut("phoEt>10 && phoEt<500");
  //  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to500_");
  anPars.cutAdd=cut && TCut("phoEt>15 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,1,1);
  
  fch.SetDefaultFullChainParameters(anPars,"lep1PhoDeltaR");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  //  anPars.cutAdd=cut && TCut("phoEt>10 && phoEt<500");
  //  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to500_");
  anPars.cutAdd=cut && TCut("phoEt>15 && phoEt<500");
  //AuxPrepareYields(anPars,0,selStage,1,1);


  fch.SetDefaultFullChainParameters(anPars,"lep2PhoDeltaR");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  //  anPars.cutAdd=cut && TCut("phoEt>10 && phoEt<500");
  //  AuxPrepareYields(anPars,0,selStage,1,1,"_pt10to500_");
  anPars.cutAdd=cut && TCut("phoEt>15 && phoEt<500");
  AuxPrepareYields(anPars,0,selStage,1,1);
  

  
  selStage=conf.FSR;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd="1";
  //AuxPrepareYields(anPars,0,selStage,1,1);

  selStage=conf.FSR_EXCLUDED;
  fch.SetDefaultFullChainParameters(anPars,"phoEt");
  anPars.channel=conf.MUON;
  anPars.vgamma=conf.Z_GAMMA;
  anPars.cutAdd="1";
  //AuxPrepareYields(anPars,0,selStage,1,1);
  
}//end of QuickPrepareYields_FSRandISR

void QuickPrepareYields_WMt(int channel){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  TPhotonCuts photon;
  TFullCuts fullcut;
  TCut cut = photon.RangePhoton(channel, conf.W_GAMMA, 2012, photon.WP_MEDIUM, 1, 1, 1, 1, 1, 1);
  cut = cut && fullcut.RangeDeltaR(conf.W_GAMMA);
  TCut cutPt("phoEt>15 && phoEt<45");
  cut = cut && cutPt;
  if (channel==conf.ELECTRON) cut = cut && fullcut.RangeZmassWindowCut();

  fch.SetDefaultFullChainParameters(anPars,"WMt");
  anPars.nKinBins=40;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+5*ib;

  anPars.cutAdd=cut;

  anPars.blind[channel][conf.W_GAMMA]=conf.UNBLIND;
  // UNBLIND is to open the VERY_PRELIMINARY files, blinding cut is further applied for plotting
  // blinding cut: fullcut.RangeBlinding(conf.BLIND_COMBINED);
  int selStage=conf.VERY_PRELIMINARY;

  anPars.channel=channel;
  anPars.vgamma=conf.W_GAMMA;
  AuxPrepareYields(anPars,0,selStage,1,1); 

}//end of QuickPrepareYields_WMt

void QuickPrepareYields_EtoGammaEnriched(){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  fch.SetDefaultFullChainParameters(anPars,"Mpholep1");
  
  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.channel=conf.ELECTRON;
  anPars.vgamma=conf.W_GAMMA;
  anPars.varKin="Mpholep1";
  anPars.nKinBins=40;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+5*ib;//0-200 GeV


  int selStage=conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT;

  // anPars.cutAdd="phoEt>15";
  //AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_"); 
  
  /*
  anPars.blind[conf.ELECTRON][conf.W_GAMMA]=conf.UNBLIND;
  anPars.channel=conf.ELECTRON;
  anPars.vgamma=conf.W_GAMMA;
  anPars.varKin="Mpholep1";
  anPars.nKinBins=40;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+5*ib;//0-200 GeV


  int selStage=conf.FULLY;
  anPars.cutAdd="phoEt>15";
  AuxPrepareYields(anPars,0,selStage,1,1,"_pt15to500_"); 
  */
  
  TCut cAdd[13];
  TString strAdd[13];
  float etogScaleB[13];
  float etogScaleE[13];

  cAdd[1]="phoEt>15 && phoEt<20"; strAdd[1]="_pt15to20_";
  cAdd[2]="phoEt>20 && phoEt<25"; strAdd[2]="_pt20to25_";
  cAdd[3]="phoEt>25 && phoEt<30"; strAdd[3]="_pt25to30_";
  cAdd[4]="phoEt>30 && phoEt<35"; strAdd[4]="_pt30to35_";
  cAdd[5]="phoEt>35 && phoEt<45"; strAdd[5]="_pt35to45_";
  cAdd[6]="phoEt>45 && phoEt<55"; strAdd[6]="_pt45to55_";
  cAdd[7]="phoEt>55 && phoEt<65"; strAdd[7]="_pt55to65_";
  cAdd[8]="phoEt>65 && phoEt<75"; strAdd[8]="_pt65to75_";
  cAdd[9]="phoEt>75 && phoEt<85"; strAdd[9]="_pt75to85_";
  cAdd[10]="phoEt>85 && phoEt<95"; strAdd[10]="_pt85to95_";
  cAdd[11]="phoEt>95 && phoEt<120"; strAdd[11]="_pt95to120_";
  cAdd[12]="phoEt>120 && phoEt<500"; strAdd[12]="_pt120to500_";

  TFile* f = new TFile("../WGammaOutput/ELECTRON_WGamma/YieldsAndBackground/DDEtoGamma_WGamma_phoEt_.root");
  TH1F* hScaleB = (TH1F*)f->Get("hScale_ONEDI_BARREL");
  TH1F* hScaleE = (TH1F*)f->Get("hScale_ONEDI_ENDCAP");

  for (int ib=1; ib<=12; ib++){
    anPars.cutAdd=cAdd[ib];
    AuxPrepareYields(anPars,0,selStage,1,1,strAdd[ib]);
    //AuxPrepareYields(anPars,0,selStage,hScaleB->GetBinContent(ib+1),hScaleE->GetBinContent(ib+1),strAdd[ib]);
  }

}//end of QuickPrepareYields_EtoGammaEnriched

void QuickPrepareYields_EtoGammaEnriched(int channel){

  FullChain fch;
  TConfiguration::AnalysisParameters anPars;

  std::cout<<"configfile="<<anPars.configfile<<std::endl;

  TConfiguration conf;
  fch.SetDefaultFullChainParameters(anPars,"Mpholep1");
  
  anPars.blind[channel][conf.W_GAMMA]=conf.UNBLIND;
  anPars.channel=channel;
  anPars.vgamma=conf.W_GAMMA;
  anPars.varKin="Mpholep1";
  anPars.nKinBins=40;
  anPars.kinBinLims=new float[anPars.nKinBins+1];
  for (int ib=0; ib<anPars.nKinBins+1; ib++)
    anPars.kinBinLims[ib]=0+5*ib;//0-200 GeV


  int selStage=conf.PRELIMINARY_FOR_E_TO_GAMMA_WITH_PSV_CUT;

   anPars.cutAdd="phoEt>15 && phoEt<45";
   AuxPrepareYields(anPars,0,selStage,1,1); 
  


}//end of QuickPrepareYields_EtoGammaEnriched(int channel)
