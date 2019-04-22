#include "Tastatur.h"

  Tastatur::Tastatur(){};
  Tastatur::~Tastatur(){};
  
/**
 * Hinweise für Entwickler und Anwender
 * */
void Tastatur::help()
{
	printf("\n Klasse Tastatur: Methoden zur Abfrage der Tastatur\n");
};
/**
 * Abfrage der Tastenkombination Strg+C
 * zur Steuerung der Schleife (zum Beenden)
 * (Dadurch kann die Schleife kontinuierlich laufen)
 * */
bool Tastatur::run_flag = true;
void Tastatur::handle_SIGINT(int unused)
{
  run_flag = false;
}

/** IN WORK
 * Liefert den Tastencode
 * 
 * */
int Tastatur::getKeyCode(void){
     // blablabla
	
	return 32;
}

/**
 * C Hilfsfunktionen zur Tastaturabfrage ++++++++++++++++++++++
 * Stoppt ein Programm bis zur betätigung einer Taste
 * http://www.linuxquestions.org/questions/programming-9/pausing-the-screen-44573/#post220112
 * */
int Tastatur::kbhit(void)
{
struct timeval tv;
fd_set read_fd;
tv.tv_sec=0;
tv.tv_usec=0;
FD_ZERO(&read_fd);
FD_SET(0,&read_fd);
if(select(1, &read_fd, NULL, NULL, &tv) == -1) return 0;
if(FD_ISSET(0,&read_fd)) return 1;
return 0;
}

/**
 * Tastencode direkt abfragen ohne mit ENTER zu bestätigen
 * Endlich mal eine Quelle die brauchbar ist:
 * http://openbook.rheinwerk-verlag.de/c_von_a_bis_z/016_c_ein_ausgabe_funktionen_006.htm#mja821f1d0ab158bed792eccc5f30e3f84 
 * */
static struct termios new_io;
static struct termios old_io;

/** Funktion schaltet das Terminal in den cbreak-Modus:        
/* Kontrollflag ECHO und ICANON auf 0 setzen                 
/* Steuerzeichen: Leseoperation liefert 1 Byte VMIN=1 VTIME=1 
 * */
int Tastatur::cbreak(int fd) {
   /*Sichern unseres Terminals*/
   if((tcgetattr(fd, &old_io)) == -1)
      return -1;
   new_io = old_io;
   /* Wir verändern jetzt die Flags für den cbreak-Modus. */
   new_io.c_lflag = new_io.c_lflag & ~(ECHO|ICANON);
   new_io.c_cc[VMIN] = 1;
   new_io.c_cc[VTIME]= 0;

   /*Jetzt setzen wir den cbreak-Modus*/
   if((tcsetattr(fd, TCSAFLUSH, &new_io)) == -1)
      return -1;
   return 1;
}
/**
 * Liefert den ASCII-Code des Zeichens der gedrückten Taste.
 * Das Programm wird fortgesetzt ohne auf Return zu warten.
 * */
int Tastatur::getch(void)
{
   int c;
   if(cbreak(STDIN_FILENO) == -1) {
      printf("Fehler bei der Funktion cbreak ... \n");
      exit(EXIT_FAILURE);
   }
   c = getchar();
   /* alten Terminal-Modus wiederherstellen */
   tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
   return c;
}



