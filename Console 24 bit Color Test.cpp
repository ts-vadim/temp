#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>

#define ESC "\x1b"
#define CSI "\x1b["


const char* docs = "https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#extended-colors";


bool EnableVTMode();

void rgb(const char* text, bool bg, int r, int g, int b)
{
    std::cout << CSI << ((bg) ? 48 : 38) << ";2;" << r << ";" << g << ";" << b << "m" << text << CSI "m\n";
}

void underline(const char* text)
{
    std::cout << CSI "4m" << text << CSI "m\n";
}

void inverse(const char* text)
{
    std::cout << CSI "7m" << text << CSI "m\n";
}

void palette()
{
    for (int i = 0; i < 256; i++)
        std::cout << CSI "48;5;" << i << "m" << "pallete color" << i << CSI "m\n";
}


int main()
{
    
	std::cout << "EnableVTMode: " << ((EnableVTMode()) ? "success" : "failed") << std::endl;

    rgb("rgb string", false, 150, 255, 178);
    std::cout << std::endl;
    underline("underline");
    std::cout << std::endl;
    inverse("inverse");
    std::cout << std::endl;
    palette();

    std::cout << "\nDocs: " << docs << std::endl;

    getchar();
}




bool EnableVTMode()
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }
    return true;
}