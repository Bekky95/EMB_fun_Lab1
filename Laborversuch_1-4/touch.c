#include <stdio.h>
#include <stdbool.h>
#include "LPC17xx.h"
#include "GLCD.h"
#include "touch.h"

#define TOUCH_MSR_Y  0x90   //读X轴坐标指令 addr:1
#define TOUCH_MSR_X  0xD0   //读Y轴坐标指令 addr:3

#define TP_CS_LOW()                 LPC_GPIO0->FIOCLR = (1<<6);
#define TP_CS_HIGH()                LPC_GPIO0->FIOSET = (1<<6);
// 读取TP_INT引脚状态,0为有按下,1为释放
// 这里为了使用方便,TP_DOWN()返回1为有按下,0为释放.
#define TP_DOWN()                  (!( LPC_GPIO2->FIOPIN & (1<<13) ) )

/* P0.7 SCK0,P0.8 MISO,P0.9 MOSI */
#define SCK_LOW()          LPC_GPIO0->FIOCLR = (1<<7);
#define SCK_HIGH()         LPC_GPIO0->FIOSET = (1<<7);
#define MOSI_LOW()         LPC_GPIO0->FIOCLR = (1<<9);
#define MOSI_HIGH()        LPC_GPIO0->FIOSET = (1<<9);
#define MISO_value()       ( LPC_GPIO0->FIOPIN & (1<<8) )


static void spi_delay(void)
{
    volatile uint32_t dl;
    for(dl=0;dl<25;dl++);
}

uint8_t SPI_WriteByte(unsigned char data)
{
    uint8_t r_data = 0;
    uint32_t i;
    for(i=0;i<8;i++)
    {
        if( data & (1<<7))
        {
            MOSI_HIGH();
        }
        else
        {
            MOSI_LOW();
        }
        data <<= 1;
        spi_delay();
        SCK_HIGH();
        if( MISO_value() )
        {
            r_data++;
        }
        r_data <<= 1;
        spi_delay();
        SCK_LOW();
    }
    return r_data;
}

void touch_init(void)
{
    LPC_GPIO2->FIODIR      &= ~(1 << 13);    /* PORT2.13 defined as input       */
    LPC_GPIOINT->IO2IntEnF |=  (1 << 13);    /* enable falling edge irq         */
    NVIC_EnableIRQ(EINT3_IRQn);              /* enable irq in nvic              */

    LPC_GPIO0->FIODIR |=  (1<<6); // P0.6 CS is output
    TP_CS_HIGH();

    // SPI config,CPOL=0,CPHA=0
    {
        LPC_GPIO0->FIODIR &= ~(1<<8); // P0.8 MISO input
        LPC_GPIO0->FIODIR |=  (1<<7) | (1<<9); // P0.7 SCK,P0.9 MOSI is output
        SCK_LOW();
    }

    TP_CS_LOW();
    SPI_WriteByte( 1<<7 ); /* 打开中断 */
    TP_CS_HIGH();

    //GUI_Text(0,20,"touch init",Green,Blue);
}

void touch_show(void)
{
    uint16_t tmpx[10];
    uint16_t tmpy[10];
    uint16_t touch_x,touch_y;
		uint16_t kx, ky;
    unsigned int i;
		char feld[20];
    //GUI_Text(0,100,"          ",White,Blue);
    //GUI_Text(0,40,"touch down",White,Blue);

    // 如果一直按下,就一直读取并显示原始坐标
    while( TP_DOWN() )
    {
        for(i=0; i<10; i++)
        {
            TP_CS_LOW();
            SPI_WriteByte(TOUCH_MSR_X);                            /* read X */
            tmpx[i] = SPI_WriteByte(0x00)<<4;                      /* read MSB bit[11:8] */
            tmpx[i] |= ((SPI_WriteByte(TOUCH_MSR_Y)>>4)&0x0F );    /* read LSB bit[7:0] and prepare read Y */
            tmpy[i] = SPI_WriteByte(0x00)<<4;                      /* read MSB bit[11:8] */
            tmpy[i] |= ((SPI_WriteByte(0x00)>>4)&0x0F );           /* read LSB bit[7:0] */
            SPI_WriteByte( 1<<7 ); /* 再次打开中断 */
            TP_CS_HIGH();
        }

        //去最高值与最低值,再取平均值
        {
            uint32_t min_x = 0xFFFF,min_y = 0xFFFF;
            uint32_t max_x = 0,max_y = 0;
            uint32_t total_x = 0;
            uint32_t total_y = 0;
            unsigned int i;

            for(i=0; i<10; i++)
            {
                if( tmpx[i] < min_x )
                {
                    min_x = tmpx[i];
                }
                if( tmpx[i] > max_x )
                {
                    max_x = tmpx[i];
                }
                total_x += tmpx[i];

                if( tmpy[i] < min_y )
                {
                    min_y = tmpy[i];
                }
                if( tmpy[i] > max_y )
                {
                    max_y = tmpy[i];
                }
                total_y += tmpy[i];
            }
            total_x = total_x - min_x - max_x;
            total_y = total_y - min_y - max_y;
            touch_x = total_x / 8;
            touch_y = total_y / 8;
        }//去最高值与最低值,再取平均值
			}
       
}
