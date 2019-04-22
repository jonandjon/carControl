// Tastatur.h
// 
/**
 * Regler.h 
 * Regler für Steuerung und Verfahrgeschwindigkeit 
 * Bearbeiter: Jonas Heinke J.H
 * von: 26.04.2017 bis: 
 * https://de.wikipedia.org/wiki/Regler#P-Regler_.28P-Anteil.29
 */
// -- BIBLIOTHEKEN ----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "pixy.h"
//++j.h++
#include <iostream>
 #include <wiringPi.h> // WiringPi-Api einbinden
  #include <cstdlib>   // für system zum Aufruf externer Programme:
  #include <fstream>
  #include <termios.h>
 #include <ctype.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>


class Regler
{

public:	




// C++C+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	static bool run_flag; 
	
public:
  Regler();
  ~Regler();
  
 
  
  
  private:
	  
 

  public:
	  static void help();
	 
	  
	  static int proportionalDirection(int eingang, int offset, int extrem, double verstaerkung);
	  
	  static double toServoSpannung(int eingang, int eingangMax, double konstante);
	  
	  //-------------------
	  
	
	  
	

};
