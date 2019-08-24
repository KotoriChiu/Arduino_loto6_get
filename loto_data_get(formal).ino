#include "WiFiEsp.h"
#include "TimedAction.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#endif
long test = 0,loto_vlaue = 0;
char ssid[] = "P883-cht-3";           
char pass[] = "as0955065969";       
int status = WL_IDLE_STATUS;
int LOTOtoINT[22]={0};
int LOTO_Comparison[22]={0};  
char c;
String text = "";
char server[] = "lotto123.org";

unsigned long lastConnectionTime = 0;         
const unsigned long postingInterval = 10000L; 

WiFiEspClient client;
void loto_DIGIT();
void loto_GET();
TimedAction lotodigit = TimedAction(1,loto_DIGIT);

int seg[7] = {2,3,4,5,6,7,8};
int red_led = 9,green_led = 10,LEDPLUS = 11;
int digit[22] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,44};
int seven_seg_digits[10][7] ={
  {1,1,1,1,1,1,0}, //0
  {0,1,1,0,0,0,0}, //1
  {1,1,0,1,1,0,1}, //2
  {1,1,1,1,0,0,1}, //3
  {0,1,1,0,0,1,1}, //4
  {1,0,1,1,0,1,1}, //5
  {1,0,1,1,1,1,1}, //6
  {1,1,1,0,0,0,0}, //7
  {1,1,1,1,1,1,1}, //8
  {1,1,1,1,0,1,1}  //9
  };

void setup(){
  pinMode(red_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(LEDPLUS,OUTPUT);
  digitalWrite(red_led,HIGH);
  digitalWrite(green_led,LOW);
  digitalWrite(LEDPLUS,HIGH);
  Serial.begin(115200);
  Serial1.begin(115200);
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
  for(int i = 0;i<7;i++)pinMode(seg[i],OUTPUT);
  for(int i = 0;i<22;i++)pinMode(digit[i],OUTPUT);
  Serial.println("You're connected to the network");
  printWifiStatus();
}

void loop(){
  digitalWrite(red_led,LOW);
  digitalWrite(green_led,HIGH);
  lotodigit.check();
  if(!lastConnectionTime) httpRequest();
  while (client.available()) {
    c = client.read();
    text = String(c);
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
            LOTOtoINT[loto_vlaue] = text.toInt();
            Serial.println(LOTOtoINT[loto_vlaue]);
            loto_vlaue++;
            break;
    }
  }
  test=0; loto_vlaue=0;
  if (millis() - lastConnectionTime > postingInterval) httpRequest();
}

void loto_DIGIT(){
  if(LOTOtoINT[0]!=0||LOTOtoINT[1]!=0||LOTOtoINT[2]!=0||LOTOtoINT[3]!=0 && LOTO_Comparison!=LOTOtoINT){ 
    for(int i = 0;i<22;i++){
      LOTO_Comparison[i] = LOTOtoINT[i];
      lightDigit(LOTOtoINT[i],i);
      delay(1);
    }
  }else{
    for(int i = 0;i<22;i++){
      lightDigit(LOTO_Comparison[i],i);
      delay(1);
    }
  } 
}

void httpRequest(){
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

void lightDigit(int num,int position){
    pickDigit(position);
    lightSegments(num);
}

void pickDigit(int value){
    for(int i = 0;i<22;i++)digitalWrite(digit[i],HIGH);
    for(int i = 0;i<22;i++){
        if(value == i){
            digitalWrite(digit[i],LOW);
            break;
        }
    }
}

void lightSegments(int num){
  for(int i = 0;i<7;i++)digitalWrite(seg[i],seven_seg_digits[num][i]);
}
