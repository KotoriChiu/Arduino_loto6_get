#include "SoftwareSerial.h"
int LOTOtoINT[22];
int LOTO_Comparison[22]={0};
int seg[7] = {2,3,4,5,6,7,8};
char int_;
//bool ctrl = false;
int max_ = 0;
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
int dash[7] = {0,0,0,1,0,0,0};
void setup(){

      Serial2.begin(115200);
      Serial.begin(9600);

      for(int i = 0;i<7;i++)
            pinMode(seg[i],OUTPUT);
      for(int i = 0;i<22;i++){
          pinMode(digit[i],OUTPUT);
          for(int i = 0;i<22;i++)digitalWrite(digit[i],LOW);
          for(int j = 0;j<7;j++)digitalWrite(seg[j],dash[j]);
      }
}

void loop(){
    if(tx_rx_get()||loto_add_count(LOTO_Comparison)>0)
        loto_DIGIT(LOTO_Comparison);  
}

boolean tx_rx_get(){
    while(Serial2.available()){
        int_ = Serial2.read();
        LOTOtoINT[max_] = int_ - '0';
        LOTO_Comparison[max_]=LOTOtoINT[max_];
        Serial.print(LOTOtoINT[max_]);
        max_++;
        
        if(max_>21){
            max_=0;
            //loto_DIGIT();
            return true;
        }
    }
    return false;
}

int loto_add_count(int LOTO[]){
    int count=0;
    fot(int i=0;i<22;i++)
        count+=LOTO[i];
    return count;
}

void loto_DIGIT(int LOTO_data[]){
    for(int i = 0;i<22;i++){
      lightDigit(LOTO_data[i],i);
      delay(1);
    }
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

void lightDigit(int num,int position_){
    pickDigit(position_);
    lightSegments(num);
}

void lightSegments(int num){
  for(int i = 0;i<7;i++)digitalWrite(seg[i],seven_seg_digits[num][i]);
}