#ifndef DRIVER_H
#define DRIVER_H
#include <stdint.h>
int check_day(unsigned char day);
int check_hour(unsigned char hour);
int check_minutes(unsigned char minutes);
int check_mes(unsigned char mes);
int check_seculo(unsigned char seculo);
int check_ano(unsigned char ano);
uint8_t read_clock_day();
uint8_t read_clock_mes();
uint8_t read_clock_seculo();
uint8_t read_clock_battery();
uint8_t read_clock_hour();
uint8_t read_clock_minutes();
uint8_t read_clock_year();

#endif