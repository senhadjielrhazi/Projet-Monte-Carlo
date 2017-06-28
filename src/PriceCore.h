/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		PriceCore.h 												 //
//																			 //
//	Contents:																 //
//																			 //
//																			 //
//	History :	O. Senhadji EL RHAZI && Abdelaziz  SAOUDI && Wassim MNEJA	 //
//  Ecole   :   Universite Pierre et Marie Curie (UPMC)						 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
//																			 //
//	PriceCore est la classe de pricing des calls et puts.					 //
//	Paris VI Projet Monte-Carlo 											 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
/**********************************************************_FILE_HEADER_END_**/




#ifndef _PRICECORE_
#define _PRICECORE_

class PriceCore  
{
public:
	
	//Cas Européen
	double Call_Europeen_BS(double S0, double maturity, double strike, double volatility, double taux);
	double Call_Europeen(double S0, double maturity, double strike, double volatility,double taux, int simulations);
	double Put_Europeen_BS(double S0, double maturity, double strike, double volatility, double taux);
	double Put_Europeen(double S0, double maturity, double strike, double volatility, double taux, int simulations);
	
	//Cas Américain par arbre binomial
	double AmericanCRR(double S0, double maturity, double strike, double volatility, double taux, int profondeur, double OptionType);
	
	//Cas Américain par Malliavin
	double EsperanceCondMalliavin(double strike, double S, int ordre, double maturity, double S0, double taux, double volatility, int profondeur, int simulations);
	double CallAmericainMalliavin(double S0, double maturity,double strike,double volatility,double taux,int profondeur, int simulations);
	double PutAmericainMalliavin(double S0, double maturity,double strike,double volatility,double taux,int profondeur, int simulations);
	double PutAmericainMalliavinLocalisant(double S0, double maturity,double strike,double volatility,double taux,int profondeur, int simulations);
	double CallAmericainMalliavinLocalisant(double S0, double maturity,double strike,double volatility,double taux,int profondeur, int simulations);
	
	//Delta Americain
	double DeltaCallAmericain(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations);
	double DeltaPutAmericain(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations);
	
	PriceCore();
	virtual ~PriceCore();

};

#endif//_PRICECORE_
