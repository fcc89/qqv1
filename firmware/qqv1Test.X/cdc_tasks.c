#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"

static uint8_t readBuffer[64];
static uint8_t writeBuffer[64];
float floatBuff[512];
unsigned char   bzy=0;
unsigned char   f_send=0;
unsigned short  s_count=0;
unsigned short  s_lim=0;

void USB_CDC_Tasks(void)
{
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
        
    }

    if( USBIsDeviceSuspended()== true )
    {
        return;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        //LED1_SetHigh();
        //USR1_SetHigh();
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));
        
        if(numBytesRead > 0){

//            sprintf (writeBuffer, "Se recibieron: %i\r\n",numBytesRead);
//            putUSBUSART(writeBuffer,strlen(writeBuffer));
            //UART3_WriteBuffer(readBuffer,numBytesRead);
            
            
        }
        
        
        
        for(i=0; i<numBytesRead; i++)
        {
            
            
//            switch(readBuffer[i])
//            {
//                /* echo line feeds and returns without modification. */
//                case 'a':
//                                       
//                case 0x0D:
//                    writeBuffer[i] = readBuffer[i];
//                    break;
//
//                /* all other characters get +1 (e.g. 'a' -> 'b') */
//                default:
//                    writeBuffer[i] = readBuffer[i] + 1;
//                    break;
//            }
//            
//            sprintf (writeBuffer, "Hola!!!\r\n");
//                    putUSBUSART(writeBuffer,strlen(writeBuffer));
//                    break; 
        }
        //float prom=sum/512.000;            
        //
        if(bzy==1&&f_send==1){
            if(s_count<s_lim){
                sprintf (writeBuffer, "Value %4d: %5.1f\r\n",s_count+1,floatBuff[s_count]);
                putUSBUSART(writeBuffer,strlen(writeBuffer));
                s_count++;
            }else{
                sprintf (writeBuffer, "Sent %4d values\r\n",s_count);
                putUSBUSART(writeBuffer,strlen(writeBuffer));
                bzy=0;
                f_send=0;
            }
            
            
        }
    }else{
        //LED1_SetLow();
        //USR1_SetLow();
    }

    CDCTxService();
}

void USB_CDC_SEND(uint8_t* bf,uint8_t len){
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
        
    }

    if( USBIsDeviceSuspended()== true )
    {
        return;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        putUSBUSART(bf,len);
    }
    CDCTxService();
}

char USB_CDC_SEND_FLOAT_ARRAY(float* fbf,unsigned short f_len){
    if(bzy==0){
        s_lim=f_len;
        s_count=0;
        while(s_count<s_lim){
            //adc_res[adc_cnt]=*adctmp;
            floatBuff[s_count]=*fbf;
            fbf++;
            s_count++;
        }
        s_count=0;
        bzy=1;
        f_send=1;
        sprintf (writeBuffer, "Float Array:\r\n");
        putUSBUSART(writeBuffer,strlen(writeBuffer));
    }else{
        return -1;
    }
    return 1;
    
}
