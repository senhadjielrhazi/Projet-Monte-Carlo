/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		VBAInterface.h												 //
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




#ifndef _VBAINTERFACE_
#define _VBAINTERFACE_

void __stdcall call_europeen_BS(double S0, double maturity, double strike, double volatility, double taux, double *result);
void __stdcall call_europeen_MC(double S0, double maturity, double strike, double volatility, double taux, long simulations, double *resultat);
void __stdcall call_americain_CRR(double S0, double maturity, double strike, double volatility, double taux, long profondeur, double *resultat);
void __stdcall call_americain_malliavin(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat);
void __stdcall call_americain_malliavin_localisant(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat); 
void __stdcall delta_call_americain(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat);

void __stdcall put_europeen_BS(double S0, double maturity, double strike, double volatility, double taux, double *result);
void __stdcall put_europeen_MC(double S0, double maturity, double strike, double volatility, double taux, long simulations, double *resultat);
void __stdcall put_americain_CRR(double S0, double maturity, double strike, double volatility, double taux, long profondeur, double *result);
void __stdcall put_americain_malliavin(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *result);
void __stdcall put_americain_malliavin_localisant(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat);
void __stdcall delta_put_americain(double S0, double maturity, double strike, double volatility, double taux, long profondeur, long simulations, double *resultat);

#endif//_VBAINTERFACE_