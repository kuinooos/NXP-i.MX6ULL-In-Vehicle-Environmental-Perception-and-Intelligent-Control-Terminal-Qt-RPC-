#ifndef WEATHER_H
#define WEATHER_H

void weather_init(void);
int weather_read(char** text, char** code,char** temperature);

#endif // WEATHER_H
