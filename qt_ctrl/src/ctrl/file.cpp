/**
 * @file  src/ctrl/file.cpp
 *
 * @brief File controller (teleoperation) class.
 *
 * @date  October 2018
 **/


#include <ctrl/file.hpp>


// Cf Controller::chooseVelocities(...)
void FileCtrl::chooseVelocities(double& trans_vel, double& rot_vel,
				std::ostream& log_str) {
  static double remain_dur = 0, duration, moving_acc, turning_acc; 
  if ( inputOK() ) {  // only works if input file is OK
    // when needed, get new accelerations from file
    if ( remain_dur < timeStep() ) {
      // read new line in input stream
      *input >> duration >> moving_acc >> turning_acc;
      if ( inputEnded() )  // stop at the end of file
	{ moving_acc = turning_acc = 0; duration = 10; }
      remain_dur += duration;
    } // apply accelerations during time step to fin future velocities
    motion_model.applyAccelerations
      (moving_velocity, turning_velocity,
       moving_acc, turning_acc, time_step);
    /* log when needed 
    log_str << "+(" << moving_acc << ", " << turning_acc
    << ", " << time_step << "/" << remain_dur << ")"; */
    // set velocities
    updateVelocities(trans_vel, rot_vel); 
    remain_dur -= timeStep();  // decrease remaining time
  } // end of if (input OK)
} // end of void FileCtrl::chooseVelocities(double&, double&, ...) ---
