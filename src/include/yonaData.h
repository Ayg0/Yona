#pragma once

#include "klibc/types.h"

enum yonaStatus {
    YONA_STATUS_STABLE,
    YONA_STATUS_ERROR,
    YONA_STATUS_UNKNOWN,
};

typedef struct yonaData {
    char        OSVersion[100];
    enum yonaStatus  status;
    bool        isPaginated;
    size_t      mainEBP;
} _yonaData;

void setYonaStatus(enum yonaStatus status);