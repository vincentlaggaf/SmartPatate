//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************
 
 
//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND
 
 
 
#define SET(x,y) (x |=(1<<y))                                                    //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))                              // |
#define CHK(x,y) (x & (1<<y))                                     // |
#define TOG(x,y) (x^=(1<<y))                                     //-+
 
 
 
#define N 160  //How many frequencies
 
float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;
 
float peakVoltage = 0;
float peakFreq = 0;
float seuilRien = 55; //trouver la bonne valeur
float seuil1doigt = 90; //trouver la bonne valeur
float seuil2doigts = 119; //trouver la bonne valeur
 
   
 
void setup()
{
 
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;
 
  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
 
  Serial.begin(115200);
 
  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+
}
 
void loop()
{
  Serial.println("coucou!");
  unsigned int d;
 
  int counter = 0;
 peakVoltage = 0;
  peakFreq = 0;
  
  for(unsigned int d=0;d<N;d++)
  {
  
 
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator
 
    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
  
    freq[d] = d;
 
//   plot(v,0);              //-Display
//   plot(results[d],1);
  // delayMicroseconds(1);
 
//  -> detection du pic maximum
  if(results[d] > peakVoltage){
               peakVoltage = results[d];
               peakFreq = d;
  }
 
  }
 
 
//PlottArray(1,freq,results);
 
 
  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)


 Serial.print("peakfreq = ");
 Serial.println(peakFreq);
  if(peakFreq < seuilRien){
               digitalWrite(3, LOW);
               digitalWrite(4, LOW);
               digitalWrite(2, HIGH);
               //allumer led "rien"
  }
  else{
                 if(peakFreq < seuil1doigt){
               digitalWrite(2, LOW);
               digitalWrite(4, LOW);
               digitalWrite(3, HIGH);
                              //allumer led "1 doigt"
                 }
                 else{
                                if(peakFreq < seuil2doigts){
                                  digitalWrite(2, LOW);
                                  digitalWrite(3, LOW);
                                  digitalWrite(4, HIGH);
                                            //allumer led "2 doigts"
                                }
                                else{
                                  digitalWrite(2, HIGH);
                                  digitalWrite(3, HIGH);
                                  digitalWrite(4, HIGH);
                                            //allumer led "poignee"
                                }
                 }
  }
}
