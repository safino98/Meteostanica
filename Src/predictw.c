#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
#include "hts221.h"
#include "lps22hb.h"
#include <math.h>
#include "ili9163.h"
#include "spi.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

unsigned long delayTime;
   int t_hour=0;
   int t_minute=0;

   int station2sealevel(int p, int height, int t){
     return (double) p*pow(1-0.0065*(double)height/(t+0.0065*(double)height+273.15),-5.275);
   }

   int myabs(int val){
     if (val<0)
       return -val;
     return val;
   }

   int calc_zambretti(int curr_pressure, int prev_pressure, int mon){
           if (curr_pressure<prev_pressure){
             //FALLING
             if (mon>=4 && mon<=9)
             //summer
             {
               if (curr_pressure>=1030)
                 return 2;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 8;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 18;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 21;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 24;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 24;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 26;
               else if(curr_pressure<970)
                 return 26;
             }
             else{
             //winter
               if (curr_pressure>=1030)
                 return 2;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 8;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 15;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 21;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 22;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 24;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 26;
               else if(curr_pressure<970)
                 return 26;
             }
           }
           else if (curr_pressure>prev_pressure){
             //RAISING
             if (mon>=4 && mon<=9){
               //summer
               if (curr_pressure>=1030)
                 return 1;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 2;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 3;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 7;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 9;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 12;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 17;
               else if(curr_pressure<970)
                 return 17;
             }
             else
               //winter
              {
               if (curr_pressure>=1030)
                 return 1;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 2;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 6;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 7;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 10;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 13;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 17;
               else if(curr_pressure<970)
                 return 17;
              }
           }
           else{
               if (curr_pressure>=1030)
                 return 1;
               else if(curr_pressure>=1020 && curr_pressure<1030)
                 return 2;
               else if(curr_pressure>=1010 && curr_pressure<1020)
                 return 11;
               else if(curr_pressure>=1000 && curr_pressure<1010)
                 return 14;
               else if(curr_pressure>=990 && curr_pressure<1000)
                 return 19;
               else if(curr_pressure>=980 && curr_pressure<990)
                 return 23;
               else if(curr_pressure>=970 && curr_pressure<980)
                 return 24;
               else if(curr_pressure<970)
                 return 26;

           }

   }

   int pressureArray[10]={0};
   byte counter=0;
   byte delta_time=0;

   lsm6ds0_init();
   hts221_init();
   lps22hb_init();


