#include <RGBTools.h>

RGBTools rgb(8,9,10);

void setup() {
  // put your setup code here, to run once:

  pinMode(22, INPUT);
  pinMode(24, INPUT);
  
  
  Serial.begin(19200);
  Serial.println("Communication started");
  
  rgb.setColor(255,255,255);
  

}

boolean previousstate_button1 = false;
boolean previousstate_button2 = false;

void loop() {
  // put your main code here, to run repeatedly:

  boolean buttonstate1 = digitalRead(22);
  boolean buttonstate2 = digitalRead(24);
  
  delay(100);

  if(previousstate_button1 != buttonstate1)
  {
      if(!buttonstate1)
      {
        Serial.println("State changed button one");
        rgb.setColor(255,255,255);
        //analogWrite(8, 254);
      }
      
      previousstate_button1 = buttonstate1;
  }
    
  
  if(previousstate_button2 != buttonstate2)
  {
      if(!buttonstate2)
      {
        Serial.println("State changed button two");
        rgb.setColor(0,0,0);
        //analogWrite(8, 0);
      }
      
      previousstate_button2 = buttonstate2;
  }
  Serial.print("Button 1:");
  Serial.print(buttonstate1);
  Serial.print(", ");
  Serial.print("Button 2:");
  Serial.println(buttonstate2);
}
