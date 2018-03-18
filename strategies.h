#ifndef strategies3kg
#define strategies3kg 
#include "sumo3kg_library.h"
/*------------------------Numero de overflows timer0--------------------------*/
#define ciclos0 2930 //3907   //número de overflows do TIMER0
#define ciclos1 30
#define ciclos_delay_movendo 10 //nº de contagens do timer1 para gerar delay de 90ms, com prescaler 2 e valor inicial 30 
/*-----------------------Delay Inicial, caso necessário-----------------------*/
#define atraso 960
#define tempo_parado 800
#define tempo_movendo 90
#define delay_arco_frente 450 //TODO: testar valores empiricamente 
#define delay_arco_inverso 350 //TODO: testar valores empiricamente
#define delay_inverte_busca 80
#if lobo
#define delay_linha 215
#elif bernadete
#define delay_linha 320
#endif
#if lobo
#define delay_rotacao 140
#elif bernadete 
#define delay_rotacao 290
#endif
/*-----------------------------Duty Cycles------------------------------------*/
#define duty_frente 60 //duty cycle padrão para adversário à frente
#define duty_conquista 40 //duty cycle frente ESTRATÉGIA CONQUISTADOR
#define duty_busca 45 //50 //duty cycle padrão para busca de adversário
#define duty_tras_menor 50 //duty cycle quando encontra a linha branca no lado 1
#define duty_tras_maior 60 //duty cycle quando encontra a linha branco no lado 2 
#define duty_tras 65 //duty cycle quando encontra a linha branca dos dois lados 
#define duty_ofensiva 50 // duty cycle para correr de um lado para outro no dojo na estratégia Busca_Ofensiva()
#define duty_maximo 100
#if lobo
#define duty_arco_frente_menor 35
#define duty_arco_frente_maior 55
#elif bernadete
#define duty_arco_frente_menor 65
#define duty_arco_frente_maior 85
#endif
#define duty_arco_inverso_menor 50
#define duty_arco_inverso_maior 70
/*--------------------------Protótipos----------------------------------------*/
void Basica(char lado); //estratégia mais básica, gira no prório eixo e ataca
void Inicializa(); //espera 5 segundos e pisca um LED a cada segundo 
void Seleciona_Estrategia();
/*-----------------------------Estratégias Bernadete--------------------------*/
   void Conquistador(char lado); //estratégia em que o robô anda e pára, no inicio (BERNADETE)
   void Busca_arco(char lado); //estratégia que faz uma busca em arco, ao invés de buscar (BERNADETE) 
   void Arco_Inverso(char lado); 
/*----------------------------Estrategias Lobo--------------------------------*/
   void Busca_Ofensiva(char lado); //estratégia para correr no dojo, LOBO
   void Arco_Frente_Ofensiva(char lado);
#endif
