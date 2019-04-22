/** Kamera Pixy
 *  Bearbeiter: Jonas Heinke (J.H)
 *  von 11.5.2017 bis:    
 *  Quellprogram der HELLO-PIXY zur Positionserkennung
 *  modifiziert und in eine statische Klasse eingebaut
 *  Ziel: Bessere Übersicht und bessere Wiederverwendbarkeit 
 * */
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

class CamPixy
{

 #define BLOCK_BUFFER_SIZE    25
 public:
  // KONSTANTEN ZUM EINBAU DER Cam
  
  ///* @param BILDKANTE_S0 Abstand von der Kamera zum nahnen Bildrand
  static const int BILDKANTE_S0=15;
  ///* @param beta		- Neigung der Kamera
  static const int Beta=0;	
  ///* @param yMax 	- Maximale Pixelanzahl der Kamera in X-Richtung
  static const int Xmax=320; //Matrixweite, in X
  ///* @param yMax 	- Maximale Pixelanzahl der Kamera in Y-Richtung
  static const int Ymax=200; //Matrixhöhe, in Y
  ///* @param Hcam		- Höhe der Kameraachse in mm in Richtung der zu detektierenden Objektunterkante
  static const int Hcam=10;  //Anbauhöhe der Kamera
  ///* @param Scam		- Kamerakonstante in Pixel, Abbildungskonstante (kann experimentell ermittelt werden)	 
  static const int Scam=10;  //Kamerakonstante, muß ermittelt werden
 
 
public:  //--> ggf private und GET-er  verwenden
 static struct Block blocks[BLOCK_BUFFER_SIZE];
 static int blocks_copied;
 static int frame;
 
 struct objectPosition{
	 int entfernung;
	 int orientierung; 
 };
 
static objectPosition objPos; 
 	
	
public:
   CamPixy();
  ~CamPixy();

  public:
	  static void help();
	 // static void handle_SIGINT(int unused);
	  static int  init();
	  static int  pos();
	  static int pos(int objektSig);
	  static void close();
};
