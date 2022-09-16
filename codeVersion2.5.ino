//4WD remot control car project
//buil by: tomer misk, omer attias and david hadad
//date: september 2020

#include <VirtualWire.h>

int ledLights = 10 ;
int motorL1 = 3 ;
int motorL2 = 4 ;
int motorLpwm = 5 ;
int buzzer = 6 ;
int motorR1 = 7 ;
int motorR2 = 8 ;
int motorRpwm = 9 ;
int RF_RX_PIN = 2 ;
void(* resetFunc) (void) = 0 ;  
int reversLights = 11 ; 
int x, y, key1, key2 ;
int ledState = LOW ;
int trigpin = 12 ;
int echopin = 13 ;

long timeBetweeinPule ;
float distance ;

void setup()
{
  Serial.begin(9600) ;
  pinMode(motorR1, OUTPUT) ;
  pinMode(motorR2, OUTPUT) ;
  pinMode(motorRpwm, OUTPUT) ;
  pinMode(motorL1, OUTPUT) ;
  pinMode(motorL2, OUTPUT) ;
  pinMode(motorLpwm, OUTPUT) ;
  pinMode(ledLights, OUTPUT) ;
  pinMode(buzzer, OUTPUT) ;
  pinMode(reversLights, OUTPUT) ;
  pinMode (trigpin, OUTPUT) ;
  pinMode (echopin, INPUT) ;
  
  // RC code part
  Serial.println("setup");
  vw_set_rx_pin(RF_RX_PIN);  // Setup receive pin.
  vw_setup(2000); // Transmission speed in bits per second.
  vw_rx_start(); // Start the PLL receiver.
}

uint8_t buffer[VW_MAX_MESSAGE_LEN]; // buffer array for data recieve over serial port 
uint8_t buflen = VW_MAX_MESSAGE_LEN;

void stady(){
  digitalWrite(motorR1 ,LOW) ;
  digitalWrite(motorR2 ,LOW) ;
  digitalWrite(motorL1 ,LOW) ;
  digitalWrite(motorL2 ,LOW) ;
  digitalWrite(reversLights ,LOW) ;
  Serial.println("stady");
}
void forward(){
  digitalWrite(motorR1 ,HIGH) ;
  digitalWrite(motorR2 ,LOW) ;
  analogWrite(motorRpwm ,255) ;
  digitalWrite(motorL1 ,HIGH) ;
  digitalWrite(motorL2 ,LOW) ;
  analogWrite(motorLpwm ,255) ;
  Serial.println("forward");
  if (distance < 10)
  {
    stady() ;
    Serial.println("STOP");
  }
}
void backward(){
  digitalWrite(motorR1 ,LOW) ;
  digitalWrite(motorR2 ,HIGH) ;
  analogWrite(motorRpwm ,255) ;
  digitalWrite(motorL1 ,LOW) ;
  digitalWrite(motorL2 ,HIGH) ;
  analogWrite(motorLpwm ,255) ;
  digitalWrite(reversLights ,HIGH) ;
  Serial.println("backward");
}
void right(){
  digitalWrite(motorR1,LOW) ;
  digitalWrite(motorR2, HIGH) ;
  analogWrite(motorRpwm, 255) ;
  digitalWrite(motorL1, HIGH) ;
  digitalWrite(motorL2, LOW) ;
  analogWrite(motorLpwm, 255) ;
  Serial.println("right");
}
void left(){
  digitalWrite(motorR1 ,HIGH) ;
  digitalWrite(motorR2 ,LOW) ;
  analogWrite(motorRpwm,255) ;
  digitalWrite(motorL1, LOW) ;
  digitalWrite(motorL2, HIGH) ;
  analogWrite(motorLpwm,255) ;
  Serial.println("left");
}
void lights() {
  ledState = !ledState ;  
  digitalWrite (ledLights, ledState) ;
}

void loop()
{
  

  String comdata = "";  
  if(vw_get_message(buffer, &buflen)) // non-blocking I/O
  {
    decode();
  }
}
void decode()
{
  if((buffer[0]!=0x7E)||(buffer[4]!=0xEF))
  {
  Serial.println("bad massage");
  resetFunc();
  digitalWrite(reversLights ,LOW) ;
  }
  uint8_t z;
  uint8_t button;
  uint8_t i = 0;
  x = buffer[1]<<2;
  y = buffer[2]<<2;
  z = buffer[3]>>3;
  button = buffer[3]&0x07;
  Serial.print("x = ");
  Serial.print(x ,DEC);
  Serial.print(","); 
  Serial.print("y = ");
  Serial.print(y ,DEC);
  Serial.print(","); 
  Serial.print("z = ");
  Serial.print(z ,DEC);
  Serial.print(","); 
  if(button)
  {
    Serial.print("KEY");
    Serial.println(button);
  }
  else
  { 
  Serial.println("NO KEY");
  }
  if ( x == 512)
  {
    if ( y == 496)
    {
      stady();  
    }
  }
      
  
  if (y > 900) 
  {
    
      forward();
      Serial.println("forward");
  }
  
 
  if (y < 100)
  {
  backward() ; 
  }
  
  if (x > 900)
  {
    right();
  }
  if (x < 100)
  {
  left() ; 
  }

    if (button == 1)
  {
      lights() ;
    }
    if (button == 2)
    {
      digitalWrite(buzzer, HIGH) ;
    }
  	else
  	{
      digitalWrite(buzzer, LOW) ;              
    } 
  digitalWrite (trigpin, HIGH);
  delayMicroseconds (10) ; 
  digitalWrite (trigpin, LOW) ; 
  timeBetweeinPule = pulseIn (echopin, HIGH) ;
  distance = timeBetweeinPule / 58 ;
  if (distance < 10)
  {
    stady() ;
    backward();
    delay(1000) ;
    Serial.println("STOP");
    stady() ;
  }

  clearBufferArray();
}
        


void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<VW_MAX_MESSAGE_LEN;i++)
    { buffer[i]=0;}                  // clear all index of array with command NULL
}
	
