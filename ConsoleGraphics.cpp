#include "ConsoleGraphics.h"

ConsoleGraphics::ConsoleGraphics(COORD _size)
{
    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);

    screen_size = _size;

    screen = new CHAR_INFO[screen_size.X * screen_size.Y];
    for (int i = 0; i < screen_size.X * screen_size.Y; i++)
    {
        fillChar(&screen[i], defaultChar, ConsoleColor::White);
    }


    layers = new CHAR_INFO * [layers_count];

    layers[0] = new CHAR_INFO[screen_size.X * screen_size.Y];
    for (int j = 0; j < screen_size.X * screen_size.Y; j++)
    {
        fillChar(&layers[0][j], defaultChar, ConsoleColor::White);
    }

    for (int i = 1; i < layers_count; i++)
    {
        layers[i] = new CHAR_INFO[screen_size.X * screen_size.Y];
        for (int j = 0; j < screen_size.X * screen_size.Y; j++)
        {
            fillChar(&layers[i][j], transparentChar, ConsoleColor::White);
        }
    }
}


ConsoleGraphics::~ConsoleGraphics()
{
    delete[] screen;

    for (int j = 0; j < layers_count; j++)
        delete[] layers[j];
    delete[] layers;
}

COORD ConsoleGraphics::GetConsoleViewSize()
{
    CONSOLE_SCREEN_BUFFER_INFO sbInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sbInfo);
    int columns = sbInfo.srWindow.Right - sbInfo.srWindow.Left + 1;
    int rows = sbInfo.srWindow.Bottom - sbInfo.srWindow.Top + 1;
    return { columns, rows };
}


void ConsoleGraphics::draw()
{
    CHAR_INFO chr;
    for (int i = 0; i < screen_size.X * screen_size.Y; i++)
    {
        for (int layer_i = 0; layer_i < layers_count; layer_i++)
        {
            if (layers[layer_i][i].Char.AsciiChar != transparentChar)
            {
                chr = layers[layer_i][i];
            }
        }
        if (chr.Char.AsciiChar == transparentChar)
        {
            chr.Char.AsciiChar = finalTransparentChar;
        }
        screen[i] = chr;
    }
    SMALL_RECT screen_write_area = { 0, 0, screen_size.X, screen_size.Y };
    WriteConsoleOutputA(hconsole, screen, screen_size, { 0, 0 }, &screen_write_area);
}


void ConsoleGraphics::chr(int x, int y, char c, WORD attribs)
{
    if (onScreen(x, y))
        fillChar(&layers[current_layer][x + y * screen_size.X], c, attribs);
}


void ConsoleGraphics::clearAll()
{
    for (int i = 0; i < screen_size.X * screen_size.Y; i++)
    {
        fillChar(&screen[i], defaultChar, ConsoleColor::White);
        fillChar(&layers[0][i], defaultChar, ConsoleColor::White);

        for (int layer_i = 1; layer_i < layers_count; layer_i++)
        {
            fillChar(&layers[layer_i][i], transparentChar, ConsoleColor::White);
        }
    }
}


void ConsoleGraphics::clear(int layer_index)
{
    if (layer_index == 0)
    {
        for (int i = 0; i < screen_size.X * screen_size.Y; i++)
            fillChar(&layers[0][i], defaultChar, ConsoleColor::White);
    }
    else
    {
        for (int i = 0; i < screen_size.X * screen_size.Y; i++)
            fillChar(&layers[layer_index][i], transparentChar, ConsoleColor::White);
    }
}



void ConsoleGraphics::line(float x1, float y1, float x2, float y2, char c, WORD attribs)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = fabs(dx);
    dy1 = fabs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        chr(x, y, c, attribs);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
            {
                px = px + 2 * dy1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                }
                else
                {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            chr(x, y, c, attribs);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        chr(x, y, c, attribs);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
            {
                py = py + 2 * dx1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            chr(x, y, c, attribs);
        }
    }
}


void ConsoleGraphics::rect(int _x, int _y, int px, int py, char fill_c, WORD fill_attrib)
{
    for (int y = 0; y < py; y++)
        for (int x = 0; x < px; x++)
            if (onScreen(_x + x, _y + y))
                fillChar(&layers[current_layer][(_x + x) + (_y + y) * screen_size.X], fill_c, fill_attrib);
}


void ConsoleGraphics::text(int x, int y, WORD attrib, char* format, ...)
{
    va_list args;
    va_start(args, format);

    vsnprintf_s(ftext_buffer, format_text_buffer_length, format, args);

    if (attrib == 0)
        attrib = ConsoleColor::White;

    for (int i = 0; i < strlen(ftext_buffer); i++)
    {
        if (onScreen(x + i, y))
            fillChar(&layers[current_layer][x + i + screen_size.X * y], ftext_buffer[i], attrib);
        else
            break;
    }

    va_end(args);
}


void ConsoleGraphics::charmap(int _x, int _y, int w, int h, char* array, WORD attrib)
{
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (onScreen(_x + x, _y + y))
                fillChar(&layers[current_layer][(_x + x) + (_y + y) * screen_size.X], array[x + y * w], attrib);
}


void ConsoleGraphics::charmap(int _x, int _y, int w, int h, CHAR_INFO* array)
{
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            if (onScreen(_x + x, _y + y))
                layers[current_layer][(_x + x) + (_y + y) * screen_size.X] = array[x + y * w];
}


void ConsoleGraphics::animFrame(int x, int y, ConsoleAnimationClip& anim)
{
    charmap(x, y, anim.width, anim.height, anim.getCurrentFrame());
}


void ConsoleGraphics::setCursor(int x, int y)
{
    SetConsoleCursorPosition(hconsole, { x, y });
}


void ConsoleGraphics::chooseLayer(int index)
{
    if (index == -1)
        current_layer = layers_count - 1;
    else if (index >= 0 && index < layers_count)
        current_layer = index;
}


int ConsoleGraphics::getLayersCount()
{
    return layers_count;
}


void ConsoleGraphics::fillChar(CHAR_INFO* chr_pointer, char chr, WORD attrib)
{
    (*chr_pointer).Char.AsciiChar = (chr) ? chr : (*chr_pointer).Char.AsciiChar;
    (*chr_pointer).Attributes = (attrib) ? attrib : (*chr_pointer).Attributes;
}


bool ConsoleGraphics::onScreen(int x, int y)
{
    if (x < screen_size.X && x >= 0 && y < screen_size.Y && y >= 0)
        return true;
    return false;
}






//
