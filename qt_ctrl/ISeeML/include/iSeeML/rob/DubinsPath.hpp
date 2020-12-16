//
// Definition of iSeeML::rob::DubinsPath, class of Dubins' paths 
// (optimal forward-only paths with discontinuous curvature 
//  profile).
//

// To only include this definition once
#ifndef ISEEML_DUBINS_HH
#define ISEEML_DUBINS_HH


// This class needs the definition of iSeeML::rob::DubinsLikePath. 
#include <iSeeML/rob/DubinsLikePath.hpp>


/** This class defines Dubins' paths, made of circular arcs
 ** tangentially connected by line segments, with no back-up 
 ** maneuvres, corresponding to an optimal forward-only motion
 ** with a lower bounded turning radius. 
 ** 
 ** @see      iSeeML::rob::FscPath.
 ** 
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::DubinsPath : public iSeeML::rob::DubinsLikePath {
  // === Object's Fields ==========================================

  // Array of nbPossiblePaths Dubins' paths. 
  static DubinsPath computationArray[nbPossiblePaths]; 

public: 
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /** The default constructor should only be used for array 
   ** initializations:  it generates a path starting from default 
   ** oriented point, with type lsl, and zero maximum curvature 
   ** and lengths. 
   ** 
   ** @see  default constructor of iSeeML::rob::DubinsLikePath. 
   **/
  DubinsPath() : DubinsLikePath(lsl, 0) {} 

  /** The 'forward' constructor:  a Dubins' path is built from 
   ** its starting oriented point, its type, its maximum curvature 
   ** and its parts' lengths. 
   **
   ** @param start    the starting configuration, 
   ** @param type     the type of Dubins' path, 
   ** @param maxCurv  the maximum curvature along the path 
   **                 (taken in absolute value), 
   ** @param length1  the length of the first  part of the path, 
   ** @param length2  the length of the second part of the path, 
   ** @param length3  the length of the third  part of the path. 
   **/
  DubinsPath(const iSeeML::rob::OrPtConfig& start, const Type& type, 
	     const double& maxCurv,  const double& length1, 
	     const double& length2,  const double& length3) :
    // Maximum curvature and type are directly given
    DubinsLikePath(type, fabs(maxCurv)) 
  { definePieces(start, length1, length2, length3); } 
  
  /** The 'goto' constructor:  a Dubins' path is built, starting 
   ** from a given iSeeML::rob::OrPtConfig and reaching an other one 
   ** with a maximum curvature.
   **
   ** @param start    the starting configuration, 
   ** @param goal     the goal configuration, 
   ** @param maxCurv  the maximum curvature along the path. 
   **/
  DubinsPath(const iSeeML::rob::OrPtConfig& start, const iSeeML::rob::OrPtConfig& goal,
	     const double& maxCurv) : 
    DubinsLikePath((Type) -1, fabs(maxCurv)) 
  { connect(start, goal); } 


  /** The copy constructor.
   **
   ** @param other  the copied path. 
   **/
  DubinsPath(const DubinsPath& other) : DubinsLikePath(other) {} 


  // === Description Methods ======================================

  /** Description method, giving a copy of the current 
   ** Dubins' path. 
   **
   ** This clone is dynamically allocated (using copy constructor), 
   ** it has to be deleted later.
   **
   ** @return  a copy/clone of the current Dubins' path. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new DubinsPath(*this)) ); } 


  /** Description method, writing a description of the path 
   ** in a given output stream.  Starting configuration (as 
   ** an oriented point) is given, followed by a short description 
   ** of each part (curvature and length), and by the final 
   ** configuration. 
   **
   ** @param  O  the output stream in which description is written. 
   **/
  void writeTo(ostream& O) const;


  /** Description method, giving the radius of the turning circle 
   ** (the circle of all the configurations which can be reached 
   **  from a fixed one). 
   **
   ** @warning  This method returns infinity 
   **           if the maximum curvature is zero 
   **           (zero curvature path should not use it). 
   **
   ** @return   the turning circle's radius. 
   **/
  double turnRadius() const { return( 1 / maxCurv() ); } 

protected:
  /** Description method, computing the number of pieces (0 or 1) 
   ** needed for a turn of given deflection. 
   **
   ** @param  defl the turn's deflection. 
   **
   ** @return the number of needed pieces, 
   **         which is 0 or 1. 
   **/
  int turnNbPieces(const double& defl) const  { 
    return( isZero(defl) ? 0 : 1 ); } 


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
   ** @param  dist   the distance between the turning centers, 
   ** @param  length the length to compute (modified by the method), 
   ** @param  angle  the angle to compute  (modified by the method). 
   **
   ** @return whether the computed was succesful or not 
   **         (whether the distance @p dist2 is greater 
   **          than a value depending of the path's type). 
   **/
  virtual bool getConnection(const double& dist, 
			     double& length, double& angle) const;


  // === Modification Methods =====================================

  /** Modification method, adding to the pieces of a path a turn 
   ** starting at a given configuration, with a given curvature 
   ** sign and length. 
   ** 
   ** Maximum curvature of the current Dubins' path is used. 
   ** 
   ** @param index the first free index in the pieces array
   **              (pieces will be added at this index and after,
   **               and the index will be incremented), 
   ** @param start a reference to the reference 
   **              of the starting configuration 
   **              (it will be changed to a ref. to the ref. 
   **               of next starting config.), 
   ** @param defl  the turn's deflection (change of orientation), 
   **              giving the curvature sign. 
   **
   ** @pre   the maximum curvature should not be zero.  
   **        If it is, an error message is generated if 
   **        @c ISEEML_CHECK_DUBINS_PATH_PRECOND is defined 
   **        (see CompilerFlags.h).
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
  DubinsPath& operator=(const DubinsPath& other) 
  { DubinsLikePath::operator=(other);  return *this; }


  // === Various Methods ==========================================

  /** Various method, computing the set of circle centers used 
   ** to turn from starting configuration of the current path, or 
   ** to turn to reach the given goal configuration. 
   ** Circles' radiuses are the opposite of the maximum curvature 
   ** of the current path (maximum curvature should not be zero). 
   **
   ** @param  start    the starting configuration, 
   ** @param  goal     the configuration to reach, 
   ** @param  centers  the set of circle centers computed. 
   **
   ** @pre    the maximum curvature should not be zero.  
   **         If it is, an error message is generated if 
   **         @c ISEEML_CHECK_DUBINS_PATH_PRECOND is defined 
   **         (see CompilerFlags.h).
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
   **          @c ISEEML_CHECK_DUBINS_PATH_PRECOND is defined, and 
   **          the number is considered as 1 if smaller and 3 if 
   **          bigger, if @c ISEEML_CHECK_ARRAY_ELEMT is defined 
   **          (see CompilerFlags.h).
   ** 
   ** @return  the array of Dubins-like paths. 
   **/
  iSeeML::rob::DubinsLikePath& getSolution(iSeeML::rob::DubinsLikePath* paths, 
			      const int index) const {
    DubinsPath *array = (DubinsPath *) paths; 
#ifdef ISEEML_CHECK_DUBINS_PATH_PRECOND
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

}; // end of class iSeeML::rob::DubinsPath


#endif  // end of definition 

