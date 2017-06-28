/**_BEGIN_FILE_HEADER_********************************************************/
//																			 //
//	Copyright	2003-2004.													 //
//																			 //
//	File:		PriceCore.cpp												 //
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




#include <cmath>
#include "iostream.h"
#include "PriceCore.h"
#include "VarNormale.h"
#include "MathUtile.h"



// Construction/Destruction
PriceCore::PriceCore()
{
}

PriceCore::~PriceCore()
{
}

//Call et Put Européens par closed formulas B&S
double PriceCore::Call_Europeen_BS(double S0, double maturity, double strike, double volatility, double taux)
{
	if(S0 == 0)
		return 0;
	
	double d1=(log(S0/strike)+(taux+pow(volatility,2)/2)*maturity)/(volatility*sqrt(maturity));
	double d2=d1-volatility*sqrt(maturity);
	
	return S0*RepartitionNormale(d1)-strike*exp(-taux*maturity)*RepartitionNormale(d2);
}

double PriceCore::Put_Europeen_BS(double S0, double maturity, double strike, double volatility, double taux)
{
	if(S0 == 0)
		return 0;
	
	double d1=(log(S0/strike)+(taux+pow(volatility,2)/2)*maturity)/(volatility*sqrt(maturity));
	double d2=d1-volatility*sqrt(maturity);
	
	return strike*exp(-taux*maturity)*RepartitionNormale(-d2)-S0*RepartitionNormale(-d1);
}


// Call et Put Européens par Monte-Carlo, Reduction de la variance avec le controle Antithétique
// Loi(Wt) = Loi(-Wt).
double PriceCore::Call_Europeen(double S0, double maturity, double strike, double volatility, double taux, int simulations)
{
	VarNormale	normale;
	double		s1 = S0;
	double		s2 = S0;
	double		prix = 0;
	
	for(int j=0;j<simulations;j++)
	{
		s1=S0*exp((taux-pow(volatility,2)/2)*maturity+volatility*sqrt(maturity)*normale.Normale(0,1));
		s2=S0*exp((taux-pow(volatility,2)/2)*maturity-volatility*sqrt(maturity)*normale.Normale(0,1));
		
		prix+=(PartiePositive(s1-strike)+PartiePositive(s2-strike))/2;
	}
	
	return exp(-taux*maturity)*(prix/simulations);
}

double PriceCore::Put_Europeen(double S0, double maturity, double strike, double volatility, double taux, int simulations)
{
	VarNormale	normale;
	double		s1 = S0;
	double		s2 = S0;
	double		prix = 0;
	
	for(int j=0;j<simulations;j++)
	{
		s1=S0*exp((taux-pow(volatility,2)/2)*maturity+volatility*sqrt(maturity)*normale.Normale(0,1));
		s2=S0*exp((taux-pow(volatility,2)/2)*maturity+volatility*sqrt(maturity)*normale.Normale(0,1));
		prix+=(PartiePositive(strike-s1)+PartiePositive(strike-s2))/2;
	}
	return exp(-taux*maturity)*prix/simulations;
}


// Call et Put américans par arbre binomial - Option_Type=1 pour le call et 0 pour le put
double PriceCore::AmericanCRR(double S0, double maturity, double strike, double volatility, double taux, int profondeur, double OptionType)
{
	double	*prices = new double[profondeur+1];
	double	*ST = new double[profondeur+1];
	double	u,u_sqr,d,pr_u,pr_d,fa;
	double	price;
	int 	i,j;
	
	u = exp(volatility*sqrt(maturity/profondeur));
	u_sqr = u*u;
	d = 1.0/u;
	pr_u = (exp(taux*maturity/profondeur)-d)/(u-d);
	pr_d = 1.0 - pr_u;
	fa = exp(-taux*maturity/profondeur);
	
	ST[0] = S0 * pow(d,profondeur);
	for (i=1; i<=profondeur; ++i)
	{
		ST[i] = u_sqr * ST[i-1];
	}
	
	if (OptionType == 1.0)
	{
		for (i=0; i<=profondeur; ++i) prices[i] = max(0.0, (ST[i]-strike));
	}
	else
	{
		for (i=0; i<=profondeur; ++i) prices[i] = max(0.0, (strike-ST[i]));
	}
	
	if (OptionType == 1.0)
	{
		for (j=profondeur-1; j>=0; --j)
		{
			for (i=0; i<=j; ++i)
			{
				prices[i] = fa * (pr_u*prices[i+1]+pr_d*prices[i]);
				ST[i] = d*ST[i+1];
				prices[i] = max(prices[i],ST[i]-strike);
			}
		}
	}
	else
	{
		for (j=profondeur-1; j>=0; --j)
		{
			for (i=0; i<=j; ++i)
			{
				prices[i] = fa * (pr_u*prices[i+1]+pr_d*prices[i]);
				ST[i] = d*ST[i+1];
				prices[i] = max(prices[i],strike-ST[i]);
			}
		}
	}
	
	price = prices[0];
	
	delete(prices);
	delete(ST);
	
	return price;
}

// Espérance conditionnelle par Malliavin - Formule due à Fournié & al
double PriceCore::EsperanceCondMalliavin(double strike,double S, int ordre, double maturity, double S0, double taux, double volatility, int profondeur, int simulations)
{
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = ordre*pas;
	double		*stock = new double[subdiv];
	double		*brownien = new double[subdiv];
	double		Numerateur = 0;
	double		Denominateur = 0;
	VarNormale	normale;
	
	for(int j=0;j<simulations;j++)
	{
		brownien[0]=0;
		stock[0]=S0;
		
		for(int i=1;i<profondeur;i++)
		{
			brownien[i]=brownien[i-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i]=stock[i-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i]-brownien[i-1]));
		}
		
		if (ordre>0)
		{
			Numerateur+=PartiePositive(stock[profondeur-1]-strike)*Heavyside(stock[ordre-1]-S)*(maturity*brownien[ordre-1]-_t*brownien[profondeur-1]+volatility*_t*(maturity-_t))/((maturity-_t)*stock[ordre-1]);
			Denominateur+=Heavyside(stock[ordre-1]-S)*(maturity*brownien[ordre-1]-_t*brownien[profondeur-1]+volatility*_t*(maturity-_t))/((maturity-_t)*stock[ordre-1]);
		}
		else
		{
			Numerateur+=PartiePositive(stock[profondeur-1]-strike)/S0;
			Denominateur+=1.0/S0;
		}
	}
	
	delete(stock);
	delete(brownien);
	
	return exp(-taux*(maturity-_t))*Numerateur/Denominateur;
}


// Prix Call US par Malliavin sans localisation
double PriceCore::CallAmericainMalliavin(double S0, double maturity, double strike, double volatility, double taux, int profondeur, int simulations)
{
	int 		scenar = simulations;
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = 0;
	double		*stock = new double[subdiv*scenar];
	double		*brownien = new double[subdiv*scenar];
	double		Numerateur = 0;
	double		Denominateur = 0;
	double		*prix = new double[simulations];
	double		s = 0;
	double		price = 0;
	VarNormale	normale;
	
	for(int i=0;i<simulations;i++)
	{
		brownien[i*subdiv]=0;
		stock[i*subdiv]=S0;
		
		for(int j=1;j<profondeur;j++)
		{
			brownien[i*subdiv+j]=brownien[i*subdiv+j-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i*subdiv+j]=stock[i*subdiv+j-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i*subdiv+j]-brownien[i*subdiv+j-1]));
		}
		prix[i]=PartiePositive(stock[i*subdiv+(profondeur-1)]-strike);
	}
	
	for(int k=profondeur-2;k=0;k--)
	{
		_t=k*pas;
		
		for(int p=0;p<simulations;p++)
		{
			Numerateur=0;
			Denominateur=0;
			s=stock[p*subdiv+k];
			
			for(int i=0;i<simulations;i++)
			{								
				Numerateur+=prix[i]*((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside(stock[i*subdiv+k]-s)/(pas*stock[i*subdiv+k]); 		
				Denominateur+=((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside(stock[i*subdiv+k]-s)/(pas*stock[i*subdiv+k]);
			}
			
			prix[p]=max(PartiePositive(s-strike),exp(-taux*pas)*Numerateur/Denominateur);
		}
	}
	
	for(int q=0;q<simulations;q++) price+=prix[q];
	
	delete(stock);
	delete(brownien);
	delete(prix);
	
	return price/simulations;
}

// Prix Put US par Malliavin sans localisation
double PriceCore::PutAmericainMalliavin(double S0, double maturity, double strike, double volatility, double taux, int profondeur, int simulations)
{
	int 		scenar = simulations;
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = 0;
	double		*stock = new double[subdiv*scenar];
	double		*brownien = new double[subdiv*scenar];
	double		Numerateur = 0;
	double		Denominateur = 0;
	double		*prix = new double[simulations];
	double		s = 0;
	double		price = 0;
	VarNormale	normale;
	
	for(int i=0;i<simulations;i++)
	{
		brownien[i*subdiv]=0;
		stock[i*subdiv]=S0;
		
		for(int j=1;j<profondeur;j++)
		{
			brownien[i*subdiv+j]=brownien[i*subdiv+j-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i*subdiv+j]=stock[i*subdiv+j-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i*subdiv+j]-brownien[i*subdiv+j-1]));
		}
		prix[i]=PartiePositive(strike-stock[i*subdiv+(profondeur-1)]);
	}
	
	for(int k=profondeur-2;k=0;k--)
	{
		_t=k*pas;
		
		for(int p=0;p<simulations;p++)
		{
			Numerateur=0;
			Denominateur=0;
			s=stock[p*subdiv+k];
			
			for(int i=0;i<simulations;i++)
			{								
				Numerateur+=prix[i]*((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside(s-stock[i*subdiv+k])/(pas*stock[i*subdiv+k]); 		
				Denominateur+=((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside(s-stock[i*subdiv+k])/(pas*stock[i*subdiv+k]);
			}
			
			prix[p]=max(PartiePositive(strike-s),exp(-taux*pas)*Numerateur/Denominateur);
		}
	}
	
	for(int q=0;q<simulations;q++) price+=prix[q];
	
	delete(stock);
	delete(brownien);
	delete(prix);
	
	return price/simulations;
}


// Prix Call US par Malliavin avec localisation
double PriceCore::CallAmericainMalliavinLocalisant(double S0, double maturity, double strike, double volatility, double taux,int profondeur, int simulations)
{
	
	int 		scenar = simulations;
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = 0;
	double		*stock = new double[subdiv*scenar];
	double		*brownien = new double[subdiv*scenar];
	double		Numerateur = 0;
	double		Denominateur = 0;
	double		*prix = new double[simulations];
	double		s = 0;
	double		price = 0;
	VarNormale	normale;
	
	
	for(int i=0;i<simulations;i++)
	{
		brownien[i*subdiv]=0;
		stock[i*subdiv]=S0;
		
		for(int j=1;j<profondeur;j++)
		{
			brownien[i*subdiv+j]=brownien[i*subdiv+j-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i*subdiv+j]=stock[i*subdiv+j-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i*subdiv+j]-brownien[i*subdiv+j-1]));
		}
		prix[i]=PartiePositive(stock[i*subdiv+(profondeur-1)]-strike);
	}
	
	for(int k=profondeur-2;k=0;k--)
	{
		_t=k*pas;
		
		for(int p=0;p<simulations;p++)
		{
			Numerateur=0;
			Denominateur=0;
			s=stock[p*subdiv+k];
			
			for(int i=0;i<simulations;i++)
			{								
				Numerateur+=prix[i]*FonctionLocalisante(stock[i*subdiv+k]-s,simulations)+prix[i]*((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Call(stock[i*subdiv+k]-s,simulations)/(pas*volatility*_t*stock[i*subdiv+k]);			
				Denominateur+=FonctionLocalisante(stock[i*subdiv+k]-s,simulations)+((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Call(stock[i*subdiv+k]-s,simulations)/(pas*volatility*_t*stock[i*subdiv+k]);
			}
			
			prix[p]=max(PartiePositive(s-strike),exp(-taux*pas)*Numerateur/Denominateur);
		}
	}
	
	for(int q=0;q<simulations;q++) price+=prix[q];
	
	delete(stock);
	delete(brownien);
	delete(prix);
	
	return price/simulations;
}

// Prix Put US par Malliavin avec localisation
double PriceCore::PutAmericainMalliavinLocalisant(double S0, double maturity, double strike, double volatility, double taux,int profondeur, int simulations)
{
	int 		scenar = simulations;
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = 0;
	double		*stock = new double[subdiv*scenar];
	double		*brownien = new double[subdiv*scenar];
	double		Numerateur = 0;
	double		Denominateur = 0;
	double		*prix = new double[simulations];
	double		s = 0;
	double		price = 0;
	VarNormale	normale;
	
	
	for(int i=0;i<simulations;i++)
	{
		brownien[i*subdiv]=0;
		stock[i*subdiv]=S0;
		
		for(int j=1;j<profondeur;j++)
		{
			brownien[i*subdiv+j]=brownien[i*subdiv+j-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i*subdiv+j]=stock[i*subdiv+j-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i*subdiv+j]-brownien[i*subdiv+j-1]));
		}
		prix[i]=PartiePositive(strike-stock[i*subdiv+(profondeur-1)]);
	}
	
	for(int k=profondeur-2;k=0;k--)
	{
		_t=k*pas;
		
		for(int p=0;p<simulations;p++)
		{
			Numerateur=0;
			Denominateur=0;
			s=stock[p*subdiv+k];
			
			for(int i=0;i<simulations;i++)
			{								
				Numerateur+=prix[i]*FonctionLocalisante(s-stock[i*subdiv+k],simulations)+prix[i]*((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Put(s-stock[i*subdiv+k],simulations)/(volatility*_t*pas*stock[i*subdiv+k]); 		
				Denominateur+=FonctionLocalisante(s-stock[i*subdiv+k],simulations)+((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Put(s-stock[i*subdiv+k],simulations)/(volatility*_t*pas*stock[i*subdiv+k]);
			}
			
			prix[p]=max(PartiePositive(strike-s),exp(-taux*pas)*Numerateur/Denominateur);
		}
	}
	
	for(int q=0;q<simulations;q++) price+=prix[q];
	
	delete(stock);
	delete(brownien);
	delete(prix);
	
	return price/simulations;
}

//Sensibilité Delta du Call et Put US
double PriceCore::DeltaCallAmericain(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations)
{
	int 		scenar = simulations;
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = 0;
	double		*stock = new double[subdiv*scenar];
	double		*brownien = new double[subdiv*scenar];
	double		Numerateur = 0;
	double		Denominateur = 0;
	double		*delta = new double[simulations];
	double		s = 0;
	double		CallDelta = 0;
	VarNormale	normale;
	
	
	for(int i=0;i<simulations;i++)
	{
		brownien[i*subdiv]=0;
		stock[i*subdiv]=S0;
		
		for(int j=1;j<profondeur;j++)
		{
			brownien[i*subdiv+j]=brownien[i*subdiv+j-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i*subdiv+j]=stock[i*subdiv+j-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i*subdiv+j]-brownien[i*subdiv+j-1]));
		}
		delta[i]=PartiePositive(stock[i*subdiv+(profondeur-1)]-strike);
	}
	
	for(int k=profondeur-2;k=0;k--)
	{
		if(k==0)
			break;
		
		_t=k*pas;
		
		for(int p=0;p<simulations;p++)
		{
			Numerateur=0;
			Denominateur=0;
			s=stock[p*subdiv+k];
			
			for(int i=0;i<simulations;i++)
			{								
				Numerateur+=delta[i]*FonctionLocalisante(stock[i*subdiv+k]-s,simulations)+delta[i]*((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Call(stock[i*subdiv+k]-s,simulations)/(pas*volatility*_t*stock[i*subdiv+k]);			
				Denominateur+=FonctionLocalisante(stock[i*subdiv+k]-s,simulations)+((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Call(stock[i*subdiv+k]-s,simulations)/(pas*volatility*_t*stock[i*subdiv+k]);
			}
			
			delta[p]=max(PartiePositive(s-strike),exp(-taux*pas)*Numerateur/Denominateur);
		}
	}
	
	for(int q=0;q<simulations;q++) CallDelta+=delta[q]*brownien[q*subdiv+1]/(volatility*S0);
	
	delete(stock);
	delete(brownien);
	delete(delta);
	
	return CallDelta/simulations;
	
}

double PriceCore::DeltaPutAmericain(double S0,double maturity, double strike, double volatility, double taux, long profondeur, long simulations)
{
	int 		scenar = simulations;
	int 		subdiv = profondeur;
	double		pas = maturity/profondeur;
	double		_t = 0;
	double		*stock = new double[subdiv*scenar];
	double		*brownien = new double[subdiv*scenar];
	double		Numerateur = 0;
	double		Denominateur = 0;
	double		*delta = new double[simulations];
	double		s = 0;
	double		PutDelta = 0;
	VarNormale	normale;
	
	
	for(int i=0;i<simulations;i++)
	{
		brownien[i*subdiv]=0;
		stock[i*subdiv]=S0;
		
		for(int j=1;j<profondeur;j++)
		{
			brownien[i*subdiv+j]=brownien[i*subdiv+j-1]+sqrt(pas)*normale.Normale(0,1);
			stock[i*subdiv+j]=stock[i*subdiv+j-1]*exp((taux-pow(volatility,2)/2)*pas+volatility*(brownien[i*subdiv+j]-brownien[i*subdiv+j-1]));
		}
		delta[i]=PartiePositive(strike-stock[i*subdiv+(profondeur-1)]);
	}
	
	for(int k=profondeur-2;k=0;k--)
	{
		if(k==0)
			break;
		
		_t=k*pas;
		
		for(int p=0;p<simulations;p++)
		{
			Numerateur=0;
			Denominateur=0;
			s=stock[p*subdiv+k];
			
			for(int i=0;i<simulations;i++)
			{								
				Numerateur+=delta[i]*FonctionLocalisante(s-stock[i*subdiv+k],simulations)+delta[i]*((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Put(s-stock[i*subdiv+k],simulations)/(volatility*_t*pas*stock[i*subdiv+k]);			
				Denominateur+=FonctionLocalisante(s-stock[i*subdiv+k],simulations)+((_t+pas)*brownien[i*subdiv+k]-_t*brownien[i*subdiv+k+1]+volatility*_t*pas)*Heavyside_modifiee_Put(s-stock[i*subdiv+k],simulations)/(volatility*_t*pas*stock[i*subdiv+k]);
			}
			
			delta[p]=max(PartiePositive(strike-s),exp(-taux*pas)*Numerateur/Denominateur);
		}
	}
	
	for(int q=0;q<simulations;q++) PutDelta+=delta[q]*brownien[q*subdiv+1]/(volatility*S0);
	
	delete(stock);
	delete(brownien);
	delete(delta);
	
	return PutDelta/simulations;
	
}				



