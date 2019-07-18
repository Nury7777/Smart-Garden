#include <ThingSpeak.h>

#include <SPI.h>

#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>


  int status = WL_IDLE_STATUS;     // the WiFi radio's status
  int temperatureC=0;
  int temperatureF=0;
  float analog;
  int lightpercent=0;
  int moisturepercent=0;
  float sumMoisture=0;
  float avgMoisture;
  char SECRET_SSID[]= "BSE_V4";
  char SECRET_PASS[]= "Bserock$";
  WiFiClient client2;
  IPAddress ip;
  WiFiServer server(80); //declare server object and spedify port, 80 is port used for internet
  unsigned long myChannelNumber= 820344;
  const char * myWriteAPIKey= "TGGMED51RDJ5NS3H";
void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600);
  ThingSpeak.begin(client2);
       if (WiFi.status() == WL_NO_SHIELD) { //if not able to connect to shield
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  while ( status != WL_CONNECTED) { //while not connected to wifi
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(SECRET_SSID, SECRET_PASS);
    Serial.println(status);
    // wait 10 seconds for connection:
    delay(1000);
  }
  /* 
WL_NO_SHIELD = 255,
WL_IDLE_STATUS = 0,
WL_NO_SSID_AVAIL = 1
WL_SCAN_COMPLETED = 2
WL_CONNECTED = 3
WL_CONNECT_FAILED = 4
WL_CONNECTION_LOST = 5
WL_DISCONNECTED = 6
   */
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network- ");
  //print ssid
  Serial.print("SSID: ");
  Serial.println(SECRET_SSID);
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  server.begin();
}


void loop() {

 WiFiClient client = server.available();   // listen for incoming clients
//wificlient creates a client that can connect to a specified internet ip address*  WiFiClient client= server.available();//server.available returns name of server if connected-false if not
    analog= analogRead(A0);
  temperatureC= ((analog*3.3/1023.0)-.5)*100;
  temperatureF = (temperatureC * 9.0/5.0) + 32.0;
  //sum of 100 moisture data
  for (int i=0; i<100; i++){
//    Serial.print(analogRead(A2));
//    Serial.print(", ");
    sumMoisture= sumMoisture + analogRead(A2);
    delay(1);
  }
  //find avg moisture
  avgMoisture= sumMoisture/100.0;
 // Serial.print("Avg moisture is ");
 // Serial.println(avgMoisture);
  //light percent
  lightpercent= analogRead(A1)*100.0/1024.0; //most that analog read will be is 1024, so find percent light
  //moisture percent
  moisturepercent= 100.0-(avgMoisture*100.0/1024.0); 
  Serial.print("Temperature is ");
  Serial.print(temperatureF);
  Serial.println(" Fahrenheit");
  Serial.print("Moisture is ");
  Serial.print(moisturepercent);
  Serial.println(" %");
  Serial.print("Light is ");
  Serial.print(lightpercent);
  Serial.println(" %");
  sumMoisture=0;
  
  ThingSpeak.setField(1, temperatureF);
  ThingSpeak.setField(2, moisturepercent);
  ThingSpeak.setField(3, lightpercent);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);   //write to channel
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
/*    Value  Meaning
200 OK / Success
404 Incorrect API key (or invalid ThingSpeak server address)
-101  Value is out of range or string is too long (> 255 characters)
-201  Invalid field number specified
-210  setField() was not called before writeFields()
-301  Failed to connect to ThingSpeak
-302  Unexpected failure during write to ThingSpeak
-303  Unable to parse response
-304  Timeout waiting for server to respond
-401  Point was not inserted (most probable cause is the rate limit of once every 15 seconds)
0 Other error
*/
  }
  if (client){        //if there is a client-- the client will be the andriod phone with the app
    Serial.println("new client");
   // String currentLine= "";
    boolean currentLineIsBlank=true;
    while (client.connected()){  //true if connected
    //  Serial.println(client.available());
      Serial.print("IP Address: ");
      Serial.println(ip);
  //temperature
  analog= analogRead(A0);
  temperatureC= ((analog*3.3/1023.0)-.5)*100;
  temperatureF = (temperatureC * 9.0/5.0) + 32.0;
  //sum of 100 moisture data
  for (int i=0; i<100; i++){
//    Serial.print(analogRead(A2));
//    Serial.print(", ");
    sumMoisture= sumMoisture + analogRead(A2);
    delay(1);
  }
  //find avg moisture
  avgMoisture= sumMoisture/100.0;
 // Serial.print("Avg moisture is ");
 // Serial.println(avgMoisture);
  //light percent
  lightpercent= analogRead(A1)*100.0/1024.0; //most that analog read will be is 1024, so find percent light
  //moisture percent
  moisturepercent= 100.0-(avgMoisture*100.0/1024.0); 
  Serial.print("Temperature is ");
  Serial.print(temperatureF);
  Serial.println(" Fahrenheit");
  Serial.print("Moisture is ");
  Serial.print(moisturepercent);
  Serial.println(" %");
  Serial.print("Light is ");
  Serial.print(lightpercent);
  Serial.println(" %");
  sumMoisture=0;
  
  ThingSpeak.setField(1, temperatureF);
  ThingSpeak.setField(2, moisturepercent);
  ThingSpeak.setField(3, lightpercent);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);   //write to channel
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
/*    Value  Meaning
200 OK / Success
404 Incorrect API key (or invalid ThingSpeak server address)
-101  Value is out of range or string is too long (> 255 characters)
-201  Invalid field number specified
-210  setField() was not called before writeFields()
-301  Failed to connect to ThingSpeak
-302  Unexpected failure during write to ThingSpeak
-303  Unable to parse response
-304  Timeout waiting for server to respond
-401  Point was not inserted (most probable cause is the rate limit of once every 15 seconds)
0 Other error
*/
  }
     Serial.print("client.available() returns ");
     Serial.println(client.available());
  /*   if (client.available()){ //returns number of bytes available to read
    //if statement is true if there are any bytes to read from client
       char c = client.read(); //reads a byte and sets it to char c
       Serial.write(c);
        if (c=='\n' && currentLineIsBlank){   //if the byte is a newline character
    */      //if current line is blank, there are two newline characters in a row
          //that is the end of the client HTTP request, so send a response
   //       if (currentLine.length()==0){
          Serial.println("hello");
            Serial.println("");
            //HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            //and a content-type so the client knows what is coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/html");
            client.println("Connection: close"); //the connection will be closed after completion of the response
            client.println("Refresh: 5"); //refresh the page automatically every 5 sec
            client.println();
  //          client.println("<DOCTYPE HTML>");
  //          client.println("<html>");
            client.print(temperatureF);
            client.print(" ");
            client.print(moisturepercent);
            client.print(" ");
            client.print(lightpercent);
    //        client.println("<br>");
            //the HTTP response ends with another blank line
    //        client.println("<html>");
            break;  //break out of the while loop
  //        }
  //        else { //if you got a newline, then clear currentLine
  //          currentLine="";
  //       }
   /*       if (c=='\n') {
            //you're starting a new line
            currentLineIsBlank=true;
        }
        else if (c != '\r'){  //if anything other than a carriage return character
          currentLineIsBlank = false;  //add it to the end of currentLine
        }
        
      }
    */
    //Serial.println("client disconnected");
    }
    delay(1);
      //close the connection
    client.stop();
    Serial.println("client disconnected");
    Serial.println();
  }

}
