/* 
 * File:   cm_accelerometer.h
 * Author: Chase
 *
 * Created on October 19, 2014, 9:20 PM
 */

#ifndef CM_ACCELEROMETER_H
#define	CM_ACCELEROMETER_H

unsigned int readADCRaw();

double readADCPercent();

int checkSpinComplete();
/*
	Input: None
	Output: 1 if a motion history indicates that 3 spins have occurred, 
			0 if otherwise
	Effects: None
*/

int checkThrustComplete();
/*
	Input: None
	Output: 1 if motion history indicates that an upward thrust has occurred,
			0 if otherwise
	Effects: None
*/

void resetMotionHistory();
/*
	Input: None
	Output: None
	Effects: Clears motion algorithm history (resets state)
*/


#endif	/* CM_ACCELEROMETER_H */

