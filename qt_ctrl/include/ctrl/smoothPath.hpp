/**
 ** @file  include/ctrl/smoothPath.hpp
 **
 ** @brief Oriented point reaching controller class 
 **        using a smooth path generator.
 **
 ** @date  June 2019
 **/


#ifndef QTCTRL_SMOOTH_PATH_CTRL
#define QTCTRL_SMOOTH_PATH_CTRL

#include <ctrl/reach.hpp>
#include <iSeeML/rob/FscPath.hpp>

/** @brief SmoothPathCtrl aims at reaching an oriented point 
 ** with the robot, using a smooth path generator.
 **
 ** A smooth path reaching the aimed oriented point is computed,
 ** and followed blindly (without recomputation with respect
 **  to odometry).
 **
 ** @since 0.3.2
 */
class SmoothPathCtrl : public ReachingCtrl {
  Q_OBJECT  // This macro is needed to handle signals
  
  iSeeML::rob::FscPath path;  ///< @brief The computed smooth path.
  double total_distance;         ///< @brief The distance to cover.
  double distance;        ///< @brief The distance already covered.

  // Cf ReachingCtrl::setGoal(const State&)
  void setGoal(const State& goal);
    
public:
  /** @brief The constructor needs a motion model, a time step and 
   **        a goal state.
   ** 
   ** @param model       the motion model,
   ** @param ts          the time step of the controller,
   ** @param goal_aimed  the goal state.
   ** 
   ** @see ReachingCtrl(const MotionModel&, const double&, 
   **                   const State&) 
   **/
  SmoothPathCtrl(const MotionModel& model, const double& ts,
		 const State& goal_aimed)
    // the path will be correctly set at first odometry data
    : ReachingCtrl(model, ts, goal_aimed) {}

  /// @brief The destructor needs to be explicitely redefined.
  virtual ~SmoothPathCtrl() {}
  
  /// @brief Get the path to the aimed oriented point.
  /// @return The path to the aimed oriented point.
  const iSeeML::rob::Path& getPath() const;
  
  /// @brief Transform the planned path into an aimed trajectory.
  /// @return The trajectory obtained from the planned path.
  const std::list<State*>& getAimedTrajectory() const;
  
  // Cf Controller::choose_velocities(double&, double&, ...)
  void chooseVelocities(double& trans_vel, double& rot_vel,
			std::ostream&);

Q_SIGNALS:  // new signal

  /// @brief Update the display of the path. 
  void pathChanged();

};  // end of class SmoothPathCtrl

#endif // QTCTRL_SMOOTH_PATH_CTRL
