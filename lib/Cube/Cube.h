#ifndef Cube_h
#define Cube_h
#include <Arduino.h>

class Row {
  public:
    void setLedBrightness(int column, int red, int green, int blue);
    byte shiftOutRedRow(byte BAM_Bit);
    byte shiftOutGreenRow(byte BAM_Bit);
    byte shiftOutBlueRow(byte BAM_Bit);
  private:
    byte red[4];
    byte green[4];
    byte blue[4];
    static byte bit_counter;
};

class Level {
  public:
    Row &getRow(byte row);
  private:
    Row rows[8];
};

class Cube {
  public:
    static void setupBoard();
    static void setLatchPinHigh();
    static void setLatchPinLow();
    static void setBlankPinHigh();
    static void setBlankPinLow();
    static void setupTimerInterrupt();
    static void setupOutputs();
    static void incrementLevel();
    static void selectLayer();
    static void LED(char level, char row, char column, int red, int green, int blue);
    static void shiftOutData();
    static Level &getLevel(byte level);
  private:
    static byte latch_bit;
    static byte blank_bit;
    static byte anode_level;
    static byte last_anode;
    static byte shift_out;
    static int BAM_Bit;
    static int BAM_Counter;
    static Level levels[8];
};

extern Cube Cube;

#endif
