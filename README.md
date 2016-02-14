# arduino-minimal-heating-solar

How to use minimal resources in Arduino to configure a widget to heat your pool using solar energy.
This uses arduino, potentiometer to regulate and a relay. Is not necessary a clock (RTC), because it simulates times using EEPROM and software.

## Arduino

You will need just:

* Arduino Uno or similar (with power cabble + USB cable to configure )
* One Relay : Turn the water pump on/off to change the hot water from solar panels to the swimming pool
* One potentionmeter : User can configure if is summer/winter,  sunny/cloudy or hot/cold day. With this user can increase or decrease the number of times that the water will change per hour. 
* 6 wires 
* Small breadboard

Project is:

![alt tag](https://raw.github.com/mariohmol/arduino-minimal-heating-solar/master/minheating.png)


## Solar Panels

Exist many ways to capture solar energy, the most common is rubber:

![alt tag](https://raw.github.com/mariohmol/arduino-solar-heating/master/solarheating.jpg)

You can install this in the roof of your house:

![alt tag](https://raw.github.com/mariohmol/arduino-solar-heating/master/ontop.jpg)

Or even in the grownd:

![alt tag](https://raw.github.com/mariohmol/arduino-solar-heating/master/onfloor.jpg)

## Libs

You have to install those libraries:

* https://github.com/PaulStoffregen/Time
   * DateTime and DatetTimeStrings

## Install

Load TimeHeating that can be found here


## Runnning

When you turn on the Arduino it will:

* Blink internal led indicating the current hour. So if is 8am it will blink 8 times
* Then after 3 seconds with led off, it will turn on for 3 seconds.
* After that it will take 3 seconds off and then blink how many times it will activate the motor in the current hour.

### Configuration

Ìn top of the code you will find some attributes where you can configure as you want. 

* What hours in the day i want to change the water?
* How many times in a hour it have to change?
* How many different ranges to change using potentiometer
* How strong will be each range. It uses %, example, 0.5 in a hour that is configure to change 4 times, it will apply 50%, so changes 2 times.

````

//Here we have frequencies and configuration for motor activate per hour
//                 0,1,2,3,4,5,6,7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
int hourTimes[] = {0,0,0,0,0,0,0,0,0,3,3,4,5,5,5,4,0,0,0,0,0,0,0}; //Max times that motor will run in an hour
int rangeAnalogic[] = { 0 , 300, 600, 900, 1023 }; //Set ranges in analogic to change motor start frequency
float rangeMult[] = { 0 , 0.4, 0.6, 0.8, 1 }; //Ranges that decrease frquency by a potentionmeter

```

