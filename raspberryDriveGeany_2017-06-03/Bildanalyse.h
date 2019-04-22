/** Bildanalyse
 *  Bearbeiter: Jonas Heinke (J.H)
 *  von 14.5.2017 bis:    
 *  Analyse der Bilder und analyse der Bildparameter
 *  
 *  Ziel: Bessere Übersicht und bessere Wiederverwendbarkeit 
 * */
// -- BIBLIOTHEKEN ----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "pixy.h"
#include <cmath>
//++j.h++
#include <iostream>
 #include <wiringPi.h> // WiringPi-Api einbinden
  #include <cstdlib>   // für system zum Aufruf externer Programme:
  #include <fstream>
  #include <termios.h>
 #include <ctype.h>
#include <sys/select.h>






class Bildanalyse
{

 
public:  //--> ggf private und GET-er  verwenden
 
	
public:
   Bildanalyse();
  ~Bildanalyse();

  public:
	  static void help();
	  static int targetXcenter(int objektSig);
	  static int targetEnfernungY(int y, int height);
	  static int targetEnfernungY_horizont( int y, int height);
	  static int targetEnfernungY_geneigt(int y, int height);
};
