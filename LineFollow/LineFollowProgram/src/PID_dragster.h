/*
 * This file is a part of Dragster car set library.
 *
 * Defines: simple PID-regulator for Dragster 
 * © Amperka LLC (https://amperka.com, dev@amperka.com)
 * 
 * Author: Yury Botov <by@amperka.com>
 * License: GPLv3, all text here must be included in any redistribution.
 */

#ifndef __PID_DRAGSTER_H__
#define __PID_DRAGSTER_H__

class PID {

public:
    PID(double* result, double proporcional, double integral, double differencial);

    void setTunings(double proporcional, double integral, double differencial);
    void setOutputLimits(double min, double max);

    void SetTunings(double prop, double integr, double diff); // deprecated
    void SetOutputLimits(double min, double max); // deprecated

    void compute(double deviation);

private:
    double clamp(double source, double min, double max);

    double _kp; // Proportional parameter
    double _ki; // Integral parameter
    double _kd; // Differencial parameter

    double _previouse; // previouse step deviation value
    double _accumulated; // accumulated deviation

    double* _output;

    double _limitMin, _limitMax; // clamp limits
};

#endif //__PID_DRAGSTER_H__
