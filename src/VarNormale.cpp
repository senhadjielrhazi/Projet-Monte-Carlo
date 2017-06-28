/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		VarNormale.cpp												 //
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




#include "VarNormale.h"
#include <cmath>

// Construction/Destruction
VarNormale::VarNormale()
{
	pi=3.14159265358979323846;
	m=100000000;
	m1=10000;
	b=31415821;
}

VarNormale::~VarNormale()
{
}

double VarNormale::Uniforme()
{
	return	(double) rand()/RAND_MAX;
}

double VarNormale::Normale(double moy, double var)
{
	double u1;
	double u2=Uniforme();
	
	do
	{
		u1=Uniforme();
	}
	while(u1==0);
	
	return moy+sqrt(var)*sqrt(-2*log(u1))*cos(2*pi*u2);
}


// Un autre générateur de variables aléatoires

long VarNormale::Multiplie(long p, long q)
{
	long p1=p/m1;
	long p0=p%m1;
	long q1=q/m1;
	long q0=q%m1;
	return (((p0*q1+p1*q0)%m1)*m1 +p0*q0)%m;
}

double VarNormale::RandUniforme()
{
	static long a;
	a = (Multiplie(a,b)+1)%m;
	return a/m;
}

double VarNormale::RandNormale()
{
	double u1;
	double u2=RandUniforme();
	
	do
	{
		u1=RandUniforme();
	}
	while(u1==0);
	
	return sqrt(-2*log(u1))*cos(2*pi*u2);	
}

