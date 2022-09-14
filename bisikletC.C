#include <pic.h>
#include <delay.c>
#include <lcd.c>
#include <stdio.h>

//Konfigurasyon ayarlari
__CONFIG(WDTDIS&LVPDIS&PWRTEN&XT);

// Reed anahtar RA0'a pinine bagli
#define SW RA0 

// Global degiskenler
unsigned char kontrol=0;
unsigned char sonuc[]="                ";
unsigned int CX;
float pi=3.14159, r=0.3302; //Teker yaricapi

//---- TIMER kesme alt programi -----
void interrupt kesme(void){
CX++;
if(CX>10000)kontrol=1;
	
T0IF=0; // Bayragi temizle
TMR0=131; // TIMER'i tekrar yukle
}

//--------- LCD alt programi----------
void LCD_yaz(void){
float hiz;
unsigned int hiz2;

lcd_clear(); // LCD'yi temizle
hiz=2*pi*r*3600/CX; // km/saat cinsinden
hiz2=(int)hiz; // hiz degerini hesapla

lcd_goto(0x00); lcd_puts(" HIZ GOSTERGESI ");

if(kontrol==0){ // Sonucu LCD'ye yazdir
sprintf(sonuc+3,"%d Km/Saat",hiz2);
lcd_goto(0x40); lcd_puts(sonuc);
}

else{ // Bisiklet duruyorsa Hiz=0 yazdir 
lcd_goto(0x41); lcd_puts("*** HIZ=0 ***");
}
	
}
//----------- ANA PROGRAM ------------

main(void)
{
       	
TRISA=0x01; // Port A'nin ilk pini giris  
TRISB=0x00; // PortB'nin hepsi cikis
CMCON=0x07; // PortA sayisal giris
PORTA=0; // Baslangic durumu ayarlari
PORTB=0;  

// LCD islemleri
DelayMs(250);
lcd_init();
lcd_clear();
lcd_write(0x0C); //imleci gizle
lcd_goto(0x00); lcd_puts(" HIZ GOSTERGESI ");

// TIMER islemleri
T0CS=0; // Dahili clock
PSA=0; // Prescaler TMR0 icin ayarli
PS0=0; PS1=1; PS2=0; // Oran 1:8
ei(); // Butun kesmeler etkin
T0IE=0; // Baslangicta TIMER pasif
T0IF=0; // Bayrak temizle

// Hiz olcum islemleri
for(;;){

CX=0; kontrol=0;
	
while(SW==0); // Giris 0 ise bekle
TMR0=131; // 125 sayim (256-131=125)
T0IE=1; // TIMER baslat
while(SW==1); // Giris 1 ise bekle
while(kontrol==0){if(SW==1)break;}

T0IE=0; // TIMER'i durdur
LCD_yaz(); // LCD alt programina git

DelayMs(250); // 250 ms bekle
while(SW==1); 

} // islemleri tekrarla
} // Programin sonu

