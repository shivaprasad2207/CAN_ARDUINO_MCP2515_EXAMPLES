
const int ledPin = 4;  // the number of the LED pin
int buttonState = 1 ;
void setup() {

 Serial.begin(115200);
 pinMode(ledPin, OUTPUT);

}
void loop(){
 if (buttonState == 1) {
  digitalWrite(ledPin, HIGH);
  Serial.println("HIGH");
  buttonState = 0;
 }
 else {
  Serial.println("LOW");
  digitalWrite(ledPin, LOW);
  buttonState = 1;
 }
 delay (2000);
}
