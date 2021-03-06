
// grpw_box_sketch
// 1.0 MEH 04 Sep 2014 use code from Adafruit and Arduino core examples
//     monitor temperature, humidity in grow box
//     write to SD
//     turn lamp on, wait to 16 hrs, turn lamp off, wait to 24 hours, repeat 

#include <stdlib.h>      //type conversion etc.
#include <Time.h>       // elementary tracking of seconds since start  
#include <DHT.h>        // DHT22 Humidity Sensor
#include <TFT.h>        // Arduino LCD library
#include <SPI.h>        // SPI (needed for TFT, serial peripheral interface)
// pin definition for the Uno
#define DHTPIN 6        // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define LCD_CS 10
#define DC   9
#define RESET  8  
const int light = 3;
long lampon = 57599;  //above this time, lamp is off
long lampoff = 86399; // at this time, clock is reset and lamp on
long sec;
float t;
float h;
float th;
float tl;
float hh;
float hl;
char temperature[5];
char humidity[3];
char ltemperature[5];
char lhumidity[3];
char htemperature[5];
char hhumidity[3];
char clock[6];
TFT TFTscreen = TFT(LCD_CS, DC, RESET);
DHT dht(DHTPIN, DHTTYPE);

// the setup routine runs once when you press reset:
void setup() {                
  t = 0;
  th = 0;
  tl = 0;
  h = 0;
  hh = 0;
  hl = 0;
  pinMode(light, OUTPUT);  
  digitalWrite(light, HIGH);   // turn the LED on (HIGH is the voltage level)
  dht.begin();
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(255,255,255);

  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  delay(1000);     // wait for a second
  sec = now();

  // black out the previous dynamic text
  TFTscreen.setTextSize(3);
  TFTscreen.stroke(0,0,0);
  TFTscreen.text(temperature, 0, 20);
  TFTscreen.text(humidity, 0, 50);
  TFTscreen.setTextSize(1);  
  TFTscreen.text(ltemperature, 100, 35);
  TFTscreen.text(htemperature, 100, 20);
  TFTscreen.text(lhumidity, 100, 65);
  TFTscreen.text(hhumidity, 100, 50);
  TFTscreen.setTextSize(3);  
  TFTscreen.text(clock, 0, 80);

  if (sec > lampon){
    digitalWrite(light, LOW);    // turn the LED off by making the voltage LOW
  }

  if (sec > lampoff){
    setTime(0);                  // reset the system clock
    digitalWrite(light, HIGH);   // turn the LED on (HIGH is the voltage level)
  }

  h = dht.readHumidity();
  t = dht.readTemperature();

  if (th==0) {
    th = t;
  }
  if (tl==0) {
    tl = t;
  }
  if (hh==0) {
    hh = h;
  }
  if (hl==0) {
    hl = h;
  }
  if (t>th) {
    th = t;
  }
  if (t<tl) {
    tl = t;
  }
  if (h>hh) {
    hh = h;
  }
  if (h<hl) {
    hl = h;
  }

  ltoa(sec,clock,10);
  dtostrf(t,4,1,temperature);
  dtostrf(h,2,0,humidity);
  dtostrf(th,4,1,htemperature);
  dtostrf(hh,2,0,hhumidity);
  dtostrf(tl,4,1,ltemperature);
  dtostrf(hl,2,0,lhumidity);

  TFTscreen.stroke(255,255,255);
  TFTscreen.text(temperature, 0, 20);
  TFTscreen.text(humidity, 0, 50);
  TFTscreen.setTextSize(1);  
  TFTscreen.stroke(32,32,255);
  TFTscreen.text(ltemperature, 100, 35);
  TFTscreen.stroke(255,32,32);
  TFTscreen.text(htemperature, 100, 20);
  TFTscreen.stroke(32,32,255);
  TFTscreen.text(lhumidity, 100, 65);
  TFTscreen.stroke(255,32,32);
  TFTscreen.text(hhumidity, 100, 50);
  TFTscreen.setTextSize(3);  
  TFTscreen.stroke(255,255,255);
  TFTscreen.text(clock, 0, 80);

  if (t>35) {
    TFTscreen.background(255, 0, 0);
    digitalWrite(light, LOW);    // turn the LED off by making the voltage LOW
    delay(86400);   
  }
}

