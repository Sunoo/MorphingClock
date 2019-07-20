#include "Digit.h"

const char sA = 0;
const char sB = 1;
const char sC = 2;
const char sD = 3;
const char sE = 4;
const char sF = 5;
const char sG = 6;
/*const int segHeight = 5;
const int segWidth = segHeight;*/

char digitBits[] = {
  0b11111100, // 0 ABCDEF--
  0b01100000, // 1 -BC-----
  0b11011010, // 2 AB-DE-G-
  0b11110010, // 3 ABCD--G-
  0b01100110, // 4 -BC--FG-
  0b10110110, // 5 A-CD-FG-
  0b10111110, // 6 A-CDEFG-
  0b11100000, // 7 ABC-----
  0b11111110, // 8 ABCDEFG-
  0b11110110, // 9 ABCD_FG-
};

Color black = Color(0, 0, 0);

Digit::Digit(Canvas &d, char value, uint16_t xo, uint16_t yo, uint16_t segLength, Color color) {
  _display = &d;
  _value = value;
  xOffset = xo;
  yOffset = yo;
  segHeight = segLength;
  segWidth = segLength;
  _color = color;
}

void Digit::SetColor(Color color)
{
  _color = color;
}

char Digit::Value()
{
  return _value;
}

void Digit::drawPixelFlip(uint16_t x, uint16_t y, Color c)
{
  drawPixel(x, (2 * segHeight + 3) - 1 - y, c);
}

void Digit::drawPixel(uint16_t x, uint16_t y, Color c)
{
  _display->SetPixel(xOffset + x, y + yOffset, c.r, c.g, c.b);
}

void Digit::drawLineFlip(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, Color c)
{
  drawLine(x, (2 * segHeight + 3) - 1 - y, x2, (2 * segHeight + 3) - 1 - y2, c);
}

void Digit::drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, Color c)
{
  DrawLine(_display, xOffset + x, y + yOffset, xOffset + x2, y2 + yOffset, c);
}

void Digit::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color)
{
  for (int16_t x_pos = x; x_pos < x + w; x_pos++)
  {
    for (int16_t y_pos = y; y_pos < y + h; y_pos++)
    {
      _display->SetPixel(x_pos, y_pos, color.r, color.g, color.b);
    }
  }
}

void Digit::drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color c)
{
  fillRect(xOffset + x, y + yOffset, w, h, c);
}

void Digit::DrawColon(Color c)
{
  // Colon is drawn to the left of this digit
  drawFillRect(-3, segHeight+1-3, 2, 2, c);
  drawFillRect(-3, segHeight+3, 2, 2, c);
}

void Digit::drawSeg(char seg)
{
  switch (seg) {
    case sA: drawLine(1, 0, segWidth, 0, _color); break;
    case sB: drawLine(segWidth + 1, 1, segWidth + 1, segHeight, _color); break;
    case sC: drawLine(segWidth + 1, segHeight * 2 + 1, segWidth + 1, segHeight + 2, _color); break;
    case sD: drawLine(1, segHeight * 2 + 2, segWidth, segHeight * 2 + 2, _color); break;
    case sE: drawLine(0, segHeight * 2 + 1, 0, segHeight + 2, _color); break;
    case sF: drawLine(0, 1, 0, segHeight, _color); break;
    case sG: drawLine(1, segHeight + 1, segWidth, segHeight + 1, _color); break;
  }
}

bool Digit::bitRead(char b, int bitPos)
{
  int x = b & (1 << bitPos);
  return x == 0 ? false : true;
}

void Digit::Draw(char value) {
  char pattern = digitBits[value];
  if (bitRead(pattern, 7)) drawSeg(sA);
  if (bitRead(pattern, 6)) drawSeg(sB);
  if (bitRead(pattern, 5)) drawSeg(sC);
  if (bitRead(pattern, 4)) drawSeg(sD);
  if (bitRead(pattern, 3)) drawSeg(sE);
  if (bitRead(pattern, 2)) drawSeg(sF);
  if (bitRead(pattern, 1)) drawSeg(sG);
  _value = value;
}

void Digit::Morph2() {
  // TWO
  for (int i = 0; i <= segWidth; i++)
  {
    if (i < segWidth) {
      drawPixelFlip(segWidth - i, segHeight * 2 + 2, _color);
      drawPixelFlip(segWidth - i, segHeight + 1, _color);
      drawPixelFlip(segWidth - i, 0, _color);
    }

    drawLineFlip(segWidth + 1 - i, 1, segWidth + 1 - i, segHeight, black);
    drawLineFlip(segWidth - i, 1, segWidth - i, segHeight, _color);
    usleep(animSpeed);
  }
}

void Digit::Morph3() {
  // THREE
  for (int i = 0; i <= segWidth; i++)
  {
    drawLineFlip(0 + i, 1, 0 + i, segHeight, black);
    drawLineFlip(1 + i, 1, 1 + i, segHeight, _color);
    usleep(animSpeed);
  }
}

void Digit::Morph4() {
  // FOUR
  for (int i = 0; i < segWidth; i++)
  {
    drawPixelFlip(segWidth - i, segHeight * 2 + 2, black); // Erase A
    drawPixelFlip(0, segHeight * 2 + 1 - i, _color); // Draw as F
    drawPixelFlip(1 + i, 0, black); // Erase D
    usleep(animSpeed);
  }
}

void Digit::Morph5() {
  // FIVE
  for (int i = 0; i < segWidth; i++)
  {
    drawPixelFlip(segWidth + 1, segHeight + 2 + i, black); // Erase B
    drawPixelFlip(segWidth - i, segHeight * 2 + 2, _color); // Draw as A
    drawPixelFlip(segWidth - i, 0, _color); // Draw D
    usleep(animSpeed);
  }
}

void Digit::Morph6() {
  // SIX
  for (int i = 0; i <= segWidth; i++)
  {
    // Move C right to left
    drawLineFlip(segWidth - i, 1, segWidth - i, segHeight, _color);
    if (i > 0) drawLineFlip(segWidth - i + 1, 1, segWidth - i + 1, segHeight, black);
    usleep(animSpeed);
  }
}

void Digit::Morph7() {
  // SEVEN
  for (int i = 0; i <= (segWidth + 1); i++)
  {
    // Move E left to right
    drawLineFlip(0 + i - 1, 1, 0 + i - 1, segHeight, black);
    drawLineFlip(0 + i, 1, 0 + i, segHeight, _color);

    // Move F left to right
    drawLineFlip(0 + i - 1, segHeight * 2 + 1, 0 + i - 1, segHeight + 2, black);
    drawLineFlip(0 + i, segHeight * 2 + 1, 0 + i, segHeight + 2, _color);

    // Erase D and G gradually
    drawPixelFlip(1 + i, 0, black); // D
    drawPixelFlip(1 + i, segHeight + 1, black); // G
    usleep(animSpeed);
  }
}

void Digit::Morph8() {
  // EIGHT
  for (int i = 0; i <= segWidth; i++)
  {
    // Move B right to left
    drawLineFlip(segWidth - i, segHeight * 2 + 1, segWidth - i, segHeight + 2, _color);
    if (i > 0) drawLineFlip(segWidth - i + 1, segHeight * 2 + 1, segWidth - i + 1, segHeight + 2, black);

    // Move C right to left
    drawLineFlip(segWidth - i, 1, segWidth - i, segHeight, _color);
    if (i > 0) drawLineFlip(segWidth - i + 1, 1, segWidth - i + 1, segHeight, black);

    // Gradually draw D and G
    if (i < segWidth) {
      drawPixelFlip(segWidth - i, 0, _color); // D
      drawPixelFlip(segWidth - i, segHeight + 1, _color); // G
    }
    usleep(animSpeed);
  }
}

void Digit::Morph9() {
  // NINE
  for (int i = 0; i <= (segWidth + 1); i++)
  {
    // Move E left to right
    drawLineFlip(0 + i - 1, 1, 0 + i - 1, segHeight, black);
    drawLineFlip(0 + i, 1, 0 + i, segHeight, _color);
    usleep(animSpeed);
  }
}

void Digit::Morph0() {
  // ZERO
  for (int i = 0; i <= segWidth; i++)
  {
    if (_value==1) { // If 1 to 0, slide B to F and E to C  
      // slide B to F 
      drawLineFlip(segWidth - i, segHeight * 2+1 , segWidth - i, segHeight + 2, _color);
      if (i > 0) drawLineFlip(segWidth - i + 1, segHeight * 2+1, segWidth - i + 1, segHeight + 2, black);

      // slide E to C
      drawLineFlip(segWidth - i, 1, segWidth - i, segHeight, _color);
      if (i > 0) drawLineFlip(segWidth - i + 1, 1, segWidth - i + 1, segHeight, black);

      if (i<segWidth) drawPixelFlip(segWidth - i, segHeight * 2 + 2 , _color); // Draw A
      if (i<segWidth) drawPixelFlip(segWidth - i, 0, _color); // Draw D
    }
    
    if (_value==2) { // If 2 to 0, slide B to F and Flow G to C
      // slide B to F 
      drawLineFlip(segWidth - i, segHeight * 2+1 , segWidth - i, segHeight + 2, _color);
      if (i > 0) drawLineFlip(segWidth - i + 1, segHeight * 2+1, segWidth - i + 1, segHeight + 2, black);
    
      drawPixelFlip(1+i, segHeight + 1, black); // Erase G left to right
      if (i<segWidth) drawPixelFlip(segWidth + 1, segHeight + 1- i, _color);// Draw C
    }

    if (_value==3) { // B to F, C to E
      // slide B to F 
      drawLineFlip(segWidth - i, segHeight * 2+1 , segWidth - i, segHeight + 2, _color);
      if (i > 0) drawLineFlip(segWidth - i + 1, segHeight * 2+1, segWidth - i + 1, segHeight + 2, black);
      
      // Move C to E
      drawLineFlip(segWidth - i, 1, segWidth - i, segHeight, _color);
      if (i > 0) drawLineFlip(segWidth - i + 1, 1, segWidth - i + 1, segHeight, black);

      // Erase G from right to left
      drawPixelFlip(segWidth - i, segHeight + 1, black); // G
    }
    
    if (_value==5) { // If 5 to 0, we also need to slide F to B
      if (i<segWidth) {
        if (i>0) drawLineFlip(1 + i, segHeight * 2 + 1, 1 + i, segHeight + 2, black);
        drawLineFlip(2 + i, segHeight * 2 + 1, 2 + i, segHeight + 2, _color);
      }
    }
    
    if (_value==5 || _value==9) { // If 9 or 5 to 0, Flow G into E
      if (i<segWidth) drawPixelFlip(segWidth - i, segHeight + 1, black);
      if (i<segWidth) drawPixelFlip(0, segHeight - i, _color);
    }
    usleep(animSpeed);
  }
}

void Digit::Morph1() {
  // Zero or two to One
  for (int i = 0; i <= (segWidth + 1); i++)
  {
    // Move E left to right
    drawLineFlip(0 + i - 1, 1, 0 + i - 1, segHeight, black);
    drawLineFlip(0 + i, 1, 0 + i, segHeight, _color);

    // Move F left to right
    drawLineFlip(0 + i - 1, segHeight * 2 + 1, 0 + i - 1, segHeight + 2, black);
    drawLineFlip(0 + i, segHeight * 2 + 1, 0 + i, segHeight + 2, _color);

    // Gradually Erase A, G, D
    drawPixelFlip(1 + i, segHeight * 2 + 2, black); // A
    drawPixelFlip(1 + i, 0, black); // D
    drawPixelFlip(1 + i, segHeight + 1, black); // G

    usleep(animSpeed);
  }
}

void Digit::Morph(char newValue) {
  switch (newValue) {
    case 2: Morph2(); break;
    case 3: Morph3(); break;
    case 4: Morph4(); break;
    case 5: Morph5(); break;
    case 6: Morph6(); break;
    case 7: Morph7(); break;
    case 8: Morph8(); break;
    case 9: Morph9(); break;
    case 0: Morph0(); break;
    case 1: Morph1(); break;
  }
  _value = newValue;
}

