
#include <ESP8266WiFi.h>
 
const char* ssid = "node2";
const char* password = "12345678";

uint8_t Pwm1 = D1; //Nodemcu PWM pin 
uint8_t Pwm2 = D2; //Nodemcu PWM pin

//Seven segment pins attachecd with nodemcu pins  
int a0 = 15;  //Gpio-15 of nodemcu esp8266  
int a1 = 13;  //Gpio-13 of nodemcu esp8266    
int a2 = 12;  //Gpio-12 of nodemcu esp8266   
int a3 = 14;  //Gpio-14 of nodemcu esp8266   


WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
//Declaring l293d control pins as Output
pinMode(a0, OUTPUT);     
pinMode(a1, OUTPUT);     
pinMode(a2, OUTPUT);
pinMode(a3, OUTPUT);    
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 int Ms=0,dir=0,Pw=0;
 
 // Match the request
 if (request.indexOf("/start=1") != -1)  {  
  digitalWrite(a0, HIGH); //Start first motor
  digitalWrite(a1, LOW);

  digitalWrite(a2, HIGH); //Start second motor
  digitalWrite(a3, LOW);
  Ms=0;
  dir=0;
}

if (request.indexOf("/stop=1") != -1)  {  
  digitalWrite(a0, LOW); //Stop first motor
  digitalWrite(a1, LOW);

  digitalWrite(a2, LOW); //Stop second motor
  digitalWrite(a3, LOW);
  Ms=0;
}

if (request.indexOf("/tog=1") != -1)  {
  digitalWrite(a0, LOW);  //Change First motor rotation direction

  digitalWrite(a1, HIGH);
  
  digitalWrite(a2, LOW); //Change Second motor rotation direction

  digitalWrite(a3, HIGH);
  dir=0;
}



  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>Nodemcu Dc motor control over WiFi</h1><br><br>");
  client.println("<br><br>");
  client.println("<a href=\"/start=1\"\"><button>Start Motor </button></a><br/>");
  client.println("<a href=\"/stop=1\"\"><button>Stop Motor </button></a><br/>");
  client.println("<a href=\"/tog=1\"\"><button>Toggle Direction</button></a><br/>");


  if(Ms==1){
    client.println("Motor Powered Working<br/>" );
    }
    else
    client.println("Motor at Halt<br/>" );

  if(dir==1){
    client.println("Motor rotating in forward direction<br/>" );
    }
    else
    client.println("Motor rotating in backward direction<br/>" );


  
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
