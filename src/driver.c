#include "driver.h"
uint32_t system_ticks = 0;
int check_day(unsigned char day){ 
    if (day > 31){ 
        return 0;
    }
    if (day < 1){ 
        return 0;
    }
    return 1;
}

int check_hour(unsigned char hour){ 
    if (hour > 23){ 
        return 0;
    }
    return 1;
}

int check_minutes(unsigned char minutes){ 
    if (minutes > 59){ 
        return 0;
    }
    return 1;
}
int check_mes(unsigned char mes){ 
    if (mes > 12){ 
        return 0;
    }
    if (mes < 1){ 
        return 0;
    }
    return 1;
}

int check_seculo(unsigned char seculo){ 
    if (seculo > 99){ 
        return 0;
    }
    return 1;
}
int check_ano(unsigned char ano){ 
    if (ano > 99){ 
        return 0;
    }
    return 1;
}
uint8_t read_clock_day(){
    uint8_t day;
    __asm__ volatile(
        "mov $0x07, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (day)
    );
    return day;

}

uint8_t read_clock_mes(){
    uint8_t mes;
    __asm__ volatile(
        "mov $0x08, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (mes)
    );
    return mes;

}

uint8_t read_clock_seculo(){
    uint8_t seculo;
    __asm__ volatile(
        "mov $0x32, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (seculo)
    );
    return seculo;

}

uint8_t read_clock_battery(){
    uint8_t power;
    __asm__ volatile(
        "mov $0x0D, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (power)
    );
    return power;

}
uint8_t read_clock_hour(){
    uint8_t hour;
    __asm__ volatile(
        "mov $0x04, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (hour)
    );
    return hour;
}

uint8_t read_clock_minutes(){
    uint8_t minutes;
    __asm__ volatile(
        "mov $0x02, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (minutes)
    );
    return minutes;
}
uint8_t read_clock_year(){
    uint8_t year;
    __asm__ volatile(
        "mov $0x09, %%al;"
        "out %%al, $0x70;"
        "in $0x71, %%al;"
        : "=a" (year)
    );
    return year;

}