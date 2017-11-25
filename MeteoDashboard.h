#define ALIGN_STD     0
#define ALIGN_H       1
#define ALIGN_V       2
#define ALIGN_VH      3
#define ALIGN_TOPC    4

class NumberIndicator{
public:
  NumberIndicator();
  //function takes pointer to display, x coord, y coord, font, alignment style
  NumberIndicator(Adafruit_ILI9341 &dsp, int x, int y, const GFXfont *font, int alignment);
  void set(float value, uint16_t color);
  void setFormat(int comma, const char *unit);
  void setFormat(int comma, const char *unit, bool averaging);
private:
  char* deblank(char* origstring);
  char* addUnits(char* origstring);
  float getAverage(float value);
  Adafruit_ILI9341 *tft;
  const GFXfont *font;
  int W, H; //display width and height
  int x, y; //coordinates for font placement
  char unitIndicator[5] = {0}; //this is array for measurement units
  char outputWithUnits[10] = {0}; //this holds combined array from value and unit

  float measureList[20] = {0}; //average over array
  int measureNumber = 0; //this holds measurement number for calc average

  uint16_t color = ILI9341_WHITE;
  int decimalPlace = 1;
  int alignment = ALIGN_STD; //defines how text should be aligned
  bool averaging = false; //defines how text should be aligned
  int fontSize = 1; //font size
  float oldvalue = 99.9;
  float value = 0.0;
  int16_t  txtX, txtY; //current coordinates of text boundary
  uint16_t txtW, txtH; //current text width and height
  int16_t  o_txtX = 0, o_txtY = 0; //keep old coordinates of text boundary
  uint16_t o_txtW = 0, o_txtH = 0; //keep old koordinates of text boundary
};

class LevelIndicator{
public:
  LevelIndicator();
  LevelIndicator(Adafruit_ILI9341 &dsp, int x, int y, int w, int h, int maxvalue);
  void set(float value);
  void setAvg(float value);
  void setFormat(int comma, const char *unit, uint16_t color);
private:
  Adafruit_ILI9341 *tft;
  const GFXfont *font;
  NumberIndicator level_value;
  int fontHeight, fontSize;
  int W, H, x, y;
  uint16_t color = ILI9341_WHITE;
  char unitIndicator[5] = {0};
  int decimalPlace = 0;
  int maxvalue = 10;
  int barWidth = 0, barSpace = 4;
  int valueCount = 10; //bar quantity in level indicator
  float measureList[15] = {0}; //average over array
  int measureNr = 0;
};

class DirectionIndicator{
public:
  DirectionIndicator();
  DirectionIndicator(Adafruit_ILI9341 &dsp, int x, int y, int radius, uint16_t color);
  void set(float value, int direction);
private:
    int getAngleAverage(int value);
    Adafruit_ILI9341 *tft;
    double radianCoef = 0.0174532925;
    NumberIndicator indicator;
    const GFXfont *font;
    int x, y, radius, direction, old_direction;

    int measureList[20] = {0}; //average over array
    int measureNumber = 0; //this holds measurement number for calc average

    int old_markerX, old_markerY;
    int old_markerX1, old_markerY1;
    int old_markerX2, old_markerY2;
    float value, old_value;
    uint16_t color;
};
