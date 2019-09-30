int LOTOtoINT[22] = {0};
int LOTO_Comparison[22] = {0};  

int seg[7] = {2,3,4,5,6,7,8};
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
  for(int i = 0;i < 7;i++)pinMode(seg[i],OUTPUT);
  for(int i = 0;i < 22;i++)pinMode(digit[i],OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //接收uno的http數據 並把數據傳給loto_DIGIT函式
  char LOTOchar[] = Serial.read();
  for(int i = 0;i < 22;i++)LOTOtoINT[i] = LOTOchat[i].toInt();
  loto_DIGIT(LOTOtoINT);
}



void loto_DIGIT(int LOTO[]){
  if(LOTO[4] !=0 || LOTO[5] != 0 || LOTO[6] != 0 || LOTO[7] != 0 && LOTO_Comparison != LOTO){ 
    for(int i = 0;i < 22;i++){
      LOTO_Comparison[i] = LOTO[i];
      lightDigit(LOTO[i],i);
      delay(1);
    }
  }else{
    for(int i = 0;i < 22;i++){
      lightDigit(LOTO_Comparison[i],i);
      delay(1);
    }
  } 
}

void lightDigit(int num,int position){
    pickDigit(position);
    lightSegments(num);
}

void pickDigit(int value){
    for(int i = 0;i < 22;i++)digitalWrite(digit[i],HIGH);
    for(int i = 0;i < 22;i++){
        if(value == i){
            digitalWrite(digit[i],LOW);
            break;
        }
    }
}

void lightSegments(int num){
  for(int i = 0;i < 7;i++)digitalWrite(seg[i],seven_seg_digits[num][i]);
}
