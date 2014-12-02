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
    int charging;

} HammerState;


void initHammerState();

HammerState * getHammerStatePtr();

void healthUp();
void healthDown();

void permaDeath();
void stun();

#endif	/* HAMMERSTATE_H */

