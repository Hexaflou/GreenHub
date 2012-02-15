/*
 * Test.h
 *
 * Created on: 19 janv. 2012
 * Author: nphan
 */

#ifndef TEST_H_
#define TEST_H_

#include "Component.h"
#include "EEP.h"

void initializeConfig(Sensor ** pp_sensorList, Actuator ** pp_actuatorList, EEP* EEPList);
void initializeSensorAndEEPListFirst();

#endif /* TEST_H_ */
