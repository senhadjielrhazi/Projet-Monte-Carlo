/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		MathUtile.cpp												 //
//																			 //
//	Contents:																 //
//																			 //
//																			 //
//	History :	O. Senhadji EL RHAZI && Abdelaziz  SAOUDI && Wassim MNEJA	 //
//  Ecole   :   Universite Pierre et Marie Curie (UPMC)						 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
//																			 //
//	MathUtile apporte tout les operations mathématiques necessaires. 		 //
//	Paris VI Projet Monte-Carlo 											 //
//																			 //
///////////////////////////////////////////////////////////////////////////////
/**********************************************************_FILE_HEADER_END_**/




#include <cmath>


double max(double x, double y)
{
	return (x>y)?x:y;
	
}

double PartiePositive(double x)
{
	return max(x,0);
}

double Heavyside(double x)
{
	return (x<0)?0:1;
}

double FonctionLocalisante(double x, double simulations)
{
	return -sqrt(simulations)*exp(-sqrt(simulations)*x);
}

double FonctionRepartitionLocalisante(double x, double simulations)
{
	return exp(-sqrt(simulations)*x);
}

double Heavyside_modifiee_Call(double x, double simulations)
{
	return Heavyside(x)-FonctionRepartitionLocalisante(x,simulations);
}

double Heavyside_modifiee_Put(double x, double simulations)
{
	return 1-Heavyside(x)-FonctionRepartitionLocalisante(x,simulations);
}


double ValeurFonction(double (*f)(double),double x)
{
	return f(x);
}

double RepartitionNormale(double x)
{
	double p=  0.2316419;
	double c1= 0.319381530;
	double c2=-0.356563782;
	double c3= 1.781477937;
	double c4=-1.821255978;
	double c5= 1.330274429;
	double pi=3.14159265358979323846;
	
	if (x>0)
	{
		double t=1.0/(1+p*x);
		return 1-exp(-pow(x,2)/2)*(c1*t+c2*pow(t,2)+c3*pow(t,3)+c4*pow(t,4)+c5*pow(t,5))/sqrt(2*pi);
	}
	else
	{
		double t=1.0/(1-p*x);
		return exp(-pow(x,2)/2)*(c1*t+c2*pow(t,2)+c3*pow(t,3)+c4*pow(t,4)+c5*pow(t,5))/sqrt(2*pi);
	}
}


