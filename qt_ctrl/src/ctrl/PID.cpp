/**
 * @file  src/ctrl/PID.cpp
 *
 * @brief Path following controller class, using a PID.
 *
 * @date  November 2018
 **/


#include <ctrl/PID.hpp>


// Cf Controller::chooseVelocities(...)
void PIDCtrl::chooseVelocities(double& trans_vel, double& rot_vel,
			       std::ostream& log_str) {
  // updates the goal to be one time step after the robot's date
  searchGoal(time_step);
  const iSeeML::rob::OrPtConfig& robot_cfg = state.configuration(),
    goal_cfg = (*goal)->configuration();
  const State new_goal( (*goal)->date(),
			robot_cfg.projection(goal_cfg),
			(*goal)->translationVelocity(),
			(*goal)->rotationVelocity() );
  // sets the goal of the local controller, in the robot's frame
  reachCtrl.changeGoal(new_goal);
  // uses the local controller
  reachCtrl.chooseVelocities(trans_vel, rot_vel, log_str);
} // end of void PIDCtrl::chooseVelocities(double&, double&, ...) ----
