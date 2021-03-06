{
  gROOT->ProcessLine(".x ../Include/rootlogon.C");

  std::cout<<"loaded ../Include/rootlogon.C"<<std::endl;

  TConfiguration conf;

  bool noClosure[2][2][2]; //channel, vgamma, templ
  noClosure[conf.MUON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  noClosure[conf.MUON][conf.W_GAMMA][conf.TEMPL_CHISO]=0;
  noClosure[conf.MUON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  noClosure[conf.MUON][conf.Z_GAMMA][conf.TEMPL_CHISO]=0;
  noClosure[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_SIHIH]=1;
  noClosure[conf.ELECTRON][conf.W_GAMMA][conf.TEMPL_CHISO]=0;
  noClosure[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_SIHIH]=1;
  noClosure[conf.ELECTRON][conf.Z_GAMMA][conf.TEMPL_CHISO]=0;

  TConfiguration::AnalysisParameters anPars;
  FullChain fch; 
  fch.SetDefaultFullChainParameters(anPars, "phoEt");

  for (int ch=0; ch<=1; ch++){
    for (int vg=0; vg<=1; vg++){
      anPars.blind[ch][vg]=TConfiguration::UNBLIND;
    }
  }


  anPars.cutAdd="1";


  for (int ch=0; ch<=1; ch++){
    for (int vg=0; vg<=1; vg++){
      for (int tm=0; tm<=1; tm++){
        if (!noClosure[ch][vg][tm]){
          anPars.channel=ch;//MUON, ELECTRON
          anPars.vgamma=vg;//W_GAMMA, Z_GAMMA
          anPars.templFits=tm;
          AuxTemplatesSystSidebandVariation(anPars,1);// 1 - isMCclosure
          //AuxTemplates(anPars,1);// 1 - isMCclosure
	  //  AuxSubtractBackground(anPars);
        }//end of if (!noClosure[ch][vg][tm])
      }// end of loop over tm
      if (!noClosure[ch][vg][0] && !noClosure[ch][vg][1]){
          anPars.templFits=tm;
	  // AuxSubtractBackground(anPars);
      }
    }//end of loop over vg

  }//end of loop over ch
}
