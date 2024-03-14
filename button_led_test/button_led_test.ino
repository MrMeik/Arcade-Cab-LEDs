#define LED_PIN_RED 5
#define LED_PIN_GREEN 6
#define LED_PIN_BLUE 7

void setup() {
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN_RED, HIGH);
    digitalWrite(LED_PIN_GREEN, HIGH);  
    digitalWrite(LED_PIN_BLUE, LOW);

    // Add a delay to keep the LED on for some time
    delay(200); 

    digitalWrite(LED_PIN_RED, HIGH);
    digitalWrite(LED_PIN_GREEN, LOW);  
    digitalWrite(LED_PIN_BLUE, LOW);

    // Add a delay to keep the LED on for some time
    delay(200);   

    digitalWrite(LED_PIN_RED, HIGH);
    digitalWrite(LED_PIN_GREEN, LOW);  
    digitalWrite(LED_PIN_BLUE, HIGH);

    // Add a delay to keep the LED on for some time
    delay(200);   

    digitalWrite(LED_PIN_RED, LOW);
    digitalWrite(LED_PIN_GREEN, LOW);  
    digitalWrite(LED_PIN_BLUE, HIGH);

    // Add a delay to keep the LED on for some time
    delay(200);     

    digitalWrite(LED_PIN_RED, LOW);
    digitalWrite(LED_PIN_GREEN, HIGH);  
    digitalWrite(LED_PIN_BLUE, HIGH);

    // Add a delay to keep the LED on for some time
    delay(200);       

    digitalWrite(LED_PIN_RED, LOW);
    digitalWrite(LED_PIN_GREEN, HIGH);  
    digitalWrite(LED_PIN_BLUE, LOW);

    // Add a delay to keep the LED on for some time
    delay(200);  

    digitalWrite(LED_PIN_RED, LOW);
    digitalWrite(LED_PIN_GREEN, LOW);  
    digitalWrite(LED_PIN_BLUE, LOW);

    // Add a delay to keep the LED on for some time
    delay(200);  

    
    digitalWrite(LED_PIN_RED, HIGH);
    digitalWrite(LED_PIN_GREEN, HIGH);  
    digitalWrite(LED_PIN_BLUE, HIGH);

    // Add a delay to keep the LED on for some time
    delay(200);   
}
