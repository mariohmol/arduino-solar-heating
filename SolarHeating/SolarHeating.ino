/*
  TimerHeating

 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.

 */
 //https://github.com/PaulStoffregen/Time
#include <TimeLib.h>
#include <EEPROM.h>
#include <Arduino.h>  // for type definitions


struct config_t
{
    long datetime;
} configuration;


//Configure pin in Arduino
int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int motorPin = 7;     //pin to connect relay that actives the motor
int sensorValue = 0;  // variable to store the value coming from the sensor

//Here we have frequencies and configuration for motor activate per hour
//                 0,1,2,3,4,5,6,7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
int hourTimes[] = {0,0,0,0,0,0,0,0,0,3,3,4,5,5,5,4,0,0,0,0,0,0,0}; //Max times that motor will run in an hour
int rangeAnalogic[] = { 0 , 300, 600, 900, 1023 }; //Set ranges in analogic to change motor start frequency
float rangeMult[] = { 0 , 0.4, 0.6, 0.8, 1 }; //Ranges that decrease frquency by a potentionmeter

//Every cycle it checks if hour or minute changes, to activate the motor
int lasthour=0; //hour that was in last cycle
int lastminute=0; //mintes that was in last cycle
int runtimes=0; //number of times that motor ran in this hour
long activetime = 120000; //Milli seconds to let motor running

//Initialization
void setup() {
    // declare the ledPin as an OUTPUT:
    pinMode(ledPin, OUTPUT);
    pinMode(motorPin, OUTPUT);
    Serial.begin(9600);
    digitalWrite(motorPin, HIGH);
    //Set a fix datetime every time it starts

    //setTime(17, 14, 00, 9, 02, 2016); updatetime();
    
    Serial.println("---Trying to get last saved time");
    EEPROM_readAnything(0, configuration);
    if(configuration.datetime==-1){
      Serial.println("* Time not found, initialize manually");
      //setTime(hours, minutes, seconds, days, months, years);
      setTime(9, 00, 00, 07, 02, 2016);
      updatetime();
      Serial.println(configuration.datetime);
    }else{
      Serial.println("* Time found, initialize with saved");
      setTime(configuration.datetime);
    }
    
    
    lasthour=hour();
    lastminute=minute();
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  
  // stop the program for for <sensorValue> milliseconds:
  delay(sensorValue*3);
  printDigits();
  showtime();
  digitalWrite(ledPin, HIGH);
   delay(3000);
   digitalWrite(ledPin, LOW);
   int mult = showmult();
   resetled();
   updatelast();
}

void showtime(){
  for(int i=0;i<hour();i++){
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  delay(3000);
}

void updatelast(){
  int change=0;
  if(lastminute!=minute()){
    lastminute=minute();
    change=1;
  }
  if(lasthour!=hour()){
    lasthour==hour();
    change=1;
    runtimes=0;
    updatetime();
  }
  if(change>0){
    Serial.println("-----Time changed ");
    int mult = getmult();
    if(mult==0) return;
    Serial.println("Times to activate motor in this hour: ");
    Serial.print(mult);
    int interval=60/mult;
    if(minute()>interval*runtimes){
      Serial.println("-----Activating Motor: ");
      //run
      digitalWrite(ledPin, HIGH);
      digitalWrite(motorPin, LOW);   // sets the LED on
      delay(activetime);                  // waits for a second
      digitalWrite(motorPin, HIGH);    // sets the LED off
      digitalWrite(ledPin, LOW);
      delay(1000);  
      runtimes=runtimes+1;
      Serial.println("-----End Motor ");
    }
    
  }
}

int showmult(){
  int mult = getmult();
  
  Serial.println("-----Show mult: ");
  Serial.print(mult);
  delay(1000);
  for(int i=0;i<mult;i++){
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
    Serial.print(i);
  }
  Serial.println("-----end show mult");
  delay(5000);
  return mult;
}

void resetled(){
  Serial.println("-------reset");
  for(int i=0;i<5;i++){
   digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(300);
  }
  delay(2000);
}


int getmult(){
  sensorValue = analogRead(sensorPin);
  Serial.println("get mult");
  for(int i=0;i<5;i++){
    Serial.print(sensorValue);
    Serial.print(" <- sensor , range -> ");
    Serial.print(rangeAnalogic[i]);
    
    if(rangeAnalogic[i]>=sensorValue){
      Serial.println("--------");
      Serial.print("if ge ");
       Serial.print(rangeMult[i]);
      Serial.print("=");
       Serial.print(hourTimes[hour()]);
       Serial.print("==");
       Serial.print(rangeMult[i]*hourTimes[hour()]);
       Serial.print(" return getmult -- ");
       return rangeMult[i]*hourTimes[hour()];
    }
    
  }
  return 0;
}

void printDigits(){
  Serial.println("------printDate");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.print("-----");
}

void updatetime(){
  configuration.datetime=now();
  EEPROM_writeAnything(0, configuration);
}


/**
 * WRITING/READING IN EEPROM
 */

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}
