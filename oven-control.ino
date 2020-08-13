#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PID_v1.h>

//#define ONE_WIRE_BUS 2

OneWire ambient(7);
OneWire scope(4);
int fanPWM = 10;
int heaterPower = 5;
int greenLED = 9;
int blueLED = 6;
int redLED = 3; //pwm pin, so can take same commands as heater for visualization


DallasTemperature sensorambient(&ambient);
DallasTemperature sensorscope(&scope);



double Setpoint, Input, Output;

double aggKp=25, aggKi=1, aggKd=50;
double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, aggKp, aggKi, aggKd, DIRECT);
//PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);



LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  Serial.begin(9600);
  sensorambient.begin();
  sensorscope.begin();
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(heaterPower, OUTPUT);
  pinMode(fanPWM, OUTPUT);
  
  //setPwmFrequency(fanPWM, 1024);
  myPID.SetMode(AUTOMATIC);




  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD. 
  lcd.backlight();
  lcd.setCursor(1,0); //(+right, line 0/1)
  lcd.print("Temp");
  lcd.setCursor(10,0);
  lcd.print("Target");
  lcd.setCursor(0,1);
//  lcd.print("0");
  lcd.setCursor(8,1);
//  lcd.print("0");



}















void loop()
{

//  digitalWrite(heaterPower, HIGH);
  analogWrite(fanPWM, 50);
/*  digitalWrite(greenLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, HIGH);*/
  sensorambient.requestTemperatures();
  sensorscope.requestTemperatures();

  Serial.print("ambient: ");
  Serial.print(sensorambient.getTempFByIndex(0));
  Serial.print(", ");
 // Serial.println(sensorscope.getTempFByIndex(0));


  //delay(1000);
  /*
  digitalWrite(heaterPower, LOW);
  analogWrite(fanPWM, 0);
//  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, LOW);



  delay(500);*/
  tempTrack(125,1); //205

}





















void tempTrack(int tempF, int seconds) {
      unsigned long startTime = millis();;
        while (((millis()-startTime)/1000) <= seconds ){ //time is in milli-seconds
          sensorscope.requestTemperatures();

            double currentTemp = 0;
             currentTemp = sensorscope.getTempFByIndex(0);

                Serial.print("current: ");
                Serial.print(currentTemp);
                Serial.print(", Target: ");
                Serial.println(tempF);


/*                double gap = abs(Setpoint-Input); //distance away from setpoint
                  if(gap<100) //15
                  {  //we're close to setpoint, use conservative tuning parameters
                    //myPID.SetTunings(consKp, consKi, consKd);
                  }
                  else
                  {
                     //we're far from setpoint, use aggressive tuning parameters
                     myPID.SetTunings(aggKp, aggKi, aggKd);
                  }
*/

                  Setpoint = tempF;
                  Input = currentTemp;
                  myPID.Compute();
                  analogWrite(5,Output);
                  analogWrite(redLED,Output);

                Serial.print("output: ");
                Serial.println(Output);


  
            //adjustTemp(celcius, currentTemp);
                        showTemps(Setpoint, currentTemp); //currentTemp);  



           delay(1000);
        }
}







void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}






void showTemps(int target, float current){
    lcd.setCursor(0,1);
    lcd.print("       "); //overwrite everything on single line, works better than clearing the display for some reason
    lcd.setCursor(1,1);
    char charCurrent[6];
    //sprintf(charCurrent, "%05d", current);
    lcd.print(current);
    //lcd.setCursor(3,1);
    //lcd.print("/");
    lcd.setCursor(11,1);
    char charTarget[6];
    //sprintf(charTarget, "%03d", target);
    lcd.print(target);
}
