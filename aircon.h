#ifndef _AIRCON_H_
#define _AIRCON_H_
#include "macro.h"

void powerOn(aircon_t *aircon);
void powerOff(aircon_t *aircon);
void calmOn(aircon_t *aircon);
void calmOff(aircon_t *aircon);
void changeMode(aircon_t *aircon);
void changeModeTarget(aircon_t *aircon, mode_t mode);

void tempUp(aircon_t *air);
void tempDown(aircon_t *air);
void tempTarget(aircon_t *aircon, int target);
void fanUp(aircon_t *air);
void fanDown(aircon_t *air);
void fanTarget(aircon_t *aircon, fanspeed_t target);

#endif