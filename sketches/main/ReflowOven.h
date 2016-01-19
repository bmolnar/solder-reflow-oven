#ifndef REFLOWOVEN_H_
#define REFLOWOVEN_H_

#include <Stream.h>

#include <max6675.h>

class ReflowOven
{
 private:
  Stream& remote_;
  MAX6675 thermo_;
  int heat_ctrl_pin_;
  double temp_c_;
  char inbuf_[16];
  unsigned char inlen_;

  unsigned long last_ms_;
  unsigned long cumul_ms_;
  unsigned char state_;

  void onchar(char inchar);
  void oncmd();

 public:
  ReflowOven(Stream& remote, int thermo_clk_pin, int thermo_cs_pin, int thermo_do_pin, int heat_ctrl_pin);
  void run();
  void setTempC(double temp_c);
};

#endif REFLOWOVEN_H_

