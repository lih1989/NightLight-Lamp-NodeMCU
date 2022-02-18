/*
  Скетч к проекту "Эффектный светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/gyverlight/
  Исходники на GitHub: https://github.com/AlexGyver/gyverlight/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  https://AlexGyver.ru/
*/

/*
   Управление кнопкой/сенсором
  - Удержание - яркость
  - 1х тап - вкл/выкл
  - 2х тап - переключ режима
  - 3х тап - вкл/выкл белый свет
  - 4х тап - старт/стоп ночника
*/

/*
   Версия 1.3 - пофикшен баг с fillAll
*/

// ************************** НАСТРОЙКИ ***********************
#define CURRENT_LIMIT 2000  // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define NIGHTLIGHT_TIME 60    // время света ночника в секундах

#define NUM_LEDS 36         // количсетво светодиодов в одном отрезке ленты
#define NUM_STRIPS 4        // количество отрезков ленты (в параллели)
#define LED_PIN D6           // пин ленты
#define BTN_PIN D2           // пин кнопки/сенсора
#define MD_PIN D1           // пин сенсора движения
#define MIN_BRIGHTNESS 5  // минимальная яркость при ручной настройке
#define BRIGHTNESS 250      // начальная яркость
#define FIRE_PALETTE 0      // разные типы огня (0 - 3). Попробуй их все! =)

// ************************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
#define MODES_AMOUNT 9

#include "GyverButton.h"
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

#include <FastLED.h>
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

#include "GyverTimer.h"
GTimer_ms effectTimer(60);
GTimer_ms brightTimer(20);
GTimer_ms nightLightTimer((long)NIGHTLIGHT_TIME * 1000);

int brightness = BRIGHTNESS;
int tempBrightness;
byte thisMode;

bool gReverseDirection = false;
boolean loadingFlag = true;
boolean powerDirection = true;
boolean powerActive = false;
boolean powerState = true;
boolean whiteMode = false;
boolean brightDirection = true;
boolean nightLightMode = false;
boolean wasStep = false;


// залить все
void fillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = newcolor;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisPixel) {
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

void setup() {
  pinMode(MD_PIN, INPUT);
  Serial.begin(115200);
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT / NUM_STRIPS);
  FastLED.setBrightness(brightness);
  FastLED.show();

  randomSeed(analogRead(0));
  touch.setTimeout(300);
  touch.setStepTimeout(50);

  if (FIRE_PALETTE == 0) gPal = HeatColors_p;
  else if (FIRE_PALETTE == 1) gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  else if (FIRE_PALETTE == 2) gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  else if (FIRE_PALETTE == 3) gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
}

void loop() {
  touch.tick();
  if (touch.hasClicks()) {
    byte clicks = touch.getClicks();
    switch (clicks) {
      case 1:
        powerDirection = !powerDirection;
        powerActive = true;
        tempBrightness = brightness * !powerDirection;
        break;
      case 2: if (!whiteMode && !powerActive) {
          nextMode();
        }
        break;
      case 3: if (!powerActive) {
          whiteMode = !whiteMode;
          if (whiteMode) {
            effectTimer.stop();
            fillAll(CRGB::White);
            FastLED.show();
          } else {
            effectTimer.start();
          }
        }
        break;
      case 4:
        Serial.println("nightLightMode CHANGE");
        nightLightMode = !nightLightMode;
        if (nightLightMode) {
          nightLightTimer.start();
        } else {
          nightLightTimer.stop();
        }
        powerDirection = false;
        powerActive = true;
        break;
      default:
        break;
    }
  }

  if (touch.isStep()) {
    if (!powerActive) {
      wasStep = true;
      if (brightDirection) {
        brightness += 5;
      } else {
        brightness -= 5;
      }
      brightness = constrain(brightness, MIN_BRIGHTNESS, 255);
      FastLED.setBrightness(brightness);
      FastLED.show();
    }
  }
  
  if (touch.isRelease()) {
    if (wasStep) {
      wasStep = false;
      brightDirection = !brightDirection;
    }
  }

  if (nightLightMode) {
    if (digitalRead(MD_PIN) == HIGH) {
        powerDirection = true;
        powerActive = true;
    } else if (nightLightTimer.isReady()) {
        powerDirection = false;
        powerActive = true;
    }
  }

    if (effectTimer.isReady() && powerState) {
    switch (thisMode) {
      case 0: lighter();
        break;
      case 1: lightBugs();
        break;
      case 2: colors();
        break;
      case 3: rainbow();
        break;
      case 4: sparkles();
        break;
      case 5: fire();
        break;
      case 6:
        gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
        fire();
        break;
      case 7:
        gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
        fire();
        break;
      case 8:
        gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
        fire();
        break;
      case 9: vinigret();
        break;
    }
    FastLED.show();
  }

  brightnessTick();
}

void nextMode() {
  thisMode++;
  if (thisMode >= MODES_AMOUNT) thisMode = 0;
  loadingFlag = true;
  FastLED.clear();
}

void brightnessTick() {
  if (powerActive) {
    if (brightTimer.isReady()) {
      if (powerDirection) {
        powerState = true;
        tempBrightness += 10;
        if (tempBrightness > brightness) {
          tempBrightness = brightness;
          powerActive = false;
        }
        FastLED.setBrightness(tempBrightness);
        FastLED.show();
      } else {
        tempBrightness -= 10;
        if (tempBrightness < 0) {
          tempBrightness = 0;
          powerActive = false;
          powerState = false;
        }
        FastLED.setBrightness(tempBrightness);
        FastLED.show();
      }
    }
  }
}
