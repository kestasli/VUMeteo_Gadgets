class NumberIndicator{
public:
  NumberIndicator();
  NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y);
  //Temp(Adafruit_ILI9341 &dsp, int x, int y, GFXfont *font);
  NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y, const GFXfont *font, int size);
  void set(float value);
  void setFormat(int comma, char *unit, uint16_t color);
private:
  char* deblank(char* origstring);
  char* addUnits(char* origstring);
  Adafruit_ILI9341 *tft;
  const GFXfont *font;
  int W, H, x, y; //display width and height
  char unitIndicator[5] = {0};
  //char *outputWithUnits;
  char outputWithUnits[10] = {0};
  uint16_t color;
  int decimalPlace = 1;
  int size; //font size
  float oldvalue = 99.9, value = 0;
  int16_t  txtX, txtY; //current coordinates of text boudary
  uint16_t txtW, txtH; //current coordinates of text boudary
  int16_t  o_txtX = 0, o_txtY = 0; //keep old coordinates of text boundary
  uint16_t o_txtW = 0, o_txtH = 0; //keep old koordinates of text boundary
};

class LevelIndicator{
public:
  LevelIndicator();
  LevelIndicator(Adafruit_ILI9341 &dsp, int x, int y, int w, int h, int maxvalue);
  void set(float value);
  void setFormat(int comma, char *unit, uint16_t color);
private:
  Adafruit_ILI9341 *tft;
  const GFXfont *font;
  NumberIndicator level_value;
  int fontHeight, fontSize;
  int W, H, x, y;
  uint16_t color = ILI9341_WHITE;
  char *unitIndicator;
  int decimalPlace = 0;
  int maxvalue = 10;
  int barWidth = 0, barSpace = 4;
  int valueCount = 10;
  int averageover = 1; //average over some measures
  int measureslist[20];
};
