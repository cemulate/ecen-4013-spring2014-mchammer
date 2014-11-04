/* 
 * File:   HammerState.h
 * Author: Chase
 *
 * Created on October 19, 2014, 9:44 PM
 */

#ifndef HAMMERSTATE_H
#define	HAMMERSTATE_H

typedef struct {

    int health;
    double chargeStatus;
    double chargeRate;
    int charging;

    int invincible;
    

} HammerState;


void initHammerState();

HammerState * getHammerStatePtr();

void HammerState_doCharge(double dt);

#endif	/* HAMMERSTATE_H */

