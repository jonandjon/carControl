#include "CamPixy.h"
  
  struct Block CamPixy::blocks[BLOCK_BUFFER_SIZE];
  int CamPixy::blocks_copied;
  int CamPixy::frame=0;

  CamPixy::CamPixy(){};
  CamPixy::~CamPixy(){};
/**
 * Beliebiger Hilfetext zur Abfrage durch den Entwickler oder Hinweise für den ANwender
 * */
void CamPixy::help()
{
	printf("\n Klasse CamPixy: Methoden zur Kamerasteuerung und zur Objektanalyse\n\n");
};

/**
 * Initialisiert die Pixy-Kamera. Das geschieht nur einmal,
 * zum Beispiel im Hauptprogramm.
 * */
int CamPixy::init()
{
// --  lokale Deklarationen  ---
  int      index;
  int      pixy_init_status;
  char     buf[128];
  
//---------------Vorbereitung --------
  // signal(SIGINT, handle_SIGINT);   // Catch CTRL+C (SIGINT) signals //
  printf("Drive Crt,  libpixyusb Version: %s\n", __LIBPIXY_VERSION__);
  pixy_init_status = pixy_init();   // Connect to Pixy //

  if(!pixy_init_status == 0)   // Was there an error initializing pixy? //
  {
    // Error initializing Pixy //
    printf("pixy_init(): ");
    pixy_error(pixy_init_status);
    return pixy_init_status;
  }

  // Request Pixy firmware version //
  {
    uint16_t major;
    uint16_t minor;
    uint16_t build;
    int      return_value;
    return_value = pixy_get_firmware_version(&major, &minor, &build);
    // Ausnahmebehandlungen
    
    if (return_value) {
      // Error //
      printf("Failed to retrieve Pixy firmware version. ");
      pixy_error(return_value);
      return return_value;
    } else {
      // Success //
       printf(" Pixy Firmware Version: %d.%d.%d\n", major, minor, build);
    }
    
  }
 return 0;  //frame
}

/**
 * Liefert alle Positionswerte der erkannten Objekte
 * */
int CamPixy::pos(){
   // Wait for new blocks to be available // DAS SIND DIE WICHTIGSTEN ZEILEN !!!!!!!!!!!!!!!!!!!!!!!
   while(!pixy_blocks_are_new());// && run_flag
   blocks_copied = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);// Get blocks from Pixy
   if(blocks_copied < 0) {
      // Error: pixy_get_blocks //
      printf("pixy_get_blocks(): ");
      pixy_error(blocks_copied);
   }
   frame++;
return frame;
}



int CamPixy::pos(int objektSig){
	   // Wait for new blocks to be available // DAS SIND DIE WICHTIGSTEN ZEILEN !!!!!!!!!!!!!!!!!!!!!!!
   while(!pixy_blocks_are_new());// && run_flag
   blocks_copied = pixy_get_blocks(BLOCK_BUFFER_SIZE, &blocks[0]);// Get blocks from Pixy
   
   //IN WORK: Entfernung und orientirung des Objektes berechnen
   
   
   
   if(blocks_copied < 0) {
      // Error: pixy_get_blocks //
      printf("pixy_get_blocks(): ");
      pixy_error(blocks_copied);
    }
    frame++;
return frame;
}

/**
 * Schließt die Kamera
 * */
void CamPixy::close(){
	 pixy_close();
}



