#include <TimerOne.h>

/* traffic light demo for udemy course */
/* By Peter Larsen */

#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

/* 
 *  A light state represents what state a light shows. Nummerically we force the start to 0 (this should be the default)
 */
enum lightState { RED=0, GREEN=1, YELLOW=2 };

/* 
 *  A traffic light is 3 LEDs, one red, one yellow and one green. The state indicates the current state of the light
 *  (which lights are on).
 *  The content of red, yellow and green are the digital pins where the LED are connected to.
 */
struct Light {
  int red, yellow, green;
  lightState state;
};

/* We have 2 lights for our simple traffic light */
Light light1 = { D8, D9, D10, RED };
Light light2 = { D11, D12, D13, RED };

/* Initialize a single light's output pins */
void setupLight(Light l) {
  pinMode(l.red, OUTPUT);
  pinMode(l.yellow, OUTPUT);
  pinMode(l.green, OUTPUT);
}

void setup() {
  setupLight(light1);
  setupLight(light2);

  // Pedistian button on D7
  pinMode(D7, INPUT);

  // Setup Timer to change light
  Timer1.initialize(1000l*1000l); // One Second
  Timer1.attachInterrupt(changeLightState);

  Serial.begin(9600);
}

/* Turn the LEDs to reflect the state of the light */
void setLight(Light light, lightState state) {
  switch (state) {
    case RED: { digitalWrite(light.red, HIGH);
                digitalWrite(light.yellow, LOW);
                digitalWrite(light.green, LOW);
                break;
    }
    case YELLOW: { digitalWrite(light.red, LOW);
                   digitalWrite(light.yellow, HIGH);
                   digitalWrite(light.green, LOW);
                   break;
    }
    case GREEN: { digitalWrite(light.red, LOW);
                  digitalWrite(light.yellow, LOW);
                  digitalWrite(light.green, HIGH);
                  break;
    }
  }
}

/* "global" states possible for all traffic lights */
enum TrafficLightState{ALL_RED, RED_GREEN, RED_YELLOW, GREEN_RED, GREEN_YELLOW};

/* A mode consists of a state and a delay in micro-seconds that the state should be held */
struct TrafficModeType {
  TrafficLightState state;
  long delay;
};

/* A sequence list of the traffice modes */
const TrafficModeType trafficModes[] = { { ALL_RED, 300}, { RED_GREEN, 2000 }, { RED_YELLOW, 300 }, { ALL_RED, 300 }, { GREEN_RED, 2000}, {GREEN_YELLOW, 300} };
int trafficMode = 0;

/* Set the lights to reflect the set state */
void setLightState(int mode) {
  switch (trafficModes[mode].state) {
    case ALL_RED:      { setLight(light1, RED); setLight(light2, RED); break; }
    case RED_GREEN:    { setLight(light1, RED); setLight(light2, GREEN); break; }
    case RED_YELLOW:   { setLight(light1, RED); setLight(light2, YELLOW); break; }
    case GREEN_RED:    { setLight(light1, GREEN); setLight(light2, RED); break; }
    case GREEN_YELLOW: { setLight(light1, YELLOW); setLight(light2, RED); break; }
  };  
}

/* Force current traffic state. Used by the pedestian button */
int newState(int newState) {
  for (int i=0; i<=6; i++) {
    if (trafficModes[i].state == newState) {
      trafficMode = i; // Set the mode to the location where we found things
      setLightState(trafficMode);
      Timer1.setPeriod(6000l*1000l); // Extra long wait
      return 0;
    }
  }
  return 1;
  // Just ignore the mode change
}

/* Timer called function to change/advance the light state */
void changeLightState()  {
  Serial.println(trafficMode);
  if (trafficMode >= 6) trafficMode=0;
  Timer1.setPeriod(trafficModes[trafficMode].delay*2000l);
  Timer1.attachInterrupt(changeLightState);
  setLightState(trafficMode++);
} 

void loop() {
  // Do something with the button
  if (digitalRead(D7) == HIGH) {
    Serial.println("Button Pushed");
    newState(GREEN_RED);
  }
  int test;
  if (test=(5+8) > 10) {} 
}
