#include "RapidMomentumSmear.h"

#include <iostream>

#include "TMath.h"
#include "TRandom.h"

TLorentzVector RapidMomentumSmear::smearMomentum(TLorentzVector p) {

    double kp, kptx, kpty, norm, smear;
    kp = p.P();
    kptx = p.Px()/p.Pz();
    kpty = p.Py()/p.Pz();
    int iHist=0;
    while( true ) {
	    if( iHist==thresholds_.size() ) break;
	    if( kp < thresholds_[iHist] ) break;
	    ++iHist;
    }
    smear = histos_[iHist]->GetRandom()*kp;
    //smear = 1.0*ran.Gaus(0,1)*dGraph->Eval(1000*kp)*kp;
    kp += smear;

    // smear the slopes
    double slope_smear = TMath::Sqrt(TMath::Power(6.2e-5,2) + TMath::Power(2.1e-3/kp,2)); //TODO
    kptx += slope_smear*gRandom->Gaus(1,0);
    kpty += slope_smear*gRandom->Gaus(1,0);
    norm = sqrt(1 + kptx*kptx + kpty*kpty);

    p.SetXYZM( kptx*kp/norm, kpty*kp/norm, kp/norm, p.M() );
    return p;

}

void RapidMomentumSmear::init(std::vector<double> thresholds, std::vector<TH1F*> histos) {
	if(thresholds.size() < histos.size() - 1) {
		std::cout << "WARNING in RapidMomentumSmear::init : too many histograms provided. Number of histograms should be one more than number of thresholds." << std::endl;
		std::cout << "                                      excess histograms ignored." << std::endl;

		while(thresholds.size() < histos.size() - 1) {
			histos.pop_back();
		}
	} else if(thresholds.size() > histos.size() - 1) {
		std::cout << "WARNING in RapidMomentumSmear::init : too few histograms provided. Number of histograms should be one more than number of thresholds." << std::endl;
		std::cout << "                                      excess thresholds ignored." << std::endl;

		while(thresholds.size() > histos.size() - 1) {
			thresholds.pop_back();
		}
	}

	thresholds_ = thresholds;
	histos_ = histos;
}