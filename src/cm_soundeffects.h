/* 
 * File:   cm_soundeffects.h
 * Author: Chase
 *
 * Created on October 19, 2014, 9:32 PM
 */

#ifndef CM_SOUNDEFFECTS_H
#define	CM_SOUNDEFFECTS_H

#define HS_BOOT                 0
#define HS_SPINCOMPLETE         1
#define HS_CHARGING             2
#define HS_CHARGECOMPLETE       3
#define HS_FIRE                 4
#define HS_DAMAGED              5
#define HS_HEALED               6
#define HS_STUNNED              7
#define HS_DIE                  8

#define CS_BOOT                 0
#define CS_FIRE                 1

#define HS_BOOT_LEN             1420
#define HS_SPINCOMPLETE_LEN     1255
#define HS_CHARGING_LEN         31570
#define HS_CHARGECOMPLETE_LEN   2130
#define HS_FIRE_LEN             1390
#define HS_DAMAGED_LEN          630
#define HS_HEALED_LEN           1160
#define HS_STUNNED_LEN          1010
#define HS_DIE_LEN              3350

#define CS_BOOT_LEN             1350
#define CS_FIRE_LEN             4870


void audioReset();

void playSound(int sound);
void setVolume(int vol);


#endif	/* CM_SOUNDEFFECTS_H */

