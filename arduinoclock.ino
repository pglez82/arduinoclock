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

int luzContador;

int pulsacionesSelect;
boolean edicionHoras; //True si estamos editando la hora y false si no lo estamos
boolean edicionMinutos;
boolean parpadeoHoras;
boolean parpadeoMinutos;


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
    lcd.setCursor(4,1);
  
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


void setup() {
  Serial.begin(9600);
  pinMode(pin_BL, OUTPUT);
  analogWrite(pin_BL, 10);
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
}

void loop() {  
  lcd.setCursor(10,1);
  
  gestionBotones();
  actualizarHora();
  imprimirHora();
  
  if (luzContador>0)
    luzContador = luzContador - 1;
  
  if (luzContador==0)
  {
    analogWrite(pin_BL, 10);
    luzContador = luzContador - 1;
  }

  delay(193);
} 

