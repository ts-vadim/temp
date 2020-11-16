// Primitive Types.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#define DYNAMICARRAY_DEBUG
#include "types/types.h"
#include "types/util.h"


class Tester
{
public:
    int data = 0;

    Tester() : data(-888)
    {
    }
    Tester(int newData) : data(newData)
    {
    }
    Tester(Tester const& other) : data(other.data)
    {
        std::cout << "Tester(" << data << "): Copy\n";
    }
    Tester(Tester&& other) : data(other.data)
    {
        std::cout << "Tester(" << data << "): Move\n";
    }
    ~Tester()
    {
        std::cout << "Tester(" << data << "): Destroy\n";
    }

    Tester& operator=(Tester const& other)
    {
        std::cout << "Tester(" << data << "): Copy\n";
        data = other.data;
        return *this;
    }

    Tester& operator=(Tester&& other)
    {
        std::cout << "Tester(" << data << "): Move\n";
        data = other.data;
        return *this;
    }
};




template <typename T>
void printArrayInfo(DynamicArray<T>& array)
{
    std::cout << "Info:\n";
    std::cout << "  size:" << array.size << "\n";
    std::cout << "  capacity:" << array.capacity << "\n";
}



int main()
{
    DynamicArray<Tester> array;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "Adding\n----------------------\n";
        array.Add(Tester(i));
        printArrayInfo(array);
        printArray(array.GetData(), array.capacity);
        std::cout << "Tester[" << i << "] data: " << array[i].data << "\n\n";
    }

    std::cout << "Checking\n----------------------\n";
    for (int i = 0; i < 3; i++)
    {
        std::cout << "Tester[" << i << "] data: " << array[i].data << "\n\n";
    }

    std::cout << "Removing [0]\n----------------------\n";
    array.Remove(0);
    printArrayInfo(array);

    std::cout << "\Clearing\n----------------------\n";
    array.Clear();
    printArrayInfo(array);
}
