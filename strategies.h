#ifndef strategies3kg
#define strategies3kg 
#include "sumo3kg_library.h"
/*------------------------Numero de overflows timer0--------------------------*/
#define ciclos0 2930 //3907   //n�mero de overflows do TIMER0
#define ciclos1 30
#define ciclos_delay_movendo 10 //n� de contagens do timer1 para gerar delay de 90ms, com prescaler 2 e valor inicial 30 
/*-----------------------Delay Inicial, caso necess�rio-----------------------*/
#define atraso 960
#define tempo_parado 800
#define tempo_movendo 90
#define delay_arco_frente 420 //TODO: testar valores empiricamente 
#define delay_arco_inverso 350 //TODO: testar valores empiricamente
#define delay_inverte_busca 80
#define delay_reposicionamento 280
#define delay_linha 215
#define delay_lateral 220
#define delay_rotacao 140 //TODO: TESTAR EMPIRICAMENTE PARA O METAL

/*-----------------------------Duty Cycles------------------------------------*/  
#define duty_maximo 100

/*------------------------Duty Cycles primeira busca--------------------------*/
#define duty_arco_frente_menor 35
#define duty_arco_frente_maior 55

#define duty_arco_inverso_menor 50
#define duty_arco_inverso_maior 70

/*------------------------Duty Cycles reposicionamento------------------------*/
#define velocidade_frente 40
#define velocidade_tras   40

/*-----------------------Constru��o da String Estrat�gia----------------------*/
char reposicionar;
char busca_inicial;
char busca_padrao;
char modo_execucao; //seleciona modo debbuging, luta ou algum outro a ser implementado
char string_estrategia[TAMANHO_STRING];
char char_delay_inicial[4]; //implementar depois 
char char_velocidade_reposicionar[4];
char char_velocidade_ataque[4];
char char_velocidade_busca[4];
char char_velocidade_retorno[4]; //velocidade com que o rob� volta da linha 

int velocidade_reposicionar =0;
int velocidade_ataque =0;
int velocidade_busca = 0;
int velocidade_retorno = 0;
int delay_inicial =0;


/*--------------------------Prot�tipos----------------------------------------*/

void Inicializa(); //espera 5 segundos e pisca um LED a cada segundo 
void Seleciona_Estrategia();
/*-----------------------Estrat�gias Metal(busca padr�o)----------------------*/
void Basica(char lado); //estrat�gia mais b�sica, gira no pr�rio eixo e ataca
void Conquistador(char lado); //estrat�gia em que o rob� anda e p�ra, no in�cio (pica-pau)
void Busca_arco(char lado); //estrat�gia que faz uma busca em arco, ao inv�s de buscar  
void Arco_Inverso(char lado); 
void Busca_Ofensiva(char lado); //estrat�gia para correr no dojo
void Arco_Frente_Ofensiva(char lado);
void Reposicionar_Frente();
void Reposicionar_Tras();
void Exibe_Estrategias(char repositioning, char first_search, char standard_search); //na falta de um nome significativo, utilizar nome em ingl�s
void Reseta_Estrategia();
void Recebe_String();   //recebe a string enviado por bluetooth com todas as informa��es para as estrat�gias 
#endif
