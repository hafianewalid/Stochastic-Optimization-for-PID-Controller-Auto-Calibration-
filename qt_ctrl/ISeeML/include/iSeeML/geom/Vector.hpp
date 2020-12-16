//
// Definition of iSeeML::geom::Vector, class of ISeeML' 2D geometric 
// vectors.
//

// For Borland C++
#ifdef __BORLANDC__
#pragma warn -8022  // to remove some stupid warnings
#endif

// To only include this definition once
#ifndef ISEEML_GEOM_VECTOR_HH
#define ISEEML_GEOM_VECTOR_HH


// This class needs the definition of iSeeML::geom::BasicObject. 
#include <iSeeML/geom/BasicObject.hpp>


/** This class defines 2D geometric vectors, which can be defined 
 ** by their Polar or Cartesian coordinates. 
 ** 
 ** This class contains description methods giving Polar and 
 ** Cartesian coordinates as well as the @ref writeTo method, 
 ** modification methods giving translation, rotation, scaling and 
 ** symmetry of a vector, the corresponding operators as well as 
 ** the equality operator, and @ref FresnelInt "a last method" 
 ** computing Cartesian coordinates of a reference clothoid 
 ** (which are Fresnel integrals). 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::geom::Vector : public iSeeML::geom::BasicObject {
  // === Object's Fields ==========================================

  double XCoord;   ///< First Cartesian coordinate. 
  double YCoord;   ///< Second Cartesian coordinate. 

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /** The default constructor, creating the zero vector with
   ** Cartesian coordinates (0,0). 
   **/
  Vector() : XCoord(0), YCoord(0) {}

  /** A simple constructor, creating the unit vector of given 
   ** orientation. 
   **
   ** The vector of Polar coordinates <tt>(rho, theta)</tt> 
   ** can thus be obtained using 
   ** <tt>iSeeML::geom::Vector(theta).multiply(rho)</tt>. 
   ** 
   ** @param theta the orientation of the vector. 
   **
   ** @see   orientation, length, multiply. 
   **/
  Vector(const double& theta) :
    XCoord( cos(theta) ), YCoord( sin(theta) ) {} 

  /** The main constructor, creating a vector from its Cartesian 
   ** coordinates. 
   ** 
   ** @param x the first  Cartesian coordinate of the vector, 
   ** @param y the second Cartesian coordinate of the vector. 
   **
   ** @see   xCoord, yCoord.
   **/
  Vector(const double& x, const double& y) : 
    XCoord(x), YCoord(y) {}

  // The default copy constructor is OK. 


  // === Description Methods ======================================

  /** Description method, giving a copy of the current vector. 
   **
   ** This clone is dynamically allocated (using default copy 
   **  constructor), it has to be deleted later.
   **
   ** @return  a copy/clone of the current vector. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new Vector(*this)) ); } 


  /** Description method, writing a description of the vector into 
   ** a stream: Cartesian coordinate in each dimension is written, 
   ** between paranthesis and separated by commas. 
   **
   ** @param  O  the output stream in which description 
   **            is written. 
   **/
  void writeTo(ostream& O) const { algWriteTo(O); } 


  /** Description method, giving the vector's first Cartesian 
   ** coordinate. 
   **
   ** @return    the vector's first Cartesian coordinate. 
   **/
  const double& xCoord() const { return XCoord; } 

  /** Description method, giving the vector's second Cartesian 
   ** coordinate. 
   **
   ** @return    the vector's second Cartesian coordinate. 
   **/
  const double& yCoord() const { return YCoord; } 


  /** Description method, giving the vector's orientation.
   **
   ** The couple (@ref length, orientation) gives Polar 
   ** coordinates of the vector, while the couple (@ref xCoord, 
   ** @ref yCoord) gives Cartesian coordinates. 
   ** Orientation is always uniquely defined, except for zero 
   ** vector.
   ** In that case, we choose to return 0.
   **
   ** @return    the vector's orientation, between - @f$\pi@f$ 
   **            (excluded) and @f$\pi@f$ (included), if vector
   **            is not zero, or zero. 
   **
   ** @see   iSeeML::Object::isPositive, iSeeML::Object::isNegative, 
   **        iSeeML::Object::sign, atan.
   **/
  double orientation() const 
  { return
      // if XCoord is positive, direct computation using atan
      ( isPositive(XCoord) ? atan(YCoord / XCoord) : 
	// if XCoord is negative, atan value is corrected 
	// wrt YCoord precise sign 
	isNegative(XCoord) ? ( YCoord < 0 ? 
			       atan(YCoord / XCoord) - M_PI : 
			       atan(YCoord / XCoord) + M_PI ) : 
	// if XCoord is zero, result can be 0 or +/- pi / 2
	sign(YCoord) * M_PI_2 ); }
	    
  /** Description method, giving the vector's length.
   **
   ** The couple (length, @ref orientation) gives Polar 
   ** coordinates of the vector, while the couple (@ref xCoord, 
   ** @ref yCoord) gives Cartesian coordinates. 
   **
   ** @return    the vector's length. 
   **
   ** @see       sqrLength. 
   **/
  double length() const    { return sqrt(sqrLength()); }

  /** Description method, giving the square of the vector's length.
   ** 
   ** Uses the scalar product. 
   **
   ** @return    the square of the vector's length. 
   **
   ** @see       operator*(const iSeeML::geom::Vector&). 
   **/
  double sqrLength() const { return( (*this) * (*this) ); }

protected:
  /** Description method, giving the dimension of 
   ** the containing space (2) when this vector is considered 
   ** as an algebraic one. 
   **
   ** This representation is mainly used to define 
   ** @ref algWriteTo "a unified writeTo". 
   ** Dimension cannot be static, as it is virtual. 
   **/ 
  int algDimension() const { return 2; } 

  /** Description method, giving Cartesian coordinate of given 
   ** index for the vector. 
   **
   ** @param  i  the index of the searched Cartesian coordinate.
   **
   ** @pre    the parameter is an index, and should be equal to one 
   ** or two.  If not, and if @c ISEEML_CHECK_GEOM_VECT_PRECOND is 
   ** defined (see CompilerFlags.h), an error is generated. 
   **
   ** @return    the Cartesian coordinate associated to the index 
   **            if this one is between 1 and dimension, or 0.
   **
   ** @see       xCoord, yCoord, algDimension. 
   **/
  double algCoord(const int i) const { 
    switch (i) {  case 1 : return xCoord(); 
    case 2 : return yCoord();  default: 
#ifdef ISEEML_CHECK_GEOM_VECT_PRECOND
      cerr << ClassName << "::coord: parameter " << i << endl
	   << " outside allowed values (1 or 2), returning zero..." 
	   << endl; 
#endif
      return 0; } } 

public:
  // === Modification Methods =====================================

  /** Modification method, moving the point to a given position. 
   ** 
   ** The original vector is changed. 
   ** 
   ** @param x  the first coordinate of the new position, 
   ** @param y  the second coordinate of the new position. 
   ** 
   ** @return   the vector, after tranformation. 
   **
   ** @see      xCoord, yCoord. 
   **/
  Vector& moveTo(const double& x, const double& y) 
  { XCoord = x;  YCoord = y;  return *this; } 

  /** Modification method, translating the vector along an other. 
   ** 
   ** The original vector is changed, addition without 
   ** modification is obtained using operator+(const Vector&) const. 
   ** 
   ** @param v  the vector of the translation.
   **
   ** @return   the new vector, after translation.
   **
   ** @see      add.
   **/
  Vector& translate(const Vector& v) { return( add(v) ); } 


  /** Modification method, rotating the vector of a given angle. 
   ** 
   ** The original vector is changed. 
   ** 
   ** @param theta  the angle of the rotation.
   **
   ** @return       the new vector, after rotation.
   **/
  Vector& rotate(const double& theta) { 
    const double x = XCoord, y = YCoord, 
      ct = cos(theta), st = sin(theta); 
    XCoord = x * ct - y * st; YCoord = x * st + y * ct; 
    return *this; } 


  /** Modification method, adding a vector to the current one. 
   ** 
   ** The original vector is changed, addition without 
   ** modification is obtained using operator+(const Vector&) const. 
   ** 
   ** @param v  the vector of the translation.
   **
   ** @return   the new vector, after translation / addition.
   **/
  Vector& add(const Vector& v) 
  { XCoord += v.xCoord();  YCoord += v.yCoord();  return *this; } 


  /** Modification method, multiplying a vector by a real factor. 
   ** 
   ** The original vector is changed, multiplication without 
   ** modification is obtained using operator*() const. 
   ** 
   ** @param f  a real factor.
   **
   ** @return   the new vector, after multiplication.
   **/
  Vector& multiply(const double& f) 
  { XCoord *= f;  YCoord *= f;  return *this; } 

  /** Division method, dividing a vector by a real factor. 
   ** 
   ** The original vector is changed, division without 
   ** modification is obtained using operator/() const. 
   ** 
   ** @param f  a real factor.
   **
   ** @pre      the dividing factor should not be zero.  If it is, 
   ** and @c ISEEML_CHECK_GEOM_VECT_PRECOND is defined (see 
   **  CompilerFlags.h), an error is generated. 
   **
   ** @return   the new vector, after division.
   **
   ** @see   multiply. 
   **/
  Vector& divide(const double& f) { 
#ifdef ISEEML_CHECK_GEOM_VECT_PRECOND
    if ( isZero(f) ) 
      cerr << ClassName << "::divide: zero factor!!!" << endl; 
#endif
    return multiply(1 / f); } 


  /** Modification method, transforming a vector into its symmetric
   ** with respect to the X axis. 
   ** 
   ** @return   the new vector, after transformation.
   **/
  Vector& symmetryOx() { YCoord *= -1; return *this; } 

  /** Modification method, transforming a vector into its symmetric
   ** with respect to the Y axis. 
   ** 
   ** @return   the new vector, after transformation.
   **/
  Vector& symmetryOy() { XCoord *= -1; return *this; } 


  // === Operators ================================================

  /** Equality operator between vectors (differences between both
   **  Cartesian coordinates should be zero). 
   **
   ** @param   other   another vector. 
   **
   ** @return  whether the two vectors are equal. 
   **
   ** @see     xCoord, yCoord, iSeeML::Object::isZero. 
   **/
  bool operator==(const Vector& other) const 
  { return( isZero( xCoord() - other.xCoord() ) &&
	    isZero( yCoord() - other.yCoord() ) ); } 


  /** Sum operator between two vectors, giving the translation 
   ** of the first one by the other.
   **
   ** @param  v  the vector of the translation.
   **
   ** @return    the sum of the two vectors. 
   **
   ** @see       add.
   **/
  Vector operator+(const Vector& v) const
  { Vector res(*this); return res.add(v); } 

  /** Difference operator between two vectors, giving the sum 
   ** of the first vector and of the second one's opposite. 
   **
   ** @param  v  a vector.
   **
   ** @return    the current vector minus the given one. 
   **
   ** @see       operator-(), operator+.
   **/
  Vector operator-(const Vector& v) const
  { return( *this + (-v) ); } 


  /** Opposite operator for a vector, such that the sum of 
   ** the vector and of its opposite is the zero vector. 
   **
   ** @return    the opposite of the current vector. 
   **
   ** @see       multiply.
   **/
  Vector operator-() const
  { Vector res(*this); return( res.multiply(-1) ); } 


  /** Multiplication operator between a vector and a real.
   **
   ** @param  f  a real factor.
   **
   ** @return    the product of the current vector by the real 
   **            factor. 
   **
   ** @see       multiply.
   **/
  Vector operator*(const double& f) const
  { Vector res(*this); return res.multiply(f); } 

  /** Multiplication operator between a real and a vector.
   **
   ** @param  f  the real factor, 
   ** @param  v  the vector.
   **
   ** @return    the product between the factor and the vector. 
   **
   ** @see       const operator*(const double&). 
   **/
  friend Vector operator*(const double& f, const Vector& v) 
  { return(v * f); } 

  /** Division operator between a vector and a real.
   **
   ** @param  f  a real factor.
   **
   ** @pre       the dividing factor should not be zero.
   **
   ** @return    the division of the current vector by the real 
   **            factor. 
   **
   ** @see       operator*(const double&), divide.
   **/
  Vector operator/(const double& f) const
  { Vector res(*this); return res.divide(f); } 

  /** Division operator between a real and a vector.
   **
   ** @param  f  the real factor, 
   ** @param  v  the vector.
   **
   ** @pre       the dividing factor should not be zero.
   **
   ** @return    the division between the factor and the vector. 
   **
   ** @see       const operator/(const double&). 
   **/
  friend Vector operator/(const double& f, const Vector& v) 
  { return(v / f); } 


  /** Scalar product operator between two vectors.
   **
   ** @param  v  the second vector of the product.
   **
   ** @return    the scalar product between the current vector 
   **            and the given one. 
   **/
  double operator*(const Vector& v) const
  { return( xCoord() * v.xCoord() + yCoord() * v.yCoord() ); }

  /** Vectorial product operator between two vectors.
   **
   ** @param  v  the second vector of the product.
   **
   ** @return    the vectorial product between the current vector 
   **            and the given one. 
   **/
  double operator^(const Vector& v) const
  { return( xCoord() * v.yCoord() - yCoord() * v.xCoord() ); }


  // === Various Methods ==========================================

  /** Various method, computing the Fresnel Cosine integral. 
   **
   ** @param s  the integration length. 
   **
   ** @pre  Coordinates have only been computed for values of 
   ** the arc length @p s smaller (in absolute value) then 2, 
   ** which correspond to a deflection (change of the orientation) 
   ** of @f$\pm@f$ 2 @f$\pi@f$. 
   **
   ** @return   the integral, from zero to @p s, of 
   **           cos(@f$\pi@f$ u<SUP>2</SUP> / 2). 
   **
   ** @see      FresnelInt, xCoord.
   **/
  static double FresnelCos(const double& s) 
  { return( FresnelInt(s).xCoord() ); } 

  /** Various method, computing the Fresnel Sine integral. 
   **
   ** @param s  the integration length. 
   **
   ** @pre  Coordinates have only been computed for values of 
   ** the arc length @p s smaller (in absolute value) then 2, 
   ** which correspond to a deflection (change of the orientation) 
   ** of @f$\pm@f$ 2 @f$\pi@f$. 
   **
   ** @return   the integral, from zero to @p s, of 
   **           sin(@f$\pi@f$ u<SUP>2</SUP> / 2). 
   **
   ** @see      FresnelInt, yCoord.
   **/
  static double FresnelSin(const double& s) 
  { return( FresnelInt(s).yCoord() ); } 

  /** Various method, computing the Fresnel integrals in a vector. 
   **
   ** The returned vector correspond to the Cartesian coordinates 
   ** of the point of given arc length along a reference clothoid 
   ** (along which curvature is equal to @f$\pi@f$ times the arc 
   **  length). 
   ** These coordinates have been precomputed with a precision 
   ** of 1E-5, and are stored in two static arrays. 
   ** 
   ** @param s   the arc length along the reference clothoid of 
   **            the searched position, or the integration length
   **            of the Fresnel integrals. 
   **
   ** @pre  Coordinates have only been computed for values of 
   ** the arc length @p s smaller (in absolute value) then 2, 
   ** which correspond to a deflection (change of the orientation) 
   ** of @f$\pm@f$ 2 @f$\pi@f$. 
   ** An error message is generated if @p s is not correct and 
   ** check flags @c ISEEML_CHECK_GEOM_VECT_PRECOND or @c 
   ** ISEEML_CHECK_ARRAY_ELEMT (see CompilerFlags.h) are defined. 
   **
   ** @return    the vector whose Cartesian coordinates are 
   **            the integral, from zero to @p s, of respectively 
   **            cos(@f$\pi@f$ u<SUP>2</SUP> / 2) and 
   **            sin(@f$\pi@f$ u<SUP>2</SUP> / 2). 
   **/
  static Vector FresnelInt(const double& s); 

}; // end of class iSeeML::geom::Vector

#endif  // end of definition 


