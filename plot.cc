void plot() {
	TCanvas *c = new TCanvas();
//	c->SetLogy();
	double V[10] = {200,250,300,350,400,450,500,550,600,650};
	double G1[10] = {13.5,30.3,63.6,125.9,236.4,420.5,707.4,1140,1771.1,2625.};
	double G2[10] = {13.0,29.4,63.0,126.1,244.4,446.6,763.4,1303.5,2094.3,3112.9};
	double G3[10] = {12.5,28.5,61.1,126.7,254.1,468.7,838.9,1421.9,2346,3772.3};
	double G4[10] = {12.0,27.4,59.1,127.1,250.5,492.5,881.6,1566.3,2653.3,4220.5};
	double G5[10] = {11.5,26.4,57.4,123.8,256.2,511.4,922.3,1716.8,2889.6,4804};
	TGraph *g1 = new TGraph(10,V,G1);
	TGraph *g2 = new TGraph(10,V,G2);
	TGraph *g3 = new TGraph(10,V,G3);
	TGraph *g4 = new TGraph(10,V,G4);
	TGraph *g5 = new TGraph(10,V,G5);
	g1->Draw();
	g1->SetMaximum(3000);
	g2->Draw("SAME");
	g3->Draw("SAME");
	g4->Draw("SAME");
	g5->Draw("SAME");

	g1->SetLineColor(kRed-2);
	g1->SetLineStyle(1);
	g2->SetLineColor(kRed-1);
	g2->SetLineStyle(2);
	g3->SetLineColor(kRed);
	g3->SetLineStyle(3);
	g4->SetLineColor(kRed+1);
	g4->SetLineStyle(4);
	g5->SetLineColor(kRed+1);
	g5->SetLineStyle(5);

	TLegend *leg = new TLegend(0.1,0.7,0.3,0.9);
	leg->AddEntry(g1,"280 Torr");
	leg->AddEntry(g2,"298 Torr");
	leg->AddEntry(g3,"316 Torr");
	leg->AddEntry(g4,"334 Torr");
	leg->AddEntry(g5,"352 Torr");
	leg->Draw("SAME");
}
