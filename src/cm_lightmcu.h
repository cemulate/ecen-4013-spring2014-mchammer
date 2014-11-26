/* 
 * File:   cm_lightmcu.h
 * Author: Chase
 *
 * Created on October 21, 2014, 3:58 AM
 */

#ifndef CM_LIGHTMCU_H
#define	CM_LIGHTMCU_H

#define ANIM_NONE           0
#define ANIM_FLASHTWICE    1

void requestAnimation(int);

void sendLightMCU(unsigned char data);

void setLightMCU_Red();
void setLightMCU_Green();

#endif	/* CM_LIGHTMCU_H */

