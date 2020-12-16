//
// Definition of iSeeML::rob::Object, superclass of every other 
// ISeeML robotic objects' class.
//

// To only include this definition once
#ifndef ISEEML_ROB_OBJECT_HH
#define ISEEML_ROB_OBJECT_HH


// This class needs the definition of iSeeML::Object. 
#include <iSeeML/Object.hpp>


/** This class is the base class of robotic namespace of ISeeML: 
 ** every robotic object inherits this one.
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::Object : public iSeeML::Object {
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

}; // end of class iSeeML::rob::Object

#endif  // end of definition 

