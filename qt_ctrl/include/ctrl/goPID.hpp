/**
 ** @file  include/ctrl/goPID.hpp
 **
 ** @brief PID controller reaching an oriented point.
 **
 ** @date  December 2019
 **/


#ifndef QTCTRL_PID_REACH_CTRL
#define QTCTRL_PID_REACH_CTRL

#include <ctrl/reach.hpp>
#include <iSeeML/rob/OrPtConfig.hpp>

/** @brief PIDreachCtrl aims at reaching an oriented point 
 ** with the robot, using a PID controller.
 **
 ** @since 0.3.2
 */
class PIDreachCtrl : public ReachingCtrl {
  double trans_prop_coef;   ///< Translation proportional coefficient.
  double trans_integ_coef;  ///< Translation integration coefficient.
  double trans_deriv_coef;  ///< Translation derivative coefficient.
  double rot_prop_coef;     ///< Rotation proportional coefficient.
  double rot_integ_coef;    ///< Rotation integration coefficient.
  double rot_deriv_coef;    ///< Rotation derivative coefficient.

  double integr_dist;       ///< Integrated distance.
  double integr_angle_diff; ///< Integrated angle difference.
  
public:
  /** @brief The constructor needs a motion model, the time step 
   **        of the controller, a goal state and 
   **        the PID coefficients.
   ** 
   ** @param model            the motion model,
   ** @param ts               the time step of the controller,
   ** @param goal_aimed       the goal state,
   ** @param coef             the coefficient array.
   ** 
   ** @see ReachingCtrl(const MotionModel&, const double&, 
   **                   const iSeeML::rob::OrPtConfig&)
   **/
  PIDreachCtrl(const MotionModel& model, const double& ts,
	       const State& goal_aimed, const double coef[6])
    : ReachingCtrl(model, ts, goal_aimed),
      trans_prop_coef(coef[0]), trans_integ_coef(coef[1]),
      trans_deriv_coef(coef[2]), rot_prop_coef(coef[3]), 
      rot_integ_coef(coef[4]), rot_deriv_coef(coef[5]),
      integr_dist(0), integr_angle_diff(0) {}

  /// @brief The destructor needs to be explicitely redefined.
  virtual ~PIDreachCtrl() {}
  
  // Cf Controller::choose_velocities(double&, double&, ...)
  void chooseVelocities(double& trans_vel, double& rot_vel,
			std::ostream&);

};  // end of class PIDreachCtrl

#endif // QTCTRL_PID_REACH_CTRL
