/*----------------------Vers�o beta 0.2 - 14/06/2018------------------------------*/
/* Modifica��es previstas para esta vers�o:
 - Desfazer implementa��o que suporta desenvolvimento para dois rob�s simulta-
 neamente (retirar diretivas de compila��o condicional): FEITO, N�O TESTADO;
 - Modificar arquivo de simula��o e c�digo para se adequar � distribui��o de 
 sensores proposta: PENDENTE
   3 SENSORES INFRA FRONTAIS
   2 SENSORES INFRA LATERAIS
   1 SENSOR INFRA TRASEIRO
   2 SENSORES DE LINHA FRONTAIS 
   2 SENSORES DE LINHA TRASEIROS (CONFIRMAR)
 - Testar novo c�digo no novo arquivo de simula��o: PENDENTE
 - Adaptar c�digo para nova placa, principais modifica��es:
   * Apenas dois LEDs na placa: azul(pino RD0) e verde (pino RD1);
   * Sensores de linha: pinos RA5 e RE0 (pinos anal�gicos)
                              SL8 e SL9 (pinos anal�gicos)
                              RC4 e RC5 (interrup��o externa)
   * Sensores de dist�ncia: pinos RA0, RA1, RA2, RA3, RA4, RA5, RC0, RC1, RC2  
----------------------------||------------------------------------------------   
   Pr�ximas vers�es:
 - Retirar delays dos sensores de linha, fazer por TIMER;
 - Implementar leitura de string para Bluetooth;
 - Encontrar uma solu��o alternativa � m�quina de estados/RTOS para leitura mais
 eficiente dos sensores. Caso n�o seja encontrada uma nova solu��o, implementar 
 m�quina de estados para leitura dos sensores; 
 - Sensor de linha frontal por interup��o externa;
 - Implementar modo din�mico (seguir um objeto).
--------------------------------||----------------------------------------------
Caracteres da String Bluetooth:
 0 - Reposicionamento
 1 - Primeira Busca
 2 - Estrat�gia Padr�o
 3,4 e 5- Velocidade de Busca
 6,7 e 8 - Velocidade de Ataque 
 9, 10 e 11 - Velocidade de retorno da linha 
 
 x - delay inicial
 
*/
#include<18f4431.h>
#device adc=10 //Seta a resolu��o do conversor A/D em 10 bits
#fuses HS, NOWDT, NOPROTECT, NOBROWNOUT, PUT, NOLVP, FCMEN 
#use delay(clock=20000000)
#use rs232(BAUD=9600, UART1, XMIT=PIN_C6, RCV=PIN_C7, PARITY= N, BITS = 8, STREAM = BT)
#priority RDA, TIMER1, TIMER0
/*--------------------------Inclus�o de Bibliotecas---------------------------*/
#include "sumo3kg_library.c"  //Biblioteca entre "": mesmo diret�rio do projeto
#include <stdlib.h>
#if luta
   #include "strategies.c"
#elif teste_controlado
   #include "test.c"
#endif 

#INT_RDA
void rda_isr()
{
   #if luta  
      do{   
         Recebe_String();
         }while(confirma != 's');
      
      
   #elif teste_controlado
      for(i = 0; i < TAMANHO_STRING; i++)
      {
         string_testes[i] = getc();
      }
      
      //bluetooth = string_testes[0];
      printf("%c", string_testes[0]);
      
      switch(string_testes[0])
      {
               case 'a':  controle =0;
                          troca =0;
                          liga_led =1;
                          parar_motores =0;
                          break;
               case 'b':  controle =8;
                          liga_led =2;
                          parar_motores =0; 
                          break;
               case 'c':  controle =9;
                          liga_led =3;
                          parar_motores =0;
                          break;
               case '0': if(controle > 0)
                           controle--;
                         else if(controle ==0)
                           {
                              parar_motores =1; 
                           }
                           break;
      
               case '1':   if(controle < 9)
                              controle++;
                           else if(controle > 9)
                              controle =99;
                           break;
               case 'h': parar_motores=1;
                         controle =99;
                         troca =0;
                         break;
      }
      
      
      for(i = 0; i < 3; i++)
      {
         char_velocidade_testes[i] = string_testes[i+1];
      }
      
      char_velocidade_testes[3] = '\0';
      
      velocidade_testes = atoi(char_velocidade_testes);
      /*
      printf("\r Velocidade Testes: %u \r",velocidade_testes);
   
      printf("\rA velocidade esta correta ? S/N\r");
      
      confirma = getc(BT);
   
      if(confirma == 'S' || confirma == 's')
      { 
         parar_motores = 0;  
         
      }
      
      else
      {
         ok =0;
         parar_motores =1; 
         for(i =0; i < TAMANHO_STRING; i++)
         {
            string_testes[i] = '\0';
         }
         
         bluetooth = '\0';
         
         for(i = 0; i < 3; i++)
         {
            char_velocidade_testes[i] = string_testes[i+3];
         }
     
         char_velocidade_testes[3] = '\0';
      } 
      */
   #endif   
}


void main()
{
   Portas_IO();
   Habilita_Motores(ON);
   Configura_AD();
   Configura_Timer0(OFF);
   Configura_Timer1(OFF);
   
   printf("Software MetalGarurumon\r" "Data: " __DATE__ "\r" "Horario: "__TIME__ "\r");
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
   while(true)
   {
      //Configura_Timer0(ON);
      do
      {
         #if luta
            
            if(liga_led == 4)
            {
               //printf("\rEstou aqui\r");
               Inicializa();
               troca =1;
            }
            
            
         #elif teste_controlado
            if(controle < 8)
            {
               output_high(LED_GREEN);
               printf("\rModo teste: Motores\r");
               troca =1;
            }
            else if(controle == 8)
            {
               output_high(LED_GREEN);
               printf("\rModo teste: Sensores de Distancia\r");
               troca =1;
            }
         
            else if(controle == 9)
            {
               output_high(LED_GREEN);
               printf("\rModo teste: Sensores de Linha\r ");
               troca =1;
            }
         #endif 
      }while(!troca);
      
      do
      {
         #if luta
            output_high(LED_BLUE);
            Seleciona_Estrategia(); //checar defines para TIMER 0 e 1
             
         #elif teste_controlado
            Teste();
         #endif    
      }while(troca);    
   }
}
