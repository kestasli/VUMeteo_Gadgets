#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <MeteoDashboard.h>
#include <Fonts/FreeSansBold12pt7b.h>
//#include <Fonts/FreeSansBold24pt7b.h>
//#include <Fonts/FreeSansBold56pt7b.h>
//#include <Fonts/FreeSansBold44pt7b.h>

NumberIndicator::NumberIndicator(){
}

NumberIndicator::NumberIndicator(Adafruit_ILI9341 &dsp){
  tft = &dsp;
}

NumberIndicator::NumberIndicator(Adafruit_ILI9341 &dsp, int x0, int y0){
  tft = &dsp;
  W = tft->width();
  H = tft->height();
  x = x0, y = y0;
  //tft->setFont(&FreeSansBold24pt7b);
  tft->setTextSize(2);
}

NumberIndicator::NumberIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, GFXfont *font0, int size0){
  tft = &dsp;
  W = tft->width();
  H = tft->height();
  x = x0, y = y0;
  font = font0;
  size = size0;
  tft->setFont(font);
  tft->setTextSize(size);
}

void NumberIndicator::set(float value){
  tft->setTextWrap(false);
  tft->setTextColor(ILI9341_WHITE);
  tft->setFont(font);
  tft->setTextSize(size);
  //tft->setFont(&FreeSansBold56pt7b);
  //tft->setFont(&FreeSansBold44pt7b);
  //tft->setTextSize(2);

  int cursorX = x;
  int cursorY = y;

  //char value_str[] = "-00.0";
  char value_str[] = "-00.0";

  //clean old text by placing black rect over old coordinates
  tft->fillRect(o_txtX, o_txtY, o_txtW, o_txtH, ILI9341_BLACK);
  //tft->drawRect(o_txtX, o_txtY, o_txtW, o_txtH, ILI9341_GREEN);

  //convert float to text and get coordinates in the middle of screen
  dtostrf(value, 5, 1, value_str);
  char* value_nospc = deblank(value_str);
  tft->getTextBounds(value_nospc, 0, 0, &txtX, &txtY, &txtW, &txtH);
  //tft->drawCircle(x + txtW, y, 8, ILI9341_WHITE);
  //cursorX = (W - txtW)/2;
  //cursorY = txtH + 30;

  tft->setCursor(cursorX, cursorY);
  tft->print(value_nospc);
  //get values of current placement into "old coordinates"
  tft->getTextBounds(value_nospc, cursorX, cursorY, &o_txtX, &o_txtY, &o_txtW, &o_txtH);

}

char* NumberIndicator::deblank(char* input)
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

LevelIndicator::LevelIndicator(){
}

LevelIndicator::LevelIndicator(Adafruit_ILI9341 &dsp, int x0, int y0, int W0, int H0, int maxvalue){
  x = x0, y = y0, W = W0, H = H0;
  tft = &dsp;
  barWidth = W/valueCount - barSpace;
  //barWidth = 10;
  //barSpace = 2;

  font = &FreeSansBold12pt7b;
  fontSize = 1;
  tft->setFont(font);
  tft->setTextSize(fontSize);

  int16_t  txtX, txtY;
  uint16_t txtW, txtH;
  //Probe font height for number indicator
  //Only height is important to align text with level indicator
  tft->getTextBounds("0123456789", 0, 0, &txtX, &txtY, &txtW, &txtH);
  fontHeight = txtH;

  int cursorX = x + valueCount*(barWidth + barSpace) + 10;
  int cursorY = H + y - fontHeight/2;
  //NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y, GFXfont *font, int size);
  level_value = NumberIndicator(dsp, cursorX, cursorY, font, fontSize);
  //level_value = NumberIndicator(dsp);

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
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_GREEN);
    } else {
        tft->fillRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_BLACK);
        tft->drawRect(x + i*(barWidth + barSpace), y, barWidth, H, ILI9341_WHITE);
    }
  }
  level_value.set(value);
}
