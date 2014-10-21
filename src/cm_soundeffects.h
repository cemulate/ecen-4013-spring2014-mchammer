/* 
 * File:   cm_soundeffects.h
 * Author: Chase
 *
 * Created on October 19, 2014, 9:32 PM
 */

#ifndef CM_SOUNDEFFECTS_H
#define	CM_SOUNDEFFECTS_H

typedef enum {
    SOUND_SPIN_COMPLETE,
    SOUND_CHARGING,
    SOUND_CHARGING_COMPLETE
} SoundEffect;

int playSound(SoundEffect s);

#endif	/* CM_SOUNDEFFECTS_H */

