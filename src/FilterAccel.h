#pragma once

#include "IFilterBase.h"

class FilterAccel : public IFilterBase
{
  private:
  public:
    FilterAccel() {}

    // same as init but in constructor
    FilterAccel(float target, float time_parameter)
        : IFilterBase(target, time_parameter) {}

    ~FilterAccel() {}

    virtual void setTarget(float p_target) override
    {
        m_target = p_target;
    }

    virtual void update() override
    {
        float time_diff = millis() - m_millis_last;
        time_diff /= 1000;
        m_millis_last = millis();

        float factor = (1 / m_time_parameter) * time_diff;
        if (m_time_parameter == 0) // division by zero so apply change instantly
            factor = 1;
        factor = constrain(factor, 0, 1);

        m_last = m_last * (1 - factor) + m_target * factor;
    }
};