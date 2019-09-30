#include "WiFiEsp.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#endif
long test = 0,loto_value = 0;
char ssid[] = "P883-cht-3";
char pass[] = "as0955065969";
int status = WL_IDLE_STATUS,red_led = 9,green_led = 10,LEDPLUS = 11;
// int LOTOtoINT[22] = {0};
char LOTOchar[22];
char server[] = "lotto123.org";
unsigned long lastConnectionTime = 0;         
const unsigned long postingInterval = 10000L; 
WiFiEspClient clinet;

SoftwareSerial ESP8266(2,3);//RX TX
SoftwareSerial Unoto2560(4,5);//RX TX

void setup(){
    pinMode(red_led,OUTPUT);
    pinMode(green_led,OUTPUT);
    pinMode(LEDPLUS,OUTPUT);
    digitalWrite(red_led,HIGH);
    digitalWrite(green_led,LOW);
    digitalWrite(LEDPLUS,HIGH);
    Serial.begin(115200);
    ESP8266.begin(115200);
    Unoto2560.begin(9600);
    WiFi.init(&ESP8266);

    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);
    }

    while (status != WL_CONNECTED) {
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
    http_GET();
    Unoto2560.write(LOTOchar);
    //將截取到的http數據存到LOTOtoINT[] 再丟給2560
}

void http_GET() {
    if(!lastConnectionTime) httpRequest();
    while (client.available()) {
        char c = client.read();
        String text = String(c);
        test++;
        switch (test){
            case 4941:
            case 4942:
            case 4944:
            case 4945:
            case 4977:
            case 4978:
            case 4992:
            case 4993:
            case 5007:
            case 5008:
            case 5022:
            case 5023:
            case 5037:
            case 5038:
            case 5052:
            case 5053:
            case 5112:
            case 5113:
            case 5167:
            case 5168:
            case 5170:
            case 5171:  
                LOTOchar[loto_vlaue] = c;
                Serial.println(LOTOchar[loto_vlaue]);
                loto_vlaue++;
                break;
        }
    }
    test = 0; loto_vlaue = 0;
    if (millis() - lastConnectionTime > postingInterval) httpRequest();
}

void httpRequest() {
    Serial.println();
    client.stop();

    if (client.connect(server, 80)) {
        Serial.println("Connecting...");
        client.println(F("GET / HTTP/1.1"));
        client.println(F("Host: lotto123.org"));
        client.println("Connection: close");
        client.println();
        lastConnectionTime = millis();
    }else Serial.println("Connection failed");
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