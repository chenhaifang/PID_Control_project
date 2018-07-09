#include "PID.h"
#include <limits>
#include <iostream>



using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	parameter.push_back(Kp);
	parameter.push_back(Ki);
	parameter.push_back(Kd);
	
	this->p_error = 99999999.;
	this->d_error = 0.0;
	this->i_error = 0.0;
	
	//twiddle parameters
	need_twiddle = true;
	
	step = 1;
	// let every 50 steps add the cte to the total_error
	val_step = 50;
	
	for (int i = 0; i < 3; ++i) {
		 // init the change rate with the value of 0.1*parameter
		changes.push_back(0.1* parameter[i]);
	}
	index_param = 0;
	
	best_error = std::numeric_limits<double>::max();
	total_error = 0;
	// fail to make the total_error better times
	fail_counter = 0;

    
}

void PID::UpdateError(double cte) {

	  d_error = cte - prev_cte;
	  prev_cte = cte;
	  p_error = cte;
	  i_error += cte;


	  #if 1
	
	  if(need_twiddle)
	  {
		  if(step % (val_step) > 0)
		  {
			  total_error += cte;
		  }	
		  if(step % val_step == 0)
		  	{
			  std::cout<<"==============  step "<<step<<" =============="<<std::endl;
			  std::cout << "P: "<< parameter[0]<<" I: "<<parameter[1]<<" D: "<<parameter[2]<<std::endl;
			
		      if(fabs(total_error) < best_error)
			  {
					  best_error = fabs(total_error);
					  changes[index_param] *= 1.1;
					  IndexMove();
					  parameter[index_param] += changes[index_param];
					  fail_counter = 0;
				  } else if(fail_counter ==0) 
				  {
					  parameter[index_param] -= (2*changes[index_param]);
					  fail_counter++;					  

				  } else{
					  parameter[index_param] += changes[index_param];
					  changes[index_param] *= 0.9;
					  IndexMove();
					  parameter[index_param] += changes[index_param];
					  fail_counter = 0;
				  }
	
			  std::cout << "best_error: "<< best_error<<" total_error: "<<total_error<<std::endl;
			  std::cout << "change_index: "<<index_param<<" new_parameter: "<<parameter[index_param]<<std::endl;
			  std::cout <<	std::endl;
			  total_error = 0;
		  }
	  }
	  step++;
	  #endif


    
}

double PID::TotalError() {

    double steer = -parameter[0] * p_error - parameter[1] * i_error - parameter[2] * d_error;

	//std::cout << "P: "<< parameter[0]<<" I: "<<parameter[1]<<" D: "<<parameter[2]<<std::endl;
	return steer;
}

void PID::IndexMove() {
    index_param++;
    if(index_param >=3){
        index_param = 0;
    }
}


