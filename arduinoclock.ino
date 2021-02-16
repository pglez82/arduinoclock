#include <LiquidCrystal.h>
#include <DHT.h>
#include "LowPower.h"

#define DHTTYPE DHT22

//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
const int DHTPin = 2;

LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
DHT dht(DHTPin, DHTTYPE);

int horas;
int minutos;
int segundos;
int decimas;

int luzContador;

int pulsacionesSelect;
boolean edicionHoras; //True si estamos editando la hora y false si no lo estamos
boolean edicionMinutos;
boolean parpadeoHoras;
boolean parpadeoMinutos;

const int bl_intensity = 1;


void actualizarHora(){
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
  if (minutos<0)
  {
    horas = horas-1;
    minutos = 59; 
  }
  if (horas>=24)
    horas = 0;
  
  if (horas<0)
  {
    horas = 23;
  }
}

void imprimirHora(){
  lcd.setCursor(0,1);
  
  if (edicionHoras==true && parpadeoHoras==true)
  {
    lcd.print("  ");
    parpadeoHoras=false;
  }
  else
  { 
    if (horas<10)
      lcd.print("0");
    lcd.print(horas);
    parpadeoHoras=true;
  }  
  lcd.print(":");
  
  if (edicionMinutos==true && parpadeoMinutos==true)
  {
    lcd.print("  ");
    parpadeoMinutos=false;
  }
  else
  {
    if (minutos<10)
      lcd.print("0");
      
    lcd.print(minutos);
    parpadeoMinutos=true;
  }
  
  lcd.print(":");
  if (segundos<10)
    lcd.print("0");
    
  lcd.print(segundos); 
}

void gestionBotones(){
  int x = analogRead (0);
  if (x < 60) {
    horas = horas + 1;
  }
  else if (x < 200) {
    if (edicionHoras==true)
      horas = horas + 1;
    if (edicionMinutos==true)
      minutos = minutos + 1;
  }
  else if (x < 400){
    if (edicionHoras==true)
      horas = horas - 1;
    if (edicionMinutos==true)
      minutos = minutos - 1;
  }
  else if (x < 600){
    analogWrite(pin_BL, 255);
    luzContador = 50;
  }
  else if (x < 800){
    pulsacionesSelect = pulsacionesSelect + 1;
    
    if (edicionMinutos==true && pulsacionesSelect == 1)
      edicionMinutos = false;
    
    if (edicionHoras==true && pulsacionesSelect == 1)
    {
      edicionMinutos = true;
      edicionHoras = false;
    }
  }
  else{
     pulsacionesSelect = 0; 
  }
  
  if (pulsacionesSelect==5)
    edicionHoras = true;
}

void imprimirTempHum(){
  if ((segundos==0 || segundos==20 || segundos==40) && decimas==2)
  {
     float t = dht.readTemperature();
     Serial.println(t);
     lcd.setCursor(11,1);
     lcd.print(t,1);
     lcd.print("C");
  }
  if ((segundos==10 || segundos==30 || segundos==50) && decimas==2)
  {
     float h = dht.readHumidity();
     Serial.println(h);
     lcd.setCursor(11,1);
     if (h<100)
       lcd.print("  ");
     else
       lcd.print(" ");
     lcd.print(h,0);
     lcd.print("%");
    
  }
  
}


void setup() {
  Serial.begin(9600);
  Serial.println("Comienzo...");
  pinMode(pin_BL, OUTPUT);
  analogWrite(pin_BL, bl_intensity);
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
  pulsacionesSelect = 0;
  edicionHoras = false;
  edicionMinutos = false;
  parpadeoHoras=false;
  parpadeoMinutos=false;
  luzContador=0;
  lcd.createChar(0, Heart);
  dht.begin();
}

void loop() {  
  lcd.setCursor(10,1);
  
  gestionBotones();
  actualizarHora();
  imprimirHora();
  imprimirTempHum();

  
  if (luzContador>0)
    luzContador = luzContador - 1;
  
  if (luzContador==0)
  {
    analogWrite(pin_BL, bl_intensity);
    luzContador = luzContador - 1;
  }

  
  LowPower.idle(SLEEP_120MS, ADC_OFF, TIMER2_OFF, TIMER1_ON, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  delay(75);
  
} 
