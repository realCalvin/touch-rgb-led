#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

//Used for xy touchscreen input
int x, y = 0;
//Variables that are used the LED's effects
int colorful = 0, meteorColor = 0, rainbowColor = 0;
//Speed for the movingRainbow effects
float spdDelay(15);
//Variables for the LED strips' color
float red = 0, green = 0, blue = 0;
//Used to determine if clearLED is pressed
bool turnOn = true;

void setup()
{
  //Brightness Value
  int BRIGHTNESS = 250;
  delay(1000);
  //FastLED Library
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  x = readXValue(); //Reads touchscreen's X value
  y = readYValue(); //Reads touchscreen's Y value

  //Checks any input from Brightness Panel
  checkBrightness();

  //Checks any input from RGB Panel
  checkRGB();

  //Checks for a certain input on the screen to display an effect
  if (x > 65 && x < 230 && y > 660 && y < 895)
  {
    solidColor(red, blue, green);
  }
  if (x > 65 && x < 230 && y < 660 && y > 400)
  {
    //Turn switch is used to determine if clearLED is used or not
    turnSwitch();
    //turnOn is also connected to clearLED
    while (turnOn)
    {
      breathing(red, blue, green);
    }
  }
  if (x > 65 && x < 230 && y<400 & y>160)
  {
    turnSwitch();
    while (turnOn)
    {
      fading();
    }
  }
  if (x > 230 && x < 440 && y > 660 && y < 895)
  {
    rainbow();
  }
  if (x > 230 && x < 440 && y < 660 && y > 400)
  {
    turnSwitch();
    FastLED.clear();
    FastLED.setBrightness(255);
    while (turnOn)
    {
      meteor();
    }
  }
  if (x > 230 && x < 440 && y < 400 && y > 160)
  {
    turnSwitch();
    FastLED.clear();
    FastLED.setBrightness(255);
    while (turnOn)
    {
      multiMeteors();
    }
  }
  if (x > 440 && x < 630 && y > 660 && y < 895)
  {
    turnSwitch();
    while (turnOn)
    {
      movingRainbow();
    }
  }
  if (x > 440 && x < 630 && y < 660 && y > 400)
  {
    turnSwitch();
    while (turnOn)
    {
      sunlight();
    }
  }
  if (x > 440 && x < 630 && y < 400 && y > 160)
  {
    FastLED.clear();
    FastLED.show();
  }
}

/*************************************** READ TOUCHSCREEN COORDINATES ********************************/

//Reads X values on the touchscreen
int readXValue()
{
  int readX = 0;
  pinMode(14, INPUT); //A0 y-
  pinMode(15, OUTPUT); //A1 x+
  pinMode(16, INPUT); //A2 y+
  pinMode(17, OUTPUT); //A3 x-
  digitalWrite(15, LOW); //A1 is now GND
  digitalWrite(17, HIGH); //A3 is now 5V
  delay(5);
  readX = analogRead(0);
  return readX;
}
//Reads Y values on the touchscreen
int readYValue()
{
  int readY = 0;
  pinMode(14, OUTPUT); //A0 y-
  pinMode(15, INPUT); //A1 x+
  pinMode(16, OUTPUT); //A2 y+
  pinMode(17, INPUT); //A3 x-
  digitalWrite(14, LOW); //A0 is now GND
  digitalWrite(16, HIGH); //A2 is now 5V
  delay(5);
  readY = analogRead(1);
  return readY;
}


/*************************************** BRIGHTNESS AND RGB PANEL ********************************/

//Checks if user has touched the brightness panel
void checkBrightness()
{
  x = readXValue();
  y = readYValue();
  if (x > 915 && x < 965 && y > 160 && y < 885)
  {
    //Equation used to convert touchscreen coordinates to the LED's brightness range
    float brightnessValue = (y * 0.3517) - 312;
    FastLED.setBrightness(abs(brightnessValue));
    FastLED.show();
  }
}

//Checks if user has touched the RGB panel
void checkRGB()
{
  if (x > 845 && x < 915 && y > 160 && y < 885)
  {
    float redValue = (y * 0.3517) - 312;
    red = abs(redValue);
    solidColor(red, blue, green);
  }
  if (x < 845 && x > 760 && y > 160 && y < 885)
  {
    float greenValue = (y * 0.3517) - 312;
    green = abs(greenValue);
    solidColor(red, blue, green);
  }
  if (x < 760 && x > 670 && y > 160 && y < 885)
  {
    float blueValue = (y * 0.3517) - 312;
    blue = abs(blueValue);
    solidColor(red, blue, green);
  }
}


/********************************************************************************************/
/*************************************** TOUCHSCREEN EFFECTS ********************************/
/********************************************************************************************/


/***********************************   SOLID COLORS   ***************************************/
void solidColor(int r, int b, int g)
{
  //Loops through all LEDs and sets to a single color
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(r, b, g);
    FastLED.show();
  }
}

/*********************************** BREATHING EFFECT ****************************************/
void breathing(int r, int b, int g)
{
  //Sets all LEDs to a solid color
  solidColor(red, blue, green);
  //Slowly increases LED's brightness with a delay of 15ms
  for (int i = 0; i < 180; i++)     
  {
    checkClearLED();
    if (turnOn == false)
    {
      break;
    }
    FastLED.show();
    FastLED.setBrightness(i);
    delay(15);
  }
  //Slowly decreases LED's brightness with a delay of 15ms
  for (int j = 179; j >= 0; j--)
  {
    checkClearLED();
    if (turnOn == false)
    {
      break;
    }
    FastLED.show();
    FastLED.setBrightness(j);
    delay(15);
  }
}

/************************************* FADING EFFECT ******************************************/
void fading()
{
  //Transitions to different colors each "breath iteration"
  colorChange();
  //Uses previous breathing effect
  fadeBreathe();
}
void fadeBreathe()
{
  for (int i = 0; i < 180; i++)
  {
    checkClearLED();
    if (turnOn == false)
    {
      break;
    }
    FastLED.show();
    FastLED.setBrightness(i);
    delay(15);
  }
  for (int j = 179; j >= 0; j--)
  {
    checkClearLED();
    if (turnOn == false)
    {
      break;
    }
    FastLED.show();
    FastLED.setBrightness(j);
    delay(15);
  }
}
void colorChange()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].setHSV(colorful, 255, 255);
  }
  colorful += 20;
}

/************************************* RAINBOW EFFECT ******************************************/
void rainbow()
{
  //Uses FastLED built-in fill rainbow
  for (int i = 0; i < NUM_LEDS; i++)
  {
    checkBrightness();
    fill_rainbow(leds, i, 0, 50);
    FastLED.show();
  }
}

/************************************* METEOR TRAIL EFFECT ****************************************/
//Based on FastLED library
void meteor()
{
  checkClearLED();
  FastLED.setBrightness(150);
  //Fades the LEDS to (15/255) = 6% brightness
  fadeToBlackBy( leds, NUM_LEDS, 15);
  //Uses a sin curve to determine position/speed
  int place = beatsin16( 8, 0, NUM_LEDS - 1 );
  leds[place] += CHSV(meteorColor, 255, 192);

  FastLED.show();
  FastLED.delay(8);

  //Change colors every 50ms
  EVERY_N_MILLISECONDS( 50 )
  {
    meteorColor++;
  }
}

/****************************** MULTIPLE METEOR TRAILS EFFECT *************************************/
//Based on FastLED library
void multiMeteors()
{
  checkClearLED();
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte mMeteorColor = 0;
  for ( int i = 0; i < 8; i++)
  {
    leds[beatsin16( i + 3, 0, NUM_LEDS - 1 )] |= CHSV(mMeteorColor, 200, 255);
    mMeteorColor += 32;
  }
  FastLED.show();
  FastLED.delay(8);
}

/*********************************** MOVING RAINBOW EFFECT *****************************************/
void movingRainbow()
{
  //Sets led strip position, while slowly transitioning to a different color
  for (int i = 0; i < NUM_LEDS; i++)
  {
    //previousColor variables make a "longer line" of a single color before changing colors
    int previousColor = (rainbowColor / 2);
    leds[i - 1].setHSV(previousColor, 255, 255);
    int previousColor1 = (previousColor / 2);
    leds[i - 2].setHSV(previousColor, 255, 255);
    //FIRST LED; sets it to given color
    leds[i].setHSV(rainbowColor, 255, 255);
    //Iterates the color value to display multiple colors of the rainbow
    rainbowColor += 5;
  }
  checkBrightness();
  checkClearLED();
  //Setting to change how fast the rainbow moves
  if (x < 760 && x > 670 && y > 160 && y < 885)
  {
    float speedDelay = ( (y * 0.276) - 244.26);
    spdDelay = abs(speedDelay);
  }
  FastLED.show();
  FastLED.delay(spdDelay);
}

/************************************* SUNLIGHT EFFECT ******************************************/
void sunlight()
{
  //Changes the color from red to orange to yellow
  for (int color = 0; color < 256; color += 5)
  {
    //Red and green are only changed to give warm colors (red, orange, yellow)
    int red = 255 - color;
    int blue = 0;
    int green = color;
    //Shows the current color for all LEDs
    for (int i = 0; i < NUM_LEDS; i++)
    {
      checkBrightness();
      checkClearLED();
      if (turnOn == false)
      {
        break;
      }
      leds[i].setRGB(red, blue, green);
      FastLED.show();
    }
    if (turnOn == false)
    {
      break;
    }
    //Stops when color>70, due to colors changing from yellow to green
    if (color > 70)
    {
      color = 257;
      //Repeats sunlight but backwards; Goes from yellow to orange to red
      reverse();
    }
  }
}
void reverse()
{
  for (int color1 = 70; color1 > 0; color1 -= 5)
  {
    int red = 255 - color1;
    int blue = 0;
    int green = color1;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      checkBrightness();
      checkClearLED();
      if (turnOn == false)
      {
        break;
      }
      leds[i].setRGB(red, blue, green);
      FastLED.show();
    }
    if (turnOn == false)
    {
      break;
    }
  }
}

/************************************* CLEAR LED ******************************************/
void checkClearLED()
{
  //Reads x&y values and checks if the clear led button is pressed
  x = readXValue();
  y = readYValue();
  if (x > 440 && x < 630 && y < 400 && y > 160)
  {
    turnOn = false;
    FastLED.clear();
    FastLED.show();
  }
}

void turnSwitch()
{
  if (turnOn == false)
  {
    turnOn = true;
  }
}
