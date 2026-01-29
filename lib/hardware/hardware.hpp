// hardware.hpp
#pragma once
#include "hardware_interface.hpp"


class Hardware
{
public:
    static IHardware& get()
    {
        return *instance_;
    }

    static void set(IHardware& hw)
    {
        instance_ = &hw;
    }

private:
    static IHardware* instance_;
};
