#include "touch\ili_lcd_general.h"

//====================//
// Variables Globales //
//====================//
unsigned int ingame = 0; // Permet de vérifier si l'on est dans le jeu ou non
uint8_t score[1]; // Stocke le score d'une partie
unsigned int chronometer = 0; // Stocke la valeur du chronomètre à un instant T
uint8_t highscore[1]; // Stocke le plus grand score d'une session de jeu

//=================================//
// Configuration PIN Connect Block //
//=================================//
void pin_MemoryConfiguration();
void pin_ButtonConfiguration();
void pin_Configuration();

//=====//
// I2C //
//=====//
void init_i2c_eeprom();
/*
* Permet de lire une ou plusieurs données en mémoire.
* @param addr: adresse du périphérique esclave de l'échange
* @param data: données à écrire en mémoire
* @param length: Longueur de la donnée à écrire en mémoire
*/
void i2c_eeprom_read(uint16_t addr, uint8_t* data, int length);
/*
* Permet d'écrire une ou plusieurs données en mémoire.
* @param addr: adresse du périphérique esclave de l'échange
* @param data: données à écrire en mémoire
* @param length: Longueur de la donnée à écrire en mémoire
*/
void  i2c_eeprom_write(uint16_t addr, uint8_t* data, int length);

//=======//
// Timer //
//=======//
void initTimer0();
void TIMER0_IRQHandler(); // //Traitant d'interruption. D'après le fichier startup.s, l'adresse du traitant d'interruption du timer0 est 17

//===========//
// Ecran LCD //
//===========//
void draw_game();
void draw_menu();

//=====//
// Jeu //
//=====//
void init_game();
void load_game();
void load_menu();
void reset_high_score();
void increase_score();