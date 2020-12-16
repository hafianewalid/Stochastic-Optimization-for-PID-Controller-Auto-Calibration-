//
// Definition of iSeeML::rob::OrPtConfig, class of ISeeML standard  
// robotic configurations.
//

// To only include this definition once
#ifndef ISEEML_ROB_ORIENT_POINT_HH
#define ISEEML_ROB_ORIENT_POINT_HH


// This class needs the definition of iSeeML::rob::Object. 
#include <iSeeML/rob/Object.hpp>
// This class needs the definition of iSeeML::geom::Point. 
#include <iSeeML/geom/Point.hpp>


/** This class defines a standard robotic configuration, containing 
 ** the position of a reference point and the orientation of 
 ** a main axis. 
 ** 
 ** This configuration is sufficient to define the position 
 ** of most robots, and thus is enough for collision avoidance.
 ** However, taking into account dynamic constraints (as velocity 
 ** and acceleration bounds) or more precise kinematic constraints
 ** (as continuity of the directing wheels' positions) generally 
 ** requires a more complex configuration, as e.g. 
 ** @ref iSeeML::rob::CurvConfig. 
 ** 
 ** This class contains description methods giving @ref position 
 ** and @ref orientation of a standard configuration as well as 
 ** the @ref writeTo method, a modification method changing 
 ** a standard configuration into its @ref uTurn "opposite", 
 ** the equality operator, and various methods giving 
 ** the @ref opposite of a standard configuration, 
 ** the @ref distance2 "distance" between two standard 
 ** configurations' positions or whether two such configurations 
 ** verify some properties (@ref isParallelTo "parallelism", 
 ** @ref isSymmetricTo "symmetry", @ref isAlignedWith "alignment") 
 ** or whether a point is @ref hasInFront "in front" of a standard 
 ** configuration. 
 ** 
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::OrPtConfig : public iSeeML::rob::Object {
  // === Object's Fields ==========================================
  
  iSeeML::geom::Point Position;  ///< Position of the reference point.
  double Orientation;           ///< Orientation of the main axis.

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /** The default constructor should only be used for array 
   ** initializations:  it correspond to a default point with 
   ** zero orientation. 
   ** 
   ** @see  default constructor of iSeeML::geom::Point. 
   **/
  OrPtConfig() : Position(), Orientation(0) {} 

  /** The main constructor. 
   ** 
   ** @param P     the position of the reference point, 
   ** @param theta the orientation of the main axis, taken 
   **              between - @f$\pi@f$ (excluded) and @f$\pi@f$
   **              (included) modulo 2 @f$\pi@f$. 
   **
   ** @see   iSeeML::Object::mod2pi.
   **/
  OrPtConfig(const iSeeML::geom::Point& P, const double& theta) :
    Position(P), Orientation( mod2pi(theta) ) {}

  /** A usefull constructor. 
   ** 
   ** @param x     the first coordinate of the reference point, 
   ** @param y     the second coordinate of the reference point, 
   ** @param theta the orientation of the main axis, taken 
   **              between - @f$\pi@f$ (excluded) and @f$\pi@f$
   **              (included) modulo 2 @f$\pi@f$. 
   **
   ** @see   main constructor of iSeeML::geom::Point, 
   **        iSeeML::Object::mod2pi.
   **/
  OrPtConfig(const double& x, const double& y,
	     const double& theta) :
    Position(x, y), Orientation( mod2pi(theta) ) {}

  // The default copy constructor is OK. 


  /// The virtual destructor does nothing. 
  virtual ~OrPtConfig() {} 


  // === Description Methods ======================================

  /** Description method, giving a copy of the current 
   ** configuration. 
   **
   ** This clone is dynamically allocated (using default copy 
   **  constructor), it has to be deleted later.
   **
   ** @return  a copy/clone of the current configuration. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new OrPtConfig(*this)) ); } 

  /** Description method, writing the configuration in a given 
   ** output stream: coordinate in each dimension is written, 
   ** between paranthesis and separated by commas. 
   **
   ** @param  O  the output stream in which description is written. 
   **
   ** @see    algWriteTo. 
   **/
  void writeTo(ostream& O) const { algWriteTo(O); } 


  /** Description method, giving the position of the robot's 
   ** reference point in this configuration. 
   **
   ** @return    the position of the reference point. 
   **/
  const iSeeML::geom::Point& position() const { return Position; } 

  /** Description method, giving the orientation of the robot's 
   ** main axis in this configuration. 
   **
   ** @return    the orientation of the main axis. 
   **/
  const double& orientation() const { return Orientation; } 

  /** @brief Gives the projection of a point in the frame of 
   **        this configuration.
   **
   ** @param  P  the point projected into this configuration's frame. 
   **
   ** @return  the vector from the origin to the projected point.
   **
   ** @see iSeeML::geom::Point::operator-(const iSeeML::geom::Point&), 
   **      iSeeML::geom::Vector::rotate, position, orientation.
   **/
  iSeeML::geom::Vector projection(const iSeeML::geom::Point& P) const
  { return ( P - position() ).rotate( -orientation() ); } 

  /** @brief Projects a configuration in the frame of  
   **        this configuration.
   **
   ** If R = P.projection(Q), then Q = P + R.
   **
   ** @param  q  the configuration projected into 
   **            this configuration's frame. 
   **
   ** @return  the new configuration, once projected. 
   **
   ** @see OrPtConfig(const iSeeML::geom::Point&, const double&), 
   **      position, projection(const iSeeML::geom::Point&), 
   **      orientation, operator+.
   **/
  OrPtConfig projection(const OrPtConfig& q) const
  { return OrPtConfig( iSeeML::geom::Point()
		       + projection( q.position() ),
		       q.orientation() - orientation() ); } 

protected:
  /** @brief Description method, giving the dimension of 
   ** the containing space (3) when this configuration 
   ** is considered as an algebraic vector. 
   **
   ** This representation is mainly used to define 
   ** @ref algWriteTo "a unified writeTo". 
   ** Dimension cannot be static, as it is virtual. 
   **/ 
  int algDimension() const { return 3; } 

  /** @brief Description method, giving the coordinate of given index 
   ** of the configuration (both coordinates of the position, 
   **  then orientation). 
   **
   ** @param  i  the index of the searched coordinate.
   **
   ** @pre the parameter is an index, and should be equal to one, 
   ** two or three.  If not, and @c ISEEML_CHECK_ORPT_CONFIG_PRECOND 
   ** is defined (see CompilerFlags.h), an error is generated. 
   **
   ** @return    the double coordinate associated to the index 
   **            if this one is between 1 and dimension, or 0.
   **
   ** @see       orientation, position, iSeeML::geom::Point::xCoord,
   **            iSeeML::geom::Point::yCoord. 
   **/
  double algCoord(const int i) const { 
    switch (i) {  case 1: return position().xCoord(); 
    case 2: return position().yCoord();  
    case 3: return orientation();  default: 
#ifdef ISEEML_CHECK_ORPT_CONFIG_PRECOND
      cerr << ClassName << "::coord: parameter " << i << endl
	   << " outside allowed values (1, 2 or 3)," 
	   << " returning zero..." << endl;
#endif
      return 0; } } 

public:
  // === Modification Methods =====================================

  /** @brief Modification method, turning the current oriented point 
   ** to its opposite: the position does not change but 
   ** the orientation is replaced by its opposite 
   ** (@f$\pm@f$ @f$\pi@f$ is added to it). 
   **
   ** @return  the new oriented point, one transformed 
   **          to its opposite. 
   **/
  OrPtConfig& uTurn() { 
    // do not use rotate to avoid unusefull checks
    Orientation += (Orientation > 0 ? - M_PI : M_PI); 
    return *this; } 


  // === Operators ================================================

  /** @brief Equality operator between oriented points. 
   **
   ** @param  other  another oriented point. 
   **
   ** @return        whether the two points are equal 
   **                (same position and orientation). 
   **
   ** @see  position, iSeeML::geom::Point::operator==, isParallelTo. 
   **/
  bool operator==(const OrPtConfig& other) const { 
    return( ( position() == other.position() ) && 
	    isParallelTo(other) ); } 

  /** @brief Adds an oriente point to the current one: gives 
   **        the oriented point obtained when the added oriented 
   **        point is considered in the current's frame. 
   **
   ** If R = P + Q, Q = P.projection(R).
   **
   ** @param  other  the second oriented point. 
   **
   ** @see  operator+.
   **/
   void operator+=(const OrPtConfig& other) { *this = *this + other; } 

  /** @brief Addition operator between oriented points: gives 
   **        the oriented point obtained when the second oriented 
   **        point is considered in the first's frame. 
   **
   ** If R = P + Q, Q = P.projection(R).
   **
   ** @param  other  the second oriented point. 
   **
   ** @return        the oriented point whose projection 
   **                in the first oriented point's frame is 
   **                the second oriented point. 
   **
   ** @see  OrPtConfig projection(const OrPtConfig&).
   **/
   OrPtConfig operator+(const OrPtConfig& other) const {
     const double theta = orientation(); 
     iSeeML::geom::Vector v = other.position() - iSeeML::geom::Point(); 
     return( OrPtConfig(position() + v.rotate(theta), 
			other.orientation() + theta) );
   } // end of OrPtConfig operator+(const OrPtConfig&)


  // === Various Methods ==========================================

  /** Various method, giving the opposite oriented point 
   ** of the current one: the opposite oriented point
   ** has the same position but opposite orientation 
   ** (current oriented point's orientation @f$\pm@f$ @f$\pi@f$). 
   **
   ** @return  the opposite oriented point of the current one. 
   **
   ** @see  uTurn.
   **/
  OrPtConfig opposite() const 
  { OrPtConfig res(*this);  return res.uTurn(); } 


  /** Various method, giving the distance between two oriented 
   ** points' positions. 
   **
   ** <B>Note :</B> this is the standard 2-dimensional plane 
   ** distance, instead of being a distance in the 3-dimensional 
   ** space of the oriented points.
   ** Its name comes from this fact.
   **
   ** @param  other  another oriented point. 
   **
   ** @return        the distance between the oriented points' 
   **                positions. 
   **
   ** @see  position, iSeeML::geom::Point::distance. 
   **/
  double distance2(const OrPtConfig& other) const 
  { return( position().distance( other.position() ) ); } 


  /** Various method, checking whether two oriented points are 
   ** parallel, ie whether their orientations are equal (modulo 
   ** 2 @f$pi@f$). 
   **
   ** @param  other  another oriented point. 
   **
   ** @return        whether the current oriented point and 
   **                the given one have same orientation. 
   **
   ** @see  orientation, 
   **       iSeeML::Object::mod2pi, iSeeML::Object::isZero.
   **/
  bool isParallelTo(const OrPtConfig& other) const 
  { return( isZero( mod2pi( orientation() - 
			    other.orientation() ) ) ); } 

  /** Various method, checking whether two oriented points are 
   ** symmetric, ie whether their orientations are symmetric wrt
   ** the line segment connecting their position. 
   **
   ** The vector connecting the positions and the vector 
   ** of average orientation should be collinear: 
   ** their vectorial product should be zero. 
   **
   ** @param  other  another oriented point. 
   **
   ** @return        whether the current oriented point and 
   **                the given one are symmetric. 
   **
   ** @see  position, orientation, iSeeML::geom::Vector::operator^, 
   **       iSeeML::Object::isZero.
   **/
  bool isSymmetricTo(const OrPtConfig& other) const
    // Cf. end of doc. comment
   { return( isZero( ( position() - other.position() ) ^  
		     geom::Vector(( orientation() + 
				       other.orientation() ) / 2) 
		     ) ); } 

  /** Various method, checking whether two oriented points are 
   ** aligned: their orientations should be equal, and be the same 
   ** as the orientation of the segment connecting their positions. 
   **
   ** In fact, the oriented points are aligned iff they are 
   ** simultaneously parallel and symmetric. 
   **
   ** @param  other  another oriented point. 
   **
   ** @return        whether the current oriented point and 
   **                the given one are aligned. 
   **
   ** @see  isParallelTo, isSymmetricTo. 
   **/
  bool isAlignedWith(const OrPtConfig& other) const
  { return( isParallelTo(other) && isSymmetricTo(other) ); }  


  /** Various method, checking whether a point is in the front 
   ** half-plane of the current oriented point: 
   ** the vector connecting the current oriented point's 
   ** position to the given point should make an angle 
   ** between - @f$\pi@f$/ 2 and @f$\pi@f$/ 2 with 
   ** the current oriented point's orientation. 
   **
   ** The vector going from the oriented point's position to 
   ** the point and the vector of same orientation as 
   ** the oriented point should have a positive scalar product. 
   **
   ** @param  point  a point. 
   **
   ** @return        whether the given point is in the front 
   **                half-plane of the current oriented point. 
   **
   ** @see  position, orientation, iSeeML::geom::Point::operator-, 
   **       iSeeML::geom::Vector::operator*(const Vector&).
   **/
  bool hasInFront(const iSeeML::geom::Point& point) const 
    // Cf. end of doc. comment
  { return( ( point - position() ) * 
	    geom::Vector( orientation() ) >= 0 ); } 

}; // end of class iSeeML::rob::OrPtConfig

#endif  // end of definition 

