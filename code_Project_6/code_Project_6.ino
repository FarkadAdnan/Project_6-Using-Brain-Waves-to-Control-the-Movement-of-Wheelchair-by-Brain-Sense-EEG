/*
 By:Farkad Adnan
 E-mail: farkad.hpfa95@gmail.com
 inst : farkadadnan
 #farkadadnan , #farkad_adnan , فرقد عدنان#
 FaceBook: كتاب عالم الاردوينو
 inst : arduinobook
 #كتاب_عالم_الاردوينو  #كتاب_عالم_الآردوينو 
 */ 
#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

#define enA 5 
#define in1 6 
#define in2 7  
#define in3 8 
#define in4 9 
#define enB 10 

#define powercontrol 10

 byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

 long lastReceivedPacket = 0;
boolean bigPacket = false;

int Speed = 130;  
 
void setup() { 
pinMode(enA, OUTPUT);  
pinMode(in1, OUTPUT);   
pinMode(in2, OUTPUT);  
pinMode(in3, OUTPUT);   
pinMode(in4, OUTPUT);  
pinMode(enB, OUTPUT);  

analogWrite(enA, 130);  
analogWrite(enB, 130); 
  pinMode(LED, OUTPUT); 
  Serial.begin(BAUDRATE);       
}

byte ReadOneByte() {
  int ByteRead;
  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   
#endif

  return ByteRead;
}
 
void loop() {
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {
        payloadLength = ReadOneByte();
        if(payloadLength > 169)                    
        return;
        generatedChecksum = 0;        
        for(int i = 0; i < payloadLength; i++) 
        {  
        payloadData[i] = ReadOneByte();    
        generatedChecksum += payloadData[i];
        }   

        checksum = ReadOneByte();                   
        generatedChecksum = 255 - generatedChecksum;
        if(checksum == generatedChecksum) {    
          poorQuality = 200;
          attention = 0;
          meditation = 0;

          for(int i = 0; i < payloadLength; i++) {                                      
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          }
        } 
#if !DEBUGOUTPUT
        if(bigPacket) {
          if(poorQuality == 0)
             digitalWrite(LED, HIGH);
          else
            digitalWrite(LED, LOW);
          
          Serial.print("PoorQuality: ");
          Serial.print(poorQuality, DEC);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\n");
if(attention>40 && attention<60){
forword();         
}
if(attention>60 && attention<70){
backword();
}
if(attention>30 && attention<40){
turnLeft();
}
if(attention>20 && attention<30){            
turnRight();           
}
if(attention<20 || attention>70){
Stop();            
}              
}
#endif        
        bigPacket = false;        
      }
      else {
      }  
    } 
  }
}



void forword(){ 
digitalWrite(in1, HIGH); 
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW);  
digitalWrite(in4, HIGH);
}

void backword(){
  //backword
digitalWrite(in1, LOW); 
digitalWrite(in2, HIGH);
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW); 
}

void turnRight(){ 
digitalWrite(in1, LOW); 
digitalWrite(in2, HIGH);
digitalWrite(in3, LOW); 
digitalWrite(in4, HIGH);
}

void turnLeft(){ 
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW); 
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW); 
}

void Stop(){ 
digitalWrite(in1, LOW);
digitalWrite(in2, LOW); 
digitalWrite(in3, LOW); 
digitalWrite(in4, LOW); 
}
