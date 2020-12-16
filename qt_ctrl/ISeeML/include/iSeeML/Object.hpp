// 
// Definition of iSeeML::Object, superclass of every other ISeeML 
// objects' class. 
//


// To only include this definition once
#ifndef ISEEML_OBJECT_HH
#define ISEEML_OBJECT_HH


// === Inclusion of standart definitions ===
// Some classes need the definition of input/output streams.
#include <iostream>
using namespace std;  // to avoid writing std::...
// Some classes need the definition of strings.
#include <string>
// Some classes need the definition of mathematical functions.
#define _USE_MATH_DEFINES
#include <math.h>
// === Inclusion of iSeeML' definitions ===
// Definition of relations between ISeeML compilation flags.
#include <iSeeML/CompilerFlags.h>
// Definition of ISeeML' namespaces.
#include <iSeeML/NameSpaces.hpp>


/** This class is the base class of ISeeML: 
 ** every class of ISeeML inherits this one. 
 **
 ** Every ISeeML object has to overload className(), clone() 
 ** and writeTo() virtual methods. 
 ** Method className is used to detect problems 
 ** with virtual methods (e.g. 
 **  @link iSeeML::geom::Object::operator==() "equality operator" 
 **  @endlink of @ref iSeeML::geom::Object "geometric objects"). 
 ** Method clone is used in arrays of objects. 
 ** Method writeTo is used by output stream's operator<<. 
 **
 ** This class also provides a set of common methods (@ref sqr, 
 ** ..., @ref max), some of them using a private static small 
 ** value. 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::Object {
  // === Object's Fields ==========================================

  /** A small value under which doubles are considered as zero.
   ** In ISeeML, double values are never compared to zero, but they 
   ** are considered as zero iff their absolute value is smaller 
   ** than this value. 
   **/ 
  static const double smallDouble; 

  /** The class name is private, as this class is abstract. 
   **
   ** @see className. 
   **/
  static const string ClassName; 

public:
  // === Constructors and Destructors =============================

  /// This virtual class needs a virtual destructor. 
  virtual ~Object() {} 


  // === Description Methods ======================================

  /** Description method, giving the object's class name. 
   ** 
   ** This method is virtual in order to return the correct value 
   ** through polymorphism, while a static field is also defined 
   ** (but this one is private in abstract class). 
   **
   ** Class name is used in error messages, and to verify type 
   ** in redefinitions of some virtual methods.
   **
   ** @return  the class name as a string.
   **
   ** @see     ClassName. 
   **/
  virtual const string className() const { return ClassName; } 


  /** Description method, giving a copy of the current object. 
   **
   ** This clone is dynamically allocated (and can be built easily 
   **  using a copy constructor), it has to be deleted later.
   **
   ** @pre       memory should not be full.
   ** If memory is full and @c ISEEML_CHECK_NIL_POINTER is defined 
   ** (see CompilerFlags.h), an error message is generated and 
   ** the program exits. 
   **
   ** @return  a copy/clone of the current object. 
   **/
  virtual iSeeML::Object& clone() const = 0; 


  /** Description method, writing a description of the object
   ** into a given output stream. 
   ** This method is preferred to a toString method, as writting 
   ** into a stream is much easier than writting into a string. 
   **
   ** @param  O  the output stream in which description 
   **            is written. 
   **/
  virtual void writeTo(ostream& O) const = 0; 

protected:
  /** Description method for algebraic vectors, 
   ** giving the dimension of the containing space
   ** (default is zero). 
   **
   ** This method is not in a seperate class, to avoid multiple 
   ** inheritance (which drastically increases the computation time).
   **
   ** This representation is mainly used to define 
   ** @ref algWriteTo "a unified writeTo". 
   ** Dimension cannot be static, as it is virtual. 
   **/ 
  virtual int algDimension() const { return 0; } 

  /** Description method for algebraic vectors, giving coordinate 
   ** of given index for this vector. 
   **
   ** This method is not in a seperate class, to avoid multiple 
   ** inheritance (which drastically increases the computation time).
   **
   ** @param  i  the index of the searched coordinate.
   **
   ** @pre  an error message is generated if check flag 
   ** @c ISEEML_CHECK_OBJECT_PRECOND (see CompilerFlags.h) 
   ** is defined, as this method should not be called 
   ** (it should be overloaded). 
   **
   ** @return    0.
   **/
  virtual double algCoord(const int) const { 
#ifdef ISEEML_CHECK_OBJECT_PRECOND
    cerr << ClassName << "::algCoord: empty algebraic vector," 
	 << endl << " returning zero..." << endl; 
#endif
    return 0; } 

  /** Description method for algebraic vectors, 
   ** writing this vector in a given output stream: 
   ** coordinates for each dimension are written, between 
   ** paranthesis and separated by commas. 
   **
   ** This method is not in a seperate class, to avoid multiple 
   ** inheritance (which drastically increases the computation time).
   **
   ** @param  O  the output stream in which description is written. 
   **
   ** @see    algCoord, algDimension, writeTo. 
   **/
  virtual void algWriteTo(ostream& O) const { 
    int i; O << '('; for(i = 1; i <= algDimension(); i++)  
      O << algCoord(i) << (i == algDimension() ? ")" : ", "); } 

  // === Modification Methods =====================================

  // === Operators ================================================

public:
  // === Various Methods ==========================================

  /** Method verifying that a given object has the same type 
   ** (same class name) than the current one.
   **
   ** @param   other  the given object.
   **
   ** @return  whether the two objects have the same class name.
   **
   ** @see     className.
   **/
  bool sameClass(const iSeeML::Object& other) 
  { return( className() == other.className() ); }


  /** Method giving the minimum of two elements. 
   **
   ** This method uses templates, the elements do not have to be 
   ** ISeeML objects. 
   **
   ** @param a  A first element, 
   ** @param b  a second one. 
   **
   ** @return   the minimum between the two given elements. 
   **/
  template <class T>
  static const T& min(const T& a, const T& b) 
  { return( a < b ? a : b ); } 

  /** Method giving the maximum of two elements. 
   **
   ** This method uses templates, the elements do not have to be 
   ** ISeeML objects. 
   **
   ** @param a  A first element, 
   ** @param b  a second one. 
   **
   ** @return   the maximum between the two given elements. 
   **/
  template <class T>
  static const T& max(const T& a, const T& b) 
  { return( a > b ? a : b ); } 


  /** Method giving the square of a double. 
   **
   ** @param x  the double whose square is wanted. 
   **
   ** @return   the square of the given double. 
   **/
  static double sqr(const double& x)  { return( x * x ); } 


  /** Method giving the angle between - @f$\pi@f$ 
   ** (excluded) and @f$\pi@f$ (included), which is equal 
   ** to a given angle modulo 2 @f$\pi@f$. 
   **
   ** @param theta  an angle. 
   **
   ** @return   the given angle modulo 2 @f$\pi@f$, between - 
   **           @f$\pi@f$ (excluded) and @f$\pi@f$ (included). 
   **/
  static double mod2pi(const double& theta) 
  { double res = theta; const double PIx2 = 2 * M_PI; 
    while  (res > M_PI)    res -= PIx2; 
    while (res <= - M_PI)  res += PIx2;  return res; } 

  /** @brief Method transforming an angle in radian, @f$x \pi@f$ 
   **        with @f$x@f$ between -1 (excluded) and 1 (included), 
   **        into its equivalent in degree, i.e. @f$90 x@f$ . 
   ** @param theta  an angle in gradian. 
   ** @return   the angle in degree. 
   **/
  static double rad2deg(const double& theta) 
  { return theta * 180 / M_PI; } 

  /** @brief Method transforming an angle in degree, @f$90 x@f$ 
   **        with @f$x@f$ between -1 (excluded) and 1 (included), 
   **        into its equivalent in radian, i.e. @f$x \pi@f$ . 
   ** @param theta  an angle in degree. 
   ** @return   the angle in gradian. 
   **/
  static double deg2rad(const double& theta) 
  { return theta * M_PI / 180; } 


  /** Method telling whether a double is strictly 
   ** positive. 
   **
   ** @param x  the double checked. 
   **
   ** @return   true if the double is bigger than ISeeML' small 
   **           value. 
   **/
  static bool isPositive(const double& x) 
  { return( x > smallDouble ); } 

  /** Method telling whether a double is strictly 
   ** negative. 
   ** 
   ** Computes the double's opposite, and checks if it is positive. 
   **
   ** @param x  the double checked. 
   **
   ** @return   true if the opposite of the double is positive. 
   **
   ** @see      isPositive. 
   **/
  static bool isNegative(const double& x) 
  { return( isPositive(- x) ); } 

  /** Method comparing a double to zero. 
   **
   ** @param x  the double compared to zero. 
   **
   ** @return   true if the double's absolute value is smaller than 
   **           ISeeML' small value. 
   **/
  static bool isZero(const double& x) 
  { return( fabs(x) < smallDouble ); } 


  /** Method giving the sign of a double. 
   **
   ** @param x  the double which sign is searched. 
   **
   ** @return   the sign of the double, as an integer in {-1, 0, 
   **           1}. 
   **
   ** @see      isZero, isPositive, isNegative.
   **/
  static int sign(const double& x) 
  { return( isZero(x) ? 0 : ( x < 0 ? -1 : 1) ); } 

}; // end of class iSeeML::Object


/** Modification method, writing a description of a ISeeML object
 ** in a given output stream. 
 **
 ** @relates   iSeeML::Object
 **
 ** @param  O  the output stream in which description is written, 
 ** @param  o  the ISeeML object whose description is written. 
 **
 ** @return    the output stream, after writing in it.
 **
 ** @see       iSeeML::Object::writeTo.
 **/
inline ostream& operator<<(ostream& O, const iSeeML::Object& o) 
{ o.writeTo(O); return O; } 


#endif  // end of definition 

