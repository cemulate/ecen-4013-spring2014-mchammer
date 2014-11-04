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

int checkThrustComplete();


#endif	/* CM_ACCELEROMETER_H */

