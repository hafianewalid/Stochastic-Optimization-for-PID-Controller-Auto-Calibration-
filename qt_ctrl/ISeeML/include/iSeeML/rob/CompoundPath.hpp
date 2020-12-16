//
// Definition of iSeeML::rob::CompoundPath, superclass of every 
// other ISeeML compound/complex/composed robotic paths' class.
//

// To only include this definition once
#ifndef ISEEML_COMP_PATH_HH
#define ISEEML_COMP_PATH_HH


// This class needs the definition of iSeeML::rob::CurvConfig. 
#include <iSeeML/rob/CurvConfig.hpp>
// This class needs the definition of iSeeML::rob::BasicPath. 
#include <iSeeML/rob/BasicPath.hpp>

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
class iSeeML::rob::CompoundPath : public iSeeML::rob::Path { 
  // === Object's Fields ==========================================

  // The set of basic paths is defined in sub-class, and is 
  // manipulated through piece and replace virtual methods, as well
  // as through copy operator.

  /** The class name is private, as this class is abstract. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  // === Constructors and Destructors =============================

  // None, this is an abstract class with no object's fields. 

public:
  // === Description Methods ======================================

  /** Macro to process all pieces. 
   **
   ** Can be used as a for, followed by a bloc of commands in which 
   ** the current piece is given by @c pieces(i). 
   **/
#define ISEEML_ROB_COMPATH_forall_pieces(i) \
		int i;  for(i = 1; i <= nbPieces(); i++) 

  /** Description method, writing a description of the path 
   ** in a given output stream (the array of pieces is written).  
   **
   ** The array of basic paths is written between accolades ('{' 
   ** and '}'), paths being separated by commas. 
   **
   ** @param  O  the output stream in which description is written. 
   **
   ** @see  nbPieces, piece. 
   **/
  void writeTo(ostream& O) const {  
    O << '{'; ISEEML_ROB_COMPATH_forall_pieces(i)
      O << piece(i) << (i == nbPieces() ? "}" : ", "); } 


  /** Description method, giving the path's starting configuration 
   ** (first piece's starting configuration). 
   **
   ** @return  the path's starting configuration. 
   **
   ** @see     piece, BasicPath::start. 
   **/
  const iSeeML::rob::OrPtConfig& start() const  { return piece(1).start(); }

  /** Description method, giving the path's final configuration
   ** (last piece's starting configuration). 
   **
   ** @return  the path's final configuration. 
   **
   ** @see     piece, BasicPath::end. 
   **/
  const iSeeML::rob::OrPtConfig& end()   const 
  { return piece( nbPieces() ).end(); }


  /** Description method, returning the (constant) basic path 
   ** of given index in the list of which this compound path 
   ** is made.
   **
   ** This method, as well as the other @p piece method, calls 
   ** a protected one (@p getPiece).
   ** 
   ** @param   index  the index of the desired piece 
   **                 (between 1 and the result of @ref nbPieces). 
   ** 
   ** @pre     parameter is an index, and should be in 
   ** the correct interval.
   ** An error message is generated if this index is not correct 
   ** and @c ISEEML_CHECK_COMP_PATH_PRECOND is defined, and index 
   ** is corrected (to one if smaller, to @ref nbPieces if bigger) 
   ** if @c ISEEML_CHECK_ARRAY_ELEMT is defined 
   ** (see CompilerFlags.h). 
   ** 
   ** @return  the piece of the path whose index has been given. 
   **
   ** @see     nbPieces.
   **/
  const iSeeML::rob::BasicPath& piece(const int index) const { 
    // cout << this << "->CP::piece(" << index << ") const" << endl; 
    return getPiece(index); } 


  /** Description method, giving the path's length 
   ** (sum of the pieces' length). 
   **
   ** @return  the path's length. 
   **
   ** @see     piece, BasicPath::length. 
   **/
  double length() const { 
    double res = 0; ISEEML_ROB_COMPATH_forall_pieces(i)
      res += piece(i).length(); return res; } 

  /** Description method, giving the path's deflection 
   ** (sum of the pieces' deflection). 
   **
   ** @return    the path's deflection. 
   **
   ** @see     piece, BasicPath::deflection. 
   **/
  double deflection() const { 
    double res = 0; ISEEML_ROB_COMPATH_forall_pieces(i)
      res += piece(i).deflection(); return res; } 

  /** Description method, giving the number of pieces of the path. 
   ** @return  the number of pieces of the path. 
   ** @version  1.1
   **/
  virtual int nbPieces() const = 0;

  // === Modification Methods =====================================

  /** Modification method, returning (for modification) the basic 
   ** path of given index in the list of which this compound path 
   ** is made.
   **
   ** @param index    the index of the basic path to change 
   **                 (between 1 and the result of @ref nbPieces). 
   ** 
   ** @pre     parameter is an index, and should be in 
   ** the correct interval.
   ** An error message is generated if this index is not correct 
   ** and @c ISEEML_CHECK_COMP_PATH_PRECOND is defined, and index 
   ** is corrected (to one if smaller, to @ref nbPieces if bigger) 
   ** if @c ISEEML_CHECK_ARRAY_ELEMT is defined 
   ** (see CompilerFlags.h). 
   **
   ** @see   nbPieces.
   **/
  iSeeML::rob::BasicPath& piece(const int index) { 
    // cout << this << "->CP::piece(" << index << ")" << endl; 
    return getPiece(index); } 


  // === Operators ================================================

  /** Description method, giving a configuration at a given arc 
   ** length along the path. 
   **
   ** @param  s  the arc length. 
   ** 
   ** @pre    the given arc length should be positive and less 
   ** than the path's @ref length. 
   ** If this is not true, an error message is generated if 
   ** @c ISEEML_CHECK_COMP_PATH_PRECOND is defined, and the arc 
   ** length is considered as zero if smaller and as path's 
   ** @ref length if bigger, if @c ISEEML_CHECK_ARRAY_ELEMT is 
   ** defined (see CompilerFlags.h).
   **
   ** @return    the configuration at given arc length along 
   **            the path. 
   **
   ** @see     nbPieces, length, piece, BasicPath::operator[]. 
   **/
  iSeeML::rob::CurvConfig operator[](const double& s) const {
    int pieceNb = 1;   // Start from the first piece
    double arcLength = s, l = piece(pieceNb).length(); 
#ifdef ISEEML_CHECK_COMP_PATH_PRECOND
    if (s < 0)  cerr << ClassName << "::operator[]:"
		     << " negative arc length " << s << endl; 
#endif
    // Search the piece corresponding to the arc length
    while (arcLength > l) 
#ifdef ISEEML_CHECK_COMP_PATH_PRECOND
      if ( pieceNb == nbPieces() ) 
	cerr << ClassName << "::operator[]:"
	     << " arc length " << s << endl 
	     << " bigger than length " << length() << endl; 
      else 
#endif
	{ arcLength -= l; 
#ifdef ISEEML_CHECK_ARRAY_ELEMT
	if ( pieceNb == nbPieces() )  l = arcLength;  // stop 
	else 
#endif
	  l = piece(++pieceNb).length(); }
    // Return the configuration in the selected piece
    return( piece(pieceNb)[arcLength] ); } // end of operator[]

protected:
  // === Various Methods ==========================================

  /** Method returning the basic path of given index in the list 
   ** of which this compound path is made.
   ** 
   ** @param   index  the index of the desired piece 
   **                 (between 1 and the result of @ref nbPieces). 
   ** 
   ** @pre     parameter is an index, and should be in 
   ** the correct interval.
   ** An error message is generated if this index is not correct 
   ** and @c ISEEML_CHECK_COMP_PATH_PRECOND is defined, and index 
   ** is corrected (to one if smaller, to @ref nbPieces if bigger) 
   ** if @c ISEEML_CHECK_ARRAY_ELEMT is defined 
   ** (see CompilerFlags.h). 
   ** 
   ** @return  the basic path whose index has been given. 
   **
   ** @see     nbPieces, piece, _piece.
   **/
  iSeeML::rob::BasicPath& getPiece(const int index) const { 
    // cout << this << "->CP::getPiece(" << index << ")" << endl; 
#ifdef ISEEML_CHECK_COMP_PATH_PRECOND
    if ( !nbPieces() )
      cerr << ClassName << "::piece: no pieces!!!" << endl;
    else if ( (index < 1) || ( index > nbPieces() ) )
      cerr << ClassName << "::piece: abnormal index " 
	   << index << ", not in 1.." << nbPieces() << endl; 
#endif
    const int nb = 
#ifdef ISEEML_CHECK_ARRAY_ELEMT
      index < 2 ? 1 : index >= nbPieces() ? nbPieces() : 
#endif
    index;  return _piece(nb); } 

  /** Virtual method returning the basic path of given index 
   ** in the list of which this compound path is made 
   ** (no verification).
   ** 
   ** @param   index  the index of the desired piece 
   **                 (between 1 and the result of @ref nbPieces). 
   ** 
   ** @return  the basic path whose index has been given. 
   ** 
   ** @see     nbPieces, piece, getPiece.
   **/
  virtual iSeeML::rob::BasicPath& _piece(const int index) const = 0;

}; // end of class iSeeML::rob::CompoundPath

#endif  // end of definition 

