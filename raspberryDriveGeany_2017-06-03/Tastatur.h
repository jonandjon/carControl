// Tastatur.h
// 
/**
 * Tastatur.h 
 * Tastatursteuerung 
 * Bearbeiter: Jonas Heinke J.H
 * von: 15.04.2017 bis: 
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


// https://android.googlesource.com/platform/frameworks/native/+/master/include/android/keycodes.h
// http://www.thelinuxdaily.com/2010/05/grab-raw-keyboard-input-from-event-device-node-devinputevent/

class Tastatur
{

public:	




// C++C+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	static bool run_flag; 
	
public:
  Tastatur();
  ~Tastatur();
  
 
  
  
  private:
	  static int cbreak(int fd);
  

  public:
	  static void help();
	  static void handle_SIGINT(int unused);
	  static int getKeyCode(void);
	  static int kbhit(void);
	  static int getch(void);
	  
	

};
