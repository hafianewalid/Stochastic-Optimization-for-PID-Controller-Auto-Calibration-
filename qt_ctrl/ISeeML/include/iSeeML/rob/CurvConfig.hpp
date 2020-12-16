//
// Definition of iSeeML::rob::CurvConfig, class of ISeeML robotic 
// configurations with curvature.
//

// To only include this definition once
#ifndef ISEEML_ROB_CURV_CONFIG_HH
#define ISEEML_ROB_CURV_CONFIG_HH


// This class needs the definition of iSeeML::rob::OrPtConfig. 
#include <iSeeML/rob/OrPtConfig.hpp>


/** This class defines a more precise configuration than 
 ** @ref iSeeML::rob::OrPtConfig "classical ones" for mobile robots. 
 ** 
 ** This configuration contains, added to the classical position 
 ** of a reference point and orientation of a main axis, 
 ** the curvature of the curve followed by the reference point
 ** (representing the directing wheels' orientation). 
 ** 
 ** This class adds to those of @ref iSeeML::rob::OrPtConfig 
 ** a description method giving the @ref curvature, and redefines 
 ** description method @ref writeTo. 
 ** 
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::CurvConfig : public iSeeML::rob::OrPtConfig {
  // === Object's Fields ==========================================

  double Curvature;  ///< Curvature of the reference point's curve. 

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /** The default constructor should only be used for array 
   ** initializations:  it correspond to a default oriented 
   ** point with zero curvature. 
   ** 
   ** @see  default constructor of iSeeML::rob::OrPtConfig. 
   **/
  CurvConfig() : OrPtConfig(), Curvature(0) {} 

  /** The main constructor. 
   ** 
   ** @param OrPt  a classical configuration as an oriented point, 
   ** @param kappa the curvature of the reference point's curve. 
   **/
  CurvConfig(const iSeeML::rob::OrPtConfig& OrPt, const double& kappa) : 
    OrPtConfig(OrPt), Curvature(kappa) {}

  /** A first detailled constructor allows to give the classical 
   ** configuration as a point and an orientation. 
   ** 
   ** @param P     the position of the reference point, 
   ** @param theta the orientation of the main axis, 
   ** @param kappa the curvature of the reference point's curve. 
   ** 
   ** @see   main constructor of iSeeML::rob::OrPtConfig. 
   **/
  CurvConfig(const iSeeML::geom::Point& P, const double& theta, 
	     const double& kappa) : 
    OrPtConfig(P, theta), Curvature(kappa) {}

  /** A second detailled constructor allows to give the classical 
   ** configuration as two Cartesian coordinates and 
   ** an orientation. 
   ** 
   ** @param x     the X coordinate of the reference point, 
   ** @param y     the Y coordinate of the reference point, 
   ** @param theta the orientation of the main axis, 
   ** @param kappa the curvature of the reference point's curve. 
   ** 
   ** @see   usefull constructor of iSeeML::rob::OrPtConfig. 
   **/
  CurvConfig(const double& x, const double& y, 
	     const double& theta, const double& kappa) : 
    OrPtConfig(x, y, theta), Curvature(kappa) {}

  // The default copy constructor is OK. 


  // === Description Methods ======================================

  // cf. OrPtConfig
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new CurvConfig(*this)) ); } 


  // Description is given in OrPtConfigISeeML.hh. 
  void writeTo(ostream& O) const { algWriteTo(O); } 


  /** Description method, giving the intantaneous curvature, 
   ** in this configuration, of the curve followed by the robot's 
   ** reference point. 
   **
   ** @return      the intantaneous curvature of the robot's 
   **              reference point's curve. 
   **/
  const double& curvature() const  { return Curvature; } 


protected:
  /** Description method, giving the dimension of 
   ** the containing space (4) when this configuration 
   ** is considered as an algebraic vector. 
   **
   ** This representation is mainly used to define 
   ** @ref algWriteTo "a unified writeTo". 
   ** Dimension cannot be static, as it is virtual. 
   **/ 
  int algDimension() const { return 4; } 

  /** Description method, giving the coordinate of given index 
   ** of the configuration (both coordinates of the position, 
   **  then orientation, then curvature). 
   **
   ** @param  i  the index of the searched coordinate.
   **
   ** @pre the parameter is an index, and should be between one 
   ** and four.  If not, and @c ISEEML_CHECK_CURV_CONFIG_PRECOND 
   ** is defined (see CompilerFlags.h), an error is generated. 
   **
   ** @return    the double coordinate associated to the index 
   **            if this one is between 1 and dimension, or 0.
   **
   ** @see       curvature, iSeeML::rob::OrPtConfig::algCoord. 
   **/
  double algCoord(const int i) const { 
    if (i == 4) return curvature(); 
#ifdef ISEEML_CHECK_CURV_CONFIG_PRECOND
    else if ((i < 1) || (i > 3)) {
      cerr << ClassName << "::coord: parameter " << i << endl
	   << " outside allowed values (1 to 4)," 
	   << " returning zero..." << endl;
      return 0; }
#endif
    else return OrPtConfig::algCoord(i); } 


  // === Modification Methods =====================================

  // === Operators ================================================

  // === Various Methods ==========================================

}; // end of class iSeeML::rob::CurvConfig

#endif  // end of definition 

