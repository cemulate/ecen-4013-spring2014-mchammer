/* 
 * File:   cm_adc.h
 * Author: Chase Meadors
 *
 * Created on October 19, 2014, 2:35 AM
 */

#ifndef CM_ADC_H
#define	CM_ADC_H

void configureADC(int analogport);

unsigned int readADCRaw();
double readADCPercent();

#endif	/* CM_ADC_H */

