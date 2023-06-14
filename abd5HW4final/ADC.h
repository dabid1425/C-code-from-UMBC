/*
 * ADC.h
 *
 *  Created on: Nov 19, 2013
 *      Author: dan
 */

#ifndef ADC_H_
#define ADC_H_
void ADCSetup();
void ADCStartConversion();
void ADCClearConversionFlag();
int ADCIsConversionCompleteFlagSet();
uint16_t ADCGet();
int ADCAquire();


#endif /* ADC_H_ */
