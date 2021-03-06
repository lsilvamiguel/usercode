void TTMnS2 () {
  
  const int nbins=400;
  
  TFile * QCD[8];     
  QCD[0] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_30-50_plots.root");
  QCD[1] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_50-80_plots.root");
  QCD[2] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_80-120_plots.root");
  QCD[3] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_120-170_plots.root");
  QCD[4] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_170-230_plots.root");
  QCD[5] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_230-300_plots.root");
  QCD[6] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_300-380_plots.root");
  QCD[7] = new TFile("./root/TagMassVsTagNTrk_EXTRA_QCD_380-incl_plots.root");
  double QCDxs[8] = { 155929000., 20938850., 2949713., 499656., 100995.,  23855., 6391., 2821.};
  // For extra QCD:
  double NQCD[8] = { 130000., 133096., 281096, 164000., 436000., 346000., 418000., 406000. };
  // For normal QCD:
  // double NQCD[8] = { 86000., 78000., 104000., 96000., 100000., 102000., 112000., 102000.};
  
  double Lumfactor = 100000; // 100/fb of luminosity assumed in histograms
  
  TH1D * H = dynamic_cast<TH1D*> (QCD[0]->Get("MTS2_0"));

  double minx=H->GetBinLowEdge(1);
  double maxx=nbins*H->GetBinWidth(1)+minx;
  TH1D * MT[8]; 
  TH1D * MN[8]; 
  for ( int i=0; i<8; i++ ) {
    char Ename[20];
    char Pname[20];
    sprintf ( Ename, "MTS2_%d", i );
    sprintf ( Pname, "MNS2_%d", i );
    MT[i] = new TH1D ( Ename, Ename, nbins, minx, maxx );
    MN[i] = new TH1D ( Pname, Pname, nbins, minx, maxx );
  }

  for ( int iN=0; iN<8; iN++ ) {
    char nameT[20];
    char nameN[20];
    sprintf ( nameT, "MTS2_%d", iN );
    sprintf ( nameN, "MNS2_%d", iN );
    double tot1[nbins]={0.};
    double s2_tot1[nbins]={0.};
    double tot2[nbins]={0.};
    double s2_tot2[nbins]={0.};
    for ( int i=0; i<8; i++ ) {
      MTtmp = dynamic_cast<TH1D*>(QCD[i]->Get(nameT));
      MNtmp = dynamic_cast<TH1D*>(QCD[i]->Get(nameN));
      cout << "Got histogram " << iN << endl;
      for ( int ibin=1; ibin<=nbins; ibin++ ) {
	double t1=MTtmp->GetBinContent(ibin);
	tot1[ibin-1]+=t1*QCDxs[i]/NQCD[i]*Lumfactor;
	s2_tot1[ibin-1]+=t1*pow(QCDxs[i]/NQCD[i]*Lumfactor,2);
	double t2=MNtmp->GetBinContent(ibin);
	tot2[ibin-1]+=t2*QCDxs[i]/NQCD[i]*Lumfactor;
	s2_tot2[ibin-1]+=t2*pow(QCDxs[i]/NQCD[i]*Lumfactor,2);
      }
    }
    if ( iN==7 ) {  // Fix split histogram for Nt=9 && >9
      sprintf ( nameT, "MTS2_%d", iN+1 );
      sprintf ( nameN, "MNS2_%d", iN+1);
      for ( int i=0; i<8; i++ ) {
	MTtmp = dynamic_cast<TH1D*>(QCD[i]->Get(nameT));
	MNtmp = dynamic_cast<TH1D*>(QCD[i]->Get(nameN));
	for ( int ibin=1; ibin<=nbins; ibin++ ) {
	  double t1=MTtmp->GetBinContent(ibin);
	  tot1[ibin-1]+=t1*QCDxs[i]/NQCD[i]*Lumfactor;
	  s2_tot1[ibin-1]+=t1*pow(QCDxs[i]/NQCD[i]*Lumfactor,2);
	  double t2=MNtmp->GetBinContent(ibin);
	  tot2[ibin-1]+=t2*QCDxs[i]/NQCD[i]*Lumfactor;
	  s2_tot2[ibin-1]+=t2*pow(QCDxs[i]/NQCD[i]*Lumfactor,2);
	}
      }
    }
    // Now renormalize histograms
    // --------------------------
    double i1=0.;
    double i2=0.;
    for ( int ibin=1; ibin<=nbins; ibin++ ) {
      i1+=tot1[ibin-1];
      i2+=tot2[ibin-1];
    }
    for ( int ibin=1; ibin<=nbins; ibin++ ) {
      MT[iN]->SetBinContent(ibin,tot1[ibin-1]/i1);
      MT[iN]->SetBinError(ibin,sqrt(s2_tot1[ibin-1])/i1);
      MN[iN]->SetBinContent(ibin,tot2[ibin-1]/i2);
      MN[iN]->SetBinError(ibin,sqrt(s2_tot2[ibin-1])/i2);
    }
  } // iN
  
  TCanvas * b = new TCanvas ("b", "Total Tag masses", 700, 700 );
  b->Divide(2,2);
  
  b->cd(1);
  b->GetPad(1)->SetLogy();
  MT[0]->SetMarkerStyle(20);
  MT[0]->SetMarkerSize(0.4);
  MT[0]->SetMarkerColor(kBlue);
  MT[0]->SetLineColor(kBlue);
  MT[0]->DrawCopy("PE");
  MT[1]->SetMarkerStyle(21);
  MT[1]->SetMarkerSize(0.4);
  MT[1]->SetMarkerColor(kRed);
  MT[1]->SetLineColor(kRed);
  MT[1]->DrawCopy("PESAME");
  MT[2]->SetMarkerStyle(24);
  MT[2]->SetMarkerSize(0.4);
  MT[2]->SetMarkerColor(kBlack);
  MT[2]->SetLineColor(kBlack);
  MT[2]->DrawCopy("PESAME");
  MT[3]->SetMarkerStyle(25);
  MT[3]->SetMarkerSize(0.4);
  MT[3]->SetMarkerColor(kGreen);
  MT[3]->SetLineColor(kGreen);
  MT[3]->DrawCopy("PESAME");
  b->cd(2);
  b->GetPad(2)->SetLogy();
  MT[4]->SetMarkerStyle(20);
  MT[4]->SetMarkerSize(0.4);
  MT[4]->SetMarkerColor(kBlue);
  MT[4]->SetLineColor(kBlue);
  MT[4]->DrawCopy("PE");
  MT[5]->SetMarkerStyle(21);
  MT[5]->SetMarkerSize(0.4);
  MT[5]->SetMarkerColor(kRed);
  MT[5]->SetLineColor(kRed);
  MT[5]->DrawCopy("PESAME");
  MT[6]->SetMarkerStyle(24);
  MT[6]->SetMarkerSize(0.4);
  MT[6]->SetMarkerColor(kBlack);
  MT[6]->SetLineColor(kBlack);
  MT[6]->DrawCopy("PESAME");
  MT[7]->SetMarkerStyle(25);
  MT[7]->SetMarkerSize(0.4);
  MT[7]->SetMarkerColor(kGreen);
  MT[7]->SetLineColor(kGreen);
  MT[7]->DrawCopy("PESAME");
  b->cd(3);
  b->GetPad(3)->SetLogy();
  MN[0]->SetMarkerStyle(20);
  MN[0]->SetMarkerSize(0.4);
  MN[0]->SetMarkerColor(kBlue);
  MN[0]->SetLineColor(kBlue);
  MN[0]->DrawCopy("PE");
  MN[1]->SetMarkerStyle(21);
  MN[1]->SetMarkerSize(0.4);
  MN[1]->SetMarkerColor(kRed);
  MN[1]->SetLineColor(kRed);
  MN[1]->DrawCopy("PESAME");
  MN[2]->SetMarkerStyle(24);
  MN[2]->SetMarkerSize(0.4);
  MN[2]->SetMarkerColor(kBlack);
  MN[2]->SetLineColor(kBlack);
  MN[2]->DrawCopy("PESAME");
  MN[3]->SetMarkerStyle(25);
  MN[3]->SetMarkerSize(0.4);
  MN[3]->SetMarkerColor(kGreen);
  MN[3]->SetLineColor(kGreen);
  MN[3]->DrawCopy("PESAME");
  b->cd(4);
  b->GetPad(4)->SetLogy();
  MN[4]->SetMarkerStyle(20);
  MN[4]->SetMarkerSize(0.4);
  MN[4]->SetMarkerColor(kBlue);
  MN[4]->SetLineColor(kBlue);
  MN[4]->DrawCopy("PE");
  MN[5]->SetMarkerStyle(21);
  MN[5]->SetMarkerSize(0.4);
  MN[5]->SetMarkerColor(kRed);
  MN[5]->SetLineColor(kRed);
  MN[5]->DrawCopy("PESAME");
  MN[6]->SetMarkerStyle(24);
  MN[6]->SetMarkerSize(0.4);
  MN[6]->SetMarkerColor(kBlack);
  MN[6]->SetLineColor(kBlack);
  MN[6]->DrawCopy("PESAME");
  MN[7]->SetMarkerStyle(25);
  MN[7]->SetMarkerSize(0.4);
  MN[7]->SetMarkerColor(kGreen);
  MN[7]->SetLineColor(kGreen);
  MN[7]->DrawCopy("PESAME");
  b->Print("./ps/TTMnS2.ps");
  
  // Close files
  // -----------
  TFile * File = new TFile ("TTMnS2.root","RECREATE");
  File->cd();
  for ( int i=0; i<8; i++ ) {
    MT[i]->Write();
    MN[i]->Write();
  }
  File->Close();
  
}
