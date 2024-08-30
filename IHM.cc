//////////////////////////////////////////
// I hate Mondays (IHM)			//
// V1.0 J. Bishop Dec. 2020		//
// Calculates gas properties using	//
// V1.1					//
// Calculates gas dispersion properties	//
// GARFIELD++				//
//////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TMath.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include "MediumMagboltz.hh"
using namespace Garfield;
int main(int argc, char * argv[]) {
//
  std::cout<<std::endl<<std::endl<<"Gas drift generator"<<std::endl;
  int selection=0;
  while(selection!=1 && selection!=2) {
          selection=0;
	  std::cout<<"Please select gas mode: 1 for single gas or 2 for mixed gas"<<std::endl;
	  std::cin>>selection;
  }
  std::cout<<selection<<" selected"<<std::endl;

  MediumMagboltz* gas = new MediumMagboltz();
  TString gasid="";

  if(selection==1) {
  	std::cout<<"Type in name of gas"<<std::endl;
	std::cout<<"Examples: He,CO2,Methane,Isobutane..."<<std::endl;
	std::string gasname;
	std::cin>>gasname;
  	gas->SetComposition(gasname,100.);
	gasid=TString(gasname);

  }
  if(selection==2) {
  	std::cout<<"Type in name of gas 1"<<std::endl;
	std::cout<<"Examples: He,CO2,Methane,Isobutane..."<<std::endl;

	std::string gasname1;
	std::cin>>gasname1;
  	std::cout<<"Type in fraction (%) of gas 1"<<std::endl;
	double frac1;
	std::cin>>frac1;
  	std::cout<<"Type in name of gas 2"<<std::endl;
	std::string gasname2;
	std::cin>>gasname2;
  	std::cout<<"Type in fraction (%) of gas 2"<<std::endl;
	double frac2;
	std::cin>>frac2;
  	gas->SetComposition(gasname1,frac1,gasname2,frac2,"CH4",0.);
	gasid=TString(gasname1)+"_"+Form("%3.1f",frac1)+TString(gasname2)+"_"+Form("%3.1f",frac2);

  }
  std::cout<<"Type in pressure in Torr"<<std::endl;
  double pressure;
  std::cin>>pressure;
  std::cout<<"Type in field cage voltage (across the 13-cm gap) or negative voltage for MM bias or zero for a sweep"<<std::endl;
  double voltage;
  std::cin>>voltage;
  double efield=voltage/13.;
  bool gain=false;
  bool sweep=false;
  double meshheight=0.0128;;

  if(voltage==0) sweep=true;
  if(voltage<=0) {
	std::cout<<"Input mesh height in um"<<std::endl;
	std::cin>>meshheight;
	meshheight*=1e-4;//um->mm is 1e3, mm->cm is 10
	efield=fabs(voltage/meshheight);//MM gap
	if(!sweep)std::cout<<"Mesh height of "<<meshheight<<" cm gives an Efield of "<<efield<<" V/cm"<<std::endl;
	if(sweep)std::cout<<"Mesh height of "<<meshheight<<" cm"<<std::endl;
	gain=true;
  }
  if(voltage>0) std::cout<<"Electric field is "<<voltage/13.<<" V/cm"<<std::endl;
  if(sweep) std::cout<<"Doing a gain sweep"<<std::endl;
  gasid+=Form("mesh_%4.0f_um_%3.0f_Torr.root",meshheight*1e4,pressure);
  //Clean whitespace
  TString gasidclean="";
  for(int n=0;n<gasid.Length();n++) {
    TString thisletter(gasid(n,1));
    if(!thisletter.IsWhitespace()) gasidclean+=thisletter;
  }
  gasid=gasidclean;
  std::cout<<gasid<<std::endl;

  float temperature =  293.15;
  gas->SetTemperature(temperature);
  gas->SetPressure(pressure);
  gas->SetMaxElectronEnergy(200.);
  gas->SetFieldGrid(efield,efield,1,false);
//  gas->GenerateGasTable(1,false);
//  gas->Initialise(true);
  double vx,vy,vz,dl,dt,alpha;
  double eta,lor,vxerr,vyerr,vzerr,dlerr,dterr,alphaerr,etaerr,lorerr,alphatof;
  std::array<double, 6ul> difftens;
  /** Run Magboltz for a given electric field, magnetic field and angle.
    * \param[in] e electric field
    * \param[in] b magnetic field
    * \param[in] btheta angle between electric and magnetic field
    * \param[in] ncoll number of collisions (in multiples of 10<sup>7</sup>)
                 to be simulated
    * \param[in] verbose verbosity flag
    * \param[out] vx,vy,vz drift velocity vector
    * \param[out] dl,dt diffusion cofficients
    * \param[out] alpha Townsend cofficient
    * \param[out] eta attachment cofficient
    * \param[out] lor Lorentz angle
    * \param[out] vxerr,vyerr,vzerr errors on drift velocity
    * \param[out] dlerr,dterr errors on diffusion coefficients
    * \param[out] alphaerr,etaerr errors on Townsend/attachment coefficients
    * \param[out] lorerr error on Lorentz angle
    * \param[out] alphatof effective Townsend coefficient \f$(\alpha - \eta)\f$
    *             calculated using time-of-flight method
    * \param[out] difftens components of the diffusion tensor (zz, xx, yy, xz, yz, xy)
    */
    if(!sweep) {
       gas->RunMagboltz(efield,0,0,1,false,vx,vy,vz,dl,dt,alpha,eta,lor,vxerr,vyerr,vzerr,dlerr,dterr,alphaerr,etaerr,lorerr,alphatof,difftens);
       std::cout<<"\033[1m\033[32m"<<"Drift velocity: "<<vz<<" cm/ns"<<"\033[0m"<<std::endl;
       std::cout<<"\033[1m\033[32m"<<"Drift resolution: "<<dl<<" cm^1/2"<<"\033[0m"<<std::endl;
       std::cout<<"\033[1m\033[32m"<<"Time resolution: "<<dt<<" cm^1/2"<<"\033[0m"<<std::endl;
       if(gain) std::cout<<"\033[1m\033[32m"<<"Alpha: "<<alpha<<"+/-"<<alphaerr<<"\tMM gain: "<<TMath::Exp(alpha*meshheight)<<"\033[0m"<<std::endl;
    }
    bool pressuresweep=true;
    if(sweep) {
       std::ofstream outs;
       outs.open("sweepout.txt");
	outs<<"GASID\tPressure(Torr)\tMMBias(V)\talpha gain\talpha TOF gain"<<endl;
	int setvsweep=10;
	int setnsweep=10;
	int setpsweep=5;
	if(pressuresweep) setnsweep=setnsweep*setpsweep;
       const int nsweep=50;
       double gain_[nsweep]={0.};
       double gain1_[nsweep]={0.};
       double volts_[nsweep]={0.};
       double alpha_[nsweep]={0.};
       for(int n=0;n<nsweep;n++) {
          double volts=200+400*((n%setvsweep)/double(setvsweep));
          std::cout<<"Setting MM bias to "<<volts<<std::endl;
          efield=volts/meshheight;
	  if(pressuresweep) pressure=70+100*int(n/9.)*0.2;
	  gas->SetPressure(pressure);
          gas->RunMagboltz(efield,0,0,1,false,vx,vy,vz,dl,dt,alpha,eta,lor,vxerr,vyerr,vzerr,dlerr,dterr,alphaerr,etaerr,lorerr,alphatof,difftens);
          std::cout<<"\033[1m\033[32m"<<"Alpha: "<<alpha<<"\tMM gain: "<<TMath::Exp(alpha*meshheight)<<" for "<<volts<<"V \033[0m"<<std::endl;
          std::cout<<"\033[1m\033[32m"<<"AlphaTOF: "<<alphatof<<"+/-"<<alphaerr<<"\tMMTOF gain: "<<TMath::Exp(alphatof*meshheight)<<" for "<<volts<<"V \033[0m"<<std::endl;
          outs<<gasidclean<<"\t"<<pressure<<"\t"<<volts<<"\t"<<TMath::Exp(alpha*meshheight)<<"\t"<<TMath::Exp(alphatof*meshheight)<<std::endl;
          volts_[n]=volts;
          gain_[n]=TMath::Exp(alpha*meshheight);
          gain1_[n]=TMath::Exp(alphatof*meshheight);
          if(gain_[n]>100000 || std::isnan(alpha)) gain_[n]=0.;
          if(gain1_[n]>100000 || std::isnan(alphatof)) gain1_[n]=0.;
          alpha_[n]=alpha;
       }
       outs.close();
       TGraph *g1 = new TGraph(nsweep-1,volts_,gain_);
       TGraph *g2 = new TGraph(nsweep-1,volts_,gain1_);
       TGraph *g3 = new TGraph(nsweep-1,volts_,alpha_);
       g1->SetMarkerStyle(20);
       g2->SetMarkerStyle(43);
       TFile *fout = new TFile(gasid,"RECREATE");
       fout->cd();
       g1->Write();
       g2->Write();
       g3->Write();
       fout->Close();

    }
  return 0;
}
