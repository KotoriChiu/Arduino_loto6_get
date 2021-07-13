#include "WiFiEsp.h"
#include "TimedAction.h"

//***WIFI SET variable***
char ssid[] = "P883-cht-3";           
char pass[] = "as0955065969";   
int status = WL_IDLE_STATUS;
WiFiEspClient client;
char server[] = "lotophp.000webhostapp.com";
unsigned long lastConnectionTime = 0;         
const unsigned long postingInterval = 10000L; 

//***Arduino pin and set***
int red_led = 9;
int green_led = 10;

void setup(){
    pinMode(red_led,OUTPUT);
    pinMode(green_led,OUTPUT);
    WIFI_status_(false);

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
    WIFI_status_(true);
    printWifiStatus();
}

void loop(){
    if(!lastConnectionTime) 
        httpRequest();

    String loto_data_str = Get_loto_number();
    char loto_data_push[22];
    loto_data_str.toCharArray(loto_data_push,23);
    loto_post(loto_data_push);

    if (millis() - lastConnectionTime > postingInterval) 
        httpRequest();

}

void loto_post(char loto_str[]){
    Serial2.write(loto_str);
}

void printWifiStatus(){
    IPAddress ip = WiFi.localIP();
    long rssi = WiFi.RSSI();

    Serial.print("SSID: "+WiFi.SSID()+"\n");
    Serial.print("IP Address: "+ip+"\n");
    Serial.print("Signal strength (RSSI):"+rssi+"dBm\n");
}

void httpRequest(){
  Serial.println();
  client.stop();

  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    client.print(String("GET /") + " HTTP/1.1\r\n" + "Host: " + server + "\r\nConnection: close\r\n\r\n"); 
    lastConnectionTime = millis();
  }else 
    Serial.println("Connection failed");
}

void WIFI_status_(boolean indicator_led){
    if(indicator_led){
        digitalWrite(red_led,LOW);
        digitalWrite(green_led,HIGH);
    }else{
        digitalWrite(red_led,HIGH);
        digitalWrite(green_led,LOW);
    } 
}

String Get_loto_number(){
    char get_data;
    String loto_data_str="";
    boolean HE_ = false;

    while(client.available()){
        get_data = client.read();
        String text = String(get_data);
        if(text == "$" || HE_ == true){
            if(HE_){
                HE_ = true;
                continue;
            }
            if(text == "|"){
                HE_ = false;
                break;
            }
            loto_data_str += text;
        }
    }
    return loto_data_str;
}