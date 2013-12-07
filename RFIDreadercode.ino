#include <Servo.h>
Servo myservo; 

char tag1[13] = "67007266DBA8";
int ledPin = 13;
int RFIDResetPin = 9;

boolean checked = false;
int pos = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(RFIDResetPin, OUTPUT);
  digitalWrite(RFIDResetPin, HIGH);
  
  myservo.attach(12);
}

void loop() {
  char tagString[13];
  int index = 0;
  boolean reading = false;
  
  while(Serial.available()) {
    char readByte = Serial.read(); //read next available byte
   
    if (readByte == 2) reading = true; //beginning of tag
    if (readByte == 3) reading = false;  //end of tag
    
    if (reading && readByte != 2 && readByte != 10 && readByte != 13) {
      //store the tag
      tagString[index] = readByte;
      index ++;
     
    }   
  } 
    if (tagString[0] != '0') {
    checkTag(tagString); } //Check if it is a match
    if (checked) {
      myservo.write(0);
      delay(3000);
      myservo.write(120);
      delay(2000);
      
      checked = false;
    }
      
  clearTag(tagString); //Clear the char of all value
  
  resetReader(); //reset the RFID reader
  
 }
  
  
void checkTag(char tag[]) {
  
  if (strlen(tag) == 0) return; //empty
  
  if (compareTag(tag, tag1)) {
    lightLED(ledPin);
  }
  else {
    Serial.println("tag does not match");
    Serial.println(tag);
  }
}

void lightLED(int pin) {
  Serial.println(pin);
  
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);
  
  checked = true;
}

void resetReader(){
  
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(150);
  
}

void clearTag(char one[]){
///////////////////////////////////
//clear the char array by filling with null - ASCII 0
//Will think same tag has been read otherwise
///////////////////////////////////
  for(int i = 0; i < strlen(one); i++){
    one[i] = '0';
  }
}

boolean compareTag(char one[], char two[]){
  if(strlen(one) == 0) return false; //empty

  for(int i = 0; i < 12; i++){
    if(one[i] != two[i]) return false;
  }

  return true; //no mismatches
}

