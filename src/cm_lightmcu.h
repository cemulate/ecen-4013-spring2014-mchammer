/* 
 * File:   cm_lightmcu.h
 * Author: Chase
 *
 * Created on October 21, 2014, 3:58 AM
 */

#ifndef CM_LIGHTMCU_H
#define	CM_LIGHTMCU_H

void configureLightMCU_SPI();

void sendLightStateUpdate(double health, double charge);

#endif	/* CM_LIGHTMCU_H */

