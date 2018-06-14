#include "strategies.h"
int16 contador =0; //para controle do TIMER0 
int1 troca_estado =1; //variável de estado para alterar rotação dos motores dentro da interrupção, de acordo com a estratégia
char seleciona_lado; //seleciona o lado 
int1 achei_linha =0;
int8 momento_estrategia = 0;

char bluetooth;
int liga_led =0;
int1 parar_motores =1;
char confirma;
int1 troca =0; //variável para mudar o tipo de teste/estratégia
int1 ok =0; //variável para informar que o caracter correspondente à estratégia 
int i;


#INT_TIMER0
void timer0_isr(void)
{
   //clear_interrupt(INT_TIMER0);
   contador++;
   if(contador == ciclos0) //numero de vezes que ocorre overflow
   {
      
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
   if(Seta_Sensores('a') == 0b00000000)
   {
      MoverMotores(velocidade_ataque, velocidade_ataque, 'f');
      delay_ms(tempo_movendo);
     
      MoverMotores(0,0,'p');
      delay_ms(tempo_parado);
   }
      
   else if(Seta_Sensores('a') != 0b00000000)
   {
      momento_estrategia = 2;
   }
    
}

void Arco_Inverso(char lado)
{
   if(Seta_Sensores('a') == 0b00000000)
   {
      switch(lado)
      {
            //TODO: rever os duty cycles desses cases
         case 'r': MoverMotores(duty_arco_inverso_maior, duty_arco_inverso_menor, 'b'); delay_ms(delay_arco_inverso); momento_estrategia = 2; break;
         case 'l': MoverMotores(duty_arco_inverso_menor, duty_arco_inverso_maior, 'b'); delay_ms(delay_arco_inverso); momento_estrategia = 2; break;
      }
   }
   
   else if(Seta_Sensores('a') != 0b00000000)
   {
      momento_estrategia = 2;
   }
}

void Busca_Ofensiva(char lado)
{
   if(Seta_Sensores('a') == 0b00000000)
   {
      if(achei_linha == 0)
      {
         MoverMotores(velocidade_busca, velocidade_busca, 'f');
      }
      
      else if(achei_linha == 1)
      {
         switch(lado)
         {
            case 'l': MoverMotores(velocidade_busca, velocidade_busca, 'a'); achei_linha=0; delay_ms(delay_rotacao); break;
            case 'r': MoverMotores(velocidade_busca, velocidade_busca, 'h'); achei_linha=0; delay_ms(delay_rotacao); break;
         }
      }
   }
   
   else if(Seta_Sensores('a') >= 0b01100000 && Seta_Sensores('a') <= 0b01111111) // caso 24 ao 31
   {
      MoverMotores(velocidade_retorno,velocidade_retorno,'b');
      delay_ms(delay_linha);
   }
   
   else if(Seta_Sensores('a') >= 0b01000000 && Seta_Sensores('a') <= 0b01011111) //caso 16 ao 23
   {
      MoverMotores(velocidade_retorno,velocidade_retorno,'b'); //rever
      delay_ms(delay_linha);
   }
   
    else if(Seta_Sensores('a') >= 0b00100000 && Seta_Sensores('a') <= 0b00111111) //caso 8 ao 15
   {
      MoverMotores(velocidade_retorno,velocidade_retorno,'b'); //rever
      delay_ms(delay_linha);
   }
   //TODO: REVER CASO 5 PARA O LOBO  
   else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000101  || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 ||  Seta_Sensores('a') == 0b00000110 )
   {
      MoverMotores(velocidade_ataque,velocidade_ataque,'f');
   }
   
   else if(Seta_Sensores('a') == 0b00001000)
   {
      MoverMotores(velocidade_busca,velocidade_busca,'h');
      delay_ms(delay_lateral);
   }
   
   else if(Seta_Sensores('a') == 0b00010000)
   {
      MoverMotores(velocidade_busca, velocidade_busca, 'a');
      delay_ms(delay_lateral);
   }
   else if(Seta_Sensores('a') == 0b00000001) //caso 1, 3 e 5. Rever caso 5, pois é improvável
   {
      MoverMotores(velocidade_busca,velocidade_busca,'h');
   }
   
   else if(Seta_Sensores('a') == 0b00000100) // caso 0, 4 e 6
   {
      MoverMotores(velocidade_busca,velocidade_busca,'a');
   }
}

void Busca_arco(char lado) //TODO: MUDAR IMPLEMENTAÇÃO // QUE RAIO DE ESTRATÉGIA É ESSA ?? Não lembro
{
   if(Seta_Sensores('a')== 0)
   {
      Configura_Timer0(ON);
      switch(lado)
      {
         case 'l': 
            if(troca_estado)
            {
               MoverMotores(velocidade_busca,velocidade_busca,'a');
            }
      
            else if(!troca_estado)
            {
               MoverMotores(velocidade_busca, velocidade_busca, 'h');
            }
            break;
         case 'r': 
            if(troca_estado)
            {
               MoverMotores(velocidade_busca,velocidade_busca,'h');
            }
      
            else if(!troca_estado)
            {
               MoverMotores(velocidade_busca, velocidade_busca, 'a');
            }
            break;
      }
   }
      
   else if(Seta_Sensores('a') != 0)
   {
      momento_estrategia = 2;
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
         case 'l': MoverMotores(velocidade_busca, velocidade_busca, 'a');
                  if(sensor_anterior == 0b00000001)
                  {
                     //MoverMotores(0,0,'p');
                     delay_ms(delay_inverte_busca);
                     sensor_anterior = 0;
                     //MoverMotores(duty_busca, duty_busca, 'h');
                  }
                  break;
         case 'r': 
                  MoverMotores(velocidade_busca, velocidade_busca, 'h'); 
                  if(sensor_anterior == 0b00000100)
                  {
                     //MoverMotores(0,0,'p');
                     delay_ms(delay_inverte_busca);
                     sensor_anterior =0;
                     //MoverMotores(duty_busca, duty_busca, 'a');
                  }
                  break;
      }
   }
   
   else if(Seta_Sensores('a') >= 0b01100000 && Seta_Sensores('a') <= 0b01111111) 
   {
      MoverMotores(velocidade_retorno,velocidade_retorno,'b');
      //delay_ms(delay_linha);
   }
   
   else if(Seta_Sensores('a') >= 0b01000000 && Seta_Sensores('a') <= 0b01011111) 
   {
      MoverMotores(velocidade_retorno,velocidade_retorno,'b'); 
      //delay_ms(delay_linha);
   }
   
    else if(Seta_Sensores('a') >= 0b00100000 && Seta_Sensores('a') <= 0b00111111) 
   {
      MoverMotores(velocidade_retorno,velocidade_retorno,'b'); //rever
      //delay_ms(delay_linha);
   }
   
   else if(Seta_Sensores('a') == 0b00000010 || Seta_Sensores('a') == 0b00000101  || Seta_Sensores('a') == 0b00000111 || Seta_Sensores('a') == 0b00000011 ||  Seta_Sensores('a') == 0b00000110 )
   {
      MoverMotores(velocidade_ataque,velocidade_ataque,'f');
   }
   
   else if(Seta_Sensores('a') == 0b00010000) //Sensor Distancia Lateral Esquerdo
   {
      MoverMotores(velocidade_busca, velocidade_busca, 'a');
      delay_ms(delay_lateral);
      //printf("\rEstou aqui\r");
   }
   
   else if(Seta_Sensores('a') == 0b00001000) //Sensor Distancia Lateral Direito
   {
      MoverMotores(velocidade_busca,velocidade_busca,'h');
      delay_ms(delay_lateral);
      //printf("\rEstou aqui 2\r");
   }
   
   else if(Seta_Sensores('a') == 0b00000001)
   {
      MoverMotores(velocidade_ataque,velocidade_ataque,'h');
   }
   
   else if(Seta_Sensores('a') == 0b00000100)
   {
      MoverMotores(velocidade_busca,velocidade_busca,'a');
   }
}   

//TODO: rever implementação

void Arco_Frente_Ofensiva(char lado) //BUSCA INICIAL
{
   //if(momento_estrategia == 1)
   
      //if(Seta_Sensores('a') == 0b00000000)
      
         switch(lado)
         {
            case 'r': MoverMotores(duty_arco_frente_maior, duty_arco_frente_menor, 'f'); delay_ms(delay_arco_frente); momento_estrategia = 2; break;
            case 'l': MoverMotores(duty_arco_frente_menor, duty_arco_frente_maior, 'f'); delay_ms(delay_arco_frente); momento_estrategia = 2; break;
         }    
   
}

void Reposicionar_frente()
{
   
   MoverMotores(velocidade_frente, velocidade_frente, 'f');
   delay_ms(delay_reposicionamento);
   momento_estrategia =1;
   
}

void Reposicionar_tras()
{
   MoverMotores(velocidade_frente, velocidade_frente,'b');
   delay_ms(delay_reposicionamento);
   momento_estrategia = 1;
}

void Inicializa() //função para simular os 5 segundos, à princípio, apenas para simulação
{
   int i;
   for(i=0; i < 5; i++)
   {
      delay_ms(atraso);
      output_toggle(LED_BLUE); //TIRAR A FUNÇÃO TOGGLE PARA TESTAR ROBÔ 
   }
}

void Seleciona_Estrategia()
{
   if(momento_estrategia == 0)
   {
      switch(reposicionar)
      {
         case 'a': Reposicionar_frente(); break;
         case 'b': Reposicionar_tras(); break;
      }
   }
   
   else if(momento_estrategia == 1)
   {
      switch(busca_inicial)
      {
         case 'a': if(seleciona_lado == 'l')
                   {
                     Arco_Frente_Ofensiva('l');
                   }
                     
                   else if(seleciona_lado == 'r')
                   {
                     Arco_Frente_Ofensiva('r');
                   }
                   break;
                
      case 'b': if(seleciona_lado == 'l')
                {
                  Arco_Inverso('l');
                }
                else if(seleciona_lado == 'r')
                {
                  Arco_Inverso('r');
                }
                break;                                         
   
      }
   }
  else if(momento_estrategia == 2)
  {
      switch(busca_padrao)
      {
         case 'a':   
                  if(seleciona_lado == 'l')
                  {
                     Basica('l');
                  }
                  else if(seleciona_lado == 'r')
                  {
                     Basica('r');
                  }
                  break;
                  
         case 'b':   if(seleciona_lado == 'l')
                     {
                        Busca_Ofensiva('l');
                     }
                     else if(seleciona_lado == 'r')
                     {
                        Busca_Ofensiva('r');
                     }
                  break;   
      }
   }   
}

/* Se der tempo, usar alocação dinâmica para armazenar as strings com o nome
das estrategias, para imprimi-las ao usuário todas de uma vez, fora do switch()
*/
void Exibe_Estrategias(char repositioning, char first_search, char standard_search)  
{
   printf("\r");
   switch(repositioning)
   {
      case 'a':   printf("\rReposicionamento: Reposicionar_frente\r"); break;
      case 'b':   printf("\rReposicionamento: Reposicionar_tras\r"); break;
      default:    printf("\rNao foi escolhido um reposicionamento valido\r");
   }
   
   switch(first_search)
   {
      case 'a':   printf("\rPrimeira Busca: Arco_Frente_Ofensiva\r"); break;
      case 'b':   printf("\rPrimeira Busca: Arco_inverso\r"); break;
      default:    printf("\rNao foi escolhida uma primeira busca valida\r");
   }
   
   switch(standard_search)
   {
      case 'a':   printf("\rBusca Padrao: Basica\r"); break;
      case 'b':   printf("\rBusca Padrao: Busca_Ofensiva\r"); break;
      default:    printf("\rNao foi escolhida uma busca padrao valida\r");
   }
}

void Reseta_Estrategia()
{
    printf("\r");
    MoverMotores(0,0,'p');
    liga_led =0;
    Configura_Timer0(OFF);           
    sensor_anterior =0; 
    confirma = '\0';
    #if luta
      momento_estrategia = 0; //HABILITAR NA LUTA, COMENTAR NO MODO TESTE
      achei_linha =0; //HABILITAR NA LUTA, COMENTAR NO MODO TESTE
      ok =0;
      parar_motores =1; 
      for(i =0; i < TAMANHO_STRING; i++)
      {
         string_estrategia[i] = '\0';
      }
      
      reposicionar = '\0';
      busca_inicial = '\0';
      busca_padrao = '\0';
      
      for(i = 0; i < 3; i++)
      {
         char_velocidade_busca[i] = string_estrategia[i+3];
      }
     
      char_velocidade_busca[3] = '\0';
   
      for(i = 0; i < 3; i++)
      {
         char_velocidade_ataque[i] = string_estrategia[i+6];
      }
     
      char_velocidade_ataque[3] = '\0';
   
      for(i = 0; i < 3; i++)
      {
         char_velocidade_retorno[i] = string_estrategia[i+9];
      }
     
      char_velocidade_retorno[3] = '\0';
      

      #endif
      output_low(LED_BLUE); 
      output_low(LED_GREEN);           
}

void Recebe_String()
{
   if(troca ==1) 
         {
            bluetooth = getc();
            if(bluetooth == 'h')
            {
              Reseta_Estrategia();
            }
         }
         
         for(i = 0; i < TAMANHO_STRING; i++)
         {
            string_estrategia[i] = getc();
         }
      
         //modo_execucao = string_estrategia[0]; remanejar outros parametros quando implementar 
         reposicionar = string_estrategia[0];
         busca_inicial = string_estrategia[1];
         busca_padrao = string_estrategia[2];
      
         Exibe_Estrategias(reposicionar, busca_inicial, busca_padrao);
      
         for(i = 0; i < 3; i++)
         {
            char_velocidade_busca[i] = string_estrategia[i+3];
         }
     
         char_velocidade_busca[3] = '\0';
   
         for(i = 0; i < 3; i++)
         {
            char_velocidade_ataque[i] = string_estrategia[i+6];
         }
     
         char_velocidade_ataque[3] = '\0';
   
         for(i = 0; i < 3; i++)
         {
            char_velocidade_retorno[i] = string_estrategia[i+9];
         }
     
         char_velocidade_retorno[3] = '\0';
      
         velocidade_busca   = atoi(char_velocidade_busca);
         printf("\r Velocidade De Busca: %u \r",velocidade_busca);
   
         velocidade_ataque  = atoi(char_velocidade_ataque);
         printf("\r Velocidade De Ataque: %u \r",velocidade_ataque);
   
         velocidade_retorno = atoi(char_velocidade_retorno);
         printf("\r Velocidade De Retorno: %u \r",velocidade_retorno);
   
         printf("\rA string esta correta ? S/N\r");
   
         confirma = getc();
   
         if(confirma == 'S' || confirma == 's')
         {
            
            ok =1;  
         }
   
         else //Neste caso, houve falha na recepção da string e/ou o programa interpretou errado, zerar todas as variáveis e recemoçar
         {
            printf("\rDigite novamente a string da estrategia\r");
            ok =0;
            parar_motores =1; 
            for(i =0; i < TAMANHO_STRING; i++)
            {
               string_estrategia[i] = '\0';
            }
      
            reposicionar = '\0';
            busca_inicial = '\0';
            busca_padrao = '\0';
      
            for(i = 0; i < 3; i++)
            {
               char_velocidade_busca[i] = string_estrategia[i+3];
            }
     
            char_velocidade_busca[3] = '\0';
   
            for(i = 0; i < 3; i++)
            {
               char_velocidade_ataque[i] = string_estrategia[i+6];
            }
     
            char_velocidade_ataque[3] = '\0';
   
            for(i = 0; i < 3; i++)
            {
               char_velocidade_retorno[i] = string_estrategia[i+9];
            }
     
            char_velocidade_retorno[3] = '\0';
      
         }
         
         if(ok == 1)
         {
            printf("\rEscolha o lado do adversario\r");
            seleciona_lado =getc();
            switch(seleciona_lado)
            {
               case 'r': parar_motores = 0; liga_led =4; ok=0; break; 
               case 'l': parar_motores = 0; liga_led =4; ok=0; break;
            }
         }
}
