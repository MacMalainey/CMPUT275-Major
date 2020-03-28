#include "include/screen.h"

Screen::Screen() {
    uint16_t ID = this->readID();
    this->begin(ID);
    this->fillScreen(TFT_BLACK);
    this->setRotation(1);
    this->setTextSize(2);
}