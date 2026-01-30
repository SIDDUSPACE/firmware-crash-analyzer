#pragma once
#include <stdint.h>

#define CRASH_MAGIC 0x43524153 

typedef struct {
    uint32_t magic;

    uint32_t pc;
    uint32_t lr;
    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t bfar;
    uint32_t mmfar;

} crash_record_t;
extern crash_record_t crash_record;
