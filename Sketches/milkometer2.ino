void setup() {
  
  // Initialize Serial
  Serial.begin(9600);

  // Wait until a Serial Monitor is connected.
  while (!Serial);
  
  pinMode(7, INPUT);

}

void loop() {
  
  float sum = 0.0f;
  float count = 0.0f;
  
  for(int i = 0; i < 1024; i++) {
    int adc = analogRead(0);
    sum += (float)adc;
    count += 1.0f;  
  }
  
  float avg = sum / count;
  
  int opto = digitalRead(7);
    
  Serial.print(avg);
  Serial.print(",");
  Serial.print(opto);
  Serial.print("\n");
  
  delay(500);
  
}
