#include "Regler.h"
#include <math.h>

  Regler::Regler(){};
  Regler::~Regler(){};
  
/**
 * Hinweise für Entwickler und Anwender
 * */
void Regler::help()
{
	printf("\n Klasse Regler: Methoden zur Regeln");
};

 //-------------- LOCAL ------------------
 /**
  * define signum function
  * */
template<class T>
int signum(T val)
{
  if(val < T(0))
  {
    return T(-1);
  }
  else
  {
    return T(+1);
  }
}
//--------------------------------------------------


/** ***************************************************
 * symmetrischer Proportionalregler für Richtungen
 * 
 * */
int Regler::proportionalDirection(int eingang, int offset, int extrem, double verstaerkung)
{
	int vz=signum(eingang);
	double ausgang=  vz*offset + verstaerkung * eingang;
	if (abs(ausgang) >  extrem) ausgang= vz*extrem;
	return (int) ausgang;
 }
 
 /**
  * Verschiebt einen Eingangs-Ganzwert in einen natürlichen Ausgangswert und kalibriert auf den
  * erforderlichen Spannungsbereich 
  * */
 double Regler::toServoSpannung(int eingang, int eingangMax, double konstante)
 {
	 double servoSpannung = (double) (eingang + eingangMax) * konstante;
		//Verschiebung auf Anschluss und Empfindlichkeit
	 return servoSpannung;
 }
 
 
 
 
 //---------------------------
 

 
 
	  

	  
	

