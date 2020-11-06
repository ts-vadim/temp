#pragma once

#include "ConsoleDefines.h"
#include "ConsoleAnimationClip.h"

/*

#####> Layers

0 - background
1 - ~render
2 - UI
3 - debug render


*/

class ConsoleGraphics
{
public:
    const char transparentChar = 1;
    const char defaultChar = ' ';
    const short& width = screen_size.X;
    const short& height = screen_size.Y;

    static COORD GetConsoleViewSize();

    ConsoleGraphics(COORD _size);
    ~ConsoleGraphics();

    void draw();

    void clearAll();
    void clear(int layer_index);

    void chr(int x, int y, char c, WORD attribs = ConsoleColor::White);
    void line(float x1, float y1, float x2, float y2, char c, WORD attribs = ConsoleColor::White);
    void rect(int x, int y, int px, int py, char fill_c, WORD fill_attrib = ConsoleColor::White);
    void text(int x, int y, WORD attrib, char* text, ...);
    void charmap(int x, int y, int w, int h, char* array, WORD attrib = ConsoleColor::White);
    void charmap(int x, int y, int w, int h, CHAR_INFO* array);
    void animFrame(int x, int y, ConsoleAnimationClip& anim);

    void setCursor(int x, int y);

    void chooseLayer(int index);
    int getLayersCount();

private:
    HANDLE hconsole;
    CHAR_INFO* screen;
    int current_layer = 0;
    CHAR_INFO** layers;
    const int layers_count = 4;
    COORD screen_size;
    const char finalTransparentChar = ' ';

    static const int format_text_buffer_length = 256;
    char ftext_buffer[format_text_buffer_length];

    void fillChar(CHAR_INFO* chr_pointer, char chr, WORD attrib);
    bool onScreen(int x, int y);
};

