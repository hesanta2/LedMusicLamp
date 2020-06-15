#include <Debug.cpp>
#include <LedLamp.cpp>
#include <SoundSensor.cpp>
#include <StopWatch.cpp>

const unsigned int LED_NUMBER = 10;
const unsigned int SOUND_SENSOR_PIN = 4;
const unsigned int SOUND_SENSOR_SAMPLE_MILLISECONDS = 20;

unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;

int soundValue = 0;

Debug debug("");
LedLamp ledLamp(LED_NUMBER);
SoundSensor soundSensor(SOUND_SENSOR_PIN, SOUND_SENSOR_SAMPLE_MILLISECONDS);
StopWatch ledStopWatch;

StopWatch tempStopWatch1;
StopWatch tempStopWatch2;

void setup()
{
  Serial.begin(115200);

  ledLamp.begin();
  ledLamp.setBrightness(100);
}
int pulse = 0;

void digitalReading()
{

  soundValue = soundSensor.readValue();

  if (ledStopWatch.elapsedTime() > 5)
  {
    int sound = soundValue;
    if (sound < 200)
    {
      sound = sound * 3;
    }

    debug.println("sound: ", String(sound));
    pulse = map(sound, 0, 400, 0, 100);
    ledLamp.setPulse(pulse);

    ledStopWatch.reset();
  }
}

void hundredToZeroTesting()
{
  if (ledStopWatch.elapsedTime() > 5)
  {
    ledLamp.setPulse(pulse);
    ledStopWatch.reset();
  }

  if (tempStopWatch1.elapsedTime() > 5000)
  {
    pulse = 100;
  }

  if (tempStopWatch1.elapsedTime() > 7500)
  {
    pulse = 0;
    tempStopWatch1.reset();
  }
}

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
void analogReading()
{
  unsigned long startMillis = millis(); // Start of sample window
  float peakToPeak = 0;                 // peak-to-peak level

  unsigned int signalMax = 0;    //minimum value
  unsigned int signalMin = 1024; //maximum value

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0); //get reading from microphone
    if (sample < 1024)      // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample; // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  pulse = map(peakToPeak, 0, 1000, 0, 100);
  debug.println("pulse: ", String(pulse));

  if (ledStopWatch.elapsedTime() > 5)
  {
    ledLamp.setPulse(pulse);

    ledStopWatch.reset();
  }
}

void loop()
{
  digitalReading();
}