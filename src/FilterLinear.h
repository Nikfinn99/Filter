#pragma once

#include "IFilterBase.h"

class FilterLinear : public IFilterBase
{
private:
  float m_step;

public:
  FilterLinear() {}
  ~FilterLinear() {}

  virtual void setTarget(float p_target) override
  {
    if (p_target != m_target)
    {
      m_target = p_target;
      m_step = m_target - m_last;
    }
  }

  float linearIncrement(float target, float value, float step)
  {
    float ret;
    if (abs(target - value) <= abs(step))
    {
      ret = target;
    }
    else
    {
      ret = value + step;
    }
    return ret;
  }

  virtual void update() override
  {
    double delta_t = millis() - m_millis_last;

    if (m_time_parameter > 0)
    {
      float lin_single_step = (m_step * delta_t) / m_time_parameter;

      m_last = linearIncrement(m_target, m_last, lin_single_step);
    }
    else
    {
      m_last = m_target;
    }

    m_millis_last = millis();
  }
};