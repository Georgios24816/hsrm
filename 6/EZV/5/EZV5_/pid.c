#include "pid.h"

static double kp;
static double ki;
static double kd;
static double set_point = 0;
static bool antiwindup = false;
static double windupMax = 0;
static double iTerm=0;
static double sample_time = 0.02;
static double dt = 0.02;
static double now_time=0;
static double old_time=0;
static double outValue=0;
static double last_error = 0;
static double last_y = 0;

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

void setSetPoint(double p)
{
	set_point = p;
}

#if 0
// Main
int main()
{
    // PID regulator
    initPid(1.1,1,0.001);
    
    double y = 0;
    double y_[200];

    // PID regulator for test
    for(int i=0; i < 20; i++)
    {
	    if (i == 5)
	    {
    		set_point = 5;
	    }

        y= y + output(y);
        y_[i] = y;
    }

    for (int i = 0; i < 20; i++)
    {
	printf("%d : %lf\n", i, y_[i]);
    }

    return 0;
}
#endif
