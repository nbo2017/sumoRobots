#include "sumo3kg_library.h"

int8 sensores = 0; //verificar se o tamanho da palavra precisa ser maior para projeto Metal 2018
int8 sensor_anterior =0;
void Configura_Timer0(int1 estado)
{
   if(estado == 1)
   {
      setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2 | RTCC_8_BIT);
      set_timer0(0);
      enable_interrupts(INT_TIMER0);
      
   }
   else  
   {
      disable_interrupts(INT_TIMER0);
     
   }
}

void Configura_Timer1(int1 estado)
{
   if(estado == 1)
   {
      setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
      set_timer1(43035);
      enable_interrupts(INT_TIMER1);
     
   }
   
   else
   {
      disable_interrupts(INT_TIMER1);
   }
}

void Habilita_Motores(int1 estado)
{
      if(estado == ON)
      {
         setup_power_pwm_pins(PWM_BOTH_ON,PWM_BOTH_ON,PWM_BOTH_ON,PWM_BOTH_ON); // Configura os 4 módulos PWM.
         setup_power_pwm(PWM_FREE_RUN, 1, 0, POWER_PWM_PERIOD, 0, 1,33);
      
         if(ENA_state)
         {
            output_high(motor_esquerda); //Manda sinal para habilitar a ponteH do motor 1.
         }
         else 
         {
            output_low(motor_esquerda); //Manda sinal para desabilitar a ponteH do motor 1.
         }
   
         if(ENB_state) 
         {
            output_high(motor_direita); //Manda sinal para habilitar a ponteH do motor 2.
         }
         else 
         {
            output_low(motor_direita); //Manda sinal para desabilitar a ponteH do motor 2
         }
      }
      
      else
      {
         setup_power_pwm_pins(PWM_OFF, PWM_OFF, PWM_OFF, PWM_OFF);
         output_low(motor_esquerda);
         output_low(motor_direita);
      }
}

void Portas_IO()
{
/*------------Atualizado para projeto MetalGarurumon em 27-03-18--------------*/
/*set_tris_x(0bRX7 RX6 RX5 RX4 RX3 RX2 RX1 RX0);*/
/*Pinos não utilizados serão setados como saída*/

   set_tris_a(0b11111111);
   set_tris_b(0b00000000); 
   set_tris_c(0b10110111); //pino RC3 não é utilizado
   set_tris_d(0b00000000); //pinos RD2, RD3, RD4, RD5, RD7 não são utilizados 
   set_tris_e(0b1111);
}

void Configura_AD()
{
   setup_adc_ports(ALL_ANALOG);
   setup_adc(ADC_CLOCK_INTERNAL);
}

void Motor_esq(int16 duty_esq, char sentido_esq)
{
   switch(sentido_esq)
   {
      case 'f': set_power_pwm0_duty((int16)((POWER_PWM_PERIOD *4) * (duty_esq*0.01))); 
                set_power_pwm2_duty((int16)(0)); 
                break;
      case 'b': set_power_pwm0_duty((int16)(0));
                set_power_pwm2_duty((int16)((POWER_PWM_PERIOD *4) * (duty_esq*0.01)));
                break;
      case 'p': set_power_pwm0_duty((int16)(0));
                set_power_pwm2_duty((int16)(0));   
   }
}

void Motor_dir(int16 duty_dir, char sentido_dir)
{
   switch(sentido_dir)
   {
      case 'b': set_power_pwm4_duty((int16)((POWER_PWM_PERIOD *4) * (duty_dir*0.01))); 
                set_power_pwm6_duty((int16)(0)); 
                break;
      case 'f': set_power_pwm4_duty((int16)(0));
                set_power_pwm6_duty((int16)((POWER_PWM_PERIOD *4) * (duty_dir*0.01)));
                break;
      case 'p': set_power_pwm4_duty((int16)(0));
                set_power_pwm6_duty((int16)(0));   
   }
}

void MoverMotores(int16 duty_esquerda, int16 duty_direita, char sentido)
{
      switch(sentido)
      {
            case 'f':
            Motor_esq(duty_esquerda, 'f');
            Motor_dir(duty_direita, 'f');
            break;
         
            case 'b':
            Motor_esq(duty_esquerda, 'b');
            Motor_dir(duty_direita, 'b');
            break;
            
            case 'a': /*arbitrario*/
            Motor_esq(duty_esquerda,'b');
            Motor_dir(duty_direita,'f');
            break;
         
            case 'h': /*arbitrario*/
            Motor_esq(duty_esquerda,'f');
            Motor_dir(duty_direita,'b');
            break;
           
      case 'p': 
            Motor_dir(0,'p');
            Motor_esq(0,'p');
            break;
         
     default: 
            Motor_dir(0,'p');
            Motor_esq(0,'p');
      }
            
}

int1 Leitura(int16 threshold, int8 canal)  
{
   int16 leitura = 0;
   switch(canal)
   {
         case 6 :set_adc_channel(linha_f_direita); break;
         case 5: set_adc_channel(linha_f_esquerda); break;
         case 4: set_adc_channel(distancia_f_central); break;
         case 3: set_adc_channel(distancia_l_esquerda); break;
         case 2: set_adc_channel(distancia_f_esquerda); break;
         case 1: set_adc_channel(distancia_l_direita); break;
         case 0: set_adc_channel(distancia_f_direita); break;
         
         
      //default: leitura = 0;
   }
   
   delay_us(10);
   leitura = read_adc();
   /*ATENÇÃO: ESSE PRIMEIRO IF É PROVISÓRIO, QUANDO TESTAR NA PLACA DE 2018, TIRAR POIS TERÁ INT EXTERNA*/
   if(canal == linha_f_esquerda || canal == linha_f_direita)
   {
      if(leitura < threshold)
      {
         return 1;
      }
      
      else
      {
         return 0;
      }
   }
   /*ACRESCENTAR AO CÓDIGO 
   else if(canal == linha_t_esquerda || canal == linha_t_direita)        //Sensores de linha TRASEIROS
   {
      if(leitura < threshold)
      {
         return 1;
      }
      
      else
      {
         return 0;
      }
   }
   */
      else if(canal == distancia_f_esquerda || canal == distancia_f_central || canal == distancia_f_direita)    /*Sensores de distância FRONTAIS*/
      {
         if(leitura < threshold)
         {
            return 1;
         }
      
         else
         {
            return 0;
         }
      }
      
      else if(canal == distancia_l_esquerda || canal == distancia_l_direita)                  /*Sensores de distância LATERAIS*/
      {
         if(leitura < threshold)
         {
            return 1;
         }
         
         else
         {
            return 0;
         }
      }
}

/*
---Construção da palavra que armazena os estados dos sensores do Metal - 2018---
   0b bit7     bit6     bit5     bit4     bit3     bit2     bit1     bit0   
       ^        ^        ^        ^        ^        ^        ^        ^
       |        |        |        |        |        |        |        |
       -       LFE      LFD      DLE      DLD      DFE      DFC      DFD


A configuração dos bits acima é provisória. Na placa de 2018 os sensores de linha  
frontais serão acionados por interrupção externa, e os bits dos sensores de linha
traseiros ocuparam seus lugares na palavra 
*/
/*OBSERVAÇÃO IMPORTANTE NA FUNÇÃO A SEGUIR: NA HORA DE SETAR OU ZERAR OS BITS DA PALAVRA, NUNCA USAR CONSTANTES POIS ASSIM MODIFICA A POSIÇÃO DOS SENSORES
NA PALAVRA QUANDO SE MUDA O PINO DO CONVERSOR A/d                                                                                                                                             */
int8 Seta_Sensores(char tipo)
{
   int i; 
   switch(tipo)
   {
   
      case 'l':   /*apenas sensores de linha TRASEIROS ligados*/
      
         bit_clear(sensores, 7); //zerar o último bit da palavra, que não é utilizado para armazenar estado de sensor   
      
         if(Leitura(limiar_t_linha,linha_t_esquerda))
         {
            bit_set(sensores,linha_t_esquerda);
         }
      
         else 
         {
            bit_clear(sensores,linha_t_esquerda);
         }
      
         if(Leitura(limiar_t_linha,linha_t_direita))
         {
            bit_set(sensores,linha_t_direita);
         }
      
         else 
         {
            bit_clear(sensores,linha_t_direita);
         }
      
         for(i = 0; i < 5; i++) //zera os 5 bits dos sensores de distância
         {
            bit_clear(sensores,i);
         }
         return sensores;
         break;
      
      case 'd':   /*apenas sensores de distância FRONTAIS ligados*/
         
         bit_clear(sensores, 7);
         
         if(Leitura(limiar_f_distancia,distancia_f_esquerda))
         {
            bit_set(sensores,distancia_f_esquerda);
         }
      
         else
         {
            bit_clear(sensores,distancia_f_esquerda);
         }
         
         if(Leitura(limiar_f_distancia,distancia_f_central))
         {
            bit_set(sensores,distancia_f_central);
         }
      
         else
         {
            bit_clear(sensores,distancia_f_central);
         }
         
         if(Leitura(limiar_f_distancia,distancia_f_direita))
         {
            bit_set(sensores,distancia_f_direita);
         }
      
         else
         {
            bit_clear(sensores,distancia_f_direita);
         }
         
         for(i = 3; i < 8; i++)
         {
            bit_clear(sensores,i);
         }
         return sensores;
         break;
         
      case 's': /*apenas sensores de distância LATERAIS ligados*/
         
         bit_clear(sensores, 7);
         
         if(Leitura(limiar_l_distancia, distancia_l_esquerda))
         {
            bit_set(sensores,distancia_l_esquerda);
         }
         
         else
         {
            bit_clear(sensores,distancia_l_esquerda);
         }
         
         if(Leitura(limiar_l_distancia, distancia_l_direita))
         {
            bit_set(sensores, distancia_l_direita);
         }
         
         else
         {
            bit_clear(sensores,distancia_l_direita);
         }
         
         for(i = 2; i <= 0; i--)
         {
            bit_clear(sensores,i);
         }
         
         for(i = 5; i < 8; i++)
         {
            bit_clear(sensores,i);
         }
         
         return sensores;
         break;
      
      case 'a': /*todos os sensores ligados*/ //É O ÚNICO CASE Q ESTÁ CERTO!!!
         
         bit_clear(sensores, 7);
         
         if(Leitura(limiar_f_linha,linha_f_esquerda))
         {
            bit_set(sensores,6);
         }
      
         else 
         {
            bit_clear(sensores,6);
         }
      
         if(Leitura(limiar_f_linha,linha_f_direita))
         {
            bit_set(sensores,5);
         }
         
         else 
         {
            bit_clear(sensores,5);
         }
         
         if(Leitura(limiar_l_distancia, distancia_l_esquerda))
         {
            bit_set(sensores,4);
         }
         
         else
         {
            bit_clear(sensores,4);
         }
         
         
         if(Leitura(limiar_l_distancia, distancia_l_direita))
         {
            bit_set(sensores,3);
         }
         
         else
         {
            bit_clear(sensores,3);
         }
         
         if(Leitura(limiar_f_distancia,distancia_f_esquerda))
         {
            bit_set(sensores,2);
         }
      
         else
         {
            bit_clear(sensores,2);
         }
         
         if(Leitura(limiar_f_distancia,distancia_f_central))
         {
            bit_set(sensores,1);
         }
      
         else
         {
            bit_clear(sensores,1);
         }
         
         if(Leitura(limiar_f_distancia,distancia_f_direita))
         {
            bit_set(sensores,0);
         }
      
         else
         {
            bit_clear(sensores,0);
         }
         
         // REVER ESTE IF!!!
         if(sensores != 0 && sensores < 0b00100000)
         {
            sensor_anterior = sensores;
         }
         
         return sensores;
         break;
      
      default:
      
         for(i= 0; i < 8; i++)
         {
            bit_clear(sensores,i);
         }
         return sensores;
         
   }
}

