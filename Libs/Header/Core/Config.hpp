#ifndef CONFIG_HPP
#define CONFIG_HPP

// Header for device configuration

#include "Nvs.hpp"

typedef struct __attribute__((__packed__)) {
    uint8_t id;
    uint8_t macAddr[6];
} devId_t;

typedef struct __attribute__((__packed__)) {
    uint8_t deviceCnt;
    devId_t devArr[16];
    uint32_t crc32;
} devicesArr_t; // 117bytes

namespace Core {

class Config {

public:
    
    static Config* getInstance(void);

    uint8_t getDeviceCnt();
    bool getAllDevices(devicesArr_t &devArr);
    bool writeId(uint8_t id, devId_t &device);
    bool readId(uint8_t id, devId_t &device);

private:
    static Config* spInstance;
    Nvs nvs;
    const char* kpNvsConfigKey = "Config";
    devicesArr_t mConfig;

    bool get(devicesArr_t &devArr);
    bool set(devicesArr_t &devArr);
    
    uint32_t crc32Compute(uint8_t const * pData, uint32_t size);
    Config();
};

} // namespace Core

#endif // CONFIG_HPP