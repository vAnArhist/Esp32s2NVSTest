#include "Config.hpp"
#include "esp_log.h"

using namespace Core;

static const char* LogTag = "Config";
Config* Config::spInstance = nullptr;

Config::Config()
{
    esp_err_t stat = nvs.init();
    ESP_LOGI(LogTag, "Initialising nvs, status: %s", esp_err_to_name(stat));
    
    memset(&mConfig, 0, sizeof(mConfig));
    // ESP_ERROR_CHECK(stat);
}

Config* Config::getInstance()
{
    if (spInstance == nullptr) {
        spInstance = new Config();
    }
    return spInstance;
}

uint8_t Config::getDeviceCnt()
{
    return mConfig.deviceCnt;
}

bool Config::getAllDevices(devicesArr_t &devArr)
{
    return get(devArr);
}

/**
 * @brief Write device to NVS 
 * @param id - id in array
 * @param device - ref to devId_t struct
 * @return true - Nvs write OK
 * @return false - Nvs write !OK
 */
bool Config::writeId(uint8_t id, devId_t &device)
{
    bool stat = false;

    // read all conf from flash to local struct
    stat = get(mConfig);
    // set device params
    mConfig.devArr[id].id = device.id;
    memcpy(mConfig.devArr[id].macAddr, device.macAddr, sizeof(device.macAddr));

    mConfig.deviceCnt++;
    stat = set(mConfig);

    return stat;
}

/**
 * @brief Read device from NVS 
 * 
 * @param id - id in array
 * @param device - ref to devId_t struct
 * @return true 
 * @return false 
 */
bool Config::readId(uint8_t id, devId_t &device)
{
    bool stat = false;
    
    // read all conf from flash to local struct
    stat = get(mConfig);
    if (stat) {
        memcpy(&device, &mConfig.devArr[id], sizeof(mConfig.devArr[id]));
    }

    return stat;
}

bool Config::get(devicesArr_t &devArr)
{
    esp_err_t stat = nvs.get(kpNvsConfigKey, mConfig);
    
    bool st = false;
    if (stat == ESP_OK) {
        uint32_t crc32Calc = crc32Compute((uint8_t*)&mConfig, sizeof(devArr)-sizeof(uint32_t));
        if (mConfig.crc32 != crc32Calc) {
            ESP_LOGE(LogTag, "Crc is not mached! calc crc: %08x, read crc: %08x", (unsigned int)crc32Calc, (unsigned int)mConfig.crc32);
            return st;
        }
        st = true;
    } else {
        ESP_LOGE(LogTag, "Cannot get key %s from nvs, status: %s", 
                kpNvsConfigKey,  esp_err_to_name(stat));
    }

    memcpy(&devArr, &mConfig, sizeof(devArr));

    return st;
}

bool Config::set(devicesArr_t &devArr)
{
    uint32_t crc32Calc = crc32Compute((uint8_t*)&devArr, sizeof(devArr)-sizeof(uint32_t));
    devArr.crc32 = crc32Calc;
    memcpy(&mConfig, &devArr, sizeof(devArr));

    esp_err_t stat = nvs.set(kpNvsConfigKey, devArr);
    bool st = false;
    // Then write to NVM
    if (stat == ESP_OK) {
        st = true;
    } else {
        ESP_LOGE(LogTag, "Cannot set key %s in nvs, status: %s", 
                kpNvsConfigKey, esp_err_to_name(stat));
    }
    return st;
}

/**@brief Function for calculating CRC-32 in blocks.
 *
 * Feed each consecutive data block into this function, along with the current value of p_crc as
 * returned by the previous call of this function. The first call of this function should pass NULL
 * as the initial value of the crc in p_crc.
 *
 * @param[in] pData The input data block for computation.
 * @param[in] size   The size of the input data block in bytes.
 *
 * @return The updated CRC-32 value, based on the input supplied.
 */
uint32_t Config::crc32Compute(uint8_t const * pData, uint32_t size)
{
    uint32_t crc = 0xFFFFFFFF;

    for (uint32_t i = 0; i < size; i++)
    {
        crc = crc ^ pData[i];
        for (uint32_t j = 8; j > 0; j--)
        {
            crc = (crc >> 1) ^ (0xEDB88320U & ((crc & 1) ? 0xFFFFFFFF : 0));
        }
    }
    return ~crc;
}