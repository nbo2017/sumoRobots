#include "sumo3kg_library.h"

int8 sensores = 0;
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

/*set_tris_x(0bRX7 RX6 RX5 RX4 RX3 RX2 RX1 RX0);*/
/*Pinos não utilizados serão setados como saída*/

   set_tris_a(0b11111111);
   set_tris_b(0b00000000); //todos são saída, mas e os pinos de gravação (PGC E PGD) ?
   set_tris_c(0b10000110); //pinos RC3 e RC5 não são utilizados
   set_tris_d(0b00000000);
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
         case 5: set_adc_channel(linha_direita); break;
         case 6 :set_adc_channel(linha_esquerda); break;
         case 0: set_adc_channel(distancia_direita); break;
         case 3: set_adc_channel(distancia_esquerda); break;
         case 2: set_adc_channel(distancia_central); break;
      //default: leitura = 0;
   }
   
   delay_us(10);
   leitura = read_adc();
   
   if(canal == 5 || canal == 6)        /*Sensores de linha*/
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
   
      else if(canal == 0 || canal == 2 || canal == 3)    /*Sensores de distância*/
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

int8 Seta_Sensores(char tipo)
{
   int i; 
   switch(tipo)
   {
   
      case 'l':   /*apenas sensores de linha ligados*/
      
         if(Leitura(limiar_linha,linha_esquerda))
         {
            bit_set(sensores,4);
         }
      
         else 
         {
            bit_clear(sensores,4);
         }
      
         if(Leitura(limiar_linha,linha_direita))
         {
            bit_set(sensores,3);
         }
      
         else 
         {
            bit_clear(sensores,3);
         }
      
         for(i = 0; i < 3; i++) //zera os 3 bits dos sensores de ditância
         {
            bit_clear(sensores,i);
         }
         return sensores;
         break;
      
      case 'd':   /*apenas sensores de ditância ligados*/
      
         if(Leitura(limiar_distancia,distancia_esquerda))
         {
            bit_set(sensores,2);
         }
      
         else
         {
            bit_clear(sensores,2);
         }
         
         if(Leitura(limiar_distancia,distancia_central))
         {
            bit_set(sensores,1);
         }
      
         else
         {
            bit_clear(sensores,1);
         }
         
         if(Leitura(limiar_distancia,distancia_direita))
         {
            bit_set(sensores,0);
         }
      
         else
         {
            bit_clear(sensores,0);
         }
         for(i =3; i >= 3 && i <5; i++)
         {
            bit_clear(sensores,i);
         }
         return sensores;
         break;
         
         
      case 'a': /*todos os sensores ligados*/
         
         if(Leitura(limiar_linha,linha_esquerda))
         {
            bit_set(sensores,4);
         }
      
         else 
         {
            bit_clear(sensores,4);
         }
      
         if(Leitura(limiar_linha,linha_direita))
         {
            bit_set(sensores,3);
         }
      
         else 
         {
            bit_clear(sensores,3);
         }
         if(Leitura(limiar_distancia,distancia_esquerda))
         {
            bit_set(sensores,2);
         }
      
         else
         {
            bit_clear(sensores,2);
         }
         
         if(Leitura(limiar_distancia,distancia_central))
         {
            bit_set(sensores,1);
         }
      
         else
         {
            bit_clear(sensores,1);
         }
         
         if(Leitura(limiar_distancia,distancia_direita))
         {
            bit_set(sensores,0);
         }
      
         else
         {
            bit_clear(sensores,0);
         }
         
         if(sensores != 0 && sensores < 0b00001000)
         {
            sensor_anterior = sensores;
         }
         return sensores;
         break;
         
   
      
      default:
      
         for(i= 0; i < 5; i++)
         {
            bit_clear(sensores,i);
         }
         return sensores;
   }
}

