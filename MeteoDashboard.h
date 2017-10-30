class NumberIndicator{
public:
  NumberIndicator();
  NumberIndicator(Adafruit_ILI9341 &dsp);
  NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y);
  //Temp(Adafruit_ILI9341 &dsp, int x, int y, GFXfont *font);
  NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y, GFXfont *font, int size);
  int getTextWidth();
  int getTextHeight();
  void set(float value);
private:
  char* deblank(char* origstring);
  Adafruit_ILI9341 *tft;
  GFXfont *font;
  int W, H, x, y; //display width and height
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
private:
  Adafruit_ILI9341 *tft;
  GFXfont *font;
  NumberIndicator level_value;
  int fontHeight, fontSize;
  int W, H, x, y;
  int maxvalue = 10;
  int barWidth = 10, barSpace = 2;
  int valueCount = 10, textRectHeight = 18;
  int averageover = 1; //average over some measures
  int measureslist[20];
};
