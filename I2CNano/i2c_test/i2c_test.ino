#include <Wire.h> 

void setup() 
{ 
  Wire.begin(2);                // join i2c bus with address #2 
  Wire.onRequest(requestEvent); // register event 
  Serial.begin(9600);           // start serial for output 
} 

void loop()
{ 
  delay(50); 
} 

// function that executes whenever data is received from master 
// this function is registered as an event, see setup()
void requestEvent() 
{
  static char c[] = {'Q', 'B', 'C', 'D', '\n'};
  Wire.write(c);
}