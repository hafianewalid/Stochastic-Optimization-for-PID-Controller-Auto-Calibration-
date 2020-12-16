//
// Definition of iSeeML::geom::Object, superclass of every other 
// ISeeML 2D geometric objects' classes.
//

// To only include this definition once
#ifndef ISEEML_GEOM_OBJECT_HH
#define ISEEML_GEOM_OBJECT_HH


// This class needs the definition of iSeeML::Object. 
#include <iSeeML/Object.hpp>


/** This class is the base class of the 2D geometric namespace of 
 ** ISeeML: every geometric object inherits this one.
 **
 ** Every geometric object has to overload @ref translate method 
 ** and operator==(const iSeeML::geom::Object&) const. 
 ** 
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::geom::Object : public iSeeML::Object {
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


  // === Modification Methods =====================================

  /** Modification method, translating a iSeeML::geom::Object along 
   ** a vector. 
   ** 
   ** @param v  the vector of the translation.
   **
   ** @return   the object once modified. 
   **/ 
  virtual iSeeML::geom::Object& translate
  (const iSeeML::geom::Vector& v) = 0;


  // === Operators ================================================

  /** Equality operator between geometric objects. 
   **
   ** This cannot be a pure virtual method, as type of 
   ** the parameter will change.  This method should however 
   ** always be overloaded, and therefore returns false. 
   **
   ** @param   other   another geometric object. 
   **
   ** @return  false (this method should be overloaded). 
   **/
  virtual bool operator==(const iSeeML::geom::Object& other) const { 
    cerr << ClassName << "::operator==: invalid comparison"
	 << endl << "  (" << ClassName << '/' 
	 << other.className() << "); returns false!" << endl; 
    return false; } 


  // === Various Methods ==========================================

}; // end of class iSeeML::geom::Object

#endif  // end of definition 

