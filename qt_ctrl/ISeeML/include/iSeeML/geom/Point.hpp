//
// Definition of iSeeML::geom::Point, class of ISeeML' 2D geometric 
// points.
//

// To only include this definition once
#ifndef ISEEML_GEOM_POINT_HH
#define ISEEML_GEOM_POINT_HH


// This class needs the definition of iSeeML::geom::Vector. 
#include <iSeeML/geom/Vector.hpp>


/** This class defines 2D geometric points, defined 
 ** by their Cartesian coordinates. 
 ** 
 ** This class contains description methods giving Cartesian 
 ** coordinates as well as the @ref writeTo method, a modification 
 ** method giving @ref translate "translation" of the point, 
 ** the corresponding operator as well as the equality operator and 
 ** difference operators between a point and a vector or between  
 ** two points,and at last a @ref distance "method" computing 
 ** the distance separating two points.
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::geom::Point : public iSeeML::geom::BasicObject {
  // === Object's Fields ==========================================

  double XCoord;   ///< First coordinate. 
  double YCoord;   ///< Second coordinate. 

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  /// The default constructor, returning the frame origin (0,0). 
  Point() : XCoord(0), YCoord(0) {}

  /** The main constructor, creating a point from its Cartesian 
   ** coordinates (Polar coordinates are not used for points in 
   ** ISeeML). 
   ** 
   ** @param x the first  Cartesian coordinate of the point, 
   ** @param y the second Cartesian coordinate of the point. 
   **
   ** @see     xCoord, yCoord. 
   **/
  Point(const double& x, const double& y) : 
    XCoord(x), YCoord(y) {}

  // The default copy constructor is OK. 


  // === Description Methods ======================================

  /** Description method, giving a copy of the current point. 
   **
   ** This clone is dynamically allocated (using default copy 
   **  constructor), it has to be deleted later.
   **
   ** @return  a copy/clone of the current point. 
   **/
  iSeeML::Object& clone() const 
  { return *( (iSeeML::Object*) (new Point(*this)) ); } 


  /** Description method, writing a description of the point into 
   ** a stream: Cartesian coordinate in each dimension is written, 
   ** between paranthesis and separated by commas. 
   **
   ** @param  O  the output stream in which description 
   **            is written. 
   **/
  void writeTo(ostream& O) const { algWriteTo(O); } 


  /** Description method, giving the point's first coordinate. 
   **
   ** @return    the point's first coordinate. 
   **/
  const double& xCoord() const { return XCoord; } 

  /** Description method, giving the point's second coordinate. 
   **
   ** @return    the point's second coordinate. 
   **/
  const double& yCoord() const { return YCoord; } 

protected:
  /** Description method, giving the dimension of 
   ** the containing space (2) when this point is considered 
   ** as an algebraic vector. 
   **
   ** This representation is mainly used to define 
   ** @ref algWriteTo "a unified writeTo". 
   ** Dimension cannot be static, as it is virtual. 
   **/ 
  int algDimension() const { return 2; } 

  /** Description method, giving Cartesian coordinate of given 
   ** index of the point. 
   **
   ** @param  i  the index of the searched Cartesian coordinate.
   **
   ** @pre    the parameter is an index, and should be equal to one 
   ** or two.  If not, and if @c ISEEML_CHECK_GEOM_POINT_PRECOND is 
   ** defined (see CompilerFlags.h), an error is generated. 
   **
   ** @return    the double coordinate associated to the index 
   **            if this one is between 1 and dimension, or 0.
   **
   ** @see       xCoord, yCoord, algDimension. 
   **/
  double algCoord(const int i) const { 
    switch (i) {  case 1 : return xCoord(); 
    case 2 : return yCoord();  default: 
#ifdef ISEEML_CHECK_GEOM_POINT_PRECOND
      cerr << ClassName << "::coord: parameter " << i << endl
	   << " outside allowed values (1 or 2), returning zero..." 
	   << endl;
#endif
      return 0; } } 

public:
  // === Modification Methods =====================================

  /** Modification method, moving the point to a given position. 
   ** 
   ** The original point is changed. 
   ** 
   ** @param x  the first coordinate of the position, 
   ** @param y  the second coordinate of the position. 
   ** 
   ** @return   the point, after tranformation. 
   **
   ** @see      xCoord, yCoord. 
   **/
  Point& moveTo(const double& x, const double& y) 
  { XCoord = x;  YCoord = y;  return *this; } 

  /** Modification method, translating the point along a vector. 
   ** 
   ** The original point is changed (addition without modification 
   ** is obtained using operator+(const iSeeML::geom::Vector&) const). 
   ** 
   ** @param v  the vector of the translation.
   ** 
   ** @return   the point, after translation. 
   **/
  Point& translate(const iSeeML::geom::Vector& v)
  { XCoord += v.xCoord();  YCoord += v.yCoord();  return *this; } 


  // === Operators ================================================

  /** Equality operator between points. 
   **
   ** @param   other   another point. 
   **
   ** @return  whether the two points are equal. 
   **
   ** @see     xCoord, yCoord, iSeeML::Object::isZero. 
   **/
  bool operator==(const Point& other) const 
  { return( isZero( xCoord() - other.xCoord() ) &&
	    isZero( yCoord() - other.yCoord() ) ); } 


  /** Sum operator between a point and a vector, giving 
   ** the translation of the current point along the vector. 
   ** 
   ** @param  v  the vector of the translation.
   ** 
   ** @return    the translated point. 
   **
   ** @see       translate. 
   **/
  Point operator+(const iSeeML::geom::Vector& v) const
  { Point res(*this);  return res.translate(v); }

  /** Difference operator between a point and a vector, giving 
   ** the translation of the current point along the opposite of 
   ** the vector. 
   ** 
   ** @param  v  the opposite vector of the translation.
   ** 
   ** @return    the translated point. 
   **
   ** @see       operator+, iSeeML::geom::Vector::operator-(). 
   **/
  Point operator-(const iSeeML::geom::Vector& v) const 
  { return( *this + (-v) ); }


  /** Difference operator between two points, giving the vector 
   ** connecting these points. 
   **
   ** @param  other  another point. 
   **
   ** @return        the vector going from the second given point 
   **                to the first one (the current). 
   **/
  iSeeML::geom::Vector operator-(const Point& other) const 
  { return( Vector( xCoord() - other.xCoord(), 
		    yCoord() - other.yCoord() ) ); } 


  // === Various Methods ==========================================

  /** Various method, giving the distance between two points. 
   **
   ** It simply computes the length of the connecting vector. 
   **
   ** @param  other  another point. 
   **
   ** @return        the distance between the two points. 
   **
   ** @see       operator-(const iSeeML::geom::Point&), 
   **            iSeeML::geom::Vector::length. 
   **/
  double distance(const Point& other) const 
  { return( (other - *this).length() ); } 

  /** @brief Gives the square of the distance between two points. 
   **
   ** It simply computes the square length of the connecting vector. 
   **
   ** @param  other  another point. 
   **
   ** @return        the square of the distance between the two points. 
   **
   ** @see       operator-(const iSeeML::geom::Point&), 
   **            iSeeML::geom::Vector::sqrLength. 
   **/
  double sqrDist(const Point& other) const 
  { return( (other - *this).sqrLength() ); }
  
}; // end of class iSeeML::geom::Point

#endif  // end of definition 


