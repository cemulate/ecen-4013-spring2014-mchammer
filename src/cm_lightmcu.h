/* 
 * File:   cm_lightmcu.h
 * Author: Chase
 *
 * Created on October 21, 2014, 3:58 AM
 */

#ifndef CM_LIGHTMCU_H
#define	CM_LIGHTMCU_H

void sendLightMCU(unsigned char data);

void updateLightMCUHealth(unsigned int);
void updateLightMCUCharge(unsigned int);

void setLightMCUColor(unsigned int, unsigned int, unsigned int);

void setLightMCURainbow();
void setLightMCUOff();

void disableLightMCUUpdates();
void enableLightMCUUpdates();

void requestLightMCUUpdateTimeout();

#endif	/* CM_LIGHTMCU_H */

