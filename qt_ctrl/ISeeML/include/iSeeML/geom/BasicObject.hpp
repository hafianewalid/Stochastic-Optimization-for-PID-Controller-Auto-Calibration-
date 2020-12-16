//
// Definition of iSeeML::geom::BasicObject, superclass of every other 
// ISeeML basic 2D geometric objects' class.
//

// To only include this definition once
#ifndef ISEEML_GEOM_BASIC_OBJECT_HH
#define ISEEML_GEOM_BASIC_OBJECT_HH


// This class needs the definition of iSeeML::geom::Object. 
#include <iSeeML/geom/Object.hpp>


/** This class defines basic 2D geometric objects, from which 
 ** compound geometric objects are built. 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::geom::BasicObject : public iSeeML::geom::Object {
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

  // === Operators ================================================

  // === Various Methods ==========================================

}; // end of class iSeeML::geom::BasicObject

#endif  // end of definition 


