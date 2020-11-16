//
// FastRunTest.cpp
// Note: When every RunTest finished, it should be pushed into Cakes(Backups).

#include <iostream>
#include <iomanip>

#include "FastRunTest.h"

int main()
{
    std::cout << std::setfill('-') << std::setiosflags(std::ios::right) << std::setw(20) << "FastRunTest";
    std::cout << std::setfill('-') << std::setiosflags(std::ios::left) << std::setw(16) << '-' << std::endl;
    std::cout << std::setfill('-') << std::setiosflags(std::ios::right) << std::setw(20) << TestName;
    std::cout << std::setfill('-') << std::setiosflags(std::ios::left) << std::setw(16) << '-' << std::endl;

    {
        Test* test1 = Test::Get();
        Test* test2 = Test::Get();

        std::thread th1(access, 1);
        std::thread th2(access, 2);

        th1.join();
        th2.join();

        // When Exit.
        ResourcePool::Get()->Free();
    }
}
