int seg[7] = {2,3,4,5,6,7,8};
int digit[4] = {22,23,24,25};
void setup() {
  // put your setup code here, to run once:
    for(int i = 0;i<7;i++)pinMode(seg[i]aaOUTPUT);
    for(int i = 0;i<4;i++)pinMode(digit[i],OUTPUT);
}
void loop(){
    lightDigit(22,0);
    lightDigit(23,1);
    lightDigit(24,2);
    lightDigit(25,3); 
}
void lightDigit(int num,int position){
    pickDigit(position);
    lightSegments(num);
}

void pickDigit(int value){
    for(int i = 0;i<22;i++)digitalWrite(digit[i],HIGH);
    for(int i = 0;i<22;i++){
        if(value == i+1){
            digitalWrite(digit[i],LOW);
            break;
        }
    }
}

void lightSegments(int num){
  for(int i = 0;i<7;i++)digitalWrite(seg[i],seven_seg_digits[num][i]);
}