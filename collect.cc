void collect() {
	bool both=false;
	TCanvas *c1 = new TCanvas();
	TFile *f1 = TFile::Open("He_96.000000CO2_4.000000_275.000000_Torr.root");
	f1->ls();
	TGraph *g1a = (TGraph*)f1->Get("Graph;1");
	TGraph *g1b = (TGraph*)f1->Get("Graph;2");
	g1a->Draw("*AL");
	if(both)g1b->Draw("SAME*L");
	g1a->SetMarkerColor(kRed);
	g1b->SetMarkerColor(kRed);
	g1a->SetLineColor(kRed);
	g1b->SetLineColor(kRed);
	g1a->SetMarkerStyle(20);
	g1b->SetMarkerStyle(21);
//
	TFile *f2 = TFile::Open("He_96.000000CO2_4.000000_450.000000_Torr.root");
	f2->ls();
	TGraph *g2a = (TGraph*)f2->Get("Graph;1");
	TGraph *g2b = (TGraph*)f2->Get("Graph;2");
	g2a->Draw("SAME*L");
	if(both)g2b->Draw("SAME*L");
	g2a->SetMarkerColor(kGreen);
	g2b->SetMarkerColor(kGreen);
	g2a->SetLineColor(kGreen);
	g2b->SetLineColor(kGreen);
	g2a->SetMarkerStyle(20);
	g2b->SetMarkerStyle(21);
//
	TFile *f3 = TFile::Open("He_96.000000CO2_4.000000_580.000000_Torr.root");
	f3->ls();
	TGraph *g3a = (TGraph*)f3->Get("Graph;1");
	TGraph *g3b = (TGraph*)f3->Get("Graph;2");
	g3a->Draw("SAME*L");
	if(both)g3b->Draw("SAME*L");
	g3a->SetMarkerColor(kBlue);
	g3b->SetMarkerColor(kBlue);
	g3a->SetLineColor(kBlue);
	g3b->SetLineColor(kBlue);
	g3a->SetMarkerStyle(20);
	g3b->SetMarkerStyle(21);
//
	g1a->SetMaximum(10000);
	g1a->SetTitle(";MM bias (V);Gain");
	c1->SetLogy();
	TLegend *leg = new TLegend(0.1,0.7,0.3,0.9);
	leg->AddEntry(g1a,"275 Torr");
	leg->AddEntry(g2a,"450 Torr");
	leg->AddEntry(g3a,"580 Torr");
	leg->Draw("SAME");
}
