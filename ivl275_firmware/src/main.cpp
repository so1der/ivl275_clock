#include <Arduino.h>
#include <MsTimer2.h>
#include <Wire.h>

#define btn1 A3
#define btn2 A2

#define SECOND 0x00
#define MINUTE 0x01
#define HOUR 0x02

byte i;
byte a[4];
const byte grids[4] = {B0001110, B0001101, B0001011, B0000111};
byte randome;
byte enumerate = 4;
byte last_hour, last_minute;
unsigned long last_millis = 0;
unsigned long current_millis;
const unsigned long glitch_interval = 194150;


unsigned long lastDebounceTime = 0;  
unsigned long lastDebounceTime2 = 0;  
const byte debounceDelay = 50;  

byte zero = B00000011;
byte one = B11010111;
byte two = B01001000;
byte three = B01000100;
byte four = B10010100;
byte five = B00100100;
byte six = B00100000;
byte seven = B01010111;
byte eight = B00000000;
byte nine = B00000100;

byte cirle_animation1 = B01111111;
byte cirle_animation2 = B11011111;
byte cirle_animation3 = B11110111;
byte cirle_animation4 = B11101111;
byte cirle_animation5 = B11111011;
byte cirle_animation6 = B10111111;

byte digits[] = {zero, one, two, three, four, five, six, seven, eight, nine, 
                cirle_animation1, cirle_animation2, cirle_animation3, cirle_animation4, cirle_animation5, cirle_animation6};


char DCM_to_BCD(unsigned char value){
  unsigned char msb, lsb, bcd;
  msb = value / 10;
  lsb = value % 10;
  bcd = ((msb<<4) + lsb);
  return bcd;
}

void DS1307_adjust(char hour, char minute, char second){
  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.write(DCM_to_BCD(second));
  Wire.write(DCM_to_BCD(minute));
  Wire.write(DCM_to_BCD(hour));
  Wire.endTransmission();
  delay(5);
}

uint8_t DS1307_read(uint8_t addr){
  byte data;
  Wire.beginTransmission(0x68);
  Wire.write(addr);
  Wire.endTransmission();
  delay(1);

  Wire.requestFrom(0x68, 1);
  delay(1);
  if (Wire.available()){
    data = Wire.read();
    data = ((data>>4) * 10) + (data&0x0F);
  }
  return data;
}

void DS1307_1Hz_on(){
  Wire.beginTransmission(0x68);
  Wire.write(0x07);
  Wire.write(B10010000);
  Wire.endTransmission();
  delay(5);
}

void handle_grids(int grid_number) {
  PORTB = grids[grid_number];
  PORTD = digits[a[grid_number]];
}

void to_Timer() {
  handle_grids(i);
  i++; if (i > 3) {
    i = 0;
  }
}

void glitch_To_Timer() {
  if (i == randome) {
    i++;
  }
  handle_grids(i);
  i++;
  if (i > 3) {
    i = 0;
  }
}

void glitch() {
  randome = random(0, 4);
  for (int g = 0; g < random(1, 3); g++) {
    MsTimer2::stop();
    MsTimer2::set(1, glitch_To_Timer);
    MsTimer2::start();
    delay(random(40, 150));
    MsTimer2::stop();
    MsTimer2::set(1, to_Timer);
    MsTimer2::start();
    delay(random(40, 75));
  }
}


void setup()
{
  DDRD = B11111111;
  PORTD = B00000000;
  DDRB = B00001111;
  ADCSRA = 0; // ADC turn off ????
  Wire.begin();
  DS1307_1Hz_on();
  //clock.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //clock.adjust(DateTime(2023, 9, 29, 20, 52, 0));
  MsTimer2::set(1, to_Timer);
  MsTimer2::start();
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  last_hour = a[1];
  last_minute = a[3];
}

void cathodes_enumeration(int numbers) {
  if (numbers != 0) {
    for (int x = 0; x < 3; x++) {
      for (int s = 10; s < 16; s++ ) {
        switch (numbers) {
          case 1: a[3] = s; break;
          case 2: a[3] = s; a[2] = s; break;
          default: a[3] = s; a[2] = s; a[1] = s; a[0] = s; break;
        }
        delay(15);
      }
    }
    a[0] = DS1307_read(HOUR) / 10;
    a[1] = DS1307_read(HOUR) % 10;
    a[2] = DS1307_read(MINUTE) / 10;
    a[3] = DS1307_read(MINUTE) % 10;
    enumerate = 0;
  }
}

void handle_buttons(){
  byte hour = DS1307_read(HOUR);
  int minute = DS1307_read(MINUTE);

  bool adjust = digitalRead(btn1);
  if (!adjust){
    lastDebounceTime = millis();
  }

  bool adjust2 = digitalRead(btn2);
  if (!adjust2){
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    //clock.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() + 1, now.minute(), now.second()));
    DS1307_adjust(hour + 1, minute, 0);
    
}
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    minute = minute - 1;
    if(minute == -1){
      minute = 59;
    }
    DS1307_adjust(hour, minute, 0);
}
}

void loop()
{
  current_millis = millis();
  a[0] = DS1307_read(HOUR) / 10;
  a[1] = DS1307_read(HOUR) % 10;
  a[2] = DS1307_read(MINUTE) / 10;
  a[3] = DS1307_read(MINUTE) % 10;

  if (current_millis - last_millis >= glitch_interval) {
    glitch();
    last_millis = current_millis;
  }

  if (a[3] != last_minute) {
    last_minute = a[3];
    if (a[3] == 0){
      enumerate = 2;
    }
    else{
      enumerate = 1;
    }
  }

  if (a[1] != last_hour) {
    last_hour = a[1];
    enumerate = 4;
  }

  cathodes_enumeration(enumerate);
  handle_buttons();
}
