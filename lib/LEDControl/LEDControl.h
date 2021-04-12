#define RED_LED_PIN 4
#define GREEN_LED_PIN 6

class LEDControl {
 public:
  void setupLEDs(void);
  void panic(void);
  void steadyGreen(void);
  void steadyRed(void);
};