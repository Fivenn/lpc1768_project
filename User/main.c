//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Programme de départ
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
//===========================================================//

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"

#include "globaldec.h" // fichier contenant toutes les déclarations de variables globales. Permet de se passer de la fonction touch_read()
#include <stdio.h>

#define PRESCALERVALUE 2500
#define MATCHVALUE 100

unsigned int flagtacheclavier = 0;

//===========================================================//
// Function
//===========================================================//
	
void pin_MemoryConfiguration() {
	PINSEL_CFG_Type config_broche_memory;
	
	config_broche_memory.Portnum=PINSEL_PORT_0;
	config_broche_memory.Pinnum=PINSEL_PIN_27;
	config_broche_memory.Funcnum=PINSEL_FUNC_1;
	
	PINSEL_ConfigPin(&config_broche_memory);

	config_broche_memory.Portnum=PINSEL_PORT_0;
	config_broche_memory.Pinnum=PINSEL_PIN_28;
	config_broche_memory.Funcnum=PINSEL_FUNC_1;
	
	PINSEL_ConfigPin(&config_broche_memory);
}

void pin_ButtonConfiguration() {
	PINSEL_CFG_Type config_broche_button;
	
	config_broche_button.Portnum=PINSEL_PORT_2;
	config_broche_button.Pinnum=PINSEL_PIN_10;
	config_broche_button.Funcnum=PINSEL_FUNC_0;
	
	PINSEL_ConfigPin(&config_broche_button);
	
	config_broche_button.Portnum=PINSEL_PORT_2;
	config_broche_button.Pinnum=PINSEL_PIN_11;
	config_broche_button.Funcnum=PINSEL_FUNC_0;
	
	PINSEL_ConfigPin(&config_broche_button);
}

void pin_Configuration() {
	pin_MemoryConfiguration();
	pin_ButtonConfiguration();
}

void  init_i2c_eeprom() {
	I2C_Init(LPC_I2C0, 500000); // Initializes the I2Cx peripheral with specified parameter.
	I2C_Cmd(LPC_I2C0, ENABLE); // Enable or disable I2C peripheral's operation
}

void  i2c_eeprom_write(uint16_t addr, uint8_t* data, int length) {
	I2C_M_SETUP_Type TransferCfg;
	I2C_TRANSFER_OPT_Type Opt;
	
	TransferCfg.sl_addr7bit=addr;//0x50 = 0b1010000
	TransferCfg.tx_data=data;
	TransferCfg.tx_length=length;
	TransferCfg.rx_data=NULL;
  TransferCfg.retransmissions_count=0;	
	
	Opt=I2C_TRANSFER_POLLING;
	
	I2C_MasterTransferData(LPC_I2C0, &TransferCfg, Opt);
}

void i2c_eeprom_read(uint16_t addr, uint8_t* data, int length) {
	I2C_M_SETUP_Type TransferCfg;
	I2C_TRANSFER_OPT_Type Opt;
	uint8_t tx_data[1]={0x01};
	
	TransferCfg.sl_addr7bit=addr; //0x50 = 0b1010000
	TransferCfg.tx_data=tx_data;
	TransferCfg.tx_length=1;
	TransferCfg.rx_data=data;
	TransferCfg.rx_length=length;
	TransferCfg.retransmissions_count=0;	
	
	Opt=I2C_TRANSFER_POLLING;
	
	I2C_MasterTransferData(LPC_I2C0, &TransferCfg, Opt);
}

void initTimer0() {
	TIM_MATCHCFG_Type configMatch;
	TIM_TIMERCFG_Type configPrescaler;
	
	configMatch.MatchChannel = 0;
	configMatch.IntOnMatch = ENABLE;
	configMatch.StopOnMatch = DISABLE;
	configMatch.ResetOnMatch = ENABLE;
	configMatch.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;
	configMatch.MatchValue = MATCHVALUE;
	
	configPrescaler.PrescaleValue = PRESCALERVALUE;
	configPrescaler.PrescaleOption = TIM_PRESCALE_USVAL;
	
	TIM_ConfigMatch(LPC_TIM0, &configMatch);
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &configPrescaler);
	TIM_Cmd(LPC_TIM0, ENABLE);
		
	return;
}

void TIMER0_IRQHandler() {
	if(flagtacheclavier == 1) {
		sprintf(chaine,"X: %04d",touch_x);
		LCD_write_english_string(20,20,chaine,Cyan,Blue);
		flagtacheclavier = 0;
	} else {
		sprintf(chaine,"X: %04d",touch_x);
		LCD_write_english_string(20,20,chaine,Cyan,Blue);
	}
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}

//===========================================================//
// Main
//===========================================================//

int main(void) {
	// pin_Configuration();
	// init_i2c_eeprom();
	lcd_Initializtion();
	touch_init(); //Init pinsel tactile et init tactile
	initTimer0();



	// uint8_t tabData[2]={0x01, 0x5}; // Word address et valeur à écrire en mémoire
	// uint8_t tabDataResult[1]; // Tableau contenant le résultat de la lecture mémoire
	// char c[30]="BLBLBL";
	
	// LCD_write_english_string(120,20,c, White, Black);
	// dessiner_rect(20,10,200,40, 1, 1, White, Red);
	// dessiner_rect(20,70,80,40, 1, 0, White, Red);
	// dessiner_rect(140,70,80,40, 1, 0, White, Red);
	// dessiner_rect(20, 130, 200, 160, 1, 1, White, Yellow);
	
	// touch_read();
	// touch_read();
	NVIC->ISER[0]|=(1<<TIMER0_IRQn);
	
	while(1)
	{
		// i2c_eeprom_write(0x50, tabData, 2);
		// i2c_eeprom_read(0x50, tabDataResult, 1);
		touch_read();
	}
}

//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
