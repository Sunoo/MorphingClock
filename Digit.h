#ifndef DIGIT_H
#define DIGIT_H

#include "led-matrix.h"
#include "graphics.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using namespace rgb_matrix;

class Digit {
  
  public:
    Digit(Canvas &d, char value, uint16_t xo, uint16_t yo, Color color);
    void Draw(char value);
    void Morph(char newValue);
    char Value();
    void DrawColon(Color c);
    
  private:
    Canvas *_display;
    char _value;
    Color _color;
    uint16_t xOffset;
    uint16_t yOffset;
    int animSpeed = 30000;
    
    bool bitRead(char b, int bitPos);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color);
    void drawPixel(uint16_t x, uint16_t y, Color c);
    void drawPixelFlip(uint16_t x, uint16_t y, Color c);
    void drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color c);
    void drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, Color c);
    void drawLineFlip(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, Color c);
    void drawSeg(char seg);
    void Morph2();
    void Morph3();
    void Morph4();
    void Morph5();
    void Morph6();
    void Morph7();
    void Morph8();
    void Morph9();
    void Morph0();
    void Morph1();
};

#endif
