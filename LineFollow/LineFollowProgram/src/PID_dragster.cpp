/*
 * This file is a part of Dragster car set library.
 *
 * Implement: simple PID-regulator for Dragster 
 * © Amperka LLC (https://amperka.com, dev@amperka.com)
 * 
 * Author: Yury Botov <by@amperka.ru>
 * License: GPLv3, all text here must be included in any redistribution.
 */

#include "PID_dragster.h"

PID::PID(double* result, double proporcional, double integral, double differencial) {
    _kp = proporcional;
    _ki = integral;
    _kd = differencial * 10;
    _previouse = 0.0;
    _accumulated = 0.0;
    _output = result;
    _limitMin = -1.1;
    _limitMax = 1.1;
}

void PID::setTunings(double proporcional, double integral, double differencial) {
    _kp = proporcional;
    _ki = integral;
    _kd = differencial;
}

void PID::setOutputLimits(double min, double max) {
    _limitMin = min;
    _limitMax = max;
}

void PID::compute(double deviation) {
    double p = _kp * deviation; // proporcional reaction
    double i = _ki * _accumulated; // one-step integral reaction
    double d = _kd * (deviation - _previouse); // one-step differencial reaction
    _previouse = deviation;
    _accumulated = (7 * _accumulated + deviation) / 8; // integration
    *_output = -clamp(p + i + d, _limitMin, _limitMax); // reregulation supression
}

double PID::clamp(double source, double min, double max) {
    return (source < min) ? min : ((source > max) ? max : source);
}

/* deprecated, backward compatibility only: */

void PID::SetTunings(double prop, double integr, double diff) {
    setTunings(prop, integr, diff);
}

void PID::SetOutputLimits(double min, double max) {
    setOutputLimits(min, max);
}
