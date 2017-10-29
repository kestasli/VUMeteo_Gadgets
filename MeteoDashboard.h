class Bar{
public:
  Bar();
  Bar(Adafruit_ILI9341 &dsp, int x, int y);
  void setValue(float value);
  void setTest(int value);
private:
  Adafruit_ILI9341 *tft;
  int W;
  int H;
  int x;
  int y;
  int barWidth = 10, barSpace = 2, barMaxHeight = 42;
  int valueCount = 8, textRectHeight = 18;
  int averageover = 1; //average over some measures
  int measureslist[20];
};

class Temp{
public:
  Temp();
  Temp(Adafruit_ILI9341 &dsp);
  void set(float value);
  void set1(float value);
private:
  char* deblank(char* origstring);
  Adafruit_ILI9341 *tft;
  int W, H; //display width and height
  float oldvalue = 99.9, value = 0;
  int16_t  o_txtX = 0, o_txtY = 0; //keep old coordinates of text boundary
  uint16_t o_txtW = 0, o_txtH = 0; //keep old koordinates of text boundary
};
