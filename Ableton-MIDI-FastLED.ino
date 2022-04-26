#include <FastLED.h>

#define NUM_LEDS 242 // how many pixels
#define LED_PIN 3 // what pin

// MIDI Notes to MIDI noteNumber
const int cSharp_2 = 1;  // C#-2
const int D_2      = 2;  // D-2
const int dSharp_2 = 3;  // D#-2
const int E_2      = 4;  // E-2
const int F_2      = 5;  // F-2
const int fSharp_2 = 6;  // F#-2
const int G_2      = 7;  // G-2
const int gSharp_2 = 8;  // G#-2
const int A_2      = 9;  // A-2
const int aSharp_2 = 10; // A#-2
const int B_2      = 11; // B-2
const int C_1      = 12; // C-1
const int cSharp_1 = 13; // C#-1
const int D_1      = 14; // D-1
const int dSharp_1 = 15; // D#-1
const int E_1      = 16; // E-1
const int F_1      = 17; // F-1
const int fSharp_1 = 18; // C#-1
const int G_1      = 19; // G-1
const int gSharp_1 = 20; // G#-1
const int A_1      = 21; // A-1
const int aSharp_1 = 22; // A#-1
const int B_1      = 23; // B-1
const int C_0      = 24; // C-0
const int cSharp_0 = 25; // C#-0

// bpm changes
const int A_7      = 117;// A7
const int B_7      = 119;// B7
const int C_8      = 120;// C8
const int D_8      = 122;// D8
const int E_8      = 124;// E8
const int F_8      = 125;// F8
const int G_8      = 127;// G8

// PALETTES
uint8_t paletteIndex = 0;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending = LINEARBLEND;

CRGBPalette16 purplePalette = CRGBPalette16 (
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  
  CRGB::Magenta,
  CRGB::Magenta,
  CRGB::Linen,
  CRGB::Linen,
  
  CRGB::Magenta,
  CRGB::Magenta,
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  
  CRGB::DarkViolet,
  CRGB::DarkViolet,
  CRGB::Linen,
  CRGB::Linen
);
  
CRGBPalette16 myPal = purplePalette;

DEFINE_GRADIENT_PALETTE( blues_gp ) {
    0,   0,  0, 45,
   25,   0,  0,125,
   51,   0,  0,255,
   76,   7, 12,255,
  127,   7, 55,255,
  178,   0,138,255,
  242,   0,255,255,
  255, 255,255,255};

DEFINE_GRADIENT_PALETTE( he_gp ) {
    0,   0,  0,  0,
    3,  42,  0,  1,
    7,  42,  0, 22,
   16,  42,  0, 79,
   31,  42, 55, 21,
   63,  42,135,  8,
  127, 120,161, 45,
  255, 255,255,255};


CRGBPalette16 bluePalette = blues_gp;

CRGBPalette16 greenPalette = he_gp;

// processing data in
char serialData;

// saturation and hue parameters
uint8_t fullVal = 255;    // for functions requiring full HSV Value (going light to dark)
uint8_t hue = 0;          // for functions requiring a start at the beginning hue value in HSV
uint8_t ranHue = random8();
uint8_t startingPos = 0;

//object
CRGB leds[NUM_LEDS];

// song BPM in Q8.8 format (actual BPM of the song multiplied by 256)
float bpm = 30;           // default BPM if none is specified
float movingBPM = 30;     // default BPM if none is specified

float songA   = 39936;   // BPM of songA 156   * 256
float songB   = 51200;   // BPM of songB 200   * 256
float songC   = 35840;   // BPM of songC 140   * 256
float songD   = 40960;   // BPM of songD 160   * 256 
float songE   = 48640;   // BPM of songE 190   * 256
float songF   = 34457.6; // BPM of songF 134.6 * 256
float songG   = 34457.6; // BPM of songG 134.6 * 256
float songA_m = 30;

bool hasRun = false;

// the setup function runs once when you press reset or power the board
void setup() {

  // for safety
  delay(1000);
    
  // FAST LED INIT
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100); // max value of 255
  FastLED.setCorrection(TypicalPixelString);

  Serial.begin(9600);

  // turn the lights on for a brief moment to let the user know everything is working so far
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
  delay(250);
  FastLED.clear();
  
}

// the loop function runs over and over again forever
void loop() {

  if(Serial.available()){
    serialData = Serial.read();
  }

// write color to all pixels
  
    if(serialData == cSharp_2){
      // turn all lights green
      fill_solid(leds, NUM_LEDS, CRGB::Green);
    } 
    else if(serialData == D_2){
      // turn all lights blue
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
    } 
    else if(serialData == dSharp_2){
      // turn all lights red
      fill_solid(leds, NUM_LEDS, CRGB::Red);
    }
    else if(serialData == E_2){
      // turn all lights bright white
      fill_solid(leds, NUM_LEDS, CRGB::White);
    }
    else if(serialData == F_2){
      // White Fade
      // turn all the lights bright white
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(0, 0, fullVal);
      }

      // fade saturation from 255 to 0 by 1 every 5 milliseconds
      EVERY_N_MILLISECONDS(5){
        if(fullVal != 0){
          fullVal--;
        }
      }
      
    }
    else if(serialData == fSharp_2){

    if(hasRun == false){

      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(0, 0, random8());
      }
      hasRun = true;
    }


      // White flashing chase
      EVERY_N_MILLISECONDS(5){
        // create new white pixel for led[0]
        leds[0] = CHSV(0, 0, random8());

        for(int i = NUM_LEDS - 1; i > 0; i--){
          leds[i] = leds[i - 1]; 
        }
      }      
    }  
    else if(serialData == G_2){

      // TWINKLING PURPLE PALETTE
      
      // switch on an LED at random, choosing a random color from the palette
      EVERY_N_MILLISECONDS(5){
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(myPal, random8(), 255, LINEARBLEND);
      }
      
      // fade all leds down by 1 in brightness each time this is called
      fadeToBlackBy(leds, NUM_LEDS, 1);
     
    }
    else if(serialData == gSharp_2){

      // moving light left to right
      uint8_t sinBeat  = beatsin8(movingBPM, 0, NUM_LEDS - 1, 0, 0);
      uint8_t sinBeat2 = beatsin8(movingBPM, 0, NUM_LEDS - 1, 0, 127);
      leds[sinBeat]  = CRGB::White;
      leds[sinBeat2] = CRGB::White;
      fadeToBlackBy(leds, NUM_LEDS, 10);
     
    }
    else if(serialData == A_2){

      // pulsing purple
      uint8_t bright = beatsin8(bpm, 50, 255, 0, 0);
    
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(200, bright, bright);
      }
     
    }
    else if(serialData == aSharp_2){

      // scrolling rainbow
      uint16_t beatA = beatsin16(30, 0, 255);
      uint16_t beatB = beatsin16(20, 0, 255);
      fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);
     
    }
    else if(serialData == B_2){

      // pulsing white
      uint8_t bright = beatsin8(bpm, 50, 255, 0, 0);
    
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(0, 0, bright);
      }
     
    }
    else if(serialData == C_1){

      // eight colored dots, weaving in and out of sync with each other
      fadeToBlackBy( leds, NUM_LEDS, 20);
      uint8_t dothue = 0;
      for( int i = 0; i < 8; i++) {
        leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
        dothue += 32;
      }
           
    }
    else if(serialData == cSharp_1){

      //PAPARAZZI
      fill_solid( leds, NUM_LEDS, CRGB::Black);  
      addGlitter(150);
           
    }
    else if(serialData == D_1){

      // a colored dot sweeping back and forth, with fading trails
      fadeToBlackBy( leds, NUM_LEDS, 20);
      int pos = beatsin8(movingBPM, 0, NUM_LEDS-1 );
      leds[pos] += CHSV( ranHue, 255, 192);
           
    }
    else if(serialData == dSharp_1){

      // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
      CRGBPalette16 palette = PartyColors_p;
      uint8_t beat = beatsin8( bpm, 64, 255);
      for( int i = 0; i < NUM_LEDS; i++) { //9948
        leds[i] = ColorFromPalette(palette, hue+(i*2), beat-hue+(i*10));
      }
           
    }
    if(serialData == E_1){
      // slowly change the red
      for(int i=0; i < NUM_LEDS; i++){
        leds[i] = CRGB(hue,0,0);
      }
      // slowly fade the color in
      EVERY_N_MILLISECONDS(25){
        if(hue < 255){
          hue++;
        }
      }
      
    } 
    else if(serialData == F_1){
      // slowly change to green
      for(int i=0; i < NUM_LEDS; i++){
        leds[i] = CRGB(0,hue,0);
      }
      // slowly fade the color in
      EVERY_N_MILLISECONDS(25){
        if(hue < 255){
          hue++;
        }
      }
    } 
    else if(serialData == fSharp_1){
      // slowly change to blue
      for(int i=0; i < NUM_LEDS; i++){
        leds[i] = CRGB(0,0,hue);
      }

      // slowly fade the color in
      EVERY_N_MILLISECONDS(25){
        if(hue < 255){
          hue++;
        }
      }
    }
    else if(serialData == G_1){

      // slowly change to white
      for(int i=0; i < NUM_LEDS; i++){
        leds[i] = CRGB(hue,hue,hue);
      }

      // slowly fade the color in
      EVERY_N_MILLISECONDS(25){
        if(hue < 255){
          hue++;
        }
      }

    }
    else if(serialData == gSharp_1){

      //BEATWAVE//
      
      currentPalette = RainbowColors_p;
      
      EVERY_N_MILLISECONDS(100) {
        uint8_t maxChanges = 24; 
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
      }
    
      EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
        targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
      }
      uint8_t wave1 = beatsin8(9, 0, 255);                        // That's the same as beatsin8(9);
      uint8_t wave2 = beatsin8(8, 0, 255);
      uint8_t wave3 = beatsin8(7, 0, 255);
      uint8_t wave4 = beatsin8(6, 0, 255);
  
      Serial.print(wave1+wave2+wave3+wave4);
    
      for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, i+wave1+wave2+wave3+wave4, 255, currentBlending); 
      }

    }
    else if(serialData == A_1){

      // TWINKLING BLUE PALETTE
      
      // switch on an LED at random, choosing a random color from the palette
      EVERY_N_MILLISECONDS(5){
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(bluePalette, random8(), 255, LINEARBLEND);
      }
      
      // fade all leds down by 1 in brightness each time this is called
      fadeToBlackBy(leds, NUM_LEDS, 1);
     
    }
    else if(serialData == aSharp_1){

      // pulsing blue
      uint8_t bright = beatsin8(bpm, 50, 255, 0, 0);
    
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(160, 255, bright);
      }
     
    }
    else if(serialData == B_1){

      // pulsing red
      uint8_t bright = beatsin8(bpm, 50, 255, 0, 0);
    
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(0, 255, bright);
      }
     
    }
    else if(serialData == C_0){

      // pulsing green
      uint8_t bright = beatsin8(bpm, 50, 255, 0, 0);
    
      for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(96, 255, bright);
      }
     
    }
    else if(serialData == cSharp_0){

      // TWINKLING GREEN PALETTE
      
      // switch on an LED at random, choosing a random color from the palette
      EVERY_N_MILLISECONDS(5){
        leds[random8(0, NUM_LEDS - 1)] = ColorFromPalette(greenPalette, random8(), 255, LINEARBLEND);
      }
      
      // fade all leds down by 1 in brightness each time this is called
      fadeToBlackBy(leds, NUM_LEDS, 1);
     
    }

    // BPM CHANGE MIDI NOTES
    else if(serialData == G_8){                               // IN THIS SECTION, MIDI NOTES WILL CHANGE THE BPM PARAMETERS OF THE 
                                                              // BEATSIN8/16/88 FUNCTIONS THAT MOVE THE LED's ACROSS THE STRIP.  
      // change BPM for song FLOURISH                         
      bpm = songA;
      movingBPM = songA_m;
    }                                                         // ACCURATE BPM IN FASTLED IS CALCULATED BY MULTIPLYING YOUR SONGS BPM BY 256
    else if(serialData == F_8){                               // songA IS AT A BPM OF 156 SO THE FUNCTIONS ABOVE WITH BPM PARAMATERS
      // change BPM for song HOME AWAY FROM HOME              // WILL BE SET AT 39936, TRIGGERING BEATSIN8 FUNCTIONED LIGHTS AT A BPM OF 156.
      bpm = songB; 
    } 
    else if(serialData == E_8){                               
      // change BPM for song TKO            
      bpm = songC; 
    } 
    else if(serialData == D_8){                              
      // change BPM for song EXCUSE ME MISS            
      bpm = songD; 
    }
    else if(serialData == C_8){                              
      // change BPM for song SHINOBI FIRE              
      bpm = songE; 
    } 
    else if(serialData == B_7){                              
      // change BPM for song BITTER TASTE OF TEA           
      bpm = songF; 
    }
    else if(serialData == A_7){                              
      // change BPM for song GREEN EYES             
      bpm = songG; 
    }  


    // reset some values
    if(!serialData) {
      ranHue = random8();
      hue = 0;
      fullVal = 255;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      hasRun = false;
    }

    FastLED.show();

}

void addGlitter( fract8 chanceOfGlitter) {
  
    if( random8() < chanceOfGlitter) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
    
}
