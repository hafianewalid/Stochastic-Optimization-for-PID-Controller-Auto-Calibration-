//
// Definition of iSeeML::rob::ArrayPaths, example of compound robotic
// path made of an array of paths.
//

// To only include this definition once
#ifndef ISEEML_ARRAY_PATHS_HH
#define ISEEML_ARRAY_PATHS_HH

// This class needs the definition of iSeeML::rob::CompoundPath. 
#include <iSeeML/rob/CompoundPath.hpp>

/** This class defines compound (or complex, or composed) paths, 
 ** which are made of a set 
 ** of @ref iSeeML::rob::BasicPath "basic paths". 
 **
 ** Nearly all virtual methods of iSeeML::rob::Path are defined in 
 ** this class (except className and clone), sub-classes ``just'' 
 ** have to define this one and constructors (this is not so easy, 
 **  as it generally requires a planning method). 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::ArrayPaths : public iSeeML::rob::CompoundPath {
  class CoupleTypePointer {
  public:
    bool isBasicPath;
    iSeeML::rob::Path *path;
    CoupleTypePointer() : isBasicPath(true), path(NULL) {}
    CoupleTypePointer(const bool basic, iSeeML::rob::Path *ptr)
      : isBasicPath(basic), path(ptr) {}
    int nbPieces() const {
      return ( isBasicPath ? 1 :
	       ( (iSeeML::rob::CompoundPath*)(path) )->nbPieces() );
    } // end of int nbPieces() const
  }; // end of class CoupleTypePointer
  
  // === Object's Fields ==========================================

  /// This path is made of a set of paths, dynamically allocated.
  CoupleTypePointer *array;

  /// The number of paths.
  const int nb_paths;

public:
  /** The class name is public, as this class can be instanced. 
   **
   ** @see className. 
   **/
  static const string ClassName; 

  // === Constructors and Destructors =============================

  /** @brief The main constructor only needs a number of paths. 
   ** @param n  the number of paths in this array.
   **/
  ArrayPaths(const int n)
    : array(new CoupleTypePointer[n]), nb_paths(n) {}
  
  /** @brief The destructor frees all the paths and the array. **/
  ~ArrayPaths() {
    for(int idx = 0; idx < nb_paths; idx++)
      free(array[idx].path);
    free(array); 
  } // end of ~ArrayPaths()

  /** @brief Add a given (dynamically allocated) path to the array.
   ** @param index    the index of the added path,
   ** @param isBasic  whether the added path is a basic one,
   ** @param path     a pointer on the added path.
   **/
  void setPath(const int index,
	       const bool isBasic, iSeeML::rob::Path *path) {
#ifdef ISEEML_CHECK_COMP_PATH_PRECOND
    if ( (index < 0) || (index >= nb_paths) )
      cerr << ClassName << "::setPath: incorrect index "
	   << index << " (not in 0-" << nb_paths - 1 << ")!" << endl; 
#endif
    array[index] = CoupleTypePointer(isBasic, path); 
  } // end of void setPath(const int, const bool, iSeeML::rob::Path*)
  
  // === Description Methods ======================================

  // Cf Object::clone()
  iSeeML::Object& clone() const {
    ArrayPaths *new_one = new ArrayPaths(nb_paths);
    for(int idx = 0; idx < nb_paths; idx++)
      new_one->setPath( idx, array[idx].isBasicPath,
			(iSeeML::rob::Path*)
			(& array[idx].path->clone() ) );
    return *( (iSeeML::Object*)(new_one) );
  } // end of iSeeML::Object& clone() const
    
  // Cf CompoundPath::nbPieces()
  int nbPieces() const {
    int nb, idx; 
    for(nb = 0, idx = 0; idx < nb_paths; idx++)
      nb += array[idx].nbPieces();
    return nb;
  } // end of int nbPieces() const

protected:
  // === Various Methods ==========================================

  // Cf CompoundPath::_piece
  iSeeML::rob::BasicPath& _piece(const int index) const {
    int idx = index, i = 0, pieces;
    bool step;
    do {
      pieces = array[i].nbPieces();
      step = (idx > pieces); 
      if (step) { idx -= pieces; i++; }
    } while ( (i < nb_paths) && (step) );
    // if index was too big, return the last piece
    if (i >= nb_paths) { idx = pieces ; i = nb_paths - 1; }
    return ( array[i].isBasicPath ?
	     *( (iSeeML::rob::BasicPath*) (array[i].path) ) :
	     ( (iSeeML::rob::CompoundPath*) (array[i].path)
	       )->piece(idx) ); 
  } // end of iSeeML::rob::BasicPath& _piece(const int)
  
}; // end of class iSeeML::rob::ArrayPaths

#endif  // end of definition 
