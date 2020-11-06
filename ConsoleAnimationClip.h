#pragma once

#ifndef CONSOLEANIMATIONCLIP_H
#define CONSOLEANIMATIONCLIP_H

#include "ConsoleDefines.h"


class ConsoleAnimationClip
{
public:
    const int& width = clip_width;
    const int& height = clip_height;

    ConsoleAnimationClip(int _width, int _height, int _frames_count, int framerate);
    ~ConsoleAnimationClip();

    void update(float deltaTime);
    void reset();
    void setFrame(int index, char* charmap, WORD color = ConsoleColor::White);
    void setFrame(int index, CHAR_INFO* charmap);
    void setFramerate(int rate);
    void setActive(bool _active);
    CHAR_INFO* getCurrentFrame();
    CHAR_INFO* getFrame(int index);
    bool isActive();

private:
    bool active;
    int clip_width;
    int clip_height;
    int frames_count;
    int current_frame;
    CHAR_INFO** frames;
    float time_between_frames;
    float time_after_prev_frame;

    void setNextFrame();
    void fillChar(CHAR_INFO* char_info, char chr, WORD color);
};

#endif // CONSOLEANIMATIONCLIP_H












//
