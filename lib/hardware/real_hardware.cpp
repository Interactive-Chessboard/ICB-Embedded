// real_hardware.cpp
#include "real_hardware.hpp"
#ifdef ARDUINO


std::string RealHardware::getMacAddress()
{
    uint8_t mac[6];
    // Getting the bluetooth mac address
    esp_read_mac(mac, ESP_MAC_BT);

    char buf[18];
    snprintf(buf, sizeof(buf),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    return std::string(buf);
}


std::string RealHardware::getFirmwareVersion()
{
    return FW_VERSION;
}

#endif
