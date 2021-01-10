#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

int horas;
int minutos;
int segundos;
int decimas;


void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
  lcd.write(byte(0));
 lcd.print("Careses  Clock");
 lcd.write(byte(0));
 horas=0;
 minutos=0;
 segundos=0; 
 byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
  };
  lcd.createChar(0, Heart);
}

void loop() {
 int x;
 x = analogRead (0);
 lcd.setCursor(10,1);
 if (x < 60) {
   horas = horas + 1;
 }
 else if (x < 200) {
   minutos = minutos + 1;
 }
 else if (x < 400){
   lcd.print ("Down  ");
 }
 else if (x < 600){
   lcd.print ("Left  ");
 }
 else if (x < 800){
   lcd.print ("Select");
 }
 

 
 //Actualizar la hora
 decimas = decimas+2;
 if (decimas>=10)
 {
   segundos = segundos + 1;
   decimas = 0;
 }
 if (segundos>=60)
 {
   minutos = minutos+1;
   segundos = 0; 
 }
 if (minutos>=60)
 {
   horas = horas+1;
   minutos = 0; 
 }
 if (horas>=24)
   horas = 0;
 
 
 //Imprimir la hora
 lcd.setCursor(4,1);
 if (horas<10)
   lcd.print("0");
 lcd.print(horas);
 lcd.print(":");
 if (minutos<10)
   lcd.print("0");
 lcd.print(minutos);
 lcd.print(":");
 if (segundos<10)
   lcd.print("0");
 lcd.print(segundos);
 
 delay(200);
} 
