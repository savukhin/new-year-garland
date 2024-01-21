#include <Arduino.h>

#ifdef TARGET_BLUEPILL
#define LED1_PIN PB1
#define LED2_PIN PB0
#define TOGGLE_BUTTON_PIN PA11
#else
#define LED1_PIN NC
#define LED2_PIN NC
#define TOGGLE_BUTTON_PIN NC
#endif
