#ifndef pinos_sumo3kg_
#define pinos_sumo3kg_

/*---------------------Canais conversor A/D - 2018----------------------------*/
#define distancia_f_direita  0
#define distancia_f_central  4
#define distancia_f_esquerda 2
#define distancia_l_esquerda 3
#define distancia_l_direita  1
#define linha_f_direita      5
#define linha_f_esquerda     6
        //distancia_traseiro pensar o que fazer 
        
//#define linha_f_esquerda descobrir como usar interrupção externa 
//#define linha_f_direita  descobrir como usar interrupção externa
#define linha_t_esquerda     8
#define linha_t_direita      7

/*----------------------------Pinagem dos sensores----------------------------*/
/*
----------------------------Pinagem Metal - 2018-------------------------------- 
distancia_f_esquerda  PIN_RA0 (AN0)
distancia_f_central   PIN_RA1 (AN1)
distancia_f_direita   PIN_RA2 (AN2)
distancia_l_esquerda  PIN_RA3 (AN3)
distancia_l_direita   PIN_RA4 (AN4)
distancia_traseiro    PIN_RC0 (pino digital)

linha_f_esquerda      PIN_RC4 (INT_EXT1)
linha_f_direita       PIN_RC5 (INT_EXT2)
linha_t_esquerda      PIN_RA5 (AN5)
linha_t_direita       PIN_RE0 (AN6)
*/
/*---------------------------Pinagem dos Motores------------------------------*/
#define motor_esquerda PIN_B6    /*arbitrario*/
#define motor_direita PIN_B7     /*arbitrario*/
/*-------------------------Modo teste do programa-----------------------------*/
#define teste_controlado 0
/*-----------------------------------Luta-------------------------------------*/
#define luta 1
/*------------------------Habilitação da ponte H------------------------------*/
#define ENA_state 1
#define ENB_state 1
/*----------------------------Limiar Sensores---------------------------------*/
#define limiar_f_distancia 470     /*****TESTAR LIMIAR DISTÂNCIA FRONTAL****/
#define limiar_t_linha     512         /*****LIMIAR DEFINIDO APENAS PARA SENSOR DE LINHA TRASEIRO, FRONTAL INT_EXT****/
#define limiar_l_distancia 500       /***** CONSIDERANDO SENSORES DIFERENTES NA LATERAL E FRENTE *****/
#define limiar_f_linha 512          /******  PROVISÓRIO, NO WINTER HAVERÁ INTERRUPÇÃO EXTERNA 
/*-------------------------Frequência PWM-------------------------------------*/
#define POWER_PWM_PERIOD 311 //simular no Proteus. Freq = +/- 16kHz
/*--------------------Habilitar função dos motores/TIMER0---------------------*/
#define ON 1
#define OFF 0
/*--------------------------LEDs de uso geral---------------------------------*/
/*
-------------------------2018----------------------------------
*/
#define LED_BLUE PIN_D0 //modo torneio
#define LED_GREEN PIN_D1 //modo debbugging

/*-----------------------Comprimento da String Estrategias--------------------*/
   #if luta
      #define TAMANHO_STRING 12
   #elif teste_controlado
      #define TAMANHO_STRING 5
   #endif
/*--------------------------Protótipos----------------------------------------*/
void Habilita_Motores(int1 estado);
void Portas_IO();
void Configura_AD();
int1 Leitura(int16 threshold, int8 canal);
void MoverMotores(int16 duty_esquerda, int16 duty_direita, char sentido);
int8 Seta_Sensores(char tipo);
void Motor_esq(int16 duty_esq, char sentido_esq);
void Motor_dir(int16 duty_dir, char sentido_dir);
#endif
