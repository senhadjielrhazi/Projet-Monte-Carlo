/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		MathUtile.h 												 //
//																			 //
//	Contents:																 //
//																			 //
//																			 //
//	History :	O. Senhadji EL RHAZI && Abdelaziz  SAOUDI && Wassim MNEJA	 //
//  Ecole   :   Universite Pierre et Marie Curie (UPMC)						 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
//																			 //
//	MathUtile apporte tout les operations mathématiques necessaire. 		 //
//	Paris VI Projet Monte-Carlo 											 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
/**********************************************************_FILE_HEADER_END_**/




#ifndef _MATHUTILE_
#define _MATHUTILE_

double max(double x, double y);
double PartiePositive(double x);
double Heavyside(double x);
double ValeurFonction(double (*f)(double),double x);
double RepartitionNormale(double x);
double FonctionLocalisante(double x, double simulations);
double FonctionRepartitionLocalisante(double x, double simulations);
double Heavyside_modifiee_Call(double x, double simulations);
double Heavyside_modifiee_Put(double x, double simulations);

#endif//_MATHUTILE_