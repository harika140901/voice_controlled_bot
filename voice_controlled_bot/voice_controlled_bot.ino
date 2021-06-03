#include<ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "6456";
const char* password = "harika14";
String  command ="";
uint8_t TRIG=D1;
uint8_t ECHO=D0;
uint8_t leftMotor1=D5;
uint8_t leftMotor2=D6;
uint8_t rightMotor1=D7;
uint8_t rightMotor2=D8;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
  server.begin();
  pinMode(leftMotor1, OUTPUT); 
  pinMode(leftMotor2, OUTPUT);  
  pinMode(rightMotor1, OUTPUT);  
  pinMode(rightMotor2, OUTPUT);  

  digitalWrite(leftMotor1,LOW);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,LOW);
  digitalWrite(rightMotor2,LOW);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  
}
int distance;
float duration;
void loop() {
  WiFiClient client=server.available();
  if (!client) return; 
  command = checkClient ();
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  duration=pulseIn(ECHO, HIGH);
  distance=duration*0.034/2;
  Serial.println(distance);
  if (command == "forward")
{
  forwardMotor();
}
 else if (command == "left")
{
  leftMotor();
}
 else if (command == "right")
{
  rightMotor();
}
 else if (command == "reverse")
{
  reverseMotor();
}
 if (command == "stop" )
{
  stopMotor();
}
    sendBackEcho(command); // send command echo back to android device
    command = "";
}

/* command motor forward */
void forwardMotor(void)   
{   
  digitalWrite(leftMotor1,HIGH);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,HIGH);
  digitalWrite(rightMotor2,LOW);
}

/* command motor backward */
void reverseMotor(void)   
{
  digitalWrite(leftMotor1,LOW);
  digitalWrite(leftMotor2,HIGH);
  digitalWrite(rightMotor1,LOW);
  digitalWrite(rightMotor2,HIGH);
}

/* command motor turn left */
void leftMotor(void)   
{
  digitalWrite(leftMotor1,LOW);
  digitalWrite(leftMotor2,HIGH);
  digitalWrite(rightMotor1,HIGH);
  digitalWrite(rightMotor2,LOW);
}

/* command motor turn right */
void rightMotor(void)   
{
  digitalWrite(leftMotor1,HIGH);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,LOW);
  digitalWrite(rightMotor2,HIGH);
}

/* command motor stop */
void stopMotor(void)   
{
  digitalWrite(leftMotor1,LOW);
  digitalWrite(leftMotor2,LOW);
  digitalWrite(rightMotor1,LOW);
  digitalWrite(rightMotor2,LOW);
}
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}

/* send command echo back to android device */
void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop();
  delay(1);
}
