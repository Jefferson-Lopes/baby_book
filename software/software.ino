/**
 * @file software.ino
 * @author Jefferson Lopes (jefferson.lopes@ee.ufcg.edu.br)
 * @brief the book "Baby Engineer: Logic Gates" is designed to teach logic gates 
 *        to kids and teenagers. It is especially appealing to kids with autism 
 *        since it uses visual and auditory signs to keep their interest.
 * @version 0.1
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// pinout
#define PIN_PIEZO 3
#define PIN_LED_RED 11
#define PIN_LED_BLUE 10
#define PIN_LED_GREEN 9
#define PIN_BUTTON_GREEN 4
#define PIN_BUTTON_YELLOW 2

// configs
#define TH 100 // THRESHOLD
#define INTRO_TIME 500 // intro delay time

// states
typedef enum {
    INIT,
    AND,
    OR,
    NAND,
    NOR,
    XOR,
    LATCH
} state_t;

// global vars
bool button_Y, button_G;
state_t state_gate = INIT;
state_t last_state_gate = INIT;
uint16_t a0, a1, a2, a3, a4, a5;

void setup(){
    pinMode(PIN_PIEZO, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_BUTTON_GREEN, INPUT_PULLUP);
    pinMode(PIN_BUTTON_YELLOW, INPUT_PULLUP);

    // init and config serial monitor
    Serial.begin(9600);

    // initial beep
    analogWrite(PIN_PIEZO, 125); delay(100);
    analogWrite(PIN_PIEZO, 0);   delay(100);
    analogWrite(PIN_PIEZO, 125); delay(100);
    analogWrite(PIN_PIEZO, 0);

    // RGB intro
    analogWrite(PIN_LED_RED, 255); delay(INTRO_TIME);
    analogWrite(PIN_LED_BLUE, 255); delay(INTRO_TIME);
    analogWrite(PIN_LED_GREEN, 255); delay(INTRO_TIME);
    analogWrite(PIN_LED_RED, 0); delay(INTRO_TIME);
    analogWrite(PIN_LED_BLUE, 0); delay(INTRO_TIME);
    analogWrite(PIN_LED_GREEN, 0); delay(INTRO_TIME);
}

void loop(){
    define_state();

    update_logic();

    delay(10);
}

void define_state(void){
    a0 = analogRead(A0);
    a1 = analogRead(A1);
    a2 = analogRead(A2);
    a3 = analogRead(A3);
    a4 = analogRead(A4);
    a5 = analogRead(A5);

    if (a0 > TH && a1 < TH && a2 < TH && a3 < TH && a4 < TH && a5 < TH){
        // AND
        last_state_gate = state_gate;
        state_gate = AND;
    }
    else if (a0 > TH && a1 > TH && a2 < TH && a3 < TH && a4 < TH && a5 < TH){
        // OR
        last_state_gate = state_gate;
        state_gate = OR;
    }
    else if (a0 > TH && a1 > TH && a2 > TH && a3 < TH && a4 < TH && a5 < TH){
        // NAND
        last_state_gate = state_gate;
        state_gate = NAND;
    }
    else if (a0 > TH && a1 > TH && a2 > TH && a3 > TH && a4 < TH && a5 < TH){
        // NOR
        last_state_gate = state_gate;
        state_gate = NOR;
    }
    else if (a0 > TH && a1 > TH && a2 > TH && a3 > TH && a4 > TH && a5 < TH){
        // XOR
        last_state_gate = state_gate;
        state_gate = XOR;
    }
    else if (a0 > TH && a1 > TH && a2 > TH && a3 > TH && a4 > TH && a5 > TH){
        // LATCH
        last_state_gate = state_gate;
        state_gate = LATCH;
    }

    if (last_state_gate != state_gate){
        // turn off LED
        analogWrite(PIN_LED_RED, 0);
        analogWrite(PIN_LED_BLUE, 0);
        analogWrite(PIN_LED_GREEN, 0);

        // debug message
        Serial.print("New state: ");
        Serial.println(state_gate);

        delay(10);
    }
}

void update_logic(){
    button_Y = !digitalRead(PIN_BUTTON_YELLOW); // inverted logic
    button_G = !digitalRead(PIN_BUTTON_GREEN);  // inverted logic

    if (state_gate == AND){
        // AND
        if (button_Y & button_G){
            // turn on
            analogWrite(PIN_LED_RED, 255);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
        else{
            // turn off
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
    }
    else if (state_gate == OR){
        // OR
        if (button_Y | button_G){
            // turn on
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 255);
            analogWrite(PIN_LED_GREEN, 0);
        }
        else{
            // turn off
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
    }
    else if (state_gate == NAND){
        // NAND
        if (!(button_Y & button_G)){
            // turn on
            analogWrite(PIN_LED_RED, 255);
            analogWrite(PIN_LED_BLUE, 255);
            analogWrite(PIN_LED_GREEN, 0);
        }
        else{
            // turn off
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
    }
    else if (state_gate == NOR){
        // NOR
        if (!(button_Y | button_G)){
            // turn on
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 255);
        }
        else{
            // turn off
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
    }
    else if (state_gate == XOR){
        // XOR
        if (button_Y ^ button_G){
            // turn on
            analogWrite(PIN_LED_RED, 255);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 255);
        }
        else{
            // turn off
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
    }
    else if (state_gate == LATCH){
        // LATCH
        if (button_Y && !button_G){
            // set
            // turn on
            analogWrite(PIN_LED_RED, 255);
            analogWrite(PIN_LED_BLUE, 255);
            analogWrite(PIN_LED_GREEN, 255);
        }
        else if (!button_Y && button_G){
            // reset
            // turn off
            analogWrite(PIN_LED_RED, 0);
            analogWrite(PIN_LED_BLUE, 0);
            analogWrite(PIN_LED_GREEN, 0);
        }
    }
}
