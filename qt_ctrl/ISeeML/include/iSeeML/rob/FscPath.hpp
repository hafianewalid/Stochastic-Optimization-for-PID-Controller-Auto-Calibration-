//
// Definition of iSeeML::rob::FscPath, class of FSC paths 
// (Forward-only Sub-optimal Continuous-curvature paths).
//

// To only include this definition once
#ifndef ISEEML_FSC_PATH_HH
#define ISEEML_FSC_PATH_HH


// This class needs the definition of iSeeML::rob::DubinsLikePath. 
#include <iSeeML/rob/DubinsLikePath.hpp>


/** Objects of this class are Forward-only Sub-optimal 
 ** Continuous-curvature (FSC) paths, made of circular arcs, 
 ** pieces of clothoid and line segments.
 ** 
 ** They are similar to @ref iSeeML::rob::DubinsPath "Dubins' paths", 
 ** but their curvature have a continuous profile (pieces of 
 ** clothoid connecting line segments and circular arcs). 
 ** 
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::FscPath : public iSeeML::rob::DubinsLikePath {
  // === Object's Fields ==========================================

  /// Maximum curvature's derivative along the path.
  double MaximumCurvDerivative; 

  /** @name Often used values
   **
   ** The three following fields correspond to values often used 
   ** in computations.
   ** They are added to the path's description to speed up 
   ** these computations.
   **/
  //@{ 

  /** Limit deflection: a turn of lower (but not zero) deflection 
   ** is degenerated (it is made of two pieces and does not reach
   **  maximum curvature derivative nor maximum curvature), a turn 
   ** of bigger deflection is normal (it is made of three pieces, 
   **  the middle one being a circular arc of maximum curvature, 
   **  and the other two clothoid pieces of maximum curvature 
   **  derivative). 
   **/ 
  double LimDefl; 


  /** Turning radius, computed from curvature and curvature's 
   ** derivative maxima.
   **
   ** @see turnRadius.
   **/ 
  double TurnRadius; 

  /** Turning angle, computed from curvature and curvature's 
   ** derivative maxima.
   **
   ** @see turnAngle.
   **/ 
  double TurnAngle; 
  //@}

  // Array of nbPossiblePaths Dubins' paths. 
  static FscPath computationArray[nbPossiblePaths]; 

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /** The default constructor should only be used for array 
   ** initializations:  it generates a path starting from default 
   ** oriented point, with type lsl, and zero maximum curvature,
   ** maximum curvature's derivative and lengths. 
   **
   ** @pre       memory should not be full.
   ** If memory is full and @c ISEEML_CHECK_NIL_POINTER is defined 
   ** (see CompilerFlags.h), an error message is generated and 
   ** the program exits. 
   ** 
   ** @see  default constructor of iSeeML::rob::DubinsLikePath. 
   **/
  FscPath() : DubinsLikePath(lsl, 0), MaximumCurvDerivative(0), 
    LimDefl(0), TurnRadius(-1), TurnAngle(-M_PI) {} 

  /** The 'forward' constructor:  a FSC path is built from 
   ** its starting oriented point, its type, its maximum curvature,  
   ** maximum curvature's derivative and parts' lengths. 
   **
   ** @param start    the starting configuration, 
   ** @param type     the type of Dubins' path, 
   ** @param maxCurv  the maximum curvature along the path 
   **                 (taken in absolute value), 
   ** @param maxCDer  the maximum curvature's derivative along 
   **                 the path (taken in absolute value), 
   ** @param length1  the length of the first  part of the path, 
   ** @param length2  the length of the second part of the path, 
   ** @param length3  the length of the third  part of the path. 
   **/
  FscPath(const iSeeML::rob::OrPtConfig& start, const Type& type, 
	  const double& maxCurv,   const double& maxCDer, 
	  const double& length1,   const double& length2, 
	  const double& length3) :
    // Max curv., Max curv.'s deriv. and type are directly given
    DubinsLikePath(type, fabs(maxCurv)), 
    MaximumCurvDerivative( fabs(maxCDer) ) { 
    // the rest is computed (values and pieces) 
    computeValues(maxCurv, maxCDer, 
		  LimDefl, TurnRadius, TurnAngle); 
    definePieces(start, length1, length2, length3); } 
  
  /** The complete 'forward' constructor:  a FSC path is built from 
   ** its starting oriented point, its type, its maximum curvature,  
   ** maximum curvature's derivative, limit deflection, turning 
   ** radius, turning angle, and parts' lengths. 
   **
   ** @warning  this constuctor only works if limit deflection, 
   ** turning radius and turning angle have been correctly computed
   ** (using method @ref computeValues). 
   **
   ** @param start    the starting configuration, 
   ** @param type     the type of Dubins' path, 
   ** @param maxCurv  the maximum curvature along the path 
   **                 (taken in absolute value), 
   ** @param maxCDer  the maximum curvature's derivative along 
   **                 the path (taken in absolute value), 
   ** @param limDefl  the limit deflection (taken in abs. value), 
   ** @param turnRad  the turning radius (taken in abs. value), 
   ** @param turnAng  the turning angle (taken in abs. value), 
   ** @param length1  the length of the first  part of the path, 
   ** @param length2  the length of the second part of the path, 
   ** @param length3  the length of the third  part of the path. 
   **/
  FscPath(const iSeeML::rob::OrPtConfig& start, const Type& type, 
	  const double& maxCurv,   const double& maxCDer, 
	  const double& limDefl,   const double& turnRad, 
	  const double& turnAng,   const double& length1, 
	  const double& length2,   const double& length3) :
    // Max curv., Max curv.'s deriv. and type are directly given
    DubinsLikePath(type, fabs(maxCurv)), 
    MaximumCurvDerivative(fabs(maxCDer)), LimDefl( fabs(limDefl) ), 
    TurnRadius( fabs(turnRad) ), TurnAngle( fabs(turnAng) )  { 
    definePieces(start, length1, length2, length3); } 
  
  /** The 'goto' constructor:  a FSC path is built, starting 
   ** from a given iSeeML::rob::OrPtConfig and reaching an other one 
   ** with a maximum curvature and curvature's derivative.
   **
   ** @param start    the starting configuration, 
   ** @param goal     the goal configuration, 
   ** @param maxCurv  the maximum curvature along the path 
   **                 (taken in absolute value), 
   ** @param maxCDer  the maximum curvature's derivative along 
   **                 the path (taken in abs. value). 
   **/
  FscPath(const iSeeML::rob::OrPtConfig& start,
	  const iSeeML::rob::OrPtConfig& goal, 
	  const double& maxCurv,   const double& maxCDer) : 
    // set main fields values
    DubinsLikePath((Type) -1, fabs(maxCurv)), 
    MaximumCurvDerivative( fabs(maxCDer) ) { 
    // compute the other fields values
    computeValues(maxCurv, maxCDer, 
		  LimDefl, TurnRadius, TurnAngle); 
    connect(start, goal); } 

  /** The complete 'goto' constructor:  a FSC path is built, 
   ** starting from a given iSeeML::rob::OrPtConfig and reaching 
   ** an other one with a maximum curvature, curvature's 
   ** derivative, limit deflection, turning radius and turning 
   ** angle.
   **
   ** @warning   this constuctor only works if limit deflection, 
   ** turning radius and turning angle have been correctly computed
   ** (using the method @ref computeValues). 
   **
   ** @param start    the starting configuration, 
   ** @param goal     the goal configuration, 
   ** @param maxCurv  the maximum curvature along the path 
   **                 (taken in absolute value), 
   ** @param maxCDer  the maximum curvature's derivative along 
   **                 the path (taken in abs. value), 
   ** @param limDefl  the limit deflection (taken in abs. value), 
   ** @param turnRad  the turning radius (taken in abs. value), 
   ** @param turnAng  the turning angle (taken in abs. value). 
   **/
  FscPath(const iSeeML::rob::OrPtConfig& start, 
	  const iSeeML::rob::OrPtConfig& goal, 
	  const double& maxCurv,   const double& maxCDer, 
	  const double& limDefl, 
	  const double& turnRad,   const double& turnAng) : 
    // set main fields values
    DubinsLikePath((Type) -1, fabs(maxCurv)), 
    MaximumCurvDerivative(fabs(maxCDer)), LimDefl( fabs(limDefl) ), 
    TurnRadius( fabs(turnRad) ), TurnAngle( fabs(turnAng) )
    // compute the other fields values
  { connect(start, goal); } 


  /** The copy constructor.
   **
   ** @param other  the copied path. 
   **/
  FscPath(const FscPath& other) : DubinsLikePath(other), 
    MaximumCurvDerivative( other.maxCurvDeriv() ), 
    LimDefl( other.limDefl() ), TurnRadius( other.turnRadius() ), 
    TurnAngle( other.turnAngle() ) {} 


  // === Description Methods ======================================

  /** Description method, computing from maximum curvature and 
   ** maximum curvature's derivative the values of limit 
   ** deflection, turning radius and turning angle for a FSC path. 
   **
   ** If maximum curvature or curvature's derivative is too small, 
   ** limit deflection is set to zero and turning radius and angle
   ** are given negative values. 
   **
   ** @param maxCurv  the maximum curvature along the path 
   **                 (taken in absolute value), 
   ** @param maxCDer  the maximum curvature's derivative along 
   **                 the path (taken in abs. value), 
   ** @param limDefl  the corresponding limit deflection, 
   ** @param turnRad  the corresponding turning radius, 
   ** @param turnAng  the corresponding turning angle. 
   **
   ** @see   limDefl, turnRadius, turnAngle.
   **/
  static void computeValues(const double& maxCurv, 
			    const double& maxCDer, double& limDefl, 
			    double& turnRad, double& turnAng); 


  /** Description method, giving a copy of the current FSC path. 
   **
   ** This clone is dynamically allocated (using copy constructor), 
   ** it has to be deleted later.
   **
   ** @return  a copy/clone of the current FSC path. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new FscPath(*this)) ); } 


  /** Description method, writing a description of the path 
   ** in a given output stream.  Starting configuration (as 
   ** an oriented point with curvature) is given, followed by 
   ** a short description of each piece (curvature's derivative and 
   ** length), and by the final configuration. 
   **
   ** @param  O  the output stream in which description is written. 
   **/
  void writeTo(ostream& O) const;


  /** Description method, giving the path's maximum curvature's 
   ** derivative. 
   **
   ** @return    the path's maximum curvature's derivative. 
   **/
  double maxCurvDeriv() const { return MaximumCurvDerivative; } 


  /** Description method, giving the limit deflection: 
   ** a turn of lower (but not zero) deflection is degenerated 
   ** (it is made of two pieces and does not reach 
   **  maximum curvature derivative nor maximum curvature), 
   ** a turn of bigger deflection is normal 
   ** (it is made of three pieces, the middle one being 
   **  a circular arc of maximum curvature, and the other two 
   **  being clothoid pieces of maximum curvature derivative). 
   **
   ** @return    the turning circle's radius. 
   **
   ** @pre  This method returns zero if the maximum curvature 
   **       is zero (zero curvature path should not use it). 
   **/
  double limDefl() const { return LimDefl; } 


  /** Description method, giving the radius of the turning circle 
   ** (the circle of all the configurations which can be reached 
   **  from a fixed one). 
   **
   ** @return    the turning circle's radius. 
   **
   ** @pre  This method returns -1 if the maximum curvature 
   **       is zero (zero curvature path should not use it). 
   **/
  double turnRadius() const { return TurnRadius; } 

  /** Description method, giving the constant angle between 
   ** the turning circle's tangent and the orientation of 
   ** the configurations which can be reached. 
   **
   ** @return    the turning circle's angle. 
   **
   ** @pre  This method returns -@f$\pi@f$ if the maximum curvature 
   **       is zero (zero curvature path should not use it). 
   **
   ** @see turnRadius.
   **/
  double turnAngle() const { return TurnAngle; } 

protected:
  /** Description method, checking whether the given length is 
   ** too short with respect to the maximum curvature's derivative 
   ** to reach maximum curvature along a turn. 
   **
   ** @param  defl    the turn's deflection. 
   **
   ** @return         true if the length is too short, else false. 
   **/
  bool isShortTurn(const double& defl) const 
  { return( fabs(defl) <= limDefl() ); } 

  /** Description method, computing the number of pieces needed  
   ** for a turn of given deflection. 
   **
   ** @param  defl    the turn's deflection. 
   **
   ** @return         the number of needed pieces, 
   **                 which is 1, 2 or 3. 
   **/
  int turnNbPieces(const double& defl) const 
  { return( isZero(defl) ? 1 : isShortTurn(defl) ? 2 : 3 ); } 


  /** Description method, trying to compute the length of the line 
   ** segment connecting the turning circles and the angle between 
   ** the segment connecting the turning centers and the previous 
   ** one. 
   **
   ** The angle can only be computed if the square distance between 
   ** the turning centers is greater than a value depending of 
   ** the path's type. 
   ** Length is not computed for @b lrl or @b rlr path's type, and 
   ** is negative when previous angle cannot be computed.
   **
   ** @param dist   the distance between the turning centers, 
   ** @param length the length to compute (modified by the method), 
   ** @param angle  the angle to compute  (modified by the method). 
   ** @return       whether the computed was succesful or not 
   **               (whether the distance @p dist2 is greater 
   **                than a value depending of the path's type). 
   **/
  virtual bool getConnection(const double& dist, 
			     double& length, double& angle) const;


  // === Modification Methods =====================================

  /** Modification method, adding to the pieces of a path a turn 
   ** starting at a given configuration, with a given curvature 
   ** sign and length. 
   ** 
   ** Maximum curvature of the current FSC path is used. 
   ** 
   ** @param  index     the first free index in the pieces array
   **                   (pieces will be added at this index and 
   **                    after, and the index will be incremented), 
   ** @param  start     a reference to the reference of 
   **                   the starting configuration (it will be 
   **                    changed to a ref. to the ref. of next 
   **                    starting config.), 
   ** @param  defl      the turn's deflection 
   **                   (change of orientation), 
   **                   giving the curvature sign. 
   **
   ** @pre  This method only works if the maximum curvature is not 
   **       zero (otherwise, it writes an error on @c stderr). 
   **/
  void addTurn(int& index, const iSeeML::rob::OrPtConfig **start, 
	       const double& defl); 

public:
  // === Operators ================================================

  /** Copy operator (default one is not correct). 
   **
   ** @param other  the path to copy. 
   **
   ** @return       the current path, after modification.
   **/
  FscPath& operator=(const FscPath& other) { 
    DubinsLikePath::operator=(other); 
    MaximumCurvDerivative = other.maxCurvDeriv(); 
    LimDefl = other.limDefl();   TurnRadius = other.turnRadius(); 
    TurnAngle = other.turnAngle();  return *this; }


  // === Various Methods ==========================================

  /** Various method, computing the set of circle centers used 
   ** to turn from starting configuration of the current path, or 
   ** to turn to reach the given goal configuration. 
   **
   ** @param  start    the starting configuration, 
   ** @param  goal     the configuration to reach, 
   ** @param  centers  the set of circle centers computed. 
   **
   ** @pre  the maximum curvature should not be zero.  
   ** If it is, an error message is generated if 
   ** @c ISEEML_CHECK_FSC_PATH_PRECOND is defined 
   ** (see CompilerFlags.h).

   **/
  void computeCenters(const iSeeML::rob::OrPtConfig& start,
		      const iSeeML::rob::OrPtConfig& goal, 
		      iSeeML::geom::Point centers[nbTurningCircles]) 
    const;

protected:
  // cf. DubinsLikePath
  iSeeML::rob::DubinsLikePath* connectArray() const { 
    // fill the array with clones of the current path
    int i; for(i = 0; i < nbPossiblePaths; i++) 
      computationArray[i] = *this; 
    // return it 
    return( (DubinsLikePath*) computationArray ); } 

  /** Various method, getting in an array of Dubins-like paths 
   ** the element of given index. 
   **
   ** @param   paths  the array of Dubins-like paths,
   ** @param   index  the index of the searched path. 
   **
   ** @pre     first parameter should be an array of paths 
   **          obtained using @ref connectArray. 
   ** @pre     second parameter should be a correct index 
   **          for first parameter array 
   **          (between 0 and @ref nbPossiblePaths - 1). 
   **          If not, an error message is generated if 
   **          @c ISEEML_CHECK_FSC_PATH_PRECOND is defined, and 
   **          the number is considered as 1 if smaller and 3 if 
   **          bigger, if @c ISEEML_CHECK_ARRAY_ELEMT is defined 
   **          (see CompilerFlags.h).
   ** 
   ** @return  the array of Dubins-like paths. 
   **/
  iSeeML::rob::DubinsLikePath& getSolution
  (iSeeML::rob::DubinsLikePath* paths, const int index) const {
    FscPath *array = (FscPath *) paths; 
#ifdef ISEEML_CHECK_FSC_PATH_PRECOND
    if ( (index < 0) || (index >= nbPossiblePaths) )
      cerr << ClassName << "::getConnection: abnormal index"
	   << " parameter (" << index << ")!" << endl; 
#endif
    const int i = 
#ifdef ISEEML_CHECK_ARRAY_ELEMT
      index < 0 ? 0 : index > nbPossiblePaths ? nbPossiblePaths : 
#endif
      index;
    return array[i]; }

}; // end of class iSeeML::rob::FscPath

#endif  // end of definition 

