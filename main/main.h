#ifndef MAIN_H
#define MAIN_H

#include "Config.hpp"

class Main final
{
public:
    bool setup(void);
    void run(void);

private:
    Core::Config* smpConfig;
};

#endif /* MAIN_H */