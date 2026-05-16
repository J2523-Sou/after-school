#include <xc.h>         
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h" 

#define RX_BUFFER_SIZE 9 

volatile uint8_t rx_buffer[RX_BUFFER_SIZE];     //8ビットデータ配列（受信したやつ）
volatile uint8_t rx_count = 0;                  //受信中のビット番号
volatile bool rx_complete = false;              //受信完了フラグ
uint8_t before_a;


//メイン関数
void main(void)
{
    SYSTEM_Initialize(); 
    
    while (1)        
    {
        
//        RA3 = 1;
        
        //受信準備完了
        if (UART1_is_rx_ready()) 
        {
            //データを1バイト(8ビット)読む
            uint8_t received_byte = UART1_Read();
            
            //ヘッダが0xAAでなければ初めから
            if (rx_count == 0 && received_byte != 0xAA) {
                continue;
            }

            //受信完了フラグが立っていなければ
            if (rx_complete == false) {
                rx_buffer[rx_count] = received_byte;
                rx_count++;
//                RA3 = 1;
                
                if (rx_count == RX_BUFFER_SIZE) {
                    rx_complete = true;
                    rx_count = 0;
                }
                
                RA3 = 0;
            }
            
        }
        
        //受信完了フラグが立ったら
        if (rx_complete == true)
        {          
            
            if (rx_buffer[1] == 1) {
                RA0 = 1;
            } else {
                RA0 = 0;
            }
//            
            if (rx_buffer[2] == 1) {
                RA1 = 1;
            } else {
                RA1 = 0;
            }
//            
//            if (rx_buffer[2] == 1) {
//                RA2 = 1;
//            } else {
//                RA2 = 0;
//            }
            
            RA3 = 1;
            
            rx_complete = false;
            
        }
    }
    
}