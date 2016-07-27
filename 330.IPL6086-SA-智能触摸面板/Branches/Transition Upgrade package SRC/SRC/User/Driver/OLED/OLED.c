/***************************Copyright BestFu ***********************************
**  文    件：  OLED.c
**  功    能：  <<驱动层>> 1.3寸OLED液晶驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.06.30
**  修改日期：  2014.08.20
**  说    明：  SH1106 驱动程序
**              OLED 即有机发光二极管
**              优点：自发光，无需背光，对比度高，厚度薄，视角广
*******************************************************************************/

/******************************************************************************
**        OLED Module Message
** LCD Module:        VGM12864C3W01 -- 1.3 inch
** Display Size:      132x64
** Driver/Controller: SH1106
** Interface:         IIC串行接口(400KHz) (还可支持 8080/SPI接口)
******************************************************************************/
#include "OLED.h"
#include "IIC.h"
#include "delay.h"
#if SUPPORT_OLED_PUT_ASCII
 #include "Font.h"
#endif
//#include "OLED_pic.h"


/******************************************************************************
**函    数:  OLED_WriteCmd()
**功    能:  OLED写命令
**参    数:  Command     命令
**返    回:  void
******************************************************************************/
#define OLED_WriteCmd(Command)      IIC_WriteByte(0x00,Command)

/******************************************************************************
**函    数:  OLED_WriteData()
**功    能:  OLED写数据
**参    数:  Data        数据
**返    回:  void
*******************************************************************************/
#define OLED_WriteData(Data)        IIC_WriteByte(0x40,Data)

/******************************************************************************
**函    数:  Init_OLED()
**功    能:  OLED12864液晶驱动  -- IIC 接口模式
**参    数:  null
**返    回:  null
**说    明： 可初始化 SSD1306/SH1106
*******************************************************************************/
void Init_OLED()        
{
    IIC_InitPort();
    GPIOx_Cfg(PORT_OLED_POW, PIN_OLED_POW, OUT_PP_400K);
    OLED_POW_1;
    GPIOx_Cfg(PORT_OLED_RST, PIN_OLED_RST, OUT_PP_400K);
    GPIOx_Rst(PORT_OLED_RST, PIN_OLED_RST); 
    delay_ms(20);
    GPIOx_Set(PORT_OLED_RST, PIN_OLED_RST);

    OLED_WriteCmd(0xAE);    //display off
    OLED_WriteCmd(0x20);    //Set Memory Addressing Mode    
    OLED_WriteCmd(0x10);    //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_WriteCmd(0xb0);    //Set Page Start Address for Page Addressing Mode,0-7
    //OLED_WriteCmd(0xc8);    //Set COM Output Scan Direction
    OLED_WriteCmd(0xc0);    //Set COM Output Scan Direction
    OLED_WriteCmd(0x00);    //--set low column address
    OLED_WriteCmd(0x10);    //--set high column address
    OLED_WriteCmd(0x40);    //--set start line address
    OLED_WriteCmd(0x81);    //--设置对比度  0x00--0xFF  
    OLED_WriteCmd(0x7f);    
    //OLED_WriteCmd(0xa1);    //--set segment re-map 0 to 127
    OLED_WriteCmd(0xa0);    //--set segment re-map 0 to 127
    OLED_WriteCmd(0xa6);    //--set normal display
    OLED_WriteCmd(0xa8);    //--set multiplex ratio(1 to 64)
    OLED_WriteCmd(0x3F);    //
    OLED_WriteCmd(0xa4);    //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_WriteCmd(0xd3);    //-set display offset
    OLED_WriteCmd(0x00);    //-not offset
    OLED_WriteCmd(0xd5);    //--set display clock divide ratio/oscillator frequency
    OLED_WriteCmd(0xf0);    //--set divide ratio
    OLED_WriteCmd(0xd9);    //--set pre-charge period
    OLED_WriteCmd(0x22);
    OLED_WriteCmd(0xda);    //--set com pins hardware configuration
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0xdb);    //--set vcomh
    OLED_WriteCmd(0x20);    //0x20,0.77xVcc
    OLED_WriteCmd(0x8d);    //--set DC-DC enable
    OLED_WriteCmd(0x14);    //
    delay_ms(5);
    OLED_ClearScreen();   
    //OLED_ShowPicture(Bestfu_BMP);
    OLED_WriteCmd(0xAF);    //--turn on oled panel 
}

/*******************************************************************************
**函    数:  OLED_ClearScreen()
**功    能:  清屏
**参    数:  null
**返    回:  null
**说    明：  
*******************************************************************************/
void OLED_ClearScreen(void)
{
  unsigned char x,y;
  for(y=0;y<8;y++)
    {
      OLED_WriteCmd(0xb0+y);
      OLED_WriteCmd(0x00);
      OLED_WriteCmd(0x10);
      for(x=0;x<132;x++)
        {
          OLED_WriteData(0);
        }
    }
}
/*******************************************************************************
**函    数:  OLED_SetCursor()
**功    能:  设置光标
**参    数:  null
**返    回:  null
**说    明： 横坐标扩展8倍
*******************************************************************************/
void OLED_SetCursor(u8 XPos, u8 YPos)
{
    u8 Xtmp;
    XPos*=8;    
    Xtmp = XPos&0x0F;               //X 低四位
    XPos = ((XPos>>4)&0x0F)+0x10;   //X 高四位
    OLED_WriteCmd(YPos+0xB0);       //Y
    OLED_WriteCmd(Xtmp+2);          //设置列低四位地址
    OLED_WriteCmd(XPos);            //设置列高四位地址
}
/*******************************************************************************
**函    数:  OLED_SetCursor_Normal()
**功    能:  设置光标
**参    数:  null
**返    回:  null
**说    明： 横坐标不带扩展（实际位置） 
*******************************************************************************/
void OLED_SetCursor_Normal(u8 XPos, u8 YPos)
{
    u8 Xtmp;
    Xtmp = XPos&0x0F;               //X 低四位
    XPos = ((XPos>>4)&0x0F)+0x10;   //X 高四位
    OLED_WriteCmd(YPos+0xB0);       //Y
    OLED_WriteCmd(Xtmp+2);          //设置列低四位地址
    OLED_WriteCmd(XPos);            //设置列高四位地址
}
/*******************************************************************************
**函    数:  OLED_ShowPicture()
**功    能:  OLED显示图片
**参    数:  null
**返    回:  null
**说    明： 显示整屏图片 
*******************************************************************************/
void OLED_ShowPicture(const u32 *p_Pic)
{
  unsigned char x,y;
  u32 ptemp;
  for(y=0;y<8;y++)
    {
      OLED_WriteCmd(0xb0+y);
      OLED_WriteCmd(0x00+2);
      OLED_WriteCmd(0x10);
      for(x=0;x<32;x++)
        {
//          OLED_WriteData(*p_Pic++);
            ptemp = *p_Pic++ ;
            OLED_WriteData( (ptemp)    &0xFF);
            OLED_WriteData( (ptemp>>8) &0xFF);
            OLED_WriteData( (ptemp>>16)&0xFF);
            OLED_WriteData( (ptemp>>24)&0xFF);
        }
    }
}

/*******************************************************************************
**函    数:  OLED_ShowPicture()
**功    能:  OLED显示图片
**参    数:  null
**返    回:  null
**说    明： 显示整屏图片 
*******************************************************************************/
void OLED_ShowPicAt(u8 x0, u8 y0, u8 xlen, u8 ylen,const u8 *p_Pic)
{
  unsigned char xtemp,ytemp;
    
  for(ytemp=y0; ytemp< (y0+(ylen/8));ytemp++)
    {
      OLED_SetCursor(x0,ytemp);
      for( xtemp = 0; xtemp < xlen ; xtemp++ )
        {
          OLED_WriteData(*p_Pic++);
        }
    }
}

/*******************************************************************************
**函    数:  OLED_ShowHalfPicAt()
**功    能:  OLED指定位置显示半屏（64*64）图片
**参    数:  x0       -- 起点横坐标
**返    回:  null
**说    明： 显示整屏图片 
*******************************************************************************/
void OLED_ShowHalfPicAt(u8 x0,const u32 *p_Pic)
{
    u8 x,y;
    u32 ptemp;
    
    for( y=0; y< 8; y++ )
    {
        OLED_SetCursor(x0,y);
        for( x =0; x < 16 ; x++ )
        {
            ptemp = *p_Pic++ ;
            OLED_WriteData( (ptemp)    &0xFF);
            OLED_WriteData( (ptemp>>8) &0xFF);
            OLED_WriteData( (ptemp>>16)&0xFF);
            OLED_WriteData( (ptemp>>24)&0xFF);
        }
    }
}
/*******************************************************************************
**函    数:  OLED_ShowHalfPicAt()
**功    能:  OLED指定位置显示四分之一屏（64*32）图片
**参    数:  x0       -- 起点横坐标
**返    回:  null
**说    明： 显示整屏图片 
*******************************************************************************/
void OLED_ShowQuartPicAt(u8 x0,u8 y0,const u32 *p_Pic)
{
    u8 x,y;
    u32 ptemp;
    
    for( y=0; y< 4; y++ )
    {
        OLED_SetCursor(x0,y+y0);
        for( x =0; x < 16 ; x++ )
        {
            ptemp = *p_Pic++ ;
            OLED_WriteData( (ptemp)    &0xFF);
            OLED_WriteData( (ptemp>>8) &0xFF);
            OLED_WriteData( (ptemp>>16)&0xFF);
            OLED_WriteData( (ptemp>>24)&0xFF);
        }
    }
}

//显示32*32图片
void OLED_Show32x323PicAt(u8 x0,u8 y0,const u32 *p_Pic)
{
    u8 x,y;
    u32 ptemp;
    
    for( y=0; y< 4; y++ )
    {
        OLED_SetCursor(x0,y+y0);
        for( x =0; x < 8 ; x++ )
        {
            ptemp = *p_Pic++ ;
            OLED_WriteData( (ptemp)    &0xFF);
            OLED_WriteData( (ptemp>>8) &0xFF);
            OLED_WriteData( (ptemp>>16)&0xFF);
            OLED_WriteData( (ptemp>>24)&0xFF);
        }
    }
}
/*******************************************************************************
**函    数:  OLED_ShowChar()
**功    能:  ASCII字符显示 （ 8*16 ）
**参    数:  (X0,Y0)  -- 坐标位置（已折算字符码8*16间距） 
**           DspChar  -- 显示的字符
**           DspMode  -- 0 ->正显   1 ->反显
**返    回:  null
*******************************************************************************/
#if SUPPORT_OLED_PUT_ASCII
void OLED_ShowChar(unsigned char X0, unsigned char Y0, char DspChar, unsigned char DspMode)
{
    u8  i,k;
    u8  DspData;
    const u8 *DspTbl = NULL;

    DspTbl = &ASCII_1608[DspChar - ' '][0];
   
    for(i=0;i<2;i++)
    {
        OLED_SetCursor(X0,Y0*2+i);  
        for(k=0;k<8;k++)
        {
            DspData = *DspTbl++;
            if(DspMode)  DspData = ~DspData;    
            OLED_WriteData(DspData);
        }
    }
}
/*******************************************************************************
**函    数:  OLED_ShowCN()
**功    能:  单个汉字显示（ 16*16 ）
**参    数:  (X0,Y0)  -- 坐标位置（已折算字符码16*16间距） 
**           *DspCN   -- 显示的汉字
**           DspMode  -- 0 ->正显   1 ->反显
**返    回:  null
*******************************************************************************/
void OLED_ShowCN(unsigned char X0, unsigned char Y0,const char *DspCN,unsigned char DspMode)
{
    u8  i,j,k;
    u8  DspData;
    const u8   *DspTbl = NULL;

    for(j=0; j<sizeof(GB1616_ZMo)/3; j++)
    {   
        if((*DspCN == GB1616_ZMo[j][0])&&(*(DspCN+1) == GB1616_ZMo[j][1]))
        {
            DspTbl = &GB1616[j][0];
        }
    }
   
    for(i=0;i<2;i++)
    {
        OLED_SetCursor(X0,Y0*2+i);  
        for(k=0;k<16;k++)
        {
            DspData = *DspTbl++;
            if(DspMode)  DspData = ~DspData;    
            OLED_WriteData(DspData);
        }
    }
}

/*******************************************************************************
**函    数:  OLED_ShowString()
**功    能:  字符串(包括汉字)输出 （ 8*16 ）
**参    数:  (X0,Y0)  -- 坐标位置（已折算字符码8*16间距） 
**           *DspStr  -- 字符串指针
**           DspMode  -- 0 ->正显   1 ->反显
**返    回:  null
*******************************************************************************/
void OLED_ShowString(unsigned char X0, unsigned char Y0,const char *DspStr,unsigned char DspMode)
{

    while( *DspStr != '\0')
    {
        if(*DspStr > 0xA0)  {   //汉字
            OLED_ShowCN(X0, Y0, DspStr, DspMode);
            X0+=2;
            DspStr+=2;
        }
        else {                  //字符  <0x80
            OLED_ShowChar(X0, Y0, *DspStr, DspMode);
            X0++;
            DspStr++;
        }
        
    }
}
/*******************************************************************************
**函    数:  LCD_Pow()
**功    能:  m^n次方
**参    数:  m    -- 底数
**           n    -- 指数
**返    回:  n次方结果
*******************************************************************************/
//static unsigned int LCD_Pow(unsigned char m, unsigned char n)
//{
//    unsigned int result = 1; 

//    while(n--)   result*= m;

//    return  result;
//}

/*******************************************************************************
**函    数:  OLCD_ShowNumber()
**功    能:  显示 int 型数字
**参    数:  (X0,Y0)  -- 坐标位置（已折算字符码8*16间距） 
**           Data     -- 显示的数值
**           len      -- 长度  小于5
**           DspMode  -- 0 ->正显   1 ->反显
**返    回:  null
*******************************************************************************/
//void OLED_ShowNumber(unsigned char X0, unsigned char Y0,unsigned int Data,unsigned char len , unsigned char DspMode)
//{
//    unsigned char t,temp;
//    if(len>5) return;
//    for(t=0;t<len;t++)
//    {
//        temp=(Data/LCD_Pow(10,len-t-1))%10;
//        OLED_ShowChar(X0+t, Y0 , temp+'0', DspMode);
//    }
//}
#endif
/*******************************************************************************
**函    数:  OLED_DrawLine()
**功    能:  画线条
**参    数:  当前仅能画竖线
**返    回:  null
*******************************************************************************/
void OLED_DrawLine(unsigned char x0)//,unsigned char y0,unsigned char x1,unsigned char y1)
{
    unsigned char i;
    
    for( i=0 ; i<8 ; i++)
    {
        OLED_SetCursor_Normal(x0,i);
        OLED_WriteData(0xff);
    }
}

//显示数字 18*32 字体
void OLED_ShowNum18x32(unsigned char x0, unsigned char y0,  const u32 *p_Pic)
{
    u8    x,y;
    u32   ptemp;
    const u8 *DspTbl = (u8*)&p_Pic[0];
    
    for( y=0; y< 4; y++ )
    {
        OLED_SetCursor_Normal(x0,y+y0);
        for( x =0; x < 18 ; x++ )
        {
            ptemp = *DspTbl++ ;
            OLED_WriteData(ptemp);
        }
    }
}

/********************************* END FILE ***********************************/
