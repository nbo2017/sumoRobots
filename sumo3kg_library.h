#ifndef pinos_sumo3kg_
#define pinos_sumo3kg_

#define lobo 1
#define bernadete 0
/*----------------------------Canais conversor A/D----------------------------*/
#define linha_direita 5
#define linha_esquerda 6
#define distancia_direita 0
#define distancia_central 2
#define distancia_esquerda 3
/*----------------------------Pinagem dos sensores----------------------------*/
/*
#define linha_direita PIN_A6
#define linha_esquerda PIN_A5
#define distancia_esquerda PIN_A3
#define distancia_central PIN_A0
#define distancia_direita PIN_A4
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
#define limiar_distancia 470     /*****LIMIAR BERNADETE, REVER****/
#define limiar_linha 800         /*****FAZER TESTES COM ELETRÔNICA****/
/*-------------------------Frequência PWM-------------------------------------*/
#define POWER_PWM_PERIOD 311 //simular no Proteus. Freq = +/- 16kHz
/*--------------------Habilitar função dos motores/TIMER0---------------------*/
#define ON 1
#define OFF 0
/*--------------------------LEDs de uso geral---------------------------------*/
#define LED1 PIN_D0
#define LED2 PIN_D1
#define LED3 PIN_D2
#define LED4 PIN_D3
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
