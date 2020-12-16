//
// Definition of iSeeML::rob::LinCurvPath, class of only ISeeML 
// elementary robotic paths.
//

// To only include this definition once
#ifndef ISEEML_CONT_CURV_PATH_HH
#define ISEEML_CONT_CURV_PATH_HH


// This class needs the definition of iSeeML::rob::BasicPath. 
#include <iSeeML/rob/BasicPath.hpp>
// This class needs the definition of iSeeML::rob::CurvConfig. 
#include <iSeeML/rob/CurvConfig.hpp>


/** This class defines linear curvature @ref iSeeML::rob::BasicPath 
 ** "elementary paths", for which the curvature derivative (with 
 ** respect to the arc length) is constant. 
 ** These are the only @ref iSeeML::rob::BasicPath "elementary paths" 
 ** of ISeeML, the other paths being made of paths of this kind 
 ** (they are @ref iSeeML::rob::CompoundPath "compound paths"). 
 ** 
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::LinCurvPath : public iSeeML::rob::BasicPath {
  // === Object's Fields ==========================================

  /// Constant curvature derivative with respect to the arc length.
  double Sharpness; 

  double Length;     ///< Length of the path.

  /// Deflection (change of the orientation) of the path. 
  double Deflection; 

  /// Starting configuration (position, orientation and curvature).
  iSeeML::rob::CurvConfig Start;

  /// Final configuration (position, orientation and curvature).
  iSeeML::rob::CurvConfig End; 

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/ 
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /** The default constructor should only be used for array 
   ** initializations:  it generates a path starting from default 
   ** configuration, with constant curvature and zero length. 
   ** 
   ** @see  default constructor of iSeeML::rob::CurvConfig. 
   **/
  LinCurvPath() : Sharpness(0), Length(0), Deflection(0), 
    Start(), End() {} 

  /** The main constructor. 
   **
   ** @param start      the starting configuration, 
   ** @param sharpness  the constant curvature derivative 
   **                   with respect to the arc length (default 
   **                    value is zero), 
   ** @param length     the length of the path (default value is 
   **                    zero).
   ** 
   ** @see  operator[].
   **/
  LinCurvPath(const iSeeML::rob::CurvConfig& start, 
	      const double& sharpness = 0, const double& length = 0) : 
    Sharpness(sharpness), Length(length), 
    Deflection(start.curvature() * length + 
	       sharpness * sqr(length) / 2), 
    Start(start), End( operator[]( length ) ) {}

  // The default copy constructor is OK. 


  // === Description Methods ======================================

  /** Description method, giving the path's starting configuration. 
   ** (as a configuration for mobile robot with curvature). 
   **
   ** @return    the path's starting configuration. 
   **/
  const iSeeML::rob::CurvConfig& start() const { return Start; }

  /** Description method, giving the path's final configuration
   ** (as a configuration for mobile robot with curvature). 
   **
   ** @return    the path's final configuration. 
   **/
  const iSeeML::rob::CurvConfig& end()   const { return End; }


  /** Description method, giving the path's constant curvature 
   ** derivative with respect to the arc length. 
   **
   ** @return    the path's curvature derivative. 
   **/
  const double& sharpness()  const { return( Sharpness ); }

  // Description is given in PathISeeML.hh
  double length()     const { return( Length ); }

  // Description is given in PathISeeML.hh
  double deflection() const { return( Deflection ); }


  /** Description method, giving a copy of the current 
   ** linear curvature path. 
   **
   ** This clone is dynamically allocated (using default copy 
   **  constructor), it has to be deleted later.
   **
   ** @return  a copy/clone of the current linear curvature path. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new LinCurvPath(*this)) ); } 


  /** Description method, writing a description of the path
   ** in a given output stream. 
   **
   ** The starting configuration is given, then the path parameters 
   ** (sharpness and length) between braces, then the final 
   ** configuration, the whole between brackets. 
   **
   ** @param  O  the output stream in which description is written. 
   **/
  void writeTo(ostream& O) const 
  { O << "[" << start() << "-{" << sharpness() << ", " 
      << length() << "}->" << end() << "]"; } 


public:
  // === Operators ================================================

  /** Description method, giving a configuration at a given arc 
   ** length along the path. 
   **
   ** @param  s  the arc length. 
   ** 
   ** @pre the given arc length should be positive and less than 
   ** the path's @ref length. 
   ** If this is not true, an error message is generated if 
   ** @c ISEEML_CHECK_LIN_PATH_PRECOND is defined, and the arc 
   ** length is considered as zero if smaller and as path's 
   ** @ref length if bigger, if @c ISEEML_CHECK_ARRAY_ELEMT is 
   ** defined (see CompilerFlags.h).
   **
   ** @return    the configuration at given arc length along 
   **            the path. 
   **/
  iSeeML::rob::CurvConfig operator[](const double& s) const; 


  // === Various Methods ==========================================

}; // end of class iSeeML::rob::LinCurvPath

#endif  // end of definition 

