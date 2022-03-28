//ORIGINAL CODE: https://create.arduino.cc/projecthub/mircemk/diy-fft-audio-spectrum-analyzer-ca2926
#include "U8glib.h"
#include "fix_fft.h"

#define SAMPLES 128

#define AUDIO A0

U8GLIB_KS0108_128 lcd(4,5,6,7,8,9,10,11,A5,A1,A2,A4,A3); // serial use, PSB = GND

char im[SAMPLES];
char data[SAMPLES];
int barht[SAMPLES];
int peaks[SAMPLES];

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
  
  for(i = 0, j = 0; i < SAMPLES / 2; i++, j ++)
  {
    barht[i] = barht[j] + barht[j + 1];
  }
  
 
  barchart(64, barht);
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
    lcd.drawStr(30, 10, "Audio Spectrum");
    lcd.drawLine(0, 63, 128, 63);
    lcd.drawLine(0, 63, 0, 0);
    lcd.drawLine(0, 0, 128, 0);
    lcd.drawLine(127, 0, 127, 63);
    
    for(i = 0; i < n; i++)
    {
      if(64 - bh[i] < 1){
      } else{
        lcd.drawLine(i * 2, 64, i * 2, 64 - bh[i]);
      }
    }
      
  }while(lcd.nextPage()); 
}
