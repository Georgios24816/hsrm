#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

double kp;
double ki;
double kd;

double set_point = 0;
bool antiwindup = false;
double windupMax = 0;

double iTerm=0;

double sample_time = 0.02;
double dt = 0.02;
double now_time=0;
double old_time=0;

double outValue=0;
double last_error = 0;
double last_y = 0;

void initPid(double kp_, double ki_, double kd_)
{
	kp = kp_;
	ki = ki_;
	kd = kd_;

	windupMax = windupMax;
	antiwindup = (windupMax != 0);

	assert(!antiwindup); 
}

// PID output according to y_measured
double output(double y_measured)
{
	double error = set_point - y_measured;

       	//Get elapsed time

       	iTerm = iTerm + ki * error * dt;
       	double out = kp * error + iTerm + kd * (last_y - y_measured)/dt;

       	last_error = error;
       	last_y = y_measured;
       	return out;
}

// Main
int main()
{
    // PID regulator
    initPid(1.1,1,0.001);
    
    double y = 0;
    double y_[200];

    // PID regulator for test
    for(int i=0; i < 200; i++)
    {
        y= y + output(y);
        if (i % 10 == 0)
        {
            set_point = i / 10;
        }
        y_[i] = y;
    }

    for (int i = 0; i < 200; i++)
    {
	printf("%d : %lf\n", i, y_[i]);
    }

    return 0;
}
