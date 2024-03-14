#define B1_PIN 2
#define B1_RED_PIN 3
#define B1_GRN_PIN 4
#define B1_BLU_PIN 5

#define B2_PIN 6
#define B2_RED_PIN 7
#define B2_GRN_PIN 8
#define B2_BLU_PIN 9

#define B3_PIN 10
#define B3_RED_PIN 11
#define B3_GRN_PIN 12
#define B3_BLU_PIN 13

void setup() {
    pinMode(B1_PIN, INPUT);
    pinMode(B1_RED_PIN, OUTPUT);
    pinMode(B1_GRN_PIN, OUTPUT);
    pinMode(B1_BLU_PIN, OUTPUT);

    pinMode(B2_PIN, INPUT);
    pinMode(B2_RED_PIN, OUTPUT);
    pinMode(B2_GRN_PIN, OUTPUT);
    pinMode(B2_BLU_PIN, OUTPUT);

    pinMode(B3_PIN, INPUT);
    pinMode(B3_RED_PIN, OUTPUT);
    pinMode(B3_GRN_PIN, OUTPUT);
    pinMode(B3_BLU_PIN, OUTPUT);
}

int b1State = 0; 
int b2State = 0; 
int b3State = 0; 

void loop() {  
    b1State = digitalRead(B1_PIN);
    if (b1State == LOW) {
      // Pressed
      digitalWrite(B1_RED_PIN, LOW);
      digitalWrite(B1_GRN_PIN, LOW);  
      digitalWrite(B1_BLU_PIN, HIGH);
    }
    else {
      digitalWrite(B1_RED_PIN, LOW);
      digitalWrite(B1_GRN_PIN, LOW);  
      digitalWrite(B1_BLU_PIN, LOW);
    } 
     
    b2State = digitalRead(B2_PIN);
    if (b2State == LOW) {
      // Pressed
      digitalWrite(B2_RED_PIN, HIGH);
      digitalWrite(B2_GRN_PIN, LOW);  
      digitalWrite(B2_BLU_PIN, LOW);
    }
    else {
      digitalWrite(B2_RED_PIN, LOW);
      digitalWrite(B2_GRN_PIN, LOW);  
      digitalWrite(B2_BLU_PIN, LOW);
    }  
    
    b3State = digitalRead(B3_PIN);
    if (b3State == LOW) {
      // Pressed
      digitalWrite(B3_RED_PIN, LOW);
      digitalWrite(B3_GRN_PIN, HIGH);  
      digitalWrite(B3_BLU_PIN, LOW);
    }
    else {
      digitalWrite(B3_RED_PIN, LOW);
      digitalWrite(B3_GRN_PIN, LOW);  
      digitalWrite(B3_BLU_PIN, LOW);
    }
}
