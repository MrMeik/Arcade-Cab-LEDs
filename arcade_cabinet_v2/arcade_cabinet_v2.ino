#include <FastLED.h>

#define B1_INPUT_PIN 2
#define B1_RED_PIN 3
#define B1_GRN_PIN 4
#define B1_BLU_PIN 5

#define B2_INPUT_PIN 6
#define B2_RED_PIN 7
#define B2_GRN_PIN 8
#define B2_BLU_PIN 9

#define B3_INPUT_PIN 10
#define B3_RED_PIN 11
#define B3_GRN_PIN 12
#define B3_BLU_PIN 13

#define JOYSTICK_LEFT_PIN 15
#define JOYSTICK_RIGHT_PIN 16
#define JOYSTICK_DOWN_PIN 17
#define JOYSTICK_UP_PIN 18

#define TOTAL_INPUTS 7

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

#define LED_PIN_1 19
#define LED_PIN_2 14

#define NUM_LEDS_1 10
#define NUM_LEDS_2 6

CRGB leds_1[NUM_LEDS_1];
CRGB leds_2[NUM_LEDS_2];

struct LedStruct {
    int num;
    CRGB buffer[];
};

LedStruct Leds[2] = {
    { NUM_LEDS_1, leds_1 },
    { NUM_LEDS_2, leds_2 },
};

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

    setToAnimPulse();
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

int animationOffset = 0;

#define ANIM_OFF 0
#define ANIM_SOLID 1
#define ANIM_ALT_FLASH 2
#define ANIM_RAINBOW 3
#define ANIM_PULSE 4

int animationState = ANIM_OFF;

CRGB animColor = CRGB::Aqua;

void loop() {
    EVERY_N_MILLISECONDS(30) {
        updateInputs();
    }

    if (animationState == ANIM_PULSE) {
        EVERY_N_MILLISECONDS(50) {
            bool useRed = buttons[BUTTON_1].isPressed;
            bool useGreen = buttons[BUTTON_2].isPressed;
            bool useBlue = buttons[BUTTON_3].isPressed;

            bool addPixel = useRed || useGreen || useBlue;

            CRGB newPixelColor = animColor;

            if (addPixel) {
                newPixelColor = CRGB::Black;
                if (useRed) {
                    newPixelColor.red = 200;
                }
                if (useGreen) {
                    newPixelColor.green = 200;
                }
                if (useBlue) {
                    newPixelColor.blue = 200;
                }
            }

            for (int ledIndex = 0; ledIndex < 2; ledIndex++) {
                LedStruct led = Leds[ledIndex];

                for (int i = led.num - 1; i > 0; i--) {
                    led.buffer[i] = led.buffer[i - 1];
                }

                led.buffer[0] = newPixelColor;
            }
        }       
    }
    else if (animationState == ANIM_RAINBOW) {
        EVERY_N_MILLISECONDS(10) {
            animationOffset++;
        }

        for (int ledIndex = 0; ledIndex < 2; ledIndex++) {
            LedStruct led = Leds[ledIndex];
            fill_rainbow(led.buffer, led.num, animationOffset);
        }
    }

    FastLED.show();
    FastLED.delay(1);
}

void setToAnimPulse() {
    for (int ledIndex = 0; ledIndex < 2; ledIndex++) {
        LedStruct led = Leds[ledIndex];
        fill_solid(led.buffer, led.num, animColor);
    }
    animationState = ANIM_PULSE;
}

void updateInputs() {
    updateButtonStates();

    if (buttons[BUTTON_1].isPressed) {
        setButtonColor(BUTTON_1, MAGENTA);
    } else {
        if (buttons[UP].isPressed) {
            setButtonColor(BUTTON_1, GREEN);
        } else if (buttons[DOWN].isPressed) {
            setButtonColor(BUTTON_1, BLUE);
        } else if (buttons[LEFT].isPressed) {
            setButtonColor(BUTTON_1, YELLOW);
        } else if (buttons[RIGHT].isPressed) {
            setButtonColor(BUTTON_1, RED);
        } else {
            setButtonColor(BUTTON_1, WHITE);
        }
    }

    if (buttons[BUTTON_2].isPressed) {
        setButtonColor(BUTTON_2, RED);
    } else {
        setButtonColor(BUTTON_2, WHITE);
    }

    if (buttons[BUTTON_3].isPressed) {
        setButtonColor(BUTTON_3, CYAN);
    } else {
        setButtonColor(BUTTON_3, WHITE);
    }

    if (!patternCompleted) {
        if (isAnyOtherJustPressed(pattern[placeInPattern])) {
            // Wrong button was pressed, reset the pattern
            placeInPattern = 0;
            framesOfNoInput = 0;
        } else if (buttons[pattern[placeInPattern]].justPressed) {
            // Correct button was pressed, advance to polling for next button
            placeInPattern++;
            framesOfNoInput = 0;
            patternCompleted = placeInPattern == PATTERN_LENGTH;
        } else if (placeInPattern != 0) {
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
}

bool isAnyOtherJustPressed(int button) {
    for (int i = 0; i < TOTAL_INPUTS; i++) {
        if (i != button && buttons[i].justPressed) {
            return true;
        }
    }
    return false;
}
