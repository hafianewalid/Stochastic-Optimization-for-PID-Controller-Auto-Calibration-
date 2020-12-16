/**
 ** @file  src/ctrl/goPID.cpp
 **
 ** @brief PID controller reaching an oriented point.
 **
 ** @date  December 2019
 **/


#include <ctrl/goPID.hpp>


// Cf Controller::chooseVelocities(...)
void PIDreachCtrl::chooseVelocities(double& trans_vel, double& rot_vel,
				    std::ostream& log_str) {
  // gets the goal
  const State& goal = getGoal();  
  // estimate the distance and angle difference to goal's config,
  // as well as its derivative and its integration
  const iSeeML::rob::OrPtConfig& goal_config = goal.configuration(),
    config = state.configuration();
  const iSeeML::geom::Vector v
    = goal_config.position() - config.position(); 
  const double dist = v.length(), dir = v.orientation(),
    // === Following formula may be changed ==========================
    angle_diff = iSeeML::Object::mod2pi( dir + goal_config.orientation()
					 - config.orientation() ),
    trans_vel_diff = goal.translationVelocity()
    - state.translationVelocity(),
    rot_vel_diff = goal.rotationVelocity()
    - state.rotationVelocity();
  // =================================================================
  integr_dist += dist * time_step;
  integr_angle_diff += angle_diff * time_step; 
  // computes the accelerations
  const double trans_acc = trans_prop_coef * dist + trans_deriv_coef 
    * trans_vel_diff + trans_integ_coef * integr_dist,
    rot_acc = rot_prop_coef * angle_diff + rot_deriv_coef 
    * rot_vel_diff + rot_integ_coef * integr_angle_diff;
  // gets the goal's velocities
  motion_model.applyAccelerations
    (moving_velocity, turning_velocity,
     trans_acc, rot_acc, time_step);
  // updates the parameters and send the update signal
  updateVelocities(trans_vel, rot_vel);
} // end of void PIDreachCtrl::chooseVelocities(double&, ...) --------
