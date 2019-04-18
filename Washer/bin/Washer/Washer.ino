//Fill in the values below with the pins that you chose to use
const int DIALPIN = A0;
const int BUTTON = 2;

const int HOT = 4;
const int COLD = 5;
const int DRY = 3;
const int LOCK = 13;

int DIAL = 0;
int buttonState = 0;

enum State {
  standby,
  start,
  econcold,
  econdry,
  deluxhot,
  deluxdry,
  suphot,
  supwarm,
  supdry,
  stopfinal,
};

State counterState = standby;

void setup() {
  Serial.begin(9600);
  pinMode(DRY, OUTPUT);
  pinMode(HOT, OUTPUT);
  pinMode(COLD, OUTPUT);
  pinMode(DIAL, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LOCK, OUTPUT);

 // digitalRead(2);
}

void loop() {
    DIAL = analogRead(DIALPIN);
    counterState = nextState(counterState);
}

State nextState(State state) {
  switch (state) {
    case standby:
      digitalWrite(COLD, LOW);
      digitalWrite(HOT, LOW);
      digitalWrite(DRY, LOW);
      digitalWrite(LOCK, LOW);
      Serial.println(digitalRead(2));
      buttonState = digitalRead(BUTTON);
      if (buttonState == HIGH) {
        state = start;
      }
      else {
        state = standby;
      }
      break;
      
    case start:
      digitalWrite(LOCK, HIGH);
      Serial.println(DIAL);
      if (analogRead(A0) > 800) {
        state = econcold;
      }
      else if (analogRead(A0) <= 800 && analogRead(A0) >= 400) {
        state = deluxhot; 
      }
      else if (analogRead(A0) < 400) {
        state = suphot;
      }
      break;
      
    case econcold:
      digitalWrite(COLD, HIGH);
      delay(5000);
      digitalWrite(COLD,LOW);
      if (analogRead(A0) <= 800 && analogRead(A0) >= 400) {
        state = deluxdry;
      }
      else if (analogRead(A0) < 400) {
        state = supdry;
      }
      else {
        state = econdry;
      }
      break;
      
    case econdry:
      digitalWrite(DRY, HIGH);
      delay(2000);
      digitalWrite(DRY, LOW);
      state = stopfinal;
      break;
      
    case deluxhot:
      digitalWrite(HOT, HIGH);
      delay(7000);
      Serial.println("test");
      digitalWrite(HOT, LOW);
      if (analogRead(A0) > 800) {
        state = econdry;
      }
      else {
        state = deluxdry;
      }
      break;
      
    case deluxdry:
      digitalWrite(DRY, HIGH);
      delay(7000);
      digitalWrite(DRY, LOW);
      state = stopfinal;
      break;
      
    case suphot:
      digitalWrite(HOT, HIGH);
      delay(7000);
      digitalWrite(HOT, LOW);
      if (analogRead(A0) >= 800) {
        state = econdry;
      }
      else {
        state = supwarm;
      }
      break;
      
    case supwarm:
      digitalWrite(COLD, HIGH);
      digitalWrite(HOT, HIGH);
      delay(7000);
      digitalWrite(COLD, LOW);
      digitalWrite(HOT, LOW);
      state = supdry;
      break;
      
    case supdry:
      digitalWrite(DRY, HIGH);
      delay(7000);
      digitalWrite(DRY, LOW);
      state = stopfinal;
      break;
      
    case stopfinal:
      digitalWrite(COLD, LOW);
      digitalWrite(HOT, LOW);
      digitalWrite(DRY, LOW);
      digitalWrite(LOCK, LOW);
      delay(100);
      state = standby;
      break;
  }
  return state;
}

