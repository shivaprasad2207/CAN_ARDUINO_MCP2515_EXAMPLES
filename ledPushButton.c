const int buttonPin = 8;  // the number of the pushbutton pin
const int ledPin = 4;  // the number of the LED pin
// variables will change:
int buttonState = 0;   // variable for reading the pushbutton status
void setup() {
 // initialize the LED pin as an output:
 Serial.begin(115200);
 // initialize the pushbutton pin as an input:
 pinMode(buttonPin, INPUT);
}
void loop(){
 // read the state of the pushbutton value:
 buttonState = digitalRead(buttonPin);
 // check if the pushbutton is pressed.
 // if it is, the buttonState is HIGH:
 if (buttonState == HIGH) {
 // turn LED on:
 Serial.println("PUSH");
 digitalWrite(ledPin, HIGH);
 }
 else {
 // turn LED off:
 Serial.println("Not PUSH");
 digitalWrite(ledPin, LOW);
 }
}
