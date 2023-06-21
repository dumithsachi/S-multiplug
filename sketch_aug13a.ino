#include <ESP8266WiFi.h>

const char* ssid     = "Dialog 4G new";  
const char* password = "992422505";

WiFiServer server(80);

String header;

String RELAY1_State = "off";
String RELAY2_State = "off";
String RELAY3_State = "off";
String RELAY4_State = "off";

const int RELAY_1 = 16;    //D0
const int RELAY_2 = 5;     //D1
const int RELAY_3 = 4;     //D2
const int RELAY_4 = 0;     //D3
const int trigPin = 12;    //D6
const int echoPin = 14;    //D5
const int buzzer = 13;     //D7

#define SOUND_VELOCITY 0.34
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;


unsigned long currentTime = millis();

unsigned long previousTime = 0; 

const long timeoutTime = 2000;


void setup() {
  Serial.begin(115200);
  
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
  digitalWrite(RELAY_3, LOW);
  digitalWrite(RELAY_4, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){

   
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distanceCm = duration * SOUND_VELOCITY/2;
    
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);


  if (distanceCm <= 10){
   tone(buzzer,900);
   delay(10);
   noTone(buzzer);
   delay(10);    
 }
  
  WiFiClient client = server.available();   

  if (client) {                            
    Serial.println("New Client");          
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                   
          if (currentLine.length() == 0) {   
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
            if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("RELAY 1 on");
              RELAY1_State = "on";
              digitalWrite(RELAY_1, HIGH);
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("RELAY 1 off");
              RELAY1_State = "off";
              digitalWrite(RELAY_1, LOW);
            } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("RELAY 2 on");
              RELAY2_State = "on";
              digitalWrite(RELAY_2, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("RELAY 2 off");
              RELAY2_State = "off";
              digitalWrite(RELAY_2, LOW);
            } else if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("RELAY 3 on");
              RELAY3_State = "on";
              digitalWrite(RELAY_3, HIGH);
            } else if (header.indexOf("GET /3/off") >= 0) {
              Serial.println("RELAY 3 off");
              RELAY3_State = "off";
              digitalWrite(RELAY_3, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("RELAY 4 on");
              RELAY4_State = "on";
              digitalWrite(RELAY_4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("RELAY 4 off");
              RELAY4_State = "off";
              digitalWrite(RELAY_4, LOW);
            }
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".head { position: fixed; left: 0; top: 0; width: 100%; background-color: #34495e; font-size:35px; font-weight: bold; color: white; text-align: center; }");
            client.println(".footer { position: fixed; left: 0; bottom: 0; background-color: #34495e; width: 100%; color: white; text-align: center;}");
            client.println(".button { background-color: #c0392b; border: none; color: white; padding: 16px 40px; width: 150px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A; width: 150px;}</style></head>");
            
            // Web Page Heading
            client.println("<div class=\"head\"> <p>SMART MULTIPLUG</p> </div></br></br></br></br></br></br></br>");
          
            client.println("<h3>PLUG 1</h3>");       
            if (RELAY1_State=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button button2\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button\">ON </button></a></p>");
            } 
                 
            client.println("<h3> USB </h3>");     
            if (RELAY2_State=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button button2\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button \">ON </button></a></p>");
            }
            
            client.println("<h3>PLUG 2</h3>");       
            if (RELAY3_State=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button button2\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button\">ON </button></a></p>");
            } 
                
            client.println("<h3>ULTRASONIC SENSOR</h3>");     
            if (RELAY4_State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button button2\">OFF</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button \">ON </button></a></p>");
            }
            
            client.println("</body></html>");
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
   
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}
