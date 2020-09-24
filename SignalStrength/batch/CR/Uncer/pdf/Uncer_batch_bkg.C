#define num 103
#define pi 3.1415926
void run( TFile*file,TString cut1,TString tag,bool turn){
	TString name=file->GetName();
	TTree*tree=(TTree*)file->Get("demo");     
	Double_t scalef,pileupWeight,pweight[703],Mjj,jet1eta,jet2eta;
	tree->SetBranchAddress("scalef",&scalef);
	tree->SetBranchAddress("pileupWeight",&pileupWeight);
	tree->SetBranchAddress("pweight",pweight);
	tree->SetBranchAddress("Mjj",&Mjj);
	tree->SetBranchAddress("jet1eta",&jet1eta);
	tree->SetBranchAddress("jet2eta",&jet2eta);
	TTreeFormula *tformula=new TTreeFormula("formula", cut1, tree);
	double actualWeight[num];
	TH1D*th1[num];
	TString th1name[num];
	for(Int_t i=0;i<num;i++){
		th1name[i]=Form("hist_%d",i);
		th1[i] = new TH1D(th1name[i],th1name[i],1,0,1);
		th1[i]->Sumw2(); 
	}
	cout<<tag<<" "<<name<<endl;
	for(int k=0;k<tree->GetEntries();k++){
		tree->GetEntry(k);
                double detajj=fabs(jet1eta-jet2eta);
		int p=0;
		int init;
		if(tag.Contains("16")&&name.Contains("EWK"))
			init=9;//16 ewk
		else if(tag.Contains("16")&&name.Contains("EWK")==0)
			init=0;//16 qcd
		else if(( tag.Contains("17")||tag.Contains("18"))&&name.Contains("EWK"))
			init=45;//17 ewk,18 ewk

		if (  tformula->EvalInstance() ){
			for(Int_t i=init;i<(num+init);i++){
				actualWeight[p]=scalef*pweight[i]*pileupWeight;
				if(Mjj>=150&&Mjj<400)th1[p]->Fill(0.5,actualWeight[p]);//0~1, 2.5~4.5 and 500~800
				p++;
			}
		}
	}
	TFile*fout;
	if(name.Contains("EWK")==0)
		fout= new TFile("hist_qcd_pdf"+tag+"CR.root","recreate");
	else{
		if(turn==1)	fout= new TFile("hist_Sigout_pdf"+tag+"CR.root","recreate");
		else  fout= new TFile("hist_ewk_pdf"+tag+"CR.root","recreate");
	}
	fout->cd();
	for(Int_t i=0;i<num;i++){
		th1[i]->Write();
	}
	fout->Close();
	for(Int_t i=0;i<num;i++){
		delete  th1[i];
	}
}
int Uncer_batch_bkg(){
	TString GenLEPmu  = "genlep==13 && genlep1pt>20 && genlep2pt>20 && fabs(genlep1eta)<2.4 && fabs(genlep2eta)<2.4&& genmassVlep>70 && genmassVlep<110";
	TString GenLEPele = "genlep==11 && genlep1pt>25 && genlep2pt>25 && fabs(genlep1eta)<2.5 && fabs(genlep2eta)<2.5&& genmassVlep>70 && genmassVlep<110";
	TString GenPhoton = "genphotonet>20 && ( (fabs(genphotoneta)<2.5&&fabs(genphotoneta)>1.566) || (fabs(genphotoneta)<1.4442) )";
	TString GenJet = "genjet1pt>30 && genjet2pt>30 && fabs(genjet1eta)<4.7 && fabs(genjet2eta)<4.7";
	TString GenDr = "gendrjj>0.5 && gendrla1>0.7 && gendrla2>0.7 && gendrj1a>0.5 && gendrj2a>0.5 && gendrj1l>0.5 && gendrj2l>0.5 && gendrj1l2>0.5 && gendrj2l2>0.5";
	TString GenControlRegion = "genMjj > 150 && genMjj<400 && genZGmass>100";

	TString LEPmu = "lep==13 &&  ptlep1 > 20. && ptlep2 > 20.&& fabs(etalep1) < 2.4 &&abs(etalep2) < 2.4 && nlooseeles==0 && nloosemus <3  && massVlep >70. && massVlep<110";
	TString LEPele = "lep==11  && ptlep1 > 25. && ptlep2 > 25.&& fabs(etalep1) < 2.5 &&abs(etalep2) < 2.5 && nlooseeles < 3 && nloosemus == 0  && massVlep >70. && massVlep<110";
	TString photon = "photonet>20 &&( (fabs(photoneta)<2.5&&fabs(photoneta)>1.566) || (fabs(photoneta)<1.4442) )";
	TString jet = "jet1pt> 30 && jet2pt > 30 && fabs(jet1eta)< 4.7 && fabs(jet2eta)<4.7";
	TString Pi=Form("%f",pi);
	TString dr = "( sqrt((jet1eta-jet2eta)*(jet1eta-jet2eta)+(2*"+Pi+"-fabs(jet1phi-jet2phi))*(2*"+Pi+"-fabs(jet1phi-jet2phi)))>0.5 ||sqrt((jet1eta-jet2eta)*(jet1eta-jet2eta)+(fabs(jet1phi-jet2phi))*(fabs(jet1phi-jet2phi)))>0.5) && drla>0.7 && drla2>0.7 && drj1a>0.5 && drj2a>0.5 && drj1l>0.5&&drj2l>0.5&&drj1l2>0.5&&drj2l2>0.5";
	TString ControlRegion = "Mjj>150 && Mjj<400 && Mva>100";
        vector<TString> tag={"16","17","18"};
        
        TFile*file1[3];
	TString dir1="/eos/user/y/yian/2016legacy/";     
	TString dir2="/eos/user/y/yian/2017cutla/";     
	TString dir3="/eos/user/y/yian/2018cutla/";     
	file1[0]=new TFile(dir1+"cutla-outZA_pweight.root");
	file1[1]=new TFile(dir2+"cutla-outZA-pweight.root");
	file1[2]=new TFile(dir3+"cutla-outZA_pweight.root");

	TString dir="/afs/cern.ch/user/y/yian/work/PKU-Cluster/Unfolding/produce/";     
        TFile*file2[3];
	file2[0]=new TFile(dir+"unfold_16outZA-EWK.root");
	file2[1]=new TFile(dir+"unfold_17outZA-EWK-pweight.root");
	file2[2]=new TFile(dir+"unfold_18outZA-EWK-pweight.root");

        for(int i=0;i<tag.size();i++){
		if(tag[i].Contains("17")){
			GenJet = " ( (!(fabs(genjet2eta)<3.14 && fabs(genjet2eta)>2.65) && !(fabs(genjet1eta)<3.14 && fabs(genjet1eta)>2.65) &&  genjet1pt<50 && genjet2pt<50 && genjet1pt>30 && genjet2pt>30 && fabs(genjet1eta)< 4.7 && fabs(genjet2eta)<4.7) || (genjet1pt>50 && genjet2pt>50 && fabs(genjet1eta)< 4.7 && fabs(genjet2eta)<4.7) ) ";
			jet=" ( (!(fabs(jet2eta)<3.14 && fabs(jet2eta)>2.65) && !(fabs(jet1eta)<3.14 && fabs(jet1eta)>2.65) &&  jet1pt<50 && jet2pt<50 && jet1pt>30 && jet2pt>30 && fabs(jet1eta)< 4.7 && fabs(jet2eta)<4.7) || (jet1pt>50 && jet2pt>50 && fabs(jet1eta)< 4.7 && fabs(jet2eta)<4.7) ) ";
		}
		else{
			GenJet = "genjet1pt>30 && genjet2pt>30 && fabs(genjet1eta)<4.7 && fabs(genjet2eta)<4.7";
			jet = "jet1pt> 30 && jet2pt > 30 && fabs(jet1eta)< 4.7 && fabs(jet2eta)<4.7";
		}
		TString Gen= "(" + GenLEPmu +"||"+GenLEPele+")"+"&&"+GenPhoton+"&&"+GenJet+"&&"+GenDr+"&&"+GenControlRegion;
		TString Reco= "(("+LEPmu+")||("+LEPele+"))"+"&&"+photon+"&&"+dr+"&&"+jet+"&&"+ControlRegion;
		TString cut1 ="("+Reco+")&&("+Gen+")";
		TString cut2 ="(("+Reco+")&& !("+Gen+"))";
		if(tag[i].Contains("16"))	
			run(file1[i],Reco,tag[i],0);
		run(file2[i],cut1,tag[i],0);
		run(file2[i],cut2,tag[i],1);
	}
	return 1;
}