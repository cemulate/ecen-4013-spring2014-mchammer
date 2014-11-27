/* 
 * File:   cm_soundeffects.h
 * Author: Chase
 *
 * Created on October 19, 2014, 9:32 PM
 */

#ifndef CM_SOUNDEFFECTS_H
#define	CM_SOUNDEFFECTS_H

#define HAMMER_SOUND_BOOT               0
#define HAMMER_SOUND_SPINCOMPLETE       1
#define HAMMER_SOUND_CHARGING           2
#define HAMMER_SOUND_FIRE               3
#define HAMMER_SOUND_DAMAGED            4
#define HAMMER_SOUND_HEALED             5
#define HAMMER_SOUND_STUNNED            6

#define CLOUD_SOUND_BOOT                0
#define CLOUD_SOUND_FIRE                1


void audioReset();

void playSound(int sound);


#endif	/* CM_SOUNDEFFECTS_H */

