//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Programme de départ
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
//===========================================================//

// Librairies permettant d'utiliser les périphériques GPIO, PIN connect block, I2C et timer
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"

// Librairies de l'écran LCD (affichage et tactile)
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"

#include "main.h" // Fichier contenant les signatures des fonctions du main.c + déclaration des variables globales

#include "globaldec.h" // Fichier contenant toutes les déclarations de variables globales. Permet de se passer de la fonction touch_read()

#define PRESCALERVALUE 500 // Valeur du Prescaler utilisée pour la configuration du timer. Le define permet de remplacer à la compilation le nom par sa valeur
#define MATCHVALUE 100 // Valeur du Match utilisée pour la configuration du timer. Le define permet de remplacer à la compilation le nom par sa valeur

//===========================================================//
// Function
//===========================================================//

//===================//
// Configuration PIN //
//===================//
void pin_MemoryConfiguration() {
	PINSEL_CFG_Type config_broche_memory; // Initialisation d'une structure permettant d'affecter les valeurs choisies pour configurer les registres du PIN connect block
	
	// Configuration du registre PINSEL1 avec le PIN P0.27 confié au signal SDA0
	config_broche_memory.Portnum=PINSEL_PORT_0;
	config_broche_memory.Pinnum=PINSEL_PIN_27;
	config_broche_memory.Funcnum=PINSEL_FUNC_1;
	PINSEL_ConfigPin(&config_broche_memory);

	// Configuration du registre PINSEL1 avec le PIN P0.28 confié au signal SCL0
	config_broche_memory.Portnum=PINSEL_PORT_0;
	config_broche_memory.Pinnum=PINSEL_PIN_28;
	config_broche_memory.Funcnum=PINSEL_FUNC_1;
	PINSEL_ConfigPin(&config_broche_memory);
}

void pin_ButtonConfiguration() {
	PINSEL_CFG_Type config_broche_button; // Initialisation d'une structure permettant d'affecter les valeurs choisies pour configurer les registres du PIN connect block
	
	// Configuration du registre PINSEL4 avec le PIN P2.10 confié au signal GPIO Port 2.10
	config_broche_button.Portnum=PINSEL_PORT_2;
	config_broche_button.Pinnum=PINSEL_PIN_10;
	config_broche_button.Funcnum=PINSEL_FUNC_0;
	PINSEL_ConfigPin(&config_broche_button);
	
	// Configuration du registre PINSEL4 avec le PIN P2.11 confié au signal GPIO Port 2.11
	config_broche_button.Portnum=PINSEL_PORT_2;
	config_broche_button.Pinnum=PINSEL_PIN_11;
	config_broche_button.Funcnum=PINSEL_FUNC_0;
	PINSEL_ConfigPin(&config_broche_button);
}

void pin_Configuration() {
	pin_MemoryConfiguration();
	pin_ButtonConfiguration();
}

//=====//
// I2C //
//=====//
void init_i2c_eeprom() {
	I2C_Init(LPC_I2C0, 500000); // Initialise le périphérique I2C avec l'adresse du périphérique I2C0 + 3 bits pour la sélection de la page (Ob1010000)
	I2C_Cmd(LPC_I2C0, ENABLE); // Active les opérations du périphérique I2C0
}

/*
* Permet de lire une ou plusieurs données en mémoire.
* @param addr: adresse du périphérique esclave de l'échange
* @param data: données à écrire en mémoire
* @param length: Longueur de la donnée à écrire en mémoire
*/
void i2c_eeprom_read(uint16_t addr, uint8_t* data, int length) {
	I2C_M_SETUP_Type TransferCfg; // Initialisation d'une structure permettant d'affecter les valeurs choisies pour configurer le transfert I2C
	I2C_TRANSFER_OPT_Type Opt; // Initialisation d'une structure permettant d'affecter l'option transfer du I2C
	uint8_t tx_data[1]; // // Word address a écrire en mémoire afin de pouvoir lire la donnée stockée à cette word address
	
	tx_data[0] = (addr&0xff); // On stocke les 8bits de poid faible de addr (l'adresse où l'on veut écrire)
	TransferCfg.sl_addr7bit=((0xA << 3) | (	addr - tx_data[0])); // addr7bit correspond à l'adresse du périphérique esclave (FM24CL16) + les trois bits de poid fort l'adresse où l'on veut écrire
	TransferCfg.tx_data=tx_data; 
	TransferCfg.tx_length=1; // Longueur de/des données à écrire en mémoire
	TransferCfg.rx_data=data; // Tableau de données permettant de stocker la valeur lue en mémoire
	TransferCfg.rx_length=length; // Longueur de la donnée à lire en mémoire
	TransferCfg.retransmissions_count=0; // Compteur permettant de configurer un nombre de retransmissions en cas d'échec de l'écriture en mémoire
	
	Opt=I2C_TRANSFER_POLLING; // Transfert en mode polling
	
	CHECK_PARAM(I2C_MasterTransferData(LPC_I2C0, &TransferCfg, Opt) == SUCCESS); // Affectation de notre structure de configuration à la structure de configuration permettant de configurer les valeurs du registre LPC_I2C0
}

/*
* Permet d'écrire une ou plusieurs données en mémoire.
* @param addr: adresse du périphérique esclave de l'échange
* @param data: données à écrire en mémoire
* @param length: Longueur de la donnée à écrire en mémoire
*/
void  i2c_eeprom_write(uint16_t addr, uint8_t* data, int length) {
	I2C_M_SETUP_Type TransferCfg; // Initialisation d'une structure permettant d'affecter les valeurs choisies pour configurer le transfert I2C
	I2C_TRANSFER_OPT_Type Opt; // Initialisation d'une structure permettant d'affecter l'option de transfert du I2C
	uint8_t tabData[2]; // Tableau contenant le reste de la Word address + la donnée à écrire en mémoire
	tabData[0]=(addr&0xff); // On stocke les 8bits de poid faible de addr (l'adresse où l'on veut écrire)
	tabData[1]=data[0]; // On stocke la donnée à écrire en mémoire après les 8 bits de poid faible de la Word address
	
	TransferCfg.sl_addr7bit=((0xA << 3) | (	addr - tabData[0])); // addr7bit correspond à l'adresse du périphérique esclave (FM24CL16) + les trois bits de poid fort l'adresse où l'on veut écrire
	TransferCfg.tx_data=tabData;
	TransferCfg.tx_length=length + 1; // Taille de la donnée + 1 pour le reste de la Word address
	TransferCfg.rx_data=NULL; // NULL car on ne souhaite pas ici recevoir de données de la mémoire
  TransferCfg.retransmissions_count=0;	 // Compteur permettant de configurer un nombre de retransmissions en cas d'échec de l'écriture en mémoire
	
	Opt=I2C_TRANSFER_POLLING; // Transfert en mode polling
	
	CHECK_PARAM(I2C_MasterTransferData(LPC_I2C0, &TransferCfg, Opt) == SUCCESS); // Affectation de notre structure de configuration à la structure de configuration permettant de configurer les valeurs du registre LPC_I2C0
}

//=======//
// Timer //
//=======//
void initTimer0() {
	// Pour configurer un timer, il nous faut deux structures.
	TIM_MATCHCFG_Type configMatch; // Initialisation d'une structure permettant d'affecter les valeurs au Match register pour le timer
	TIM_TIMERCFG_Type configPrescaler; // Initialisation d'une structure permettant d'affecter les valeurs au Prescaler register pour le timer
	
	configMatch.MatchChannel = 0; // Utilisation du Match Register 0
	configMatch.IntOnMatch = ENABLE; // Activation des interruptions timer
	configMatch.StopOnMatch = DISABLE; // Désactivation de l'arrêt du timer lors d'un match du timer
	configMatch.ResetOnMatch = ENABLE; // Activation du reset lors d'un match du timer
	configMatch.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE; // Commutation de la broche de sortie externe en cas de correspondance
	configMatch.MatchValue = MATCHVALUE; // Valeur du Match utilisée pour la configuration du timer
	
	configPrescaler.PrescaleValue = PRESCALERVALUE; // Valeur du Prescaler utilisée pour la configuration du timer
	configPrescaler.PrescaleOption = TIM_PRESCALE_USVAL; // Prescaler en valeur microsecondes
	
	// Affectation de notre structure de configuration aux structures de configuration permettant de configurer les valeurs du registre LPC_TIM0.
	TIM_ConfigMatch(LPC_TIM0, &configMatch);
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &configPrescaler);
	TIM_Cmd(LPC_TIM0, ENABLE);
		
	return;
}

// Handler exécuté à chaque interruption du timer0
void TIMER0_IRQHandler() { // //Traitant d'interruption. D'après le fichier startup.s, l'adresse du traitant d'interruption du timer0 est 17
	// touch_x et touch_y sont des variables définies dans un fichier.h utilisé entre autre par la fonction touch_read() qui affecte ces valeurs à chaque appuie
	sprintf(chaine,"X: %04d",touch_x);
	LCD_write_english_string(20,300,chaine,Cyan,Blue);
	sprintf(chaine,"Y: %04d",touch_y);
	LCD_write_english_string(120,300,chaine,Cyan,Blue);
	
	//Partie menu
	if(touch_x >= 650 && touch_x <= 3500 && touch_y >= 2500 && touch_y <= 3000 && ingame == 0) { // Si on appuie sur le bouton "New Game" dans le menu
		load_game();
	}
	
	//Partie jeu
	if(ingame == 1 && chronometer == 100) { //Si la partie est terminée
		i2c_eeprom_read(0x50, highscore, 1);
		if(score[0] >= highscore[0]) {
			i2c_eeprom_write(0x50, score, 1);
		}
		chronometer = 0;
		score[0] = 0;
		load_menu();
	}
	if(touch_x >= 650 && touch_x <= 3600 && touch_y >= 600 && touch_y <= 2400 && ingame == 1) { // Si on appuie sur le bouton "Hit!" dans le jeu
		increase_score();
	}
	if(ingame == 1) {
		++chronometer;
	}
	
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT); // Acquitter l'interuption en attente. On remet a 0 un bit dans le registre du Timer 0.
}

//===========//
// Ecran LCD //
//===========//
void draw_game() {
	char chaine[30];
	
	dessiner_rect(0, 0, 240, 320, 1, 1, Blue, Blue);
	dessiner_rect(20 ,10, 200, 40, 1, 1, White, Red);
	dessiner_rect(20, 70, 80, 40, 1, 0, White, Red);
	dessiner_rect(110,70, 130, 40, 1, 0, White, Red);
	dessiner_rect(20, 130, 200, 160, 1, 1, White, Yellow);
	sprintf(chaine, "Hit game");
	LCD_write_english_string(90, 25, chaine, Black, Red);
	sprintf(chaine, "Score: 0");
	LCD_write_english_string(25, 80, chaine, Black, Blue);
	i2c_eeprom_read(0x50, highscore, 1);
	sprintf(chaine, "High score: %d", highscore[0]);
	LCD_write_english_string(115, 80, chaine, Black, Blue);
	sprintf(chaine, "Hit!");
	LCD_write_english_string(20, 130, chaine, Black, Yellow);
}

void draw_menu() {
	char chaine[30];
	uint8_t high_score[1];
	
	dessiner_rect(0, 0, 240, 320, 1, 1, Blue, Blue);
	dessiner_rect(20 ,10, 200, 40, 1, 1, White, Red);
	dessiner_rect(20 ,70, 200, 40, 1, 1, White, Blue);
	dessiner_rect(20 ,130, 200, 40, 1, 1, White, Blue);
	sprintf(chaine, "Hit game");
	LCD_write_english_string(90, 25, chaine, Black, Red);
	sprintf(chaine, "New Game");
	LCD_write_english_string(90, 85, chaine, Black, Blue);
	i2c_eeprom_read(0x50, highscore, 1);
	sprintf(chaine, "High Score: %d", highscore[0]);
	LCD_write_english_string(70, 145, chaine, Black, Blue);
}

//=====//
// Jeu //
//=====//
void init_game() {
	pin_Configuration(); // Initalisation des PIN utilisés par la mémoire et les boutons
	init_i2c_eeprom(); // Initialisation de la mémoire I2C
	lcd_Initializtion(); // Initialisation de l'écran LCD
	touch_init(); //Initialisation du/des registres PINSEL tactile et initialisation du tactile
	initTimer0(); // Initialisation du Timer
}

void load_game() {
	ingame = 1;
	draw_game();
}

void load_menu() {
	ingame = 0;
	draw_menu();
}

void reset_high_score() {
	uint8_t tab_data[1];

	tab_data[0]=0;
	i2c_eeprom_write(0x50, tab_data, 1);
}

void increase_score() {
		++score[0];
		sprintf(chaine, "Score: %d", score[0]);
		LCD_write_english_string(25, 80, chaine, Black, Blue);
}
	
//===========================================================//
// Main
//===========================================================//
int main(void) {	
	init_game();
	reset_high_score();
	draw_menu();
		
	NVIC->ISER[0]|=(1<<TIMER0_IRQn); //eq. NVIC_EnableIRQ(TIMER0_IRQn). Permet de valider l'interruption du timer0 au niveau du controleur d'interruption NVIC.
	
	while(1)
	{
		touch_read();
	}
}

//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
