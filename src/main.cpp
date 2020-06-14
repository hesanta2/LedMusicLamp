#include <LedLamp.cpp>

#define LED_COUNT 10
const int OUT_PIN = 4;

const int SAMPLE_TIME = 5;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;

int sampleBufferValue = 0;

LedLamp ledLamp(LED_COUNT);

void setup()
{
  Serial.begin(115200);

  ledLamp.begin();
  ledLamp.setBrightness(100);
}
int pulse = 0;
void loop()
{
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;

  if (digitalRead(OUT_PIN) == LOW)
  {
    sampleBufferValue++;
  }

  if (millisElapsed > SAMPLE_TIME)
  {
    String s = "sampleBufferValue: ";
    String s2 = s + sampleBufferValue;
    Serial.println(s2);
    pulse = map(sampleBufferValue, 0, 200, 0, 100);
    ledLamp.setPulse(pulse);
  
    sampleBufferValue = 0;
    millisLast = millisCurrent;
  }
}