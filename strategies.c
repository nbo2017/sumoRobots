#include "strategies.h"
int16 contador =0; //para controle do TIMER0
int8 seleciona =0; //seleciona a estratégia a ser utilizada 
int1 troca_estado =1; //variável de estado para alterar rotação dos motores dentro da interrupção, de acordo com a estratégia
char seleciona_lado; //seleciona o lado 
int1 primeira_busca =0;
int1 achei_linha =0;

#INT_TIMER0
void timer0_isr(void)
{
   //clear_interrupt(INT_TIMER0);
   contador++;
   if(contador == ciclos0) //numero de vezes que ocorre overflow
   {
      
      //output_toggle(LED3);
      
      if(troca_estado ==1)
         troca_estado = 0;         
   }
   
   else if(contador == 2*ciclos0)
   {
      contador =0;
      if(troca_estado ==0)
      {
         troca_estado =1;
      }
      
   }
}

#INT_TIMER1
void timer1_isr(void)
{
   contador++;
   if(contador == ciclos_delay_movendo)
   {
      contador =0;
      //output_toggle(LED3);
      if(troca_estado ==1)
      troca_estado =0;
      else
      troca_estado =1;
   }
}

void Conquistador(char lado)
{
   //output_high(LED4);
   if(primeira_busca == 0)
   {
      if(Seta_Sensores('a') == 0b00000000)
      {
      MoverMotores(duty_frente, duty_frente, 'f');
      delay_ms(tempo_movendo);
     
      MoverMotores(0,0,'p');
      delay_ms(tempo_parado);
      }
      
      else if(Seta_Sensores('a') != 0b00000000)
      {
         primeira_busca =1;
      }
    }
    
    else if(primeira_busca ==1)
    {
      if(Seta_Sensores('a') == 0b00000000)
      {
         if(achei_linha == 0)
         {
            MoverMotores(duty_ofensiva, duty_ofensiva, 'f');
         }
      
         else if(achei_linha == 1)
         {
            switch(lado)
            {
               case 'l': MoverMotores(duty_busca, duty_busca, 'a'); achei_linha=0; delay_ms(delay_rotacao); break;
               case 'r': MoverMotores(duty_busca, duty_busca, 'h'); achei_linha=0; delay_ms(delay_rotacao); break;
            }
         }
      }
   
      else if(Seta_Sensores('a') >= 0b00011000 && Seta_Sensores('a') <= 0b00011111) // caso 24 ao 31
      {
         MoverMotores(duty_tras,duty_tras,'b');
         achei_linha =1;
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') >= 0b00010000 && Seta_Sensores('a') <= 0b00010111) //caso 16 ao 23
      {
         MoverMotores(duty_tras,duty_tras,'b'); //rever
         achei_linha =1;
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') >= 0b00001000 && Seta_Sensores('a') <= 0b00001111) //caso 8 ao 15
      {
         MoverMotores(duty_tras,duty_tras,'b'); //rever
         achei_linha =1;
         delay_ms(delay_linha);
      }
   //TODO: REVER CASO 5 PARA O LOBO  
      else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000101  || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 ||  Seta_Sensores('a') == 0b00000110 )
      {
         MoverMotores(duty_frente,duty_frente,'f');
      }
   
      else if(Seta_Sensores('a') == 0b00000001) //caso 1, 3 e 5. Rever caso 5, pois é improvável
      {
         MoverMotores(duty_busca,duty_busca,'h');
      }  
   
      else if(Seta_Sensores('a') == 0b00000100) // caso 0, 4 e 6
      {
         MoverMotores(duty_busca,duty_busca,'a');
      }
   }
}

void Arco_Inverso( char lado)
{
   if(primeira_busca == 0)
   {
      if(Seta_Sensores('a') == 0b00000000)
      {
         switch(lado)
         {
            //TODO: rever os duty cycles desses cases
            case 'r': MoverMotores(duty_arco_inverso_maior, duty_arco_inverso_menor, 'b'); delay_ms(delay_arco_inverso); primeira_busca =1; break;
            case 'l': MoverMotores(duty_arco_inverso_menor, duty_arco_inverso_maior, 'b'); delay_ms(delay_arco_inverso); primeira_busca =1; break;
         }
      }      
   }
   
   else if(primeira_busca == 1)
   {
      if(Seta_Sensores('a') == 0b00000000)
      {
         switch(lado)
         {
            case 'l': MoverMotores(duty_busca, duty_busca, 'a');
                     if(sensor_anterior == 0b00000001)
                     {
                        MoverMotores(duty_busca, duty_busca, 'h');
                     }
                     break;
            case 'r': MoverMotores(duty_busca, duty_busca, 'h'); 
                     if(sensor_anterior == 0b00000100)
                     {
                        MoverMotores(duty_busca, duty_busca, 'a');
                     }
                     break;
         }
      }
      else if(Seta_Sensores('a') >= 0b00011000 && Seta_Sensores('a') <= 0b00011111) // caso 24 ao 31
      {
         MoverMotores(0,0,'p');
         delay_ms(30);
         MoverMotores(duty_tras,duty_tras,'b');
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') >= 0b00010000 && Seta_Sensores('a') <= 0b00010111) //caso 16 ao 23
      {
         MoverMotores(duty_tras,duty_tras,'b'); //rever
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') >= 0b00001000 && Seta_Sensores('a') <= 0b00001111) //caso 8 ao 15
      {
         MoverMotores(duty_tras,duty_tras,'b'); //rever
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 || Seta_Sensores('a') == 0b00000101) //caso 2 e 7, adversário de frente
      {
         MoverMotores(duty_frente,duty_frente,'f');
      }
   
      else if(Seta_Sensores('a') == 0b00000001) //caso 1, 3 e 5. Rever caso 5, pois é improvável
      {
         MoverMotores(duty_busca,duty_busca,'h');
      }
   
      else if(Seta_Sensores('a') == 0b00000100 ||  Seta_Sensores('a') == 0b00000110) // caso 0, 4 e 6
      {
         MoverMotores(duty_busca,duty_busca,'a');
      }
   }
}

void Busca_Ofensiva(char lado)
{
   if(Seta_Sensores('a') == 0b00000000)
   {
      if(achei_linha == 0)
      {
         MoverMotores(duty_ofensiva, duty_ofensiva, 'f');
      }
      
      else if(achei_linha == 1)
      {
         switch(lado)
         {
            case 'l': MoverMotores(duty_busca, duty_busca, 'a'); achei_linha=0; delay_ms(delay_rotacao); break;
            case 'r': MoverMotores(duty_busca, duty_busca, 'h'); achei_linha=0; delay_ms(delay_rotacao); break;
         }
      }
   }
   
   else if(Seta_Sensores('a') >= 0b00011000 && Seta_Sensores('a') <= 0b00011111) // caso 24 ao 31
   {
      MoverMotores(duty_tras,duty_tras,'b');
      achei_linha =1;
      delay_ms(delay_linha);
   }
   
   else if(Seta_Sensores('a') >= 0b00010000 && Seta_Sensores('a') <= 0b00010111) //caso 16 ao 23
   {
      MoverMotores(duty_tras,duty_tras,'b'); //rever
      achei_linha =1;
      delay_ms(delay_linha);
   }
   
    else if(Seta_Sensores('a') >= 0b00001000 && Seta_Sensores('a') <= 0b00001111) //caso 8 ao 15
   {
      MoverMotores(duty_tras,duty_tras,'b'); //rever
      achei_linha =1;
      delay_ms(delay_linha);
   }
   //TODO: REVER CASO 5 PARA O LOBO  
   else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000101  || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 ||  Seta_Sensores('a') == 0b00000110 )
   {
      MoverMotores(duty_frente,duty_frente,'f');
   }
   
   else if(Seta_Sensores('a') == 0b00000001) //caso 1, 3 e 5. Rever caso 5, pois é improvável
   {
      MoverMotores(duty_busca,duty_busca,'h');
   }
   
   else if(Seta_Sensores('a') == 0b00000100) // caso 0, 4 e 6
   {
      MoverMotores(duty_busca,duty_busca,'a');
   }
}

void Busca_arco(char lado) //TODO: MUDAR IMPLEMENTAÇÃO
{
   if(primeira_busca == 0)
   {
      if(Seta_Sensores('a')== 0)
      {
         Configura_Timer0(ON);
         switch(lado)
         {
            case 'l': 
            if(troca_estado)
            {
               MoverMotores(duty_conquista,duty_conquista,'a');
            }
      
            else if(!troca_estado)
            {
               MoverMotores(duty_conquista, duty_conquista, 'h');
            }
            break;
            case 'r': 
            if(troca_estado)
            {
               MoverMotores(duty_conquista,duty_conquista,'h');
            }
      
            else if(!troca_estado)
            {
               MoverMotores(duty_conquista, duty_conquista, 'a');
            }
            break;
         }
      }
      else if(Seta_Sensores('a') != 0)
      {
         primeira_busca =1;
      }
   }
   
   else if(primeira_busca ==1)
   {
      if(Seta_Sensores('a') == 0)
      {
         if(achei_linha ==0)
         {
         Configura_Timer0(OFF);
         MoverMotores(duty_ofensiva, duty_ofensiva, 'f');
         
         }
         
         else if(achei_linha == 1)
         {
            Configura_Timer0(OFF);
            achei_linha=0;
            MoverMotores(duty_busca, duty_busca, 'h');
            delay_ms(delay_rotacao);
         }
      }
      else if(Seta_Sensores('a') >= 0b00011000 && Seta_Sensores('a') <= 0b00011111) // caso 24 ao 31
      {
         Configura_Timer0(OFF);
         achei_linha =1;
         MoverMotores(0,0,'p');
         delay_ms(30);
         MoverMotores(duty_tras,duty_tras,'b');
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') >= 0b00010000 && Seta_Sensores('a') <= 0b00010111) //caso 16 ao 23
      {
         Configura_Timer0(OFF);
         achei_linha =1;
         MoverMotores(0,0,'p');
         delay_ms(30);
         MoverMotores(duty_tras,duty_tras,'b'); //rever
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') >= 0b00001000 && Seta_Sensores('a') <= 0b00001111) //caso 8 ao 15
      {
         Configura_Timer0(OFF);
         achei_linha =1;
         MoverMotores(0,0,'p');
         delay_ms(30);
         MoverMotores(duty_tras,duty_tras,'b'); //rever
         delay_ms(delay_linha);
      }
   
      else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 || Seta_Sensores('a') == 0b00000101 ||  Seta_Sensores('a') == 0b00000110) //caso 2 e 7, adversário de frente
      {
         Configura_Timer0(OFF);
         MoverMotores(duty_frente,duty_frente,'f');
      }
   
      else if(Seta_Sensores('a') == 0b00000001) //caso 1
      {
         Configura_Timer0(OFF);
         MoverMotores(duty_busca,duty_busca,'h');
      }
   
      else if(Seta_Sensores('a') == 0b00000100) // caso 0, 4 e 6
      {
         Configura_Timer0(OFF);
         MoverMotores(duty_busca,duty_busca,'a');
      }
   }
}

/*---Estratégia 1(mais básica, busca rotacionando no próprio eixo e ataca )---*/
//TODO: testtar se a lógica do sensor anterior funciona para a função básica
void Basica(char lado)
{
   if(Seta_Sensores('a') == 0b00000000)
   {
      switch(lado)
      {
         case 'l': MoverMotores(duty_busca, duty_busca, 'a');
                  if(sensor_anterior == 0b00000001)
                  {
                     MoverMotores(0,0,'p');
                     delay_ms(delay_inverte_busca);
                     sensor_anterior = 0;
                     //MoverMotores(duty_busca, duty_busca, 'h');
                  }
                  break;
         case 'r': 
                  MoverMotores(duty_busca, duty_busca, 'h'); 
                  if(sensor_anterior == 0b00000100)
                  {
                     MoverMotores(0,0,'p');
                     delay_ms(delay_inverte_busca);
                     sensor_anterior =0;
                     //MoverMotores(duty_busca, duty_busca, 'a');
                  }
                  break;
      }
   }
   
   else if(Seta_Sensores('a') >= 0b00011000 && Seta_Sensores('a') <= 0b00011111) // caso 24 ao 31
   {
      MoverMotores(duty_tras,duty_tras,'b');
      delay_ms(delay_linha);
   }
   
   else if(Seta_Sensores('a') >= 0b00010000 && Seta_Sensores('a') <= 0b00010111) //caso 16 ao 23
   {
      MoverMotores(duty_tras,duty_tras,'b'); //rever
      delay_ms(delay_linha);
   }
   
    else if(Seta_Sensores('a') >= 0b00001000 && Seta_Sensores('a') <= 0b00001111) //caso 8 ao 15
   {
      MoverMotores(duty_tras,duty_tras,'b'); //rever
      delay_ms(delay_linha);
   }
   //TODO: REVER CASO 5 PARA O LOBO  
   else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000101  || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 ||  Seta_Sensores('a') == 0b00000110 )
   {
      MoverMotores(duty_frente,duty_frente,'f');
   }
   
   else if(Seta_Sensores('a') == 0b00000001) //caso 1, 3 e 5. Rever caso 5, pois é improvável
   {
      MoverMotores(duty_busca,duty_busca,'h');
   }
   
   else if(Seta_Sensores('a') == 0b00000100) // caso 0, 4 e 6
   {
      MoverMotores(duty_busca,duty_busca,'a');
   }
}   

//TODO: rever implementação

void Arco_Frente_Ofensiva(char lado)
{
   if(primeira_busca == 0)
   {
      if(Seta_Sensores('a') == 0b00000000)
      {
         switch(lado)
         {
            case 'r': MoverMotores(duty_arco_frente_maior, duty_arco_frente_menor, 'f'); delay_ms(delay_arco_frente); primeira_busca =1; break;
            case 'l': MoverMotores(duty_arco_frente_menor, duty_arco_frente_maior, 'f'); delay_ms(delay_arco_frente); primeira_busca =1; break;
         }
      }      
   }
   
   else if(primeira_busca == 1)
   {
      if(Seta_Sensores('a') == 0b00000000)
      {
         if(achei_linha == 0)
         {
            MoverMotores(duty_ofensiva, duty_ofensiva, 'f');
         }
      
      else if(achei_linha == 1)
      {
         achei_linha=0;
         MoverMotores(duty_busca, duty_busca, 'h');
         delay_ms(delay_rotacao);
      }
      }  
   
   else if(Seta_Sensores('a') >= 0b00011000 && Seta_Sensores('a') <= 0b00011111) // caso 24 ao 31
   {
      MoverMotores(duty_tras,duty_tras,'b');
      achei_linha =1;
      delay_ms(delay_linha);
   }
   
   else if(Seta_Sensores('a') >= 0b00010000 && Seta_Sensores('a') <= 0b00010111) //caso 16 ao 23
   {
      MoverMotores(duty_tras,duty_tras,'b'); //rever
      achei_linha =1;
      delay_ms(delay_linha);
   }
   
    else if(Seta_Sensores('a') >= 0b00001000 && Seta_Sensores('a') <= 0b00001111) //caso 8 ao 15
   {
      MoverMotores(duty_tras,duty_tras,'b'); //rever
      achei_linha =1;
      delay_ms(delay_linha);
   }
   //TODO: REVER CASO 5 PARA O LOBO  
   else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000101  || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 ||  Seta_Sensores('a') == 0b00000110 )
   {
      MoverMotores(duty_frente,duty_frente,'f');
   }
   
   else if(Seta_Sensores('a') == 0b00000001) //caso 1, 3 e 5. Rever caso 5, pois é improvável
   {
      MoverMotores(duty_busca,duty_busca,'h');
   }
   
   else if(Seta_Sensores('a') == 0b00000100) // caso 0, 4 e 6
   {
      MoverMotores(duty_busca,duty_busca,'a');
   }
   }
}

void Inicializa() //função para simular os 5 segundos, à princípio, apenas para simulação
{
   int i;
   for(i=0; i < 5; i++)
   {
      delay_ms(atraso);
      output_toggle(LED4);
   }
}

void Seleciona_Estrategia()
{
   switch(seleciona)
   {
      case 1: if(seleciona_lado == 'l')
               Basica('l');
              else if(seleciona_lado == 'r')
               Basica('r');
              break;
      case 2: 
               if(seleciona_lado == 'l')
                     Busca_Ofensiva('l');
               else if(seleciona_lado == 'r')
                     Busca_Ofensiva('r');
               /*#if bernadete
                  if(seleciona_lado == 'l')
                     Busca_Ofensiva('l');
                  else if(seleciona_lado == 'r')
                     Busca_Ofensiva('r');
               #elif lobo
                  if(seleciona_lado == 'l')
                     Busca_Ofensiva('l');
                  else if(seleciona_lado == 'r')
                     Busca_Ofensiva('r');
               #endif*/
               break;
      case 3:  
                  if(seleciona_lado == 'l')
                        Arco_Frente_Ofensiva('l');
                  else if(seleciona_lado == 'r')
                        Arco_Frente_Ofensiva('r');
                        
                  /*#if bernadete
                     if(seleciona_lado == 'l')
                        Arco_Frente_Ofensiva('l');
                     else if(seleciona_lado == 'r')
                        Arco_Frente_Ofensiva('r');
                  #elif lobo
                     if(seleciona_lado == 'l')
                        Arco_Frente_Ofensiva('l');
                     else if(seleciona_lado == 'r')
                        Arco_Frente_Ofensiva('r');
                  #endif*/
               break;
       case 4:    if(seleciona_lado == 'l')
                     Conquistador('l');
                  else if(seleciona_lado == 'r')
                     Conquistador('r');
               break;
      default: MoverMotores(0,0,'p');
   }
}

