//
// Definition of iSeeML::rob::BasicPath, superclass of every other 
// ISeeML basic robotic paths' class.
//

// To only include this definition once
#ifndef ISEEML_BASIC_PATH_HH
#define ISEEML_BASIC_PATH_HH


// This class needs the definition of iSeeML::rob::Path. 
#include <iSeeML/rob/Path.hpp>


/** This class defines superclass of basic paths, from which 
 ** compound paths can be built. 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::BasicPath : public iSeeML::rob::Path { 
  // === Object's Fields ==========================================

  /** The class name is private, as this class is abstract. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  // None (destructor is already virtual, due to Object).


  // === Description Methods ======================================

  /** Description method, giving the number of pieces of the path. 
   ** @return  the number of pieces of the path. 
   ** @version  1.1
   **/
  int nbPieces() const { return 1; }
  
  // === Modification Methods =====================================

  // === Operators ================================================

  // === Various Methods ==========================================

}; // end of class iSeeML::rob::BasicPath

#endif  // end of definition 

