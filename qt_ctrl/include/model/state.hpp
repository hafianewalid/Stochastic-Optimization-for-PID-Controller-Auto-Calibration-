/**
 ** @file  include/model/state.hpp
 **
 ** @brief State class, i.e. configuration and velocities.
 **
 ** @date  November 2018
 **/

#ifndef QTCTRL_STATE
#define QTCTRL_STATE


#include <iSeeML/rob/OrPtConfig.hpp>


/** 
 ** @brief This class defines a state, i.e. a configuration and
 **        its (translation and rotation) velocities.
 ** 
 ** @since 0.3.1
 */
class State : public iSeeML::rob::Object {
  double t;                        ///< Date of the state.
  iSeeML::rob::OrPtConfig config;  ///< Configuration of the state.
  double translation_vel;       ///< Translation velocity of the state.
  double rotation_vel;             ///< Rotation velocity of the state.

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 

  /** @brief The default constructor should only be used for array 
   **        initializations:  it correspond to a default point with 
   **        zero orientation. 
   ** 
   ** @see  default constructor of Config. 
   **/
  State() : t(0), config(), translation_vel(0), rotation_vel(0) {} 
  
  /** @brief The main constructor. 
   ** 
   ** @param date       the date of the state, 
   ** @param q          the configuration of the state, 
   ** @param trans_vel  the translation velocity of the state, 
   ** @param rot_vel    the rotation velocity of the state. 
   **/
  State(const double& date, const iSeeML::rob::OrPtConfig& q,
	const double& trans_vel, const double& rot_vel)
    : t(date), config(q), translation_vel(trans_vel),
      rotation_vel(rot_vel) {}
  
  // The default copy constructor is OK. 

  /** @brief Gives the configuration of the state. 
   **
   ** @return  the configuration of the state. 
   **/
  const double& date() const { return t; } 

  /** @brief Gives the configuration of the state. 
   **
   ** @return  the configuration of the state. 
   **/
  const iSeeML::rob::OrPtConfig& configuration() const
  { return config; } 

  /** @brief Gives the translation velocity of the state.
   **
   ** @return    the translation velocity of the state.
   **/
  const double& translationVelocity() const
  { return translation_vel; } 

  /** @brief Gives the rotation velocity of the state.
   **
   ** @return    the rotation velocity of the state.
   **/
  const double& rotationVelocity() const { return rotation_vel; } 

  /** @brief Equality operator between states. 
   **
   ** @param  other  another state. 
   **
   ** @return        whether the two states are equal 
   **                (same configuration and velocities). 
   **
   ** @see  configuration, Config::operator==, translation_velocity,
   **       rotation_velocity, isZero. 
   **/
  bool operator==(const State& other) const { 
    return( ( configuration() == other.configuration() ) && 
	    isZero( translationVelocity()
		    - other.translationVelocity() ) && 
	    isZero( rotationVelocity() - other.rotationVelocity() )
	    ); } 

  /** Description method, giving a copy of the current 
   ** configuration. 
   **
   ** This clone is dynamically allocated (using default copy 
   **  constructor), it has to be deleted later.
   **
   ** @return  a copy/clone of the current configuration. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new State(*this)) ); } 

  /// @brief Writes the configuration's values in an output stream.
  /// @param O  the output stream in which the description is written.
  /// @see ostream::operator<<(const Object&)
  void writeTo(std::ostream& O) const {
    const iSeeML::rob::OrPtConfig& q = configuration();
    const iSeeML::geom::Point&  P = q.position(); 
    O << '(' << P.xCoord() << ", " << P.yCoord() << ", "
      << q.orientation() << ", " << translationVelocity()
      << ", " << rotationVelocity() << ')';
  } // end of void writeTo(std::ostream&) const
  
  /** @brief Returns the addition of a configuration 
   **        to the current state. 
   **
   ** @param   q  a configuration. 
   **
   ** @return  the state in the configuration's frame. 
   **
   ** @see  operator+=, copy operator. 
   **/
  State operator+(const iSeeML::rob::OrPtConfig& q) const 
  { State res(*this);  res += q;  return(res); } 

  /** @brief Adds a configuration to the current state:
   **        moves the state in the configuration's frame. 
   **
   ** @param   q  a configuration. 
   **
   ** @see  configuration, Config::operator+, translation_velocity,
   **       rotation_velocity. 
   **/
  void operator+=(const iSeeML::rob::OrPtConfig& q) { config += q; } 

}; // end of class State

#endif // QTCTRL_STATE
