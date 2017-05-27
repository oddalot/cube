#include "Arduino.h"
#include "Cube.h"
#include <SPI.h>

byte Cube::latch_bit;
byte Cube::blank_bit;
byte Cube::anode_level = 0;
byte Cube::last_anode;
byte Cube::shift_out;
int Cube::BAM_Bit = 0;
int Cube::BAM_Counter = 0;
byte Row::bit_counter;
Level Cube::levels[8];

void Cube::setupBoard() {
  Cube::latch_bit = 4;
  Cube::blank_bit = 5;
}

void Cube::setLatchPinHigh() {
  PORTE |= 1 << Cube::latch_bit;
}

void Cube::setLatchPinLow() {
  PORTE &= ~(1 << Cube::latch_bit);
}

void Cube::setBlankPinHigh() {
  PORTE |= 1 << Cube::blank_bit;
}

void Cube::setBlankPinLow() {
  PORTE &= ~(1 << Cube::blank_bit);
}

void Cube::setupTimerInterrupt() {
  TCCR1A = B00000000;
  TCCR1B = B00001011;
  TIMSK1 = B00000010;
  OCR1A=40;
}

void Cube::setupOutputs() {
  DDRE |= B00111000;
  DDRG |= B00100011;
  DDRH |= B01111000;
  DDRB |= B00110000;
}

void Cube::incrementLevel() {
  Cube::anode_level++;
  if (Cube::anode_level == 8) {
    Cube::anode_level = 0;
  }
}

void Cube::selectLayer() {
  switch(anode_level) {
    case 0:
      PORTG &= B11011111;
      PORTB |= B00100000;
      break;
    case 1:
      PORTE &= B11110111;
      PORTG |= B00100000;
      break;
    case 2:
      PORTH &= B11110111;
      PORTE |= B00001000;
      break;
    case 3:
      PORTH &= B11101111;
      PORTH |= B00001000;
      break;
    case 4:
      PORTH &= B11011111;
      PORTH |= B00010000;
      break;
    case 5:
      PORTH &= B10111111;
      PORTH |= B00100000;
      break;
    case 6:
      PORTB &= B11101111;
      PORTH |= B01000000;
      break;
    case 7:
      PORTB &= B11011111;
      PORTB |= B00010000;
      break;
  }
}

void Cube::LED(char level, char row, char column, int red, int green, int blue) {
  level = constrain(level, 0, 7);
  row = constrain(row, 0, 7);
  column = constrain(column, 0, 7);
  red = constrain(red, 0, 15);
  green = constrain(green, 0, 15);
  blue = constrain(blue, 0, 15);

  Cube::getLevel(level).getRow(row).setLedBrightness(column, red, green, blue);
}

void Cube::shiftOutData() {
  switch(Cube::BAM_Counter) {
    case 8:
    case 24:
    case 56:
      Cube::BAM_Bit++;
      break;
    case 120:
      Cube::BAM_Counter=0;
      Cube::BAM_Bit=0;
      break;
  }
  Cube::BAM_Counter++;

  for (Cube::shift_out=0; Cube::shift_out<8; Cube::shift_out++)
    SPI.transfer(Cube::getLevel(Cube::anode_level).getRow(Cube::shift_out).shiftOutRedRow(Cube::BAM_Bit));
  for (Cube::shift_out=0; Cube::shift_out<8; Cube::shift_out++)
    SPI.transfer(Cube::getLevel(Cube::anode_level).getRow(Cube::shift_out).shiftOutGreenRow(Cube::BAM_Bit));
  for (Cube::shift_out=0; Cube::shift_out<8; Cube::shift_out++)
    SPI.transfer(Cube::getLevel(Cube::anode_level).getRow(Cube::shift_out).shiftOutBlueRow(Cube::BAM_Bit));
}

void Row::setLedBrightness(int column, int red, int green, int blue) {
  for (Row::bit_counter = 0; Row::bit_counter < 4; Row::bit_counter++) {
    bitWrite(this -> red[Row::bit_counter], column, bitRead(red, Row::bit_counter));
    bitWrite(this -> green[Row::bit_counter], column, bitRead(green, Row::bit_counter));
    bitWrite(this -> blue[Row::bit_counter], column, bitRead(blue, Row::bit_counter));
  }
}

byte Row::shiftOutRedRow(byte BAM_Bit) {
  return red[BAM_Bit];
}

byte Row::shiftOutGreenRow(byte BAM_Bit) {
  return green[BAM_Bit];
}

byte Row::shiftOutBlueRow(byte BAM_Bit) {
  return blue[BAM_Bit];
}

Row &Level::getRow(byte row) {
  return rows[row];
}

Level &Cube::getLevel(byte level) {
  return Cube::levels[level];
}
