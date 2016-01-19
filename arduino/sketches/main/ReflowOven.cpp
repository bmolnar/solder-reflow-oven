#include "ReflowOven.h"

ReflowOven::ReflowOven(Stream& remote, int thermo_clk_pin, int thermo_cs_pin, int thermo_do_pin, int heat_ctrl_pin)
  : remote_(remote),
    thermo_(thermo_clk_pin, thermo_cs_pin, thermo_do_pin),
    heat_ctrl_pin_(heat_ctrl_pin),
    temp_c_(0.0),
    inlen_(0),
    last_ms_(0), cumul_ms_(0), state_(0)
{
  pinMode(this->heat_ctrl_pin_, OUTPUT);
  digitalWrite(this->heat_ctrl_pin_, LOW);
}
void ReflowOven::run()
{
  if (this->state_ == 0)
  {
    this->last_ms_ = millis();
    this->state_ = 1;
  }


  unsigned long curr_ms = millis();
  this->cumul_ms_ += (curr_ms - this->last_ms_);
  this->last_ms_ = curr_ms;


  // check remote                                                                                                                                                                                            
  if (this->remote_.available())
  {
    int indata = this->remote_.read();
    if (indata >= 0)
    {
      this->remote_.print((char) indata);
      this->onchar((char) indata);
    }
  }

  // check temp                                                                                                                                                                                              
  while (this->cumul_ms_ > 1000)
  {
    double temp = this->thermo_.readCelsius();
    //this->remote_.print("TEMP: ");                                                                                                                                                                         
    this->remote_.println(temp);
    if (temp != NAN)
    {
      if (temp < this->temp_c_)
      {
        digitalWrite(this->heat_ctrl_pin_, HIGH);
      }
      else
      {
         digitalWrite(this->heat_ctrl_pin_, LOW);
      }
    }
    this->cumul_ms_ -= 1000;
  }
}
void ReflowOven::setTempC(double temp_c)
{
  this->temp_c_ = temp_c;
}

void ReflowOven::onchar(char inchar)
{
  switch (inchar)
  {
    case '\r':
      this->inbuf_[this->inlen_++] = '\0';
      this->oncmd();
      this->inlen_ = 0;
      break;
    case '\n':
      break;
    default:
      this->inbuf_[this->inlen_++] = inchar;
      break;
  }
}
void ReflowOven::oncmd()
{
  switch (this->inbuf_[0])
  {
    case 'S':
      {
        char *res;
        double val = strtod(&this->inbuf_[1], &res);
        if (res != &this->inbuf_[1])
        {
          this->setTempC(val);

          this->remote_.print("SET: ");
          this->remote_.println(val);
        }
      }
    default:
      break;
  }
}

