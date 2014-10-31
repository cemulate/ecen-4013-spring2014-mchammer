/* 
 * File:   cm_soundeffects.h
 * Author: Chase
 *
 * Created on October 19, 2014, 9:32 PM
 */

#ifndef CM_SOUNDEFFECTS_H
#define	CM_SOUNDEFFECTS_H

#define SOUND_ZERO      0
#define SOUND_ONE       1
#define SOUND_TWO       2

void playSound(int sound);
/*
	Input:	s: An enumerated integer identifying the sound
	Output: 0 if the sound was started, 1 if a sound is already playing
	Effect: Communicates to the sound controller to play a select sound
*/

#endif	/* CM_SOUNDEFFECTS_H */

