#include <FastLED.h>

#define B1_INPUT_PIN 10
#define B1_RED_PIN 13
#define B1_GRN_PIN 12
#define B1_BLU_PIN 11

#define B2_INPUT_PIN 6
#define B2_RED_PIN 9
#define B2_GRN_PIN 8
#define B2_BLU_PIN 7

#define B3_INPUT_PIN 2
#define B3_RED_PIN 5
#define B3_GRN_PIN 4
#define B3_BLU_PIN 3

#define JOYSTICK_LEFT_PIN 22
#define JOYSTICK_RIGHT_PIN 24
#define JOYSTICK_DOWN_PIN 26
#define JOYSTICK_UP_PIN 28

#define TOTAL_INPUTS 7

#define LED_PIN_1 34
#define LED_PIN_2 36

#define NUM_LEDS_1 6
#define NUM_LEDS_2 20

struct ButtonState {
    bool justPressed;
    bool isPressed;
    int pin;
    int currentValue;
    int lastValue;
};

ButtonState button1 = { false, false, B1_INPUT_PIN, HIGH, HIGH };
ButtonState button2 = { false, false, B2_INPUT_PIN, HIGH, HIGH };
ButtonState button3 = { false, false, B3_INPUT_PIN, HIGH, HIGH };
ButtonState joystickUp = { false, false, JOYSTICK_UP_PIN, HIGH, HIGH };
ButtonState joystickDown = { false, false, JOYSTICK_DOWN_PIN, HIGH, HIGH };
ButtonState joystickLeft = { false, false, JOYSTICK_LEFT_PIN, HIGH, HIGH };
ButtonState joystickRight = { false, false, JOYSTICK_RIGHT_PIN, HIGH, HIGH };

#define BUTTON_1 0
#define BUTTON_2 1
#define BUTTON_3 2
#define UP 3
#define DOWN 4
#define LEFT 5
#define RIGHT 6

ButtonState buttons[TOTAL_INPUTS] = {
    button1,
    button2,
    button3,
    joystickUp,
    joystickDown,
    joystickLeft,
    joystickRight
};

struct ButtonColor {
    int red;
    int green;
    int blue;
};

ButtonColor NO_COLOR = { HIGH, HIGH, HIGH };
ButtonColor RED = { LOW, HIGH, HIGH };
ButtonColor YELLOW = { LOW, LOW, HIGH };
ButtonColor GREEN = { HIGH, LOW, HIGH };
ButtonColor CYAN = { HIGH, LOW, LOW };
ButtonColor BLUE = { HIGH, HIGH, LOW };
ButtonColor MAGENTA = { LOW, HIGH, LOW };
ButtonColor WHITE = { LOW, LOW, LOW };

struct ButtonColorPins {
    int redPin;
    int greenPin;
    int bluePin;
};

ButtonColorPins buttonColorPins[3] = {
    { B1_RED_PIN, B1_GRN_PIN, B1_BLU_PIN },
    { B2_RED_PIN, B2_GRN_PIN, B2_BLU_PIN },
    { B3_RED_PIN, B3_GRN_PIN, B3_BLU_PIN },
};

CRGB leds_1[NUM_LEDS_1];
CRGB leds_2[NUM_LEDS_2];

void setup() {
    // Set pin modes for all buttons
    for (int i = 0; i < TOTAL_INPUTS; i++) {
        pinMode(buttons[i].pin, INPUT);
    }

    // Set pin modes for all button color pins
    for (int i = 0; i < 3; i++) {
        pinMode(buttonColorPins[i].redPin, OUTPUT);
        pinMode(buttonColorPins[i].greenPin, OUTPUT);
        pinMode(buttonColorPins[i].bluePin, OUTPUT);
    }

    FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds_1, NUM_LEDS_1);
    FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds_2, NUM_LEDS_2);
    FastLED.setBrightness(50);
}

void updateButtonStates() {
    for (int i = 0; i < TOTAL_INPUTS; i++) {
        // Read the current value of the button
        int buttonValue = digitalRead(buttons[i].pin);

        // Update the button state
        buttons[i].lastValue = buttons[i].currentValue;
        buttons[i].currentValue = buttonValue;
        buttons[i].isPressed = buttonValue == LOW;
        buttons[i].justPressed = (buttons[i].lastValue == HIGH && buttons[i].currentValue == LOW);
    }
}

void setButtonColor(int button, ButtonColor color) {
    digitalWrite(buttonColorPins[button].redPin, color.red);
    digitalWrite(buttonColorPins[button].greenPin, color.green);
    digitalWrite(buttonColorPins[button].bluePin, color.blue);
}

#define PATTERN_LENGTH 11
int pattern[PATTERN_LENGTH] = {
    UP,
    UP,
    DOWN,
    DOWN,
    LEFT,
    RIGHT,
    LEFT,
    RIGHT,
    BUTTON_2,
    BUTTON_3,
    BUTTON_1
 };

int placeInPattern = 0;
bool patternCompleted = false;
int framesOfNoInput = 0;

int framesUntilPatternTimeout = 20;
int offset = 0;

void loop() {
    updateButtonStates();
    
    if (buttons[BUTTON_1].isPressed) {
        setButtonColor(BUTTON_1, MAGENTA);
    }
    else {
        if (buttons[UP].isPressed) {
            setButtonColor(BUTTON_1, GREEN);
        }
        else if (buttons[DOWN].isPressed) {
            setButtonColor(BUTTON_1, BLUE);
        }
        else if (buttons[LEFT].isPressed) {
            setButtonColor(BUTTON_1, YELLOW);
        }
        else if (buttons[RIGHT].isPressed) {
            setButtonColor(BUTTON_1, RED);
        }
        else {
            setButtonColor(BUTTON_1, WHITE);
        }
    }   

    if (buttons[BUTTON_2].isPressed) {
        setButtonColor(BUTTON_2, RED);
    }
    else {
        setButtonColor(BUTTON_2, WHITE);
    }

    if (buttons[BUTTON_3].isPressed) {
        setButtonColor(BUTTON_3, CYAN);
    }
    else {
        setButtonColor(BUTTON_3, WHITE);
    }

    if (!patternCompleted) {
        if (isAnyOtherJustPressed(pattern[placeInPattern])) {
            // Wrong button was pressed, reset the pattern
            placeInPattern = 0;
            framesOfNoInput = 0;
        } 
        else if (buttons[pattern[placeInPattern]].justPressed) {
            // Correct button was pressed, advance to polling for next button
            placeInPattern++;
            framesOfNoInput = 0;
            patternCompleted = placeInPattern == PATTERN_LENGTH;
        } 
        else if (placeInPattern != 0) {
            // Keep track of how long user has been on this current step of the pattern
            framesOfNoInput++;

            if (framesOfNoInput >= framesUntilPatternTimeout) {
                // User has waited too long to input next button in pattern, reset
                placeInPattern = 0;
                framesOfNoInput = 0;
                setButtonColor(BUTTON_3, RED);
            } 
        }
    }

    if (patternCompleted) {
        setButtonColor(BUTTON_1, RED);
        setButtonColor(BUTTON_2, GREEN);
        setButtonColor(BUTTON_3, BLUE);
    }

    offset++;
    offset = offset % 256;

    fill_rainbow(leds_1, NUM_LEDS_1, offset);
    fill_rainbow(leds_2, NUM_LEDS_2, offset);
    FastLED.show();

    FastLED.delay(30);
}

bool isAnyOtherJustPressed(int button) {
    for (int i = 0; i < TOTAL_INPUTS; i++) {
        if (i != button && buttons[i].justPressed) {
            return true;
        }
    }
    return false;
}
