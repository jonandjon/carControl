/**
 * mainDrive.cpp
 *
 * Bearbeiter: Jonas Heinke J.H
 * von: 22.04.2017 bis: 
 * Programmteile: manuelle Sterung, Objektverfolgung
 *
 */

/*
 * HINWEISE
 * Copiliert mit Hilfe der "CMakeList.txt" zum Auffinden der Bibliotheken
 *  --> Erstellen --> Make
 * 
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
 //Quelle: http://docs.biicode.com/raspberrypi/examples/wiringpi.html
 //#include <softServo.h>  //für PWM
 // https://raspberrypi.stackexchange.com/questions/61043/raspberry-pi-software-driven-pwm-using-c
 #include <softPwm.h>  //Pulsweitenmodulation
  #include <cstdlib>   // für system zum Aufruf externer Programme:
  #include <fstream>
  #include <termios.h>
 #include <ctype.h>
#include <sys/select.h>
#include <fcntl.h>

// my J.
#include "Tastatur.h"
#include "CamPixy.h"
#include "Bildanalyse.h"
#include "Regler.h"

// -------------------------------------------------

int rechts=28;  	//GPIO20
int links=29;		//GPIO21
int vorwaerts=22;	//GPIO6
int zurueck=23;		//GPIO13
int PWMA=25;        //GPIO26 Mot 1 an/aus/speed
int PWMservo=0;        //GPIO17 servo
int PWMB=26;       //GPIO12 Mot 2 an/aus/speed
int speed=50;      //in % 
int steeringSpeed=50;  //in %
int speedMax=100;
int steeringSpeedMax=100;
using namespace std;
/** 
 * AUSGAENGE UND EINGÄNGE VEREINBAREN
 * WIRINGPi-Zuordnung
 * Wichtig: Hier wird das WiringPi Layout verwendet
*/
int GPIOinit(int max)
{
	 // Starte die WiringPi-Api (wichtig)
	if (wiringPiSetup() == -1)  { cout<<"Setup wiring pi failed"; return 1;}
 	
	pinMode(rechts, OUTPUT);
	pinMode(links, OUTPUT);
	pinMode(vorwaerts, OUTPUT);
	pinMode(zurueck, OUTPUT);
	
	pinMode(PWMA, OUTPUT);
	pinMode(PWMB, OUTPUT);
	pinMode(PWMservo, OUTPUT);
	
	softPwmCreate(PWMB,0,max); //PWM-Kanal, Speed , von Max (hier in %)
	softPwmCreate(PWMA,0,max);
	softPwmCreate(PWMservo,0,max);
	
	return 1;
}

//----------------- MANUELLE DYNAMISCHE STEUERUNG -------------------------------------
/// Steuerung
/// Jonas J.H
/// 17.4.2017
/// http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/
/// g++ gpio_Control.cpp -o gpio_Control -l wiringPi
int manuelDynamisch (){
	cout << "Raspberry manuell dynamisch !" << endl;
	// Anfangswerte

	char merkerSpeed='0';
	char merkerSteering='0';
	speed=0;
	steeringSpeed=speed;
	GPIOinit(speedMax);
    // Tastencode
	char key='x';
	int asciiCode=120;
	// Nichtabweisschleife
	do{
 	cout << " \nEingabe: " << endl;
    cout << " [w],[Up]   : vorwärts    | [s],[Down]: zurück     | [x],[Space]: halt\n [d],[Right]: nach rechts | [a],[Left]: nach links | [f],[Pos1] : mitte\n [e],[Ende]: zum Hauptmenü " << endl;
	asciiCode = Tastatur::getch();
	char character = (char) asciiCode;	
	cout << "asciiCode: " << asciiCode << endl;
	cout << "character         : " << character << endl;
	cout << "speed         : " << speed << " %"<<endl;
	cout << "steering speed         : " << steeringSpeed <<" %" << endl;

	// -- Lenkung  ---- --- --- --- --- --- --- ---
	if (asciiCode == 100 || asciiCode == 67) { // nach rechts d || right C
		if (merkerSteering !='r') steeringSpeed=0;
		steeringSpeed -=1;
		if (steeringSpeed < -steeringSpeedMax) steeringSpeed=-steeringSpeedMax;
		merkerSteering='r';
		
	}
	if (asciiCode == 97 || asciiCode == 68) { 	//lnach inks s || left
		if (merkerSteering !='l') steeringSpeed=0;
		steeringSpeed +=1;
		if (steeringSpeed > +steeringSpeedMax) steeringSpeed=+steeringSpeedMax;
		merkerSteering='l';
	}
	
	cout << "speed  !       : " << speed << " %"<<endl;
	cout << "steering speed         : " << steeringSpeed <<" %" << endl;
	
	int steeringServoSpannung=(int) ((steeringSpeedMax+steeringSpeed)/9);
	softPwmWrite(PWMservo, steeringServoSpannung);
	cout << "steering Servo Spannung: " << steeringServoSpannung/4 <<" Volt" << endl;
	

	// -- Fahrt ---
	if (asciiCode == 119 || asciiCode==65) { // beschleunigen w || Up
		if (merkerSpeed !='v') speed=50; //damit nicht der komplette Intervall durchlaufen werden muss
		speed +=1;
		if (speed>speedMax) speed=speedMax;
		merkerSpeed='v';
	}
	if (asciiCode == 115 || asciiCode==66) { 	//negativ beschleunigen s || Down
		if (merkerSpeed !='r') speed=-50;
		speed -=1;
		if (speed<-speedMax) speed=-speedMax;
		merkerSpeed ='r';
	}
	if (speed>0){ //vorwärts
		digitalWrite(vorwaerts, 1);
		digitalWrite(zurueck, 0);
		softPwmWrite(PWMB, speed);
		}
	if (speed<0){ //rückwerts
		digitalWrite(vorwaerts, 0);
		digitalWrite(zurueck, 1);
		softPwmWrite(PWMB, abs(speed));
		}
	if (asciiCode == 120 || speed==0 ) {   // Stop the care x || space
		digitalWrite(vorwaerts, 0);
		digitalWrite(zurueck, 0);
		softPwmWrite(PWMB, 0);
		
	}
		if (asciiCode == 32) {   //Bremsen
		digitalWrite(vorwaerts, 0);
		digitalWrite(zurueck, 0);
		digitalWrite(PWMB, 1);
		softPwmWrite(PWMB, 0);
		speed=0; //falls space zum Bremsen
	}
	
	

 }  while(asciiCode != 101 && asciiCode != 70) ; //e, F, Ende
 //Alles ausschalten
digitalWrite(vorwaerts, 0);
digitalWrite(zurueck, 0);
digitalWrite(rechts, 1);
digitalWrite(links, 1);
digitalWrite(PWMA, 0);
digitalWrite(PWMB, 0);

softPwmWrite(PWMB, 0);
softPwmStop(PWMB);
softPwmWrite(PWMA, 0);
softPwmStop(PWMA);
return 6;
}


void printObjektKoordinaten()
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

 
/**
 * Liefert die Entfernung zu einem mit sig sig spezifizierten Objektes 
 * welches sich auf der Straße befindet.
 * Die Höhe der Kamera über Straße ist mit 10 cm festgelegt. 
 * Verwendet wurde eine Geradengleichung. Das ist eine grobe Vereinfachung.
 * Vorteil: Parametrieren ist nicht erforderlich.
 * Gegebenenfalls ist eine Anpassung an das reale Modell erforderlich (Faktor 0.5).
 *  */
int targetEnfernungY(int objektSig)
{
	CamPixy::pos();
	int index=objektSig -1;
   // printObjektKoordinaten();
	return CamPixy::BILDKANTE_S0 +  (CamPixy::Ymax - CamPixy::blocks[index].y -CamPixy::blocks[index].height) *0.5; 
}


/**
 * autonome Steuerung: Objektverfolgung,
 * verfolge ein Ziel, z.B. Gelb 
 * */
int colorTarget(int objektSig){
	GPIOinit(100);
	double kSteering=0.8;
	double kSpannung=0.1;
	; //Verstärkung der Lenkausschläge
	softPwmWrite(PWMservo, 50); 
	do{
		// RICHTUNG: Abweichung der Fahrtrichtung zum Objekt
		int targetX=Bildanalyse::targetXcenter(objektSig);
		double steering= Regler::proportionalDirection(-targetX, 0, steeringSpeedMax, kSteering);
		double steeringServoSpannung=Regler::toServoSpannung(steering, steeringSpeedMax, kSpannung );//Verschiebung auf Anschluss und Empfindlichkeit
		softPwmWrite(PWMservo, steeringServoSpannung); //Servomotor ansteuern
		//T cout << "targetX: " << targetX << endl;	cout << "steeringSped: " << steeringSpeed << endl; //!
		
			
		// GESCHWINDIGKEIT
		// Liefert die Entfernung
		int targetY=targetEnfernungY(objektSig);
		double kSpeed=1.5;
		speed= targetY * kSpeed;  //
		if (speed > speedMax) speed=speedMax;
		if (speed < speedMax/2) speed= (int) (speedMax/2);
		
		// KEIN ZIEL
		if (CamPixy::blocks_copied == 0){
			cout << "Kein Ziel erkannt." << endl;
			speed=0;
			steeringSpeed=0;
			cout << " Warte, bis Ziel erscheint." << endl;
			//IN WORK
		}
		
		speed=0; //TTTTTEST-----------------------

		
			
		//FAHRT
		printf("\n Entfernung= %3d cm", targetY);
		if (targetY < CamPixy::BILDKANTE_S0) { 	//fahre d
			digitalWrite(vorwaerts, 0); 
			digitalWrite(zurueck, 1);
			softPwmWrite(PWMB, speed); 
			printf("\n ZURÜCK und suche Objekt");
			sleep(1); //fahre kurz rückwaerts
			digitalWrite(vorwaerts, 0); 
			digitalWrite(zurueck, 0);
			softPwmWrite(PWMB, speed);
			printf("\n Suche Objekt? Wo bist du");
			sleep(1); //fahre kurz rückwaerts
			// hier ist was zu optimieren, irgendwann fährt das Auto etwas um!!!!
		}

		if ( targetY > CamPixy::BILDKANTE_S0) { 	//fahre d
			digitalWrite(vorwaerts, 1); 
			digitalWrite(zurueck, 0);
			softPwmWrite(PWMB, speed); 
			printf("\n GO");
		}
		printObjektKoordinaten();
		
		// sleep(1);  //! Pause: zur besserenen Kontrolle
		cout << " ......................................" << endl;
      
	signal(SIGINT, Tastatur::handle_SIGINT);   // Catch CTRL+C (SIGINT) signals //
	} while(Tastatur::run_flag);  //true/false
	printf("\n\nSchließen mit e\n");
	int asciiCode;
	do{
		asciiCode =Tastatur::getch(); //Weiter mit Tastendruck
	} while(asciiCode != 101);  //warte solange bis Taste e
	//Alles ausschalten
    digitalWrite(vorwaerts, 0);
	digitalWrite(zurueck, 0);
	digitalWrite(rechts, 0);
	digitalWrite(links, 0);
	digitalWrite(PWMA, 0);
	digitalWrite(PWMB, 0);

	softPwmWrite(PWMB, 0);
	softPwmStop(PWMB);
	softPwmWrite(PWMA, 0);
	softPwmStop(PWMA);

	return 1;
}


//eC++++++++++ M A I N ++++++++++++++++++++++++++++++++++++++++
int main(int argc, char *argv[])
{
	Tastatur::help();
	CamPixy::help();
	Bildanalyse::help();
	
	CamPixy::init();  //Pyxi initialisieren
	CamPixy::pos();   //Koordinatenabfrage
	printObjektKoordinaten();//  TESTAUSGABE ---------
   
	char wahl='m';
	
	do{
		cout << " \n Steuerungsverfahren auswählen: " << endl;
        cout << " \n m manuell dynamisch \n f Folge dem gelben Objekt \n z zwischen grüner und roter Absperrung \n e Ende" << endl;
		cin >> wahl;  // Eingabe muss mit ENTER bestätigt werden
		// wahl=getKeyChar(); //das auch nicht
		switch (wahl){
			case 'm' : manuelDynamisch(); break;
			case 'f' : colorTarget(1);  break;  //Parameter ist das Farbobjekt sig 1
			case 'z' :   /* in work*/    break;
			
		}
		cout << "............................." << endl;
	
	}
	while (wahl!='e');	

	CamPixy::close();
	cout << " Ende des Programms!" << endl;
	return 0;
}







