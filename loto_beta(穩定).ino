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
int copy_loto[22]={0};  
char c;
String text = "";
char server[] = "lotto123.org";

unsigned long lastConnectionTime = 0;         
const unsigned long postingInterval = 60000L; 

WiFiEspClient client;
void loto_DIGIT();
void loto_GET();
//TimedAction lotoget = TimedAction(8000,loto_GET);
TimedAction lotodigit = TimedAction(1,loto_DIGIT);

int seg[7] = {2,3,4,5,6,7,8};
//int segs[7] = {24,25,26,27,28,29,30};
int digit[6] = {22,23,24,25,26,27};
int seven_seg_digit[10][7] ={
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
  //for(int i = 0;i<7;i++)pinMode(segs[i],OUTPUT);
  for(int i = 0;i<6;i++)pinMode(digit[i],OUTPUT);

  Serial.println("You're connected to the network");
  printWifiStatus();
}

void loop()
{
  lotodigit.check();
  if(lastConnectionTime==0)httpRequest();
 while (client.available()) {
    c = client.read();
    text = String(c);
    //Serial.print(text);
    //Serial.print(text);
    test++;
  
    switch (test)
    {
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
           // copy_loto[loto_value] = LOTOtoINT[loto_vlaue];
            loto_vlaue++;
            break;
    }
  }
  test=0;
  loto_vlaue=0;
  if (millis() - lastConnectionTime > postingInterval) httpRequest();
  
//  lotoget.check();
}

void loto_DIGIT(){
  if(LOTOtoINT[0]!=0||LOTOtoINT[1]!=0||LOTOtoINT[2]!=0||LOTOtoINT[3]!=0){
    for(int i = 0;i<22;i++)copy_loto[i] = LOTOtoINT[i];
    for(int i = 0;i<6;i++){
      lightDigit(LOTOtoINT[i],i);
      delay(1);
    }  
  }else if(copy_loto[0]+copy_loto[1]!=0){
    for(int i = 0;i<6;i++){
      lightDigit(copy_loto[i],i);
      delay(1);
    }
  }else{
    for(int i = 0;i<6;i++){
      lightDigit(0,i);
      delay(1);
    //Serial.print(0+" ");
    }
    //Serial.println();
  } 
}

//void loto_GET(){
    
//}

void httpRequest()
{
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


void printWifiStatus()
{
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
    for(int i = 0;i<4;i++)digitalWrite(digit[i],HIGH);
    digitalWrite(26,HIGH);
    digitalWrite(27,HIGH);
    if(value!=4&&value!=5)digitalWrite(digit[value],LOW);
    else digitalWrite(digit[value],LOW);
}

void lightSegments(int num){
  for(int i = 0;i<7;i++){
      digitalWrite(seg[i],seven_seg_digits[num][i]);
      //digitalWrite(segs[i],seven_seg_digit[num][i]);
    }
}
