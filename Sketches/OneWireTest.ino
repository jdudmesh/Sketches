#include <OneWire.h>
#include <Process.h>

#define ledPin 13

int timer1Countdown;

OneWire ds(7);
Process p;

byte addr[8];
byte addrValid = FALSE;

void setupInterrupts() {

  // initialize timer1
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  timer1Countdown = 3036;   // preload timer 65536-16MHz/256/1Hz

  TCNT1 = timer1Countdown;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

}

void setup() {

  Bridge.begin();
  
  ds.reset_search();
  if(!ds.search(addr)) {          
      Bridge.put("status", "E_NOADDR");
  }
  else {
    if(OneWire::crc8(addr, 7) == addr[7]) {
      addrValid = TRUE;
      Bridge.put("status", "OK");
    }
    else {
      Bridge.put("status", "E_NOCRC");
    }
  }
  
  setupInterrupts();

}

void loop() {

}

void uploadData(float val) {

  char buf[8];
  
  dtostrf(val, 8, 2, buf);
  
  Bridge.put("temp", buf);
  /*
  p.begin("/usr/bin/python");
  p.addParameter("/mnt/sd/arduino/upload.py");
  p.addParameter(buf);
  p.run();
  
  while (p.available()>0) {
    char c = p.read();
    Serial.print(c);
  }
  Serial.flush();  
  */
}

void readSensor() {
  
  byte i;
  byte present = 0;
  byte data[12];
  char buf[8];  

  if(!addrValid) {
    Bridge.put("status", "E_NOADDR");
    delay(1000);
    return;
  }

  ds.reset();
  ds.select(addr);
  
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for (i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  if(OneWire::crc8(data, 8) == data[8]) {
    
    float temperature = (float)(data[0] + 256 * data[1]) / 16;
    dtostrf(temperature, 8, 2, buf);
    
    Bridge.put("temp", buf);
    Bridge.put("status", "OK");
    
  }
  else {
    Bridge.put("status", "E_NOCRC");
  }

}

ISR(TIMER1_OVF_vect)        // interrupt service routine
{
  static int clock = 0;
  
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  
  if(++clock == 60) {
    readSensor();
    clock = 0;
  }
  
  TCNT1 = timer1Countdown;
}

