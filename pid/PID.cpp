
#include "PID.h"


PID::PID(double* Input, double* Output, double* Setpoint,
        double Kp, double Ki, double Kd)
{
   myOutput = Output;
   myInput = Input;
   mySetpoint = Setpoint;

   PID::SetOutputLimits(-100, 0);				

   SampleTime = 1000;							

   PID::SetTunings(Kp, Ki, Kd);
   PID::Initialize();
//   lastTime = millis()-SampleTime;
}

bool PID::Compute()
{
//   unsigned long now = millis();
 //  unsigned long timeChange = (now - lastTime);
 //  if(timeChange>=SampleTime)
   if(1)
   {
      double input = *myInput;
      double error = *mySetpoint - input;
      double dInput = (input - lastInput);
      outputSum+= (ki * error);

      if(outputSum > outMax) 
      {
         outputSum= outMax;
      }
      else if(outputSum < outMin) 
      {
         outputSum= outMin;
      }

	   double output;
      output = kp * error;
    
      output += outputSum - kd * dInput;

	   if(output > outMax) 
      {
         output = outMax;
      }
      else if(output < outMin) 
      {
         output = outMin;
      }
	   *myOutput = output;
  
      lastInput = input;
      //lastTime = now;
	   return true;
   }
   else 
   {
      return false;
   }
}
void PID::Initialize()
{
   outputSum = *myOutput;
   lastInput = *myInput;
   if(outputSum > outMax) 
   {
      outputSum = outMax;
   }
   else if(outputSum < outMin) 
   {
      outputSum = outMin;
   }
}

void PID::SetTunings(double Kp, double Ki, double Kd)
{
   if (Kp<0 || Ki<0 || Kd<0) return;

   double SampleTimeInSec = ((double)SampleTime)/1000;
   kp = Kp;
   ki = Ki * SampleTimeInSec;
   kd = Kd / SampleTimeInSec;

}

void PID::SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)SampleTime;
      ki *= ratio;
      kd /= ratio;
      SampleTime = (unsigned long)NewSampleTime;
   }
}

void PID::SetOutputLimits(double Min, double Max)
{
   if(Min >= Max) return;
   outMin = Min;
   outMax = Max;


	if(*myOutput > outMax) 
   {
      *myOutput = outMax;
   }
	else if(*myOutput < outMin) 
   {
      *myOutput = outMin;
   }

	if(outputSum > outMax) 
   {
      outputSum = outMax;
   }
	else if(outputSum < outMin) 
   {
      outputSum = outMin;
   }
   
}


