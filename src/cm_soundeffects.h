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
/*
	Input:	s: An enumerated integer identifying the sound
	Output: 0 if the sound was started, 1 if a sound is already playing
	Effect: Communicates to the sound controller to play a select sound
*/

#endif	/* CM_SOUNDEFFECTS_H */

