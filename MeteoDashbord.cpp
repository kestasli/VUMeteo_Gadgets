#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <MeteoDashboard.h>
#include <Fonts/FreeSansBold24pt7b.h>
//#include <Fonts/FreeSansBold56pt7b.h>
//#include <Fonts/FreeSansBold44pt7b.h>

Temp::Temp(){
}

Temp::Temp(Adafruit_ILI9341 &dsp){
  tft = &dsp;
  W = tft->width();
  H = tft->height();
}

void Temp::set(float value){
  tft->setTextWrap(false);
  tft->setTextColor(ILI9341_WHITE);
  tft->setFont(&FreeSansBold24pt7b);
  //tft->setFont(&FreeSansBold56pt7b);
  //tft->setFont(&FreeSansBold44pt7b);
  tft->setTextSize(2);

  int cursorX;
  int cursorY;
  int16_t  txtX, txtY;
  uint16_t txtW, txtH;
  //char value_str[] = "-00.0";
  char value_str[] = "-00.0";

  //clean old text by placing black rect over old coordinates
  tft->fillRect(o_txtX, o_txtY, o_txtW, o_txtH, ILI9341_BLACK);
  //tft->drawRect(o_txtX, o_txtY, o_txtW, o_txtH, ILI9341_GREEN);

  //convert float to text and get coordinates in the middle of screen
  dtostrf(value, 5, 1, value_str);
  char* value_nospc = deblank(value_str);
  tft->getTextBounds(value_nospc, 0, 0, &txtX, &txtY, &txtW, &txtH);
  cursorX = (W - txtW)/2;
  cursorY = txtH + 30;

  tft->setCursor(cursorX, cursorY);
  tft->print(value_nospc);
  //get values of current placement into "old coordinates"
  tft->getTextBounds(value_nospc, cursorX, cursorY, &o_txtX, &o_txtY, &o_txtW, &o_txtH);

}

char* Temp::deblank(char* input)
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
        if (input[i]!=' ')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}


void Temp::set1(float value){
  tft->setTextWrap(false);
  tft->setTextSize(2);

  int cursorX;
  int cursorY;
  int16_t  txtX, txtY;
  uint16_t txtW, txtH;
  char value_str[] = "-00.0";

  cursorX = 10;
  cursorY = 120;

  if (oldvalue != 99.9) {
    tft->setTextColor(ILI9341_BLACK);
    tft->setFont(&FreeSansBold24pt7b);
    //tft->setFont(&FreeSansBold56pt7b);
    //tft->setFont(&FreeSansBold44pt7b);
    tft->setCursor(cursorX, cursorY);
    tft->print(oldvalue, 1);
  }

  tft->setTextColor(ILI9341_WHITE);
  tft->setFont(&FreeSansBold24pt7b);
  //tft->setFont(&FreeSansBold56pt7b);
  //tft->setFont(&FreeSansBold44pt7b);
  tft->setCursor(cursorX, cursorY);
  tft->print(value, 1);
  //get values of current placement into "old coordinates"
  oldvalue = value;
}


Bar::Bar(){
}

Bar::Bar(Adafruit_ILI9341 &dsp, int x0, int y0){
  x = x0;
  y = y0;
  tft = &dsp;
  tft->fillRect(x, y + barMaxHeight + 1, valueCount * (barWidth + barSpace) - barSpace, textRectHeight, ILI9341_NAVY);
}

void Bar::setValue(float windspeed){
  tft->fillRect(x, y, (barWidth + barSpace) * valueCount, barMaxHeight + textRectHeight + 1, ILI9341_BLACK);
  tft->fillRect(x, y + barMaxHeight + 1, valueCount * (barWidth + barSpace) - barSpace, textRectHeight, ILI9341_NAVY);
  tft->setFont();
  tft->setTextSize(2);
  tft->setTextColor(ILI9341_WHITE);
  //pataisyti, kad Y koordinate butu isskaiciuojama automatiskai
  tft->setCursor(x + 2, y + barMaxHeight + 3);
  tft->print(windspeed, 1);
  tft->print("m/s");

  int barNumber = (int)(windspeed + 0.5);

  if (barNumber >= valueCount) {
    barNumber = valueCount;
  }

  for (int i = 0 ; i < barNumber ; i++) {
    tft->fillRect( x + i * (barWidth + barSpace), y, barWidth, barMaxHeight , ILI9341_LIGHTGREY);
  }
}

LevelIndicator::LevelIndicator(){
}

LevelIndicator::LevelIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, int W0, int H0, int maxvalue){
  x = x0, y = y0, W = W0, H = H0;
  tft = &dsp;
  barWidth = W/valueCount - barSpace;
  //barWidth = 10;
  //barSpace = 2;

  for (int i = 0; i < valueCount; i++){
    tft->drawRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_WHITE);
  }
  //tft->fillRect(x, y + barMaxHeight + 1, valueCount * (barWidth + barSpace) - barSpace, textRectHeight, ILI9341_NAVY);
}

void LevelIndicator::set(float value){
  if(value > maxvalue){value = maxvalue;}

  int interval = maxvalue/valueCount;
  int level = (int)(value/interval + 0.5);

  for (int i = 0; i < valueCount; i++){
    if (i < level){
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_GREENYELLOW);
    } else {
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_BLACK);
        tft->drawRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_WHITE);
    }
  }

  tft->setFont();
  tft->setTextSize(3);
  tft->setTextColor(ILI9341_WHITE);
  tft->setCursor(x + 10 + valueCount*(barWidth + barSpace), y + H/2);
  tft->print(value, 1);
  tft->print("m/s");

}
