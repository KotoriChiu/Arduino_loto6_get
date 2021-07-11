#include "WiFiEsp.h"
#include "TimedAction.h"

char ssid[] = "P883-cht-3";           
char pass[] = "as0955065969";   
char push[22];
int status = WL_IDLE_STATUS;
String text = "";
int LOTOtoINT[22]={0};
WiFiEspClient client;
char server[] = "lotophp.000webhostapp.com";
int red_led = 9,green_led = 10;
unsigned long lastConnectionTime = 0;         
const unsigned long postingInterval = 10000L; 

void setup(){
    pinMode(red_led,OUTPUT);
    pinMode(green_led,OUTPUT);
    digitalWrite(red_led,HIGH);
    digitalWrite(green_led,LOW);
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial2.begin(115200);
    WiFi.init(&Serial1);
    if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
    }
    while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
    }
    Serial.println("You're connected to the network");
    printWifiStatus();
}

void loop(){
    digitalWrite(red_led,LOW);
    digitalWrite(green_led,HIGH);
    if(!lastConnectionTime) httpRequest();
    char get_data;
    String text ="";
    String data_C="";
    boolean HE_ = false;
    int loto_vlaue = 0;
    while(client.available()){
        get_data = client.read();
        text = String(get_data);
        //Serial.print(text);
        if(text == "$" || HE_ == true){
            if(HE_ == false){
                HE_ = true;
                continue;
            }
            if(text == "|"){
                HE_ = false;
                break;
            }
            data_C+=text;
            LOTOtoINT[loto_vlaue] = text.toInt();
            Serial.print(text+" ");
            loto_vlaue++;
        }
    }
//     Serial.print(a);
     
     data_C.toCharArray(push,23); 
     Serial2.write(push);
     //Serial.print(push);
     loto_vlaue=0;
     if (millis() - lastConnectionTime > postingInterval){
      httpRequest();
      Serial.print("?");
     }
}

void printWifiStatus(){
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

void httpRequest(){
  Serial.println();
  client.stop();

  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    client.print(String("GET /") + " HTTP/1.1\r\n" + "Host: " + server + "\r\nConnection: close\r\n\r\n"); 
    lastConnectionTime = millis();
  }else Serial.println("Connection failed");
}