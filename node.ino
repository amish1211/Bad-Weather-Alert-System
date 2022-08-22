#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
/* D6- rx and D5 tx. These pins are used for communication between
node mcu and Arduino*/
SoftwareSerial uno(D6, D5);
int light_int; // stores the output voltage from the LDR sensor
/*ssid and password of the network to which node mcu will connect
for internet*/
const char *ssid = "<Your ssid>";
const char *password = "<Your password>";
/* If it's first time you will receive alert call then ring_number value
will be 1. After sending first alert call it's value becomes 2. It is used
to make sure that not more than two alert calls are sent, that is,
when the ring_number becomes 3 request won't be sent to IFTTT
and hence the applet won't be triggered and you won't receive call
after the second call.
*/
int ring_number = 1;
/* samay gives the functionality of delay to be set dynamically at the
end of loop function.
* If the first call is triggered
* then there is more delay as compared to when there is no call
triggered. If it was not
* so then the two calls would be triggered
* with very less time gap between them. If this happens then as
soon as the user picks
* the first call he would receive the second call.
* In this way user has to reject the second call to hear the message
of the first call.
* And effectively we would end with only one call.
* So to prevent this samay variable is used.
*/
int samay = 500;
const char *host = "maker.ifttt.com";
void setup()
{
  /* initilizes node mcu for serial communication at 48000 baud rate.
  Arduino also need to have the same baud rate.*/
  uno.begin(4800);
  Serial.begin(9600); // Initilizes the serial monitor for serial communication from node mcu
      // Below two lines are for indicating to which network we are
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // attempts to connect to wifi with
  internet
      // Below while loop waits till the node mcu is not connected to
      internet while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(". ");
    delay(1000);
  }
  Serial.println();
  Serial.println(WiFi.localIP()); // prints the ip address of node mcu
}

void loop()
{
  uno.write("s"); // triggers arduino to read and sent the data at A0
  /* below 4 lines checks whether there is response from the arduino,
  * if yes then node mcu reads and stores the response in ligth_int
  variable
  * further it also prints un the serial monitor for deveoloper's
  reference
  */
  if (uno.available() > 0)
  {
    light_int = uno.read();
    Serial.println(light_int);
  }
  WiFiClient client;                                        // Our node mcu is acting as client
  const int httpPort = 80;                                  // this port is used for communication
  through web browsers if (!client.connect(host, httpPort)) // attempts to connect to
      server till the connection is successful
  {
    Serial.println("connection failed");
    return;
  }
  // request is sent to the server which triggers the applet on IFTTT
  to make the user an alert call if (light_int < 200 && ring_number < 3)
  {
    ring_number = ring_number + 1;
    String url =
        "/trigger/danger/with/key/lvFa63YcGh0We3HMqhdbCxovzql2lATqC
        4KrsURrN2q ";
        Serial.print("Requesting URL: ");
    Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " +
                 host + "\r\n" + "Connection: close\r\n\r\n");
    samay = 20000;
  }
  delay(samay);
}
