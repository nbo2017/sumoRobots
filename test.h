#ifndef sumo3kg_testes
#define sumo3kg_testes

#include "sumo3kg_library.h"
/*---------------------------Duty Cycle de Teste------------------------------*/
#define duty_teste 30
/*---------------------------Tempo motor ON (modo teste)----------------------*/
#define tempo 3
/*-------------------------Sensor de Corrente do BTN--------------------------*/
#define sensor1 1
#define sensor2 2
/*---------------------------Vari�veis----------------------------------------*/
char string_testes[TAMANHO_STRING];
char char_velocidade_testes[4];
int velocidade_testes =0;
/*--------------------------Prot�tipos----------------------------------------*/
void Testa_Motor();
void Testa_Sensor();
void Teste(); //testa tudo, com controle atrav�s de aplicativo 
void Sensor_de_Corrente();
#endif

