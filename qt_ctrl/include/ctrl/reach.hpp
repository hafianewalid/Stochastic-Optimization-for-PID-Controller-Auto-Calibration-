/**
 ** @file  include/ctrl/reach.hpp
 **
 ** @brief Oriented point reaching controller abstract class.
 **
 ** @date  June 2019
 **/


#ifndef QTCTRL_REACH_CTRL
#define QTCTRL_REACH_CTRL

#include <model/state.hpp>
#include <ctrl/file.hpp>

/** 
 ** @brief ReachingCtrl aims at reaching a given state with the robot.
 **
 ** The aimed state will be considered to be given in the robot's 
 ** original frame (defined by the original oriented point, 
 **  as given by the first odometry data).
 **
 ** @since 0.3.2
 */
class ReachingCtrl : public Controller {

  /// @brief The state which is aimed.
  State goal;
  
protected:
  State state;                  ///< @brief The state of the robot.
  
  /* Cf Controller::newState(const State&) */
  void newState(const State& state) {
    this->state = state; 
    if ( firstOdometry() )
      // move the goal into the state's frame
      setGoal(goal + state.configuration()); 
    Controller::newState(state); 
  }

  /// @brief Set the oriented point which is aimed.
  /// @param The new aimed oriented point.
  virtual void setGoal(const State& new_goal) { goal = new_goal; }
    
public:
  /** @brief The constructor needs a motion model, a time step and 
   **        a goal state (in the robot's frame).
   ** 
   ** @param model       the motion model,
   ** @param ts          the time step of the controller,
   ** @param goal_aimed  the goal configuration.
   ** 
   ** @see Controller(const MotionModel&, const double&)
   **/
  ReachingCtrl(const MotionModel& model, const double& ts,
	       const State& goal_aimed)
    : Controller(model, ts), goal(goal_aimed) {}

  /// @brief Get the state which is aimed.
  /// @return The state which is aimed.
  const State& getGoal() const { return goal; }
  
  /// @brief Change the oriented point which is aimed.
  /// @param goal  the new aimed state, in the robot's frame.
  virtual void changeGoal(const State& goal)
  // Set the goal in local frame & ask for future set in global frame
  { setGoal(goal); changeFirstOdometry(); }
    
};  // end of class ReachingCtrl

#endif // QTCTRL_REACH_CTRL
