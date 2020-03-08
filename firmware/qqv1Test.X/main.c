/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.166.0
        Device            :  PIC32MM0256GPM064
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.41
        MPLAB 	          :  MPLAB X v5.30
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/




#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/tmr3.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dsplib_def.h>
#include <dsplib_dsp.h>
#include <math.h>
#include <fftc.h>


#define fftc    fft16c512
#define log2N   9
#define NS      512


int16c din[NS];
int16c dout[NS];
int16c scratch[NS];

static unsigned char data_stream_buff[NS/2+2]; //2 chars for start and end and 4*NS/2 for the float results
static unsigned char * sndBuff=data_stream_buff+1;

unsigned char prnt_en=0;

float volatile genBuff[512];
unsigned int tmr_adc=0;
unsigned int tmr_prnt=0;

unsigned int adc_cnt=0;
unsigned int volatile adc_snd=0;
unsigned char snd_buff[64];
short adc_res[512];
short * adctmp = &ADC1BUF14;


unsigned int VirtToPhys(const void* p)
//---------------------------------------------------------------------------
{
    return (int)p<0?((int)p&0x1fffffffL):(unsigned int)((unsigned char*)p+0x40000000L);
}


void TMR3_CallBack(void)
{
    //USR1_Toggle();
}

void ADC1_CallBack(void){
//    
//    short conv = 0;
//    conv = ADC1BUF14;
//    genBuff[0]=(float) conv;
//    adc_snd = 1;
}


void __attribute__ ((vector(_DMA0_VECTOR), interrupt(IPL1SOFT))) _DMA0_ISR(void)
{
    if(DCH0INTbits.CHDDIF == 1){
        ADC1_SoftwareTriggerDisable();
        adc_snd=1;
    }
    
    
    DCH0INTbits.CHBCIF=0;
    DCH0INTbits.CHCCIF=0;
    DCH0INTbits.CHDDIF=0;
    DCH0INTbits.CHDHIF=0;
    DCH0INTbits.CHERIF=0;
    DCH0INTbits.CHSDIF=0;
    DCH0INTbits.CHSHIF=0;
    DCH0INTbits.CHTAIF=0;
    IFS3bits.DMA0IF=0;
}

/*
                         Main application
 */

void dma0Config(void){
    //    /* This code example illustrates the DMA channel 0 configuration for a data transfer. */
    
    
    IEC3bits.DMA0IE=0;
    IFS3bits.DMA0IF=0;
    DMACONbits.ON = 1;          // enable global DMA controller
    DCH0CON=0x3; // channel off, priority 3, no chaining
    DCH0ECON=0; // no start or stop IRQs, no pattern match
    // program the transfer
    DCH0SSA=VirtToPhys(&ADC1BUF14); // transfer source physical address
    DCH0DSA=VirtToPhys(adc_res); // transfer destination physical address
    DCH0SSIZ=2; // source size 2 bytes
    DCH0DSIZ=sizeof(adc_res); // destination size
    DCH0CSIZ=2; // 2 bytes transferred per event
    DCH0INT=0;                    // clear all interrupts
    DCH0ECONbits.CHSIRQ = _ADC_VECTOR;
    DCH0ECONbits.SIRQEN = 1;                // enable DMA 0 for IRQ trigger
    
    DCH0INTbits.CHDDIF = 0;
    DCH0INTbits.CHDDIE = 1;     // Interrupt when the fill is done.
    
    IPC24bits.DMA0IP=1;
    IPC24bits.DMA0IS=1;
    IFS3bits.DMA0IF=0;
    IEC3bits.DMA0IE=1;
    DCH0CONbits.CHEN = 1;    // turn on DMA channel 0
}



int main(void)
{
    // initialize the device
    prnt_en = 1;
    
    SYSTEM_Initialize();
    ADC1_ChannelSelect(14);
    dma0Config();
    ADC1_SoftwareTriggerEnable();
    TMR3_Start();
    while (1)
    {
//        if(adc_snd==1){
//            USR1_Toggle();
//            USB_CDC_SEND_FLOAT_ARRAY(genBuff,1);
//            adc_snd=0;
//        }
        
        if( adc_snd== 1){
            USR1_Toggle();
            ADC1_SoftwareTriggerDisable();
            //TMR3_Stop();
            float sum=0;
            unsigned short c=0;
            for(c=0;c<NS;c++){
                din[c].re=adc_res[c]+(short)8224;
                din[c].im=0;
                //sum+=adc_res[c];
            }
            //float prom=sum/512.000;            
//            sprintf (snd_buff, "Promedio: %5.1f\r\n",prom);//: %i\r\n",numBytesRead);
            mips_fft16(dout,din,fftc,scratch,log2N);
            float imm=0;
            float ree=0;
            for(c=0;c<215;c++){
                imm=(float)dout[c].im;
                ree=(float)dout[c].re;
                float module=sqrtf(imm*imm+ree*ree);
                genBuff[c]=module;
                if(module>=8925){
                    data_stream_buff[c+1]=255;
                }else{
                    data_stream_buff[c+1]=(unsigned char)(module/35);
             
                }
            }
            data_stream_buff[0]=0x03;
            data_stream_buff[215]=0xFC;
            if(prnt_en==1){
                //USB_CDC_SEND_FLOAT_ARRAY(&genBuff[0],512);
                USB_CDC_SEND(data_stream_buff, 216);
                //prnt_en=0;
            }
//            unsigned int ll=strlen(snd_buff);
//            USB_CDC_SEND(snd_buff,ll);
            DCH0CONbits.CHEN = 1;
            
            ADC1_SoftwareTriggerEnable();
            //TMR3_Start();
            adc_snd=0;
        }
        USB_CDC_Tasks();
        
        // Add your application code
    }
    return 1; 
}
/**
 End of File
*/

