/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

float temperatura(float x){
 float result = (((x / 1023.0) * 5000.0)-400.0) / 19.53;  
  return result;
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
 

 // Serial.println(tempU);
  float k = temperatura(analogRead(A1));
Serial.println(k);
  delay(1000);  // delay in between reads for stability
}
