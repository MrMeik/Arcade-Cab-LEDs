#include <FastLED.h>

#define LED_PIN_1 34
#define LED_PIN_2 36

#define NUM_LEDS_1 15
#define NUM_LEDS_2 15

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

// ==== Button input global variables and constants ====
#define BUTTON_1 0
#define BUTTON_2 1
#define BUTTON_3 2
#define UP 3
#define DOWN 4
#define LEFT 5
#define RIGHT 6

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

ButtonState buttons[TOTAL_INPUTS] = {
    button1,
    button2,
    button3,
    joystickUp,
    joystickDown,
    joystickLeft,
    joystickRight
};

// ==== Button LED global variables and constants ====
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

// ==== LED global variables and constants ====
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

void setButtonColor(int button, ButtonColor color) {
    digitalWrite(buttonColorPins[button].redPin, color.red);
    digitalWrite(buttonColorPins[button].greenPin, color.green);
    digitalWrite(buttonColorPins[button].bluePin, color.blue);
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
}

const int solid_color_options = 7;
const CRGB solid_colors[solid_color_options] = {
    CRGB::HotPink,
    CRGB::Crimson,
    CRGB::DarkBlue,
    CRGB::DarkOrange,
    CRGB::Gold,
    CRGB::Indigo,
    CRGB::SpringGreen,
};

const int flash_palette_count = 3;
const CRGB flash_palettes[][flash_palette_count] = {
    { CRGB::Red, CRGB::Blue, CRGB::Green },
    { CRGB::DarkOrange, CRGB::SpringGreen, CRGB::HotPink },
    { CRGB::Gold, CRGB::Indigo, CRGB::Crimson },
};

#define ANIM_OFF -1
#define ANIM_SOLID 0
#define ANIM_ALT_FLASH 1
#define ANIM_RAINBOW 2
#define ANIM_PULSE 3

#define TOTAL_PATTERNS 4

uint8_t offset = 0;

uint8_t flash_offset = 0;
int color_block_size = 2;
int color_index = 0;

int currentColorIndex = 0;
int patternVariation = 0;
int animationState = ANIM_SOLID;

void increase_pattern_variation() {
    patternVariation++;
    patternVariation = patternVariation % 10000;
}

void decrease_pattern_variation() {
    patternVariation--;
    patternVariation = patternVariation % 10000;
}

void loop() {
    EVERY_N_MILLISECONDS(30) {
        updateInputs();

        if (buttons[UP].justPressed) {
            increase_pattern_variation();
        }
        if (buttons[DOWN].justPressed) {
            decrease_pattern_variation();
        }
        if (buttons[LEFT].justPressed) {
            animationState = (animationState + TOTAL_PATTERNS - 1) % TOTAL_PATTERNS;
        }
        if (buttons[RIGHT].justPressed) {
            animationState = (animationState + 1) % TOTAL_PATTERNS;
        }

        if (animationState == ANIM_ALT_FLASH) {
            if (buttons[BUTTON_1].isPressed) {
                color_block_size = min(color_block_size + 1, 40);
            }
            if (buttons[BUTTON_2].isPressed) {
                color_block_size = max(color_block_size - 1, 2);
            }
        }
    }

    if (animationState == ANIM_SOLID) {
        EVERY_N_SECONDS(4) {
            increase_pattern_variation();
        }

        CRGB color = solid_colors[patternVariation % solid_color_options];

        fill_solid(leds_1, NUM_LEDS_1, color);
        fill_solid(leds_2, NUM_LEDS_2, color);
    }
    else if (animationState == ANIM_ALT_FLASH) {
        CRGB palette[3] = flash_palettes[patternVariation % flash_palette_count];

        EVERY_N_MILLISECONDS(100) {
            flash_offset = (flash_offset + 1) % 3;

            color_index = flash_offset - 1;
            for (int i = 0; i < NUM_LEDS_1; i++) {
                if (i % color_block_size == 0) {
                    color_index = (color_index - 1 + 3) % 3;
                }

                leds_1[i] = palette[color_index];
            }

            color_index = flash_offset - 1;
            for (int i = 0; i < NUM_LEDS_2; i++) {
                if (i % color_block_size == 0) {
                    color_index = (color_index - 1 + 3) % 3;
                }

                leds_2[i] = palette[color_index];
            }
        }
    }
    else if (animationState == ANIM_PULSE) {
        EVERY_N_MILLISECONDS(20) {
            bool useRed = buttons[BUTTON_1].isPressed;
            bool useGreen = buttons[BUTTON_2].isPressed;
            bool useBlue = buttons[BUTTON_3].isPressed;

            bool addPixel = useRed || useGreen || useBlue;

            CRGB newPixelColor = solid_colors[patternVariation % solid_color_options];

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

            for (int i = NUM_LEDS_1 - 1; i > 0; i--) {
                leds_1[i] = leds_1[i - 1];
            }

            leds_1[0] = newPixelColor;

            for (int i = NUM_LEDS_2 - 1; i > 0; i--) {
                leds_2[i] = leds_2[i - 1];
            }

            leds_2[0] = newPixelColor;
        }       
    }
    else if (animationState == ANIM_RAINBOW) {
        EVERY_N_MILLISECONDS(10) {
            offset = offset + 1;
        }

        fill_rainbow(leds_1, NUM_LEDS_1, offset);
        fill_rainbow(leds_2, NUM_LEDS_2, offset);        
    }


    FastLED.show();
    FastLED.delay(10);
}
