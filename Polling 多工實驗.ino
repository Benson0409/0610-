#define NOTE_C3  131
#define NOTE_SC3 139
#define NOTE_D3  147
#define NOTE_SD3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_SG3 208
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_SC4 277

#define BUTTON 2

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

byte colDataMatrix[8] = {
  B11111110,
  B11111101,
  B11111011,
  B11110111,
  B11101111,
  B11011111,
  B10111111,
  B01111111
}; //掃描


byte rowDataMatrix[8] = {
   B00111100,
   B01000010,
   B10000001,
   B10000001,
   B10000001,
   B10000001,
   B01000010,
   B00111100
}; //圖案

byte rowDataMatrixH[8] = {
   B00000000,
   B00100100,
   B00100100,
   B00111100,
   B00111100,
   B00100100,
   B00100100,
   B00000000
}; //圖案

byte rowDataMatrixA[8] = {
   B00000000,
   B00011000,
   B00100100,
   B00100100,
   B00111100,
   B00100100,
   B00100100,
   B00000000
}; //圖案

int melody[] = {
  NOTE_D3, NOTE_A3, NOTE_D3, NOTE_E3, NOTE_D3, 0, NOTE_D3, NOTE_F3,NOTE_E3
};

int noteDurations[] = {
  8, 8, 8, 8,8,4,4,4,8
};

int thisNote =-1,noteDuration=0;
long previousTime=0,
	presentTime=0,
	pauseBetweenNotes=0;

byte previousState=1, presentState=1, patternNumber=0;

void setup() {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  
  pinMode(2, INPUT_PULLUP);
}

void checkToPlay() {
  presentTime=millis();
  if(presentTime-previousTime>=pauseBetweenNotes){
    thisNote+=1;
    if(thisNote>=8){
      thisNote=-1;
      pauseBetweenNotes=100;
      previousTime=millis();
    }
    else{
      noteDuration=500 / noteDurations[thisNote];
      tone(9,melody[thisNote], noteDuration);
      pauseBetweenNotes=noteDuration*1.2;
      previousTime=millis();
    }
  }
}
void loop(){
   presentState=digitalRead(BUTTON);
  for(int i=0;i<8;i++){
    if( presentState==0 && previousState==1){
    patternNumber++;
    if(patternNumber>2)  patternNumber=0;
  }
    if(patternNumber==0){
  	digitalWrite(latchPin,LOW);
    shiftOut(dataPin,clockPin,MSBFIRST,colDataMatrix[i]);
    shiftOut(dataPin,clockPin,MSBFIRST,rowDataMatrix[i]);
    digitalWrite(latchPin,HIGH);
    checkToPlay();
    delay(1);
  }
  else if(patternNumber==1) {
    digitalWrite(latchPin,LOW);
    shiftOut(dataPin,clockPin,MSBFIRST,colDataMatrix[i]);
    shiftOut(dataPin,clockPin,MSBFIRST,rowDataMatrixH[i]);
    digitalWrite(latchPin,HIGH);
    checkToPlay();
    delay(1);
  }
    else{
     digitalWrite(latchPin,LOW);
    shiftOut(dataPin,clockPin,MSBFIRST,colDataMatrix[i]);
    shiftOut(dataPin,clockPin,MSBFIRST,rowDataMatrixA[i]);
    digitalWrite(latchPin,HIGH);
    checkToPlay();
    delay(1);
  }
   delay(2);
   previousState=presentState;
  }
}