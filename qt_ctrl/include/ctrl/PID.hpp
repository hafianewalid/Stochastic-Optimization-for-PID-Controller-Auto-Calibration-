/**
 ** @file  include/ctrl/PID.hpp
 **
 ** @brief Path following controller class, using a PID.
 **
 ** @date  November 2018
 **/


#ifndef QTCTRL_PID_CTRL
#define QTCTRL_PID_CTRL

#include <ctrl/track.hpp>
#include <ctrl/goPID.hpp>


/** 
 ** @brief PIDCtrl uses a PID to compute the trajectory's velocities.
 **
 ** You need to find the right coefficients.
 **
 ** @since 0.3.1
 */
class PIDCtrl : public TrackingCtrl {
  PIDreachCtrl reachCtrl;   ///< The local PID reaching controller.
  
public:
  /** @brief The constructor needs a motion model, the time step 
   **        of the controller, a file name giving the path to track, 
   **        an initial configuration and the PID coefficients.
   ** 
   ** @param model            the motion model,
   ** @param ts               the time step of the controller,
   ** @param input_file_name  the input file name of the path,
   ** @param init_config      the initial configuration,
   ** @param coef             the coefficient array.
   ** 
   ** @see TrackingCtrl::TrackingCtrl
   **/
  PIDCtrl(const MotionModel& model, const double& ts,
	  const char* input_file_name,
	  const iSeeML::rob::OrPtConfig& init_config, 
	  const double coef[6])
    : TrackingCtrl(model, ts, input_file_name, init_config),
      reachCtrl(model, ts, State(), coef) {}
  
  /// @brief The destructor needs to be explicitely redefined.
  virtual ~PIDCtrl() {}
  
protected:
  // Cf Controller::choose_velocities(double&, double&, ...)
  virtual void chooseVelocities(double& trans_vel, double& rot_vel,
				std::ostream& log_str);
  
}; // end of class PIDCtrl

#endif // QTCTRL_PID_CTRL
