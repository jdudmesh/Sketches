#include <Process.h>

#define ledPin 13
#define optoPin 7

int timer1_counter;

float sum;
float count;

void setup() {
  
  count = 0;
  
  // Initialize Bridge
  Bridge.begin();

  // Initialize Serial
  Serial.begin(9600);

  // Wait until a Serial Monitor is connected.
  while (!Serial);

  // run various example processes
  //runCurl();

  pinMode(ledPin, OUTPUT);
  pinMode(optoPin, INPUT);

  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64886;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz

  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);

  int adc = analogRead(0);
  int opto = digitalRead(optoPin);
  
  Serial.print(adc);
  Serial.print(",");
  Serial.print(opto);
  Serial.print(",");
  Serial.print(count);
  Serial.print("\n");
  
  sum += (float)adc;
  count++;

  if(count == 60.0) {
    Serial.print("Upload...");
    runCurl(sum / count);
    Serial.print("...done");
    sum = count = 0;
  }
  
  TCNT1 = timer1_counter;   // preload timer
  
}

void loop() {
  // Do nothing here.
}

void runCurl(float val) {
  
  char buf[8];  
  Process p;
  
  p.begin("/root/upload.py");
  p.addParameter(dtostrf(val, 8, 1, buf));
  p.run();

  while (p.available() > 0) {
    char c = p.read();
    Serial.print(c);
  }
  
  Serial.flush();
  
}
