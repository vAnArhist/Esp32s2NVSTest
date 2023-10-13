# May be set as file(GLOB MY_SRCS dir/*), but creators of CMake 
# themselves advise not to use globbing.
# See: https://cmake.org/cmake/help/v3.15/command/file.html?highlight=glob#file
set(SOURCES ./Source/Core/Config.cpp)

# NOTE!! It is necessary to type space after every module
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . Header
                        PRIV_REQUIRES  
                            esp_event  
                            esp_common  
                            driver 
                            freertos  
                            nvs_flash )