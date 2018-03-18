/*-------------------------Versão 8.5-----------------------------------------*/
#include<18f4431.h>
#device adc=10 //Seta a resolução do conversor A/D em 10 bits
#fuses HS, NOWDT, NOPROTECT, NOBROWNOUT, PUT, NOLVP, FCMEN 
#use delay(clock=20000000)
#use rs232(BAUD=9600, UART1, XMIT=PIN_C6, RCV=PIN_C7, PARITY= N, BITS = 8, STREAM = BT)
#priority RDA, TIMER1, TIMER0
/*--------------------------Inclusão de Bibliotecas---------------------------*/
#include "sumo3kg_library.c"  //Biblioteca entre "": mesmo diretório do projeto

#if luta
   #include "strategies.c"
#elif teste_controlado
   #include "test.c"
#endif

char bluetooth;
int liga_led =0;
int1 parar_motores =1;
int1 troca; //variável para mudar o tipo de teste/estratégia
int1 ok =0; //variável para informar que o caracter correspondente à estratégia 

#INT_RDA
void rda_isr()
{
   bluetooth = getc();
   switch(bluetooth)
         {
            #if luta  //TODO: IMPLEMENTAR ESTRATÉGIAS INICIAIS
               case 'a':  seleciona =1; ok =1; break;
               case 'b':  seleciona =2; ok =1; break;
               case 'c':  seleciona =3; ok =1; break;
               case 'd':  seleciona =4; ok =1; break;
               case 'h':  parar_motores =1;  break;
  
            #elif teste_controlado
               case 'a':  controle =0;
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
     
            #endif
         }
   #if luta        
   if(ok ==1)
   {
      printf("\rEscolha o lado do adversario\r");
      seleciona_lado =getc();
      switch(seleciona_lado)
      {
         case 'r': parar_motores = 0; liga_led =4; ok=0; break; 
         case 'l': parar_motores = 0; liga_led =4; ok=0; break;
      }
   }
   
   else 
   {
      liga_led =4;
   }
   #endif
}


void main()
{
   Portas_IO();
   Habilita_Motores(ON);
   Configura_AD();
   Configura_Timer0(OFF);
   Configura_Timer1(OFF);
   
   #if bernadete
      printf("Software Bernadete\r" "Data:"__DATE__ "\r" "Horario:"__TIME__ "\r");
      //printf("Local: "__FILE__ "\r");
   #elif lobo
      printf("Software Lobo\r" "Data:"__DATE__ "\r" "Horario:"__TIME__ "\r");
      //printf("Local: "__FILE__ "\r");
   #endif
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
               Inicializa();
               troca =1;
            }
            
            
         #elif teste_controlado
            if(controle < 8)
            {
               output_high(LED1);
               printf("\rModo teste: Motores\r");
               troca =1;
            }
            else if(controle == 8)
            {
               output_high(LED2);
               printf("\rModo teste: Sensores de Distancia\r");
               troca =1;
            }
         
            else if(controle == 9)
            {
               output_high(LED3);
               printf("\rModo teste: Sensores de Linha\r ");
               troca =1;
            }
         #endif 
      }while(!troca);
      
      do
      {
         #if luta
            output_high(LED4);
            Seleciona_Estrategia(); //checar defines para TIMER 0 e 1
             
         #elif teste_controlado
            Teste();
         #endif    
          
         if(parar_motores == 1)
         {
            MoverMotores(0,0,'p');
            liga_led =0;
            Configura_Timer0(OFF);
            troca =0;
            sensor_anterior =0;
            ok= 0;
            primeira_busca = 0; //HABILITAR NA LUTA, COMENTAR NO MODO TESTE
            achei_linha =0; //HABILITAR NA LUTA, COMENTAR NO MODO TESTE
            output_low(LED1); 
            output_low(LED2);
            output_low(LED3);
            output_low(LED4);
            
         }
      }while(troca);
              
   }
}
