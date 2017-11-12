#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <MeteoDashboard.h>
#include <Fonts/FreeSansBold18pt7b.h>
//#include <Fonts/FreeMonoBold12pt7b.h>
//#include <Fonts/FreeSansBold24pt7b.h>
//#include <Fonts/FreeSansBold56pt7b.h>
//#include <Fonts/FreeSansBold44pt7b.h>

NumberIndicator::NumberIndicator(){
}

NumberIndicator::NumberIndicator(Adafruit_ILI9341 &dsp, int x0, int y0){
  tft = &dsp;
  W = tft->width();
  H = tft->height();
  x = x0, y = y0;
  tft->setTextSize(2);
}

NumberIndicator::NumberIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, const GFXfont *font0, int size0){
  tft = &dsp;
  W = tft->width();
  H = tft->height();
  x = x0, y = y0;
  font = font0;
  size = size0;
  tft->setFont(font);
  tft->setTextSize(size);
}

void NumberIndicator::set(float value0){
  tft->setTextWrap(false);
  tft->setTextColor(ILI9341_WHITE);
  tft->setFont(font);
  tft->setTextSize(size);
  value = value0;

  int cursorX = x;
  int cursorY = y;

  //char value_str[] = "-00.0";
  char value_str[6];
  //if current value is the same as old- do nothing

  if(oldvalue != value){
    //clean old text by placing black rect over old coordinates
    tft->fillRect(o_txtX, o_txtY, o_txtW, o_txtH, ILI9341_BLACK);

    //convert float to text and get coordinates in the middle of screen
    dtostrf(value, 6, decimalPlace, value_str);
    char* valueWithUits = addUnits(value_str);
    tft->getTextBounds(valueWithUits, 0, 0, &txtX, &txtY, &txtW, &txtH);

    //If both coordinates set to 0 then place authomatically top/center
    if ((x == 0) & (y == 0)){
      cursorX = (W - txtW)/2;
      cursorY = txtH;
    }

    tft->setCursor(cursorX, cursorY);
    tft->print(valueWithUits);
    //get values of current placement into "old coordinates"
    tft->getTextBounds(valueWithUits, cursorX, cursorY, &o_txtX, &o_txtY, &o_txtW, &o_txtH);
  }
  oldvalue = value;
}

void NumberIndicator::setFormat(int decimalPlace0, const char *unitIndicator0, uint16_t color0){
  decimalPlace = decimalPlace0;
  color = color0;
  for (unsigned int i = 0; i < strlen(unitIndicator0); i++){
    unitIndicator[i] = unitIndicator0[i];
  }
}

char* NumberIndicator::addUnits(char* input)
{
    unsigned int i,j;

    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
        if (input[i]!=' ')
            output[j]=input[i];
        else
            j--;
    }

    output[j]=0;
    sprintf(outputWithUnits, "%s %s", output, unitIndicator);
    //Serial.println(unitIndicator);
    return outputWithUnits;
}

LevelIndicator::LevelIndicator(){
}

LevelIndicator::LevelIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, int W0, int H0, int maxvalue0){
  x = x0, y = y0, W = W0, H = H0;
  maxvalue = maxvalue0;
  tft = &dsp;
  barWidth = W/valueCount - barSpace;

  font = &FreeSansBold18pt7b;
  fontSize = 1;
  tft->setFont(font);
  tft->setTextSize(fontSize);

  int16_t  txtX, txtY;
  uint16_t txtW, txtH;
  //Probe font height for number indicator
  //Only height is important to align text with level indicator
  char sampletext[] = "0123456789";
  tft->getTextBounds(sampletext, 0, 0, &txtX, &txtY, &txtW, &txtH);
  fontHeight = txtH;

  int cursorX = x + valueCount*(barWidth + barSpace) + 10;
  int cursorY = H + y - fontHeight/2;
  //NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y, GFXfont *font, int size);
  level_value = NumberIndicator(dsp, cursorX, cursorY, font, fontSize);

  //level_value = NumberIndicator(dsp);

}

void LevelIndicator::set(float value){
  if(value > maxvalue){value = maxvalue;}

  double interval = (double)maxvalue/(double)valueCount;
  int level = (int)(value/interval + 0.5);

  for (int i = 0; i < valueCount; i++){
    if (i < level){
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, color);
    } else {
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_BLACK);
        tft->drawRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_WHITE);
    }
  }

  level_value.set(value);
}

void LevelIndicator::setAvg(float value){

  if(value > maxvalue){value = maxvalue;}

  int size = sizeof(measureList)/sizeof(float);

  for (int i = size - 1 ; i > 0 ; i--){
    measureList[i] = measureList[i - 1];
  }

  measureList[0] = value;
  float measureSum = 0;

  for (int i = 0; i < size ; i++){
    measureSum = measureSum + measureList[i];
  }

  float average = measureSum/(float)size;

  for (int i = 0; i < size; i++){
    Serial.print(measureList[i]);
    Serial.print(", ");
  }
  Serial.print("AVG: ");
  Serial.print(average);
  Serial.println("");

  value = average;

  double interval = (double)maxvalue/(double)valueCount;
  int level = (int)(value/interval + 0.5);

  for (int i = 0; i < valueCount; i++){
    if (i < level){
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, color);
    } else {
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_BLACK);
        tft->drawRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_WHITE);
    }
  }

  level_value.set(value);
}

void LevelIndicator::setFormat(int decimalPlace0, const char *unitIndicator0, uint16_t color0){
    decimalPlace = decimalPlace0;
    color = color0;
    //unitIndicator = unitIndicator0;
    for (int i = 0; i < strlen(unitIndicator0); i++){
      unitIndicator[i] = unitIndicator0[i];
    }
    level_value.setFormat(decimalPlace, unitIndicator, color);
}
