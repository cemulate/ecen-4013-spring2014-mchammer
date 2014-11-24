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

void startTrackingSpin();
int checkSpinComplete();

void startTrackingThrust();
int checkThrustComplete();

void disableMotionTracking();
void enableMotionTracking();

// Timer routine
void TIMER_accelerometerRoutine();


#endif	/* CM_ACCELEROMETER_H */

