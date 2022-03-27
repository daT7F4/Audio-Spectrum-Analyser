//ORIGINAL CODE: https://create.arduino.cc/projecthub/mircemk/diy-fft-audio-spectrum-analyzer-ca2926

#include "U8glib.h"
#include "fix_fft.h"

#define LINEY 50
#define LINEXL 0
#define LINEXR 128

#define SAMPLES 128

#define AUDIO A0

U8GLIB_KS0108_128 lcd(4,5,6,7,8,9,10,11,A5,A1,A2,A4,A3); //If you are using a KS0108 GLCD

char im[SAMPLES];
char data[SAMPLES];
int barht[SAMPLES];

void setup()
{
  lcd.begin();
  
}

void loop()
{
  static int i, j;
  int val;
  

  for(i = 0; i < SAMPLES; i++)
  {
    val = analogRead(AUDIO); 
    data[i] = (char)(val/4 - 128);
    im[i] = 0;
  }


  fix_fft(data, im, 7, 0);

  for(i = 0; i < SAMPLES/2; i++)
  {
    barht[i] = (int)sqrt(data[i] * data[i] + im[i] * im[i]);
  }
  
  for(i = 0, j = 0; i < SAMPLES/2; i++, j += 2)
  {
    barht[i] = barht[j] + barht[j + 1];
  }
  
  barchart(SAMPLES/4, barht);
} 

void barchart(int n, int bh[])
{
  int i, s, w;
  
  s = (LINEXR - LINEXL) / n;
  w = s / 2;
  
  lcd.firstPage();
  do
  {
    lcd.setFont(u8g_font_helvR08);
    lcd.drawStr(20, 10, "FFT Audio Spectrum");
    lcd.drawLine(LINEXL, LINEY, LINEXR, LINEY);
    lcd.drawStr(0, LINEY + 10, "0");
    lcd.drawStr(29, LINEY + 10, "1k");
    lcd.drawStr(59, LINEY + 10, "2k");
    lcd.drawStr(91, LINEY + 10, "3k");
    lcd.drawStr(115, LINEY + 10, "Hz");
    lcd.drawLine(0, 50, 0, 0);
    
    for(i = 0; i < n; i++)
    {
      lcd.drawBox(LINEXL + s * i - 1, LINEY - bh[i], w, bh[i] + 1); // u8glib doesn't accept box height of 0
    }
      
  }while(lcd.nextPage()); 
}
