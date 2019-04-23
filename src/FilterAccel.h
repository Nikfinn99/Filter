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
        float delta_t = getDeltaTime();

        float factor;
        if (m_time_parameter == 0) // division by zero so apply change instantly
        {
            factor = 1;
        }
        else
        {
            factor = (1 / m_time_parameter) * delta_t;
        }

        // constrain
        if (factor < 0)
        {
            factor = 0;
        }

        if (factor > 1)
        {
            factor = 1;
        }

        m_last = m_last * (1 - factor) + m_target * factor;
    }
};
