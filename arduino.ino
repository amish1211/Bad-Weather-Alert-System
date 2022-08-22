#include <SoftwareSerial.h>
SoftwareSerial uno(5, 6); // 5-receiver 6-transmitter
void setup()
{
  uno.begin(4800); // begins serial communication
}
void loop()
{
  int data = analogRead(A0);
  /*If the arduino receives 's' then it sends the value read at analog pin A0 to the node mcu*/
  if (uno.available() > 0)
  {
    char c = uno.read();
    if (c == 's')
    {
      uno.write(data);
    }
  }
}
