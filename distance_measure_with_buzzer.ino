#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


const byte buzzer_pin = 7;
const byte trig_pin = 9;
const byte echo_pin = 10;
const int duration = 150;
long last_view_time;
long last_buzzer_off_time;
long last_buzzer_on_time;
long last_measurement_time;
bool is_buzzer_on;
float distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0); 
  lcd.print("SCANNER ON");
  delay(1000);
  lcd.clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - last_measurement_time >= 100){
    distance = measure_distance();
    last_measurement_time = millis();
  }
  
  if (millis() - last_view_time >= 250){
    view(distance);
    last_view_time = millis();
  }

  buzzer(distance);
}

float measure_distance(){
  digitalWrite(trig_pin, 0);
  delayMicroseconds(5);

  digitalWrite(trig_pin, 1);
  delayMicroseconds(10);
  digitalWrite(trig_pin, 0);

  long time = pulseIn(echo_pin, HIGH);

  float distance = (time * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  return distance;
}

void view(float distance){
  lcd.setCursor(4, 0);
  lcd.print("Distance: ");
  lcd.setCursor(4, 1);
  lcd.print(distance);
  lcd.print("cm    ");
}


void buzzer(float distance){
  int del;
  
  if(distance > 100){
    digitalWrite(buzzer_pin, 0);
    last_buzzer_off_time = millis();
    is_buzzer_on = false;
    return;
  } else if(distance <= 10){
    del = 100;
  } else if(distance <= 25){
    del = 250;
  } else if(distance <= 50){
    del = 400;
  } else if(distance <= 75){
    del = 600;
  } else {
    del = 1000;
  }

  if(!is_buzzer_on && millis() - last_buzzer_off_time >= del){
    digitalWrite(buzzer_pin, 1);
    last_buzzer_on_time = millis();
    is_buzzer_on = true;
  }
  if(is_buzzer_on && millis() - last_buzzer_on_time >= duration){
    digitalWrite(buzzer_pin, 0);
    last_buzzer_off_time = millis();
    is_buzzer_on = false;
  }
}
