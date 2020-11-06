#include "ConsoleAnimationClip.h"

ConsoleAnimationClip::ConsoleAnimationClip(int _width, int _height, int _frames_count, int framerate)
{
    clip_width = _width;
    clip_height = _height;
    frames_count = _frames_count;
    time_between_frames = 1000.0 / framerate;
    current_frame = 0;
    time_after_prev_frame = 0;
    active = true;

    frames = new CHAR_INFO * [frames_count];
    for (int i = 0; i < frames_count; i++)
    {
        frames[i] = new CHAR_INFO[clip_width * clip_height];
        for (int j = 0; j < clip_width * clip_height; j++)
        {
            fillChar(&frames[i][j], ' ', ConsoleColor::White);
        }
    }
}


ConsoleAnimationClip::~ConsoleAnimationClip()
{
    for (int i = 0; i < frames_count; i++)
        delete[] frames[i];
    delete[] frames;
}


void ConsoleAnimationClip::update(float deltaTime)
{
    if (active)
    {
        time_after_prev_frame += deltaTime;
        if (time_after_prev_frame > time_between_frames)
        {
            time_after_prev_frame = 0;
            setNextFrame();
        }
    }
}


void ConsoleAnimationClip::setFrame(int index, char* charmap, WORD color)
{
    if (index < 0 || index >= frames_count)
        return;

    for (int i = 0; i < clip_width * clip_height; i++)
    {
        fillChar(&frames[index][i], charmap[i], color);
    }

}


void ConsoleAnimationClip::setFrame(int index, CHAR_INFO* charmap)
{
    if (index < 0 || index >= frames_count)
        return;

    for (int i = 0; i < clip_width * clip_height; i++)
    {
        fillChar(&frames[index][i], charmap[i].Char.AsciiChar, charmap[i].Attributes);
    }
}


CHAR_INFO* ConsoleAnimationClip::getCurrentFrame()
{
    return frames[current_frame];
}


CHAR_INFO* ConsoleAnimationClip::getFrame(int index)
{
    if (index >= 0 && index < frames_count)
        return frames[index];
}


void ConsoleAnimationClip::setFramerate(int rate)
{
    time_between_frames = 1000.0 / rate;
}


void ConsoleAnimationClip::reset()
{
    current_frame = 0;
    time_between_frames = 0;
}


void ConsoleAnimationClip::setActive(bool _active)
{
    active = _active;
}


bool ConsoleAnimationClip::isActive()
{
    return active;
}


void ConsoleAnimationClip::setNextFrame()
{
    current_frame = (current_frame + 1 >= frames_count) ? 0 : current_frame + 1;
}


void ConsoleAnimationClip::fillChar(CHAR_INFO* char_info, char chr, WORD color)
{
    (*char_info).Char.AsciiChar = chr;
    (*char_info).Attributes = color;
}












//
