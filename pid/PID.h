#ifndef PID_h
#define PID_h


class PID
{
public:
  #define P_ON_M 0
  #define P_ON_E 1
  PID(double*, double*, double*, double, double, double);    

  bool Compute();                       
  void SetOutputLimits(double, double);
  void SetTunings(double, double, double);
  void SetSampleTime(int);            						  
  void Initialize();

private:
	double kp;                  
  double ki;                  
  double kd;                  

  double *myInput;             
  double *myOutput;            
  double *mySetpoint;          
			  
	unsigned long lastTime;
	double outputSum, lastInput;

	unsigned long SampleTime;
	double outMin, outMax;

};
#endif

