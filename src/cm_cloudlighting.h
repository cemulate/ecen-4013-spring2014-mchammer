/* 
 * File:   cm_cloudlighting.h
 * Author: Chase
 *
 * Created on November 4, 2014, 2:53 PM
 */

#ifndef CM_CLOUDLIGHTING_H
#define	CM_CLOUDLIGHTING_H

#define BRIGHTNESS_MAX      4095
#define BRIGHTNESS_OFF      0
#define BRIGHTNESS_HALF     2048

#define ALGM_OFF    0
#define ALGM_BLINK  1

void cloudLED_spiSend(unsigned int);
void sendCloudLEDState(unsigned int *p);

void cloudLightingUpdate();

void cloudLightingSetMode(int algm);
void cloudLightingSet(unsigned int, unsigned int);
void cloudLightingSetAll(unsigned int);
void cloudLightingSetActive();

#endif	/* CM_CLOUDLIGHTING_H */

