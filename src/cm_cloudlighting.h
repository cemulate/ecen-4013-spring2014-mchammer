/* 
 * File:   cm_cloudlighting.h
 * Author: Chase
 *
 * Created on November 4, 2014, 2:53 PM
 */

#ifndef CM_CLOUDLIGHTING_H
#define	CM_CLOUDLIGHTING_H

void cloudLED_spiSend(unsigned int);
void sendCloudLEDState(unsigned int *p);

void TIMER_cloudLightingRoutine();

#endif	/* CM_CLOUDLIGHTING_H */

