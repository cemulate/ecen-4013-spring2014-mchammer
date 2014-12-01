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
    int charge;

} HammerState;


void initHammerState();

HammerState * getHammerStatePtr();

#endif	/* HAMMERSTATE_H */

