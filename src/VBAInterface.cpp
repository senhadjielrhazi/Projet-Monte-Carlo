/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		VBAInterface.cpp											 //
//																			 //
//	Contents:																 //
//																			 //
//																			 //
//	History :	O. Senhadji EL RHAZI && Abdelaziz  SAOUDI && Wassim MNEJA	 //
//  Ecole   :   Universite Pierre et Marie Curie (UPMC)						 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
//																			 //
//	VBAInterface est l'interface avec le script VBA et Excel.				 //
//	Paris VI Projet Monte-Carlo 											 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
/**********************************************************_FILE_HEADER_END_**/



#include "VBAInterface.h"
#include "PriceCore.h"
#include "MathUtile.h" 


void __stdcall call_europeen_BS(double S0, double maturity, double strike, double volatility, double taux, double *resultat)
{ 
	PriceCore PrixMC;
	*resultat=PrixMC.Call_Europeen_BS(S0,maturity,strike,volatility,taux);
}

void __stdcall call_europeen_MC(double S0, double maturity, double strike, double volatility, double taux, long simulations, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.Call_Europeen(S0,maturity,strike,volatility,taux, simulations);
}

void __stdcall call_americain_CRR(double S0, double maturity, double strike, double volatility, double taux, long profondeur, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.AmericanCRR(S0,maturity,strike,volatility,taux,profondeur,1);
}

void __stdcall call_americain_malliavin(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.CallAmericainMalliavin(S0,maturity,strike,volatility,taux, profondeur,simulations);
}

void __stdcall call_americain_malliavin_localisant(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat) 
{
	PriceCore PrixMC;
	*resultat=PrixMC.CallAmericainMalliavinLocalisant(S0,maturity,strike,volatility,taux,profondeur,simulations);
}


void __stdcall put_europeen_BS(double S0, double maturity, double strike, double volatility, double taux, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.Put_Europeen_BS(S0,maturity,strike,volatility,taux);
}

void __stdcall put_europeen_MC(double S0, double maturity, double strike, double volatility, double taux, long simulations, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.Put_Europeen(S0,maturity,strike,volatility,taux,simulations);
}


void __stdcall put_americain_CRR(double S0, double maturity, double strike, double volatility, double taux, long profondeur, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.AmericanCRR(S0,maturity,strike,volatility,taux,profondeur,0);
	
}

void __stdcall put_americain_malliavin(double S0, double maturity,double strike, double volatility,double taux, long profondeur, long simulations, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.PutAmericainMalliavin(S0,maturity,strike,volatility,taux,profondeur,simulations);	
}


void __stdcall put_americain_malliavin_localisant(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.PutAmericainMalliavinLocalisant(S0,maturity,strike,volatility,taux,profondeur,simulations);	
}


void __stdcall delta_call_americain(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat)
{
	PriceCore PrixMC;
	*resultat=PrixMC.DeltaCallAmericain(S0,maturity,strike,volatility,taux,profondeur,simulations); 	
}

void __stdcall delta_put_americain(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat)
{	
	PriceCore PrixMC;
	*resultat= PrixMC.DeltaPutAmericain(S0,maturity,strike,volatility,taux,profondeur,simulations); 
}
