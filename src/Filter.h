#ifndef FILTER_H
#define FILTER_H

#include <Arduino.h>

template <typename Type = double>
class Filter
{
protected:
  Type accel_last;
  Type linear_last;
  Type linear_target;
  Type linear_step;
  bool linear_limit = false;
  Type linear_upper_lim;
  Type linear_lower_lim;
  double linear_transition_time = 500;
  double accel_factor = 1000;
  Type spike_last;
  Type spike_use;
  double spike_max_step;
  int spike_type;
  bool spike_wrong;
  long millis_last;

public:
  Filter() {millis_last = millis();}

  enum{
    SPIKE_TOP,
    SPIKE_BOT
  };

  Filter& initAccel(Type start_value, double factor){accel_last = start_value; accel_factor = factor;return *this;}
  Filter& updateAccel(Type new_val){
    double time_diff = millis() - millis_last;
    time_diff /= 1000;
    millis_last = millis();

    double factor = accel_factor * time_diff;
    factor = constrain(factor, 0, 1);
    Type r = new_val * factor + accel_last * (1 - factor);
    accel_last = r;

    return *this;
  }
  Type getAccel(){return accel_last;}

  Filter& initLinear(Type start_value, double transition_time){
    linear_last = start_value;
    linear_target = start_value;
    linear_transition_time = transition_time;
    return *this;
  }
  Filter& limitLinear(Type lower, Type upper){
    linear_limit = true;
    if(upper < lower){//change
      Type tmp = lower;
      lower = upper;
      upper = tmp;
    }
    linear_upper_lim = upper;
    linear_lower_lim = lower;
  }

  Filter& updateLinear(Type new_val){
    double delta_t = millis() - millis_last;
    if(new_val != linear_target){
      linear_target = new_val;
      linear_step = linear_target - linear_last;
    }
    Type lin_single_step = (linear_step * delta_t) / linear_transition_time;

    linear_last = linearIncrement(linear_target, linear_last, lin_single_step);

    millis_last = millis();
    return *this;
  }
  Type getLinear(){return linear_last;}



  Filter& initSpike(Type init, double max_step, int spike = SPIKE_BOT){
    spike_last = init;
    spike_use = init;
    spike_max_step = max_step;
    spike_type = spike;
    spike_wrong = false;
    return *this;
  }
  Type spike(Type val){
    Type diff = val - spike_last;
    spike_last = val;
    if(diff > spike_max_step){
      spike_wrong = (spike_type == SPIKE_BOT)? true: false;
    }
    else if(diff < -spike_max_step){
      spike_wrong = (spike_type == SPIKE_TOP)? true: false;
    }
    if(!spike_wrong){
      spike_use = spike_last;
    }
    return spike_use;
  }

  Type fixOverflow(Type x){
    if(linear_limit && x < linear_lower_lim)x = linear_lower_lim;
    if(linear_limit && x > linear_upper_lim)x = linear_upper_lim;
    return x;
  }

  Type linearIncrement(Type target, Type value, Type step){
    Type ret;
    if( abs_(target - value) < abs_(step) ){
      ret = target;
    }
    else{
      ret = fixOverflow(value + step);
    }
    return ret;
  }

  static Type map(Type x, Type in_min, Type in_max, Type out_min, Type out_max){return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}
  static Type abs_(Type x){if(x < 0){return -x;}return x;}
};

#endif // FILTER_H
