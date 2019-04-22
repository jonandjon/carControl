#include "Bildanalyse.h"
#include "CamPixy.h"
  
  
  Bildanalyse::Bildanalyse(){};
  Bildanalyse::~Bildanalyse(){};
  
  
  
void printObjektKoordinatenX()
{
	//?	pixyPos(); //muss nicht sein, der jeweils letzte Datensatz wird gedruckt
	// Nur zur Kontrolle ein Aaru aller Koordnatenwerte un aller Obekte
   for(int index = 0; index != CamPixy::blocks_copied; ++index) {
	printf(" \n komponent:  sig(index= %1d):%1d x:%4d y:%4d width:%4d height:%4d\n",
	   index,
	   CamPixy::blocks[index].signature,
	   CamPixy::blocks[index].x,
	   CamPixy::blocks[index].y,
	   CamPixy::blocks[index].width,
	   CamPixy::blocks[index].height);
  } //--//
}
//-----------------------------------------------------------  
  
  
/**
 * Beliebiger Hilfetext zur Abfrage durch den Entwickler oder Hinweise für den ANwender
 * */
void Bildanalyse::help()
{
	printf("\n Klasse Bildanalyse: Methoden zur Auwertung der Bilder und der Bildparametern!\n\n");
};



/**
 * Liefert die Abweichung der Mitte eines Objektes zur Bildmitte
 *  * in der x-Richtung 
 */
int Bildanalyse::targetXcenter(int objektSig){
	CamPixy::pos();
	int index=objektSig -1;
    //T printObjektKoordinatenX();
    int xPos=CamPixy::blocks[index].x +  (int) (CamPixy::blocks[index].width / 2);
    int xDelta=xPos - (int) (CamPixy::Xmax / 2);
   //T printf(" \n CamPixy::Xmax: %4d | CamPixy::Xpos: %4d | xDelta: %4d \n",CamPixy::Xmax,xPos, xDelta);
  
	return xDelta; // int target
}

 
/**
 * STARK VEREINFACHTES ENFERNUNGSMODELL
 * Liefert die Entfernung zu einem spezifizierten Objektes 
 * welches sich auf der Straße befindet.
 * Die Höhe der Kamera über Straße ist mit 10 cm festgelegt. 
 * Verwendet wurde eine Geradengleichung. Das ist eine grobe Vereinfachung.
 * Vorteil: Parametrieren ist nicht erforderlich.
 * Gegebenenfalls ist eine Anpassung an das reale Modell erforderlich (Faktor 0.5).
 *  */
int Bildanalyse::targetEnfernungY(int y, int height)
{
	return CamPixy::BILDKANTE_S0 +  (CamPixy::Ymax - y - height) *0.5; 
}

/**
 * VEREINFACHTES ENFERNUNGSMODELL
 * Liefert die Entfernung zu einem Objektes 
 * welches sich auf der Straße befindet.
 * Randbedingung: Optische Achse verläuft exakt horizonal (parallel zur Straße)
 * @param y 		- y-Koordinate in Pixel der Objektoberkante in Pixel
 * @param height 	- Objektabbild (Höhe) in y-Richtung in Pixel (Objektunterkante= y + height)
 * @return 		- Entfernung des Objektes in mm (Objektunterkante) in Bezug zur Kamera
 *  */
int Bildanalyse::targetEnfernungY_horizont(int y, int height){
	
	return (int) (CamPixy::Hcam * CamPixy::Scam / (y + height - CamPixy::Ymax/2));
}
/**
 * Y-EBENEN-ENFERNUNGSMODELL
 * Liefert die Entfernung zu einem Objektes 
 * welches sich auf der Straße befindet.
 * Optische Achse kann auch um den Winkel Beta geneigt sein
 * @param y 		- y-Koordinate in Pixel der Objektoberkante in Pixel
 * @param height 	- Objektabbild (Höhe) in y-Richtung in Pixel (Objektunterkante= y + height)
 * @return 		- Entfernung des Objektes in mm (Objektunterkante) in Bezug zur Kamera
 *  */
int Bildanalyse::targetEnfernungY_geneigt(int y, int height){
	int yhmax=y+height - CamPixy::Ymax/2;
	double tanBeta= tan(CamPixy::Beta); 
	return (int) (CamPixy::Hcam  * (CamPixy::Scam  + yhmax*tanBeta ) / (CamPixy::Scam  *tanBeta + yhmax));
}



