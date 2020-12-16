//
// Definition of iSeeML::rob::Path, superclass of every other ISeeML 
// robotic paths' class.
//

// To only include this definition once
#ifndef ISEEML_ROB_PATH_HH
#define ISEEML_ROB_PATH_HH


// This class needs the definition of iSeeML::rob::Object. 
#include <iSeeML/rob/Object.hpp>
#include <iSeeML/rob/CurvConfig.hpp>


/** This class is the base class of all robotic paths in 
 ** ISeeML.
 **
 ** A path is the geometric aspect of a robot's motion. 
 ** It is therefore a continuous set of 
 ** @ref iSeeML::rob::OrPtConfig "configurations", and transitions 
 ** between these configurations respect the kinematic constraints 
 ** of this robot as, for example, continuity of the robot's 
 ** orientation, bounds on the curvature (the inverse of 
 ** the turning radius), continuity of the curvature, bounds on 
 ** its derivative, etc.
 ** 
 ** This class contains the virtual methods common to all robotic 
 ** paths' classes, which are only description methods, 
 ** an order operator<(const Path&), which uses 
 ** the virtual description method @ref length, and a description 
 ** operator, operator[](const double&), returning 
 ** the @ref iSeeML::rob::CurvConfig "configuration" at a given arc 
 ** length.
 **
 ** Note that @ref start "starting" and @ref end "final" 
 ** configurations are referenced as 
 ** @ref iSeeML::rob::OrPtConfig "classical configurations" 
 ** (to which 
 **  a @ref iSeeML::rob::CurvConfig "curvature configuration"'s 
 **  reference is automatically transformed), while configurations 
 ** at a given arc length are computed when needed, and are given 
 ** as @ref iSeeML::rob::CurvConfig "curvature configuration" 
 ** (to which 
 **  a @ref iSeeML::rob::OrPtConfig "classical configuration" is 
 **  automatically transformed). 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::Path : public iSeeML::rob::Object {
  // === Object's Fields ==========================================

  /** The class name is private, as this class is abstract. 
   **
   ** @see className. 
   **/
  static const string ClassName; 

public:
  // === Constructors and Destructors =============================

  // None (destructor is already virtual, due to Object).


  // === Description Methods ======================================

  /** Description method, giving the path's starting configuration. 
   **
   ** @return    the path's starting configuration. 
   **/
  virtual const iSeeML::rob::OrPtConfig& start() const = 0;

  /** Description method, giving the path's final configuration. 
   **
   ** @return    the path's final configuration. 
   **/
  virtual const iSeeML::rob::OrPtConfig& end()   const = 0;


  /** Description method, giving the path's length. 
   **
   ** @return    the path's length. 
   **/
  virtual double length()       const = 0;

  /** Description method, giving the path's deflection (change 
   ** of orientation). 
   **
   ** @return    the path's deflection. 
   **/
  virtual double deflection()   const = 0;


  // === Modification Methods =====================================

  // === Operators ================================================

  /** Order operator, defining an order relation on the paths' set, 
   ** based on the path's length. 
   **
   ** In other words, a path is smaller than a second one iff it 
   ** is shorter (its length is smaller than the second one's). 
   ** However, zero length paths are generally not correct: 
   ** such a path is consider as shorter if and only if the second 
   ** one is also a zero length path. 
   **
   ** @param  other  another path. 
   **
   ** @return        whether the current path is smaller (shorter) 
   **                than the given one. 
   **
   ** @see           length, iSeeML::Object::isPositive. 
   **/
  bool operator<(const Path& other) const 
    // Cf. previous comment
  { return( ( isPositive( length() ) && ( length() < other.length() ) ) 
	    || (! isPositive( other.length() ) ) ); }


  /** Description method, giving a configuration at a given arc 
   ** length along the path. 
   **
   ** @param  s  the arc length. 
   ** 
   ** @pre    the given arc length should be positive and less 
   ** than the path's @ref length. 
   **
   ** @return    the configuration at given arc length along 
   **            the path. 
   **/
  virtual iSeeML::rob::CurvConfig operator[](const double& s) const = 0; 

  // === Various Methods ==========================================

}; // end of class iSeeML::rob::Path

#endif  // end of definition 

