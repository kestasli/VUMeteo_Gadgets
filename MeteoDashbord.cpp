#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <MeteoDashboard.h>
//#include <Fonts/FreeSansBold18pt7b.h>
#include <math.h>
//#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
//#include <Fonts/FreeSansBold56pt7b.h>
//#include <Fonts/FreeSansBold44pt7b.h>

NumberIndicator::NumberIndicator(){
}

NumberIndicator::NumberIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, const GFXfont *font0, int alignment0){
  tft = &dsp;
  W = tft->width();
  H = tft->height();
  x = x0, y = y0;
  font = font0;
  alignment = alignment0;
  tft->setFont(font);
  tft->setTextSize(fontSize);
}

void NumberIndicator::set(float value0, uint16_t color0){
  tft->setTextWrap(false);
  tft->setTextColor(color0);
  tft->setFont(font);
  tft->setTextSize(fontSize);
  value = value0;

  int cursorX = x;
  int cursorY = y;

  //char value_str[] = "-00.0";
  char value_str[6];

  //if set to TRUE takes range of values and average
  if(averaging){
    value = getAverage(value);
  }

  //if current value is the same as old- do nothing
  if(oldvalue != value){
    //clean old text by placing black rect over old coordinates
    tft->fillRect(o_txtX, o_txtY, o_txtW, o_txtH, ILI9341_BLACK);

    //convert float to text and get coordinates in the middle of screen
    dtostrf(value, 6, decimalPlace, value_str);
    char* valueWithUits = addUnits(value_str);
    tft->getTextBounds(valueWithUits, 0, 0, &txtX, &txtY, &txtW, &txtH);

    //If both coordinates set to 0 then place authomatically top/center
    if (alignment == ALIGN_TOPC){
      cursorX = (W - txtW)/2;
      cursorY = txtH;
    }

    if (alignment == ALIGN_STD){
      cursorX = x, cursorY = y;
    }

    if (alignment == ALIGN_VH){
      cursorX = x - txtW/2;
      cursorY = y + txtH/2;
    }


    tft->setCursor(cursorX, cursorY);
    tft->print(valueWithUits);
    //get values of current placement into "old coordinates"
    tft->getTextBounds(valueWithUits, cursorX, cursorY, &o_txtX, &o_txtY, &o_txtW, &o_txtH);
  }
  oldvalue = value;
}

void NumberIndicator::setFormat(int decimalPlace0, const char *unitIndicator0){
  decimalPlace = decimalPlace0;
  for (unsigned int i = 0; i < strlen(unitIndicator0); i++){
    unitIndicator[i] = unitIndicator0[i];
  }
}

void NumberIndicator::setFormat(int decimalPlace0, const char *unitIndicator0, bool averaging0){
  decimalPlace = decimalPlace0;
  averaging = averaging0;
  averaging = averaging0;
  for (unsigned int i = 0; i < strlen(unitIndicator0); i++){
    unitIndicator[i] = unitIndicator0[i];
  }
}


float NumberIndicator::getAverage(float value){
  int size = sizeof(measureList)/sizeof(float);

  measureNumber++;
  if (measureNumber > size){
    measureNumber = size;
  }

  for (int i = size - 1 ; i > 0 ; i--){
    measureList[i] = measureList[i - 1];
  }

  measureList[0] = value;
  float measureSum = 0;

  for (int i = 0; i < measureNumber ; i++){
    measureSum = measureSum + measureList[i];
  }

  float average = measureSum/(float)measureNumber;

  //it is needed to remove unsignificant decimals
  //as indicator refreshes only if prev value differs from current
  //this can cause indicator refresh every time as two floats will never be the same

  average = ((int)(average * 10 ))/ 10.0;

  for (int i = 0; i < size; i++){
    Serial.print(measureList[i]);
    Serial.print(", ");
  }

  Serial.print("AVG: ");
  Serial.print(average);
  Serial.print(", ");
  Serial.print(measureNumber);
  Serial.println("");

  return average;
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
    sprintf(outputWithUnits, "%s%s", output, unitIndicator);
    //Serial.println(unitIndicator);
    return outputWithUnits;
}

//this stupid function is needed if set to average over values
float NumberIndicator::getCurrentValue(){
  return value;
}

LevelIndicator::LevelIndicator(){
}

LevelIndicator::LevelIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, int W0, int H0, int maxvalue0){
  x = x0, y = y0, W = W0, H = H0;
  maxvalue = maxvalue0;
  //tft = &dsp;
  tft = &dsp;
  barWidth = W/valueCount - barSpace;

  font = &FreeSansBold24pt7b;
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
  level_value = NumberIndicator(*tft, cursorX, cursorY, font, fontSize);

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

  level_value.set(value, ILI9341_WHITE);
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

  level_value.set(value, ILI9341_WHITE);
}

void LevelIndicator::setFormat(int decimalPlace0, const char *unitIndicator0, uint16_t color0){
    decimalPlace = decimalPlace0;
    color = color0;
    //copy values from parameter to class local variables
    for (int i = 0; i < strlen(unitIndicator0); i++){
      unitIndicator[i] = unitIndicator0[i];
    }
    level_value.setFormat(decimalPlace, unitIndicator);
}

DirectionIndicator::DirectionIndicator(){
}

DirectionIndicator::DirectionIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, int r0, uint16_t color0){
  x = x0, y = y0, radius = r0, color = color0;
  //tft = &dsp;
  tft = &dsp;
  int W = tft->width();
  int H = tft->height();
  font = &FreeSansBold24pt7b;

  if (x == 0){x = W/2;}
  if (y == 0){y = H/2;}

  //tft->drawCircle(x, y, radius, color);
  //tft->drawCircle(x, y, radius + 1, color);
  indicator = NumberIndicator(*tft, x, y, font, ALIGN_VH);
  indicator.setFormat(1, "", true);
}

void DirectionIndicator::set(float value0, int direction0){
  //calculate new marker position
  int markerLenght = radius * 1.2;
  int markerExt = radius * 1.1;
  int markerInt = radius * 0.7;
  //marker widt, degrees
  int markerWidth = 8;
  value = value0;

  int direction = getAngleAverage(direction0);

  //mumbo jumbo to avoid converting coordinate system
  direction = -direction + 180;
  //calc triangle coordinates

  int markerX = x + markerInt * sin(direction * radianCoef);
  int markerY = y + markerInt * cos(direction * radianCoef);
  int markerX1 = x + markerExt * sin((direction + markerWidth) * radianCoef);
  int markerY1 = y + markerExt * cos((direction + markerWidth) * radianCoef);
  int markerX2 = x + markerExt * sin((direction - markerWidth) * radianCoef);
  int markerY2 = y + markerExt * cos((direction - markerWidth) * radianCoef);

  //remove old marker
  tft->fillTriangle(old_markerX1, old_markerY1, old_markerX2, old_markerY2, old_markerX, old_markerY, ILI9341_BLACK);
  tft->drawCircle(x, y, radius, color);
  tft->drawCircle(x, y, radius - 1, color);
  //tft->fillTriangle(markerX1, markerY1, markerX2, markerY2, markerX, markerY, ILI9341_RED);
  tft->fillTriangle(markerX1, markerY1, markerX2, markerY2, markerX, markerY, ILI9341_ORANGE);

  //indicator.set(value, ILI9341_LIGHTBLUE);
  indicator.set(value, ILI9341_ORANGE);

  old_markerX = markerX;
  old_markerY = markerY;
  old_markerX1 = markerX1;
  old_markerY1 = markerY1;
  old_markerX2 = markerX2;
  old_markerY2 = markerY2;
}

int DirectionIndicator::getAngleAverage(int value){

    int size = sizeof(measureList)/sizeof(int);

    measureNumber++;
    if (measureNumber > size){
      measureNumber = size;
    }

    //move all existing values one position towards the end
    for (int i = size - 1 ; i > 0 ; i--){
      measureList[i] = measureList[i - 1];
    }

    //add new value in the beginning of array
    measureList[0] = value;

    float s = 0;
    float c = 0;
    float angle_rad;

    for (int i = 0; i < measureNumber ; i++){
      angle_rad = (M_PI * measureList[i])/180;
      s = s + sin(angle_rad);
      c = c + cos(angle_rad);
    }

    s = s/measureNumber;
    c = c/measureNumber;

    float average_rad = atan2(s, c);
    int average_deg = (average_rad * 180)/M_PI;
    if (average_deg < 0){
      average_deg = average_deg + 360;
    }

    for (int i = 0; i < size; i++){
      Serial.print(measureList[i]);
      Serial.print(", ");
    }

    Serial.print("AVG: ");
    Serial.print(average_deg);
    Serial.print(", ");
    Serial.print(measureNumber);
    Serial.println("");
    return average_deg;
}

int DirectionIndicator::convertWindDirection(char* direction){
  const char S[] = "Piet\\u0173";
  const char N[] = "\\u0160iaur\\u0117s";
  const char E[] = "Ryt\\u0173";
  const char W[] = "Vakar\\u0173";

  const char NE[] = "\\u0160iaur\\u0117s ryt\\u0173";
  const char SE[] = "Pietry\\u010di\\u0173";
  const char SW[] = "Pietvakari\\u0173";
  const char NW[] = "\\u0160iaur\\u0117s vakar\\u0173";

  if(strcmp(direction, S) == 0) { return 180;}
  if(strcmp(direction, N) == 0) { return 0;}
  if(strcmp(direction, E) == 0) { return 90;}
  if(strcmp(direction, W) == 0) { return 270;}
  if(strcmp(direction, NE) == 0) { return 45;}
  if(strcmp(direction, SE) == 0) { return 135;}
  if(strcmp(direction, SW) == 0) { return 225;}
  if(strcmp(direction, NW) == 0) { return 315;}
  return 0;
}

/*
Pietry\u010di\u0173 'Pietryčių' 135
Vakar\u0173 'Vakarų' 270
\u0160iaur\u0117s 'Šiaurės' 0
Ryt\u0173 'Rytų' 90
\u0160iaur\u0117s ryt\u0173 'Šiaurės rytų' 45
Pietvakari\u0173 'Pietvakarių' 225
\u0160iaur\u0117s vakar\u0173 'Šiaurės vakarų' 315
Piet\u0173 'Pietų' 180
*/
