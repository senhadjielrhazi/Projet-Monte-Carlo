/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		VarNormale.h												 //
//																			 //
//	Contents:																 //
//																			 //
//																			 //
//	History :	O. Senhadji EL RHAZI && Abdelaziz  SAOUDI && Wassim MNEJA	 //
//  Ecole   :   Universite Pierre et Marie Curie (UPMC)						 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
//																			 //
//	VarNormale est la classe de calcul des va normales.a					 //
//	Paris VI Projet Monte-Carlo 											 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
/**********************************************************_FILE_HEADER_END_**/




#ifndef _VARNORMALE_
#define _VARNORMALE_

#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include <cmath>

class VarNormale  
{
public:
	double RandNormale();
	long Multiplie(long p,long q);
	double RandUniforme();
	double Normale(double moy, double var);
	double Uniforme();
	VarNormale();
	virtual ~VarNormale();
	
private:
	long b;
	long m1;
	long m;
	double pi;
};

#endif//_VARNORMALE_

