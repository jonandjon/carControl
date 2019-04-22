
#include "mbed.h"
#include "Servo.h"
#include "math.h"



Servo myservo(PB_0);
//+ Servo servDrive(PA_12);
 //Test-LED
 DigitalOut ledStatus(LED1);
//Bit-Code für Servoposition: 0..16 Positionen
DigitalIn bit0(PA_0);    // A0
DigitalIn bit1(PA_1);   // A1
DigitalIn bit2(PA_3);   // A2
DigitalIn bit3(PA_4);  // A3
// steuert gleichzeitige Servopositionsübernahme
DigitalIn crtBit(PF_0);      // D7


// std::bitset<4> bits; // leider nicht verfügbar
   


int main()
{
    printf("Servo-Programm mit Paralle-Schnittstelle!\n");
    // Grundzustandm, Anfangsbedingungen
    float range = 0.00085;
    int bytePosition=7;  
    float position = 0.5;  //position=f(byteResult)
    myservo.calibrate(range, 45.0); 
    myservo = position;
    ledStatus = 0;
    wait(2);

    while(1) 
    {
        if (bit0) {bytePosition +=1; }
        if (bit1) {bytePosition +=2; }
        if (bit2) {bytePosition +=4; }
        if (bit3) {bytePosition +=8; }
        //Synchrone Auswärtung aller Eingänge
        if (!(crtBit)) // if crtBit connect with ground
        {
            position= (float)((bytePosition) / 15.0); //skalieren
            bytePosition=0;  //eingabe zurücksetzen
            ledStatus = 1;
            wait(0.2); //Totzeit
            myservo = position; 
        }
        ledStatus = 0;
    }
}
    


/*
Int: bit. 3  2  1  0   position
0         0  0  0  0     0.0
1         0  0  0  1     
2         0  0  1  0 
3
4
5
6
7                        0.5
8
9
10                       0.7
11
12
13
14
15        1  1   1  1    1.0
*/



