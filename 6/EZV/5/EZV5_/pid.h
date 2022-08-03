#ifndef PID_H_
#define PID_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

void initPid(double kp_, double ki_, double kd_);
double output(double y_measured);
void setSetPoint(double p);
#endif
