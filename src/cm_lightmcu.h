/* 
 * File:   cm_lightmcu.h
 * Author: Chase
 *
 * Created on October 21, 2014, 3:58 AM
 */

#ifndef CM_LIGHTMCU_H
#define	CM_LIGHTMCU_H

void sendLightStateUpdate(double health, double charge);
/*
	Input: double health, double charge
	Output: None
	Effect: Communicates to the light co-processor to change/update the 
			state of the lights
*/

#endif	/* CM_LIGHTMCU_H */

