#include "touch\ili_lcd_general.h"

//====================//
// Variables Globales //
//====================//
unsigned int ingame = 0; // Permet de v�rifier si l'on est dans le jeu ou non
uint8_t score[1]; // Stocke le score d'une partie
unsigned int chronometer = 0; // Stocke la valeur du chronom�tre � un instant T
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
* Permet de lire une ou plusieurs donn�es en m�moire.
* @param addr: adresse du p�riph�rique esclave de l'�change
* @param data: donn�es � �crire en m�moire
* @param length: Longueur de la donn�e � �crire en m�moire
*/
void i2c_eeprom_read(uint16_t addr, uint8_t* data, int length);
/*
* Permet d'�crire une ou plusieurs donn�es en m�moire.
* @param addr: adresse du p�riph�rique esclave de l'�change
* @param data: donn�es � �crire en m�moire
* @param length: Longueur de la donn�e � �crire en m�moire
*/
void  i2c_eeprom_write(uint16_t addr, uint8_t* data, int length);

//=======//
// Timer //
//=======//
void initTimer0();
void TIMER0_IRQHandler(); // //Traitant d'interruption. D'apr�s le fichier startup.s, l'adresse du traitant d'interruption du timer0 est 17

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