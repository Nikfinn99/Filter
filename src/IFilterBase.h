#pragma once

#include <Arduino.h>

class IFilterBase
{
  protected:
    unsigned long m_millis_last;

    float m_last;
    float m_target;
    float m_time_parameter;

    bool m_limit = false;
    float m_limit_upper;
    float m_limit_lower;

  public:
    IFilterBase()
        : IFilterBase(0, 0) {}

    // same as init but in constructor
    IFilterBase(float target, float time_parameter)
        : m_millis_last(millis()),
          m_last(target),
          m_target(target),
          m_time_parameter(time_parameter) {}

    ~IFilterBase() {}

    IFilterBase &init(float start, float time_parameter)
    {
        m_last = start;
        m_target = start;
        m_time_parameter = time_parameter;
        return *this;
    }

    IFilterBase &setStartValue(float start)
    {
        m_last = start;
        m_target = start;
        return *this;
    }

    IFilterBase &setLimit(float upper, float lower)
    {
        m_limit = true;
        m_limit_upper = upper;
        m_limit_lower = lower;
        return *this;
    }

    IFilterBase &setTimeParameter(float p_time)
    {
        m_time_parameter = p_time;
        return *this;
    }

    float getTimeParameter()
    {
        return m_time_parameter;
    }

    float getValue()
    {
        if (m_limit) // limit enabled
        {
            if (m_last > m_limit_upper) // exceeded upper limit
            {
                return m_limit_upper;
            }
            if (m_last < m_limit_lower) // exceeded lower limit
            {
                return m_limit_lower;
            }
        }
        return m_last; // value ok
    }

    virtual void setTarget(float p_target) = 0;
    virtual void update() = 0;
};