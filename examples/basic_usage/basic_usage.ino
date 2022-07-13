#include <Arduino.h>
#include "CircularBuffer.h"

void setup()
{
  //Our buffer will hold 5 integers
  CircularBuffer<int> buffer(5);

  //Making an array of numbers
  int numbers[5] = { 1, 2, 3, 4, 5 };

  Serial.begin(115200);
  while(!Serial);

  //Inserting the numbers into the buffer
  for(int i = 0; i < 5; i++)
  {
    buffer.insert(numbers[i]);
  }

  Serial.print(F("Is our buffer empty? "));
  Serial.println(buffer.empty());
  Serial.println(F("Getting... "));

  //Printing out our contents one by one
  for(int i = 0; i < 5; i++)
  {
    Serial.println(buffer.get());
  }

  Serial.print(F("Buffer size is: ")); Serial.println(buffer.size());


}

void loop()
{
//Nothing here
}
