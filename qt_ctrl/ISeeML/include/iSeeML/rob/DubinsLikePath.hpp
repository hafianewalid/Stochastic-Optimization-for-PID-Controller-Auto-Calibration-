//
// Definition of iSeeML::rob::DubinsLikePath, superclass 
// of every ISeeML robotic paths similar 
// to @ref iSeeML::rob::DubinsPath "Dubins paths".
//

// To only include this definition once
#ifndef ISEEML_DUBINS_LIKE_PATH_HH
#define ISEEML_DUBINS_LIKE_PATH_HH


// This class needs the definition of iSeeML::rob::CompPath. 
#include <iSeeML/rob/CompoundPath.hpp>
// This class needs the definition of iSeeML::rob::LinCurvPath. 
#include <iSeeML/rob/LinCurvPath.hpp>


/** This class defines Dubins like paths, which are similar 
 ** to @ref iSeeML::rob::DubinsPath "Dubins paths". 
 **
 ** A Dubins like path is made of at most three parts, which can be 
 ** a turn or a straight line. 
 ** @ref iSeeML::rob::DubinsPath "Dubins paths"' parts are circular 
 ** arcs or line segments, with a ponctual change of curvature 
 ** at each part's end, while @ref iSeeML::rob::FscPath "FSC paths"' 
 ** turns start and finish with zero curvature. 
 ** Each part is represented by one to three pieces, which are 
 ** @ref iSeeML::rob::LinCurvPath "linear curvature paths". 
 **
 ** @author   @ref AjS "Alexis Scheuer".
 ** @version  1.0
 **/
class iSeeML::rob::DubinsLikePath : public iSeeML::rob::CompoundPath 
{ 

  // === Inner Type Definitions ===================================
public:
  /** Dubins-like paths can be of six types: their first and last 
   ** parts are turns, their middle part is tangent to the first 
   ** and last parts but can be a line segment or a turn. 
   ** 
   ** Thus, Dubins-like paths' types can be named by three 
   ** letters: the first and third are @a l or @a r (Left or Right 
   ** turn) and the second is @a l, @a r or @a s (Left or Right 
   ** turn, or Segment). 
   **/
  enum Type {
    /// Path made of a left turn, a straight line and a left turn. 
    lsl, 
    /// Path made of a left turn, a straight line and a right turn. 
    lsr, 
    /// Path made of a right turn, a straight line and a left turn. 
    rsl, 
    /// Path made of a right turn, a straight line and a right turn.
    rsr, 
    /// Path made of a left turn, a right turn and a left turn. 
    lrl, 
    /// Path made of a right turn, a left turn and a right turn.  
    rlr  }; 

  /// Some methods will need these integer values. 
  enum {
    /// the number of possible paths (6).  @see Type.
    nbPossiblePaths = 6, 
    /// the number of usefull turning circles (4).
    nbTurningCircles = 4  }; 

private:
  // === Object's Fields ==========================================

  /** The class name is private, as this class is abstract. 
   **
   ** @see className. 
   **/
  static const string ClassName; 


  /** Number of 
   ** @ref iSeeML::rob::LinCurvPath "linear curvature paths" 
   ** in this Dubins like paths.
   **/
  int NbPieces; 
  /// Corresponding array of linear curvature paths.
  iSeeML::rob::LinCurvPath *LcPieces; 

  Type PathType;   ///< Type of the Dubins-like path. 

  /// Maximum curvature along the Dubins-like path.
  double MaximumCurvature; 
  
protected:
  // === Constructors and Destructor ==============================

  /** The structure can be filled using this constructor, which is 
   ** protected (only defined for sub-classes). 
   ** No other constructor is defined, as this class is virtual. 
   **
   ** @param type      the type of Dubins' path, 
   ** @param maxCurv   the maximum curvature along the path 
   **                  (taken in absolute value).
   **/ 
  DubinsLikePath(const Type& type, const double& maxCurv) :
    NbPieces(0), LcPieces(NULL), PathType(type), 
    MaximumCurvature(fabs(maxCurv)) {} 

  /** The copy constructor (the default one is not correct).
   ** 
   ** The array of pieces of the other path is copied. 
   **
   ** @param other  the copied path. 
   **/
  DubinsLikePath(const DubinsLikePath& other) : 
    NbPieces(0), LcPieces(NULL) { *this = other; } 


  /** The destructor (the default one is not correct).
   ** 
   ** The array of pieces of the other path is freed. 
   **/
  ~DubinsLikePath() { if (LcPieces != NULL)  delete[] LcPieces; } 


  // === Description Methods ======================================

  /** Description method, giving the sign (1 for left, -1 for 
   ** right, 0 for a segment) of the turn or segment whose 
   ** number is given for a path of given type. 
   **
   ** As an example, a @b lsr type path has respective parts signs 
   ** 1, 0 and -1, when a @b rlr type path has respective parts 
   ** signs -1, 1 and -1. 
   **
   ** @param  number  the number of the part whose sign is wanted, 
   ** @param  type    the type of the considered path. 
   **
   ** @pre    the given number of the part should be between 1 
   **         and 3.  If not, an error message is generated if 
   **         @c ISEEML_CHECK_DLIKE_PATH_PRECOND is defined, and 
   **         the number is considered as 1 if smaller and 3 if 
   **         bigger, if @c ISEEML_CHECK_ARRAY_ELEMT is defined 
   **         (see CompilerFlags.h).
   **
   ** @return the part's sign. 
   **/
  static int turnSign(const int number, const Type& type) { 
    static int values[3][nbPossiblePaths] = 
    { {1,  1, -1, -1,  1, -1}, {0,  0,  0,  0, -1,  1}, 
      {1, -1,  1, -1,  1, -1} }; 
#ifdef ISEEML_CHECK_DLIKE_PATH_PRECOND
    if ( (number < 1) || (number > 3) )
      cerr << ClassName << "::turnSign: abnormal number " 
	   << number << endl; 
#endif
    const int nb = 
#ifdef ISEEML_CHECK_ARRAY_ELEMT
      number < 1 ? 0 : (number > 3) ? 2 : 
#endif
      number - 1; 
    return(values[nb][(int) type]); } // end of int turnSign(...)


public:
  // cf. CompoundPath 
  int nbPieces() const { return NbPieces; } 

  /** Description method, giving the path's type 
   ** (from @ref lsl to @ref rlr). 
   **
   ** @return    the path's type. 
   **/
  const Type&   type()     const { return PathType; }

  /** Description method, giving the path's maximum curvature. 
   **
   ** @return    the path's maximum curvature. 
   **/
  const double& maxCurv()  const { return MaximumCurvature; }


  /** Description method, returning the (linear curvature) piece 
   ** of the path with a given index. 
   ** 
   ** @param index  the index of the desired piece (between 1 and 
   **               the result of nbPiece()). 
   **
   ** @pre    the given index should be between one and 
   ** the result of @ref nbPieces. 
   **
   ** @return       the piece of the path whose index has been 
   **               given. 
   **
   ** @see          piece. 
   **/
  const iSeeML::rob::LinCurvPath& lcPiece(const int index) const {
    return( *((LinCurvPath*) &( piece(index) )) ); } 


  /** Description method, giving the radius of the turning circle 
   ** (the circle of all the configurations which can be reached 
   **  from a fixed one). 
   **
   ** @ref maxCurv "Maximum curvature" of the current path is used. 
   ** It should not be zero, or this method will return infinity. 
   **
   ** @return  the turning circle's radius. 
   **/
  virtual double turnRadius() const = 0; 


protected:
  /** Description method, computing the number of pieces needed 
   ** for a turn of given length. 
   **
   ** @param  length  the turn's length. 
   **
   ** @return         the number of needed pieces. 
   **/
  virtual int turnNbPieces(const double& length) const = 0; 


  /** Description method, trying to compute the length of the line 
   ** segment connecting the turning circles and the angle between 
   ** the segment connecting the turning centers and the previous 
   ** one. 
   **
   ** The angle can only be computed if the square distance between 
   ** the turning centers is greater than a value depending of 
   ** the path's type. 
   ** Length is not computed for @ref lrl or @ref rlr path's type, 
   ** and is negative when previous angle cannot be computed.
   **
   ** @param dist   the distance between the turning centers, 
   ** @param length the length to compute (modified by the method), 
   ** @param angle  the angle to compute  (modified by the method). 
   **
   ** @return       whether the computation was succesful or not 
   **               (whether the distance @p dist2 is greater 
   **                than a value depending of the path's type). 
   **/
  virtual bool getConnection(const double& dist, double& length, 
			     double& angle) const = 0; 


  // === Modification Methods =====================================

  /** Modification method, returning for modification 
   ** the (linear curvature) piece of the path with a given index. 
   ** 
   ** @param index  the index of the desired piece (between 1 and 
   **               the result of nbPiece()). 
   **
   ** @pre    the given index should be between one and 
   ** the result of @ref nbPieces. 
   **
   ** @return       the (modifiable) piece of the path 
   **               whose index has been given. 
   **
   ** @see          piece. 
   **/
  iSeeML::rob::LinCurvPath& lcPiece(const int index) {
    return( *((LinCurvPath*) &( piece(index) )) ); } 

private:
  /** Modification method, changing the set of basic paths 
   ** from which this Dubins-like path is made to a given number 
   ** of clones of a given basic path (these clone can be changed 
   **  later using @ref replace).
   **
   ** @param newNbPieces  the new number of pieces, 
   ** @param stdPath      the default path, cloned to define 
   **                     each piece.
   **
   ** @pre    the number of pieces should be positive.
   ** if not,  path is made of zero pieces and, 
   ** if @c ISEEML_CHECK_DLIKE_PATH_PRECOND is defined 
   ** (see CompilerFlags.h), an error message is generated.
   **
   ** @pre       memory should not be full.
   ** If memory is full and @c ISEEML_CHECK_NULL_POINTER is defined 
   ** (see CompilerFlags.h), an error message is generated and 
   ** the program exits. 
   **/
  void reset(const int newNbPieces, 
	     const iSeeML::rob::LinCurvPath& stdPath) { 
    // cout << this << "->DLP::reset(" << newNbPieces << ")..." << endl; 
    // free previous LinCurvPath array
    if (LcPieces != NULL)  delete[] LcPieces; 
#ifdef ISEEML_CHECK_DLIKE_PATH_PRECOND
    if (newNbPieces < 0) 
      cerr << ClassName << "::reset: negative nb of pieces," 
	   << endl << "  considered as zero!" << endl; 
#endif
    NbPieces = newNbPieces > 0 ? newNbPieces : 0; 
    if (NbPieces) { 
      LcPieces = new LinCurvPath[NbPieces]; 
#ifdef ISEEML_CHECK_NULL_POINTER
      if (LcPieces == NULL) { 
	cerr << ClassName << "reset: out of memory!!!" << endl; 
	exit(-1); }
#endif
      ISEEML_ROB_COMPATH_forall_pieces(i)  lcPiece(i) = stdPath; } 
    else
      LcPieces = NULL; }


  /** Modification method, computing the pieces of the current path 
   ** so as to reach a given configuration from a starting one, 
   ** using the given set of turning circles' centers. 
   ** 
   ** @warning 
   ** this method may not find a set of pieces connecting the given 
   ** configurations, due to the @ref type of the path, its @ref 
   ** maxCurv "maximum curvature" (which can be too small), etc.
   ** In that case, the set of path is reduced to a zero length 
   ** line segment at the given starting configuration (see @ref 
   ** setNoPiece). 
   ** 
   ** @param  start    the starting configuration, 
   ** @param  goal     the configuration to reach, 
   ** @param  centers  the set of @ref nbTurningCircles circle 
   **                  centers used to turn. 
   **
   ** @pre    memory should not be full (see @ref setNoPiece or 
   ** @ref forwardDefinePieces "the other definePieces").
   **
   ** @post
   ** if a possible set of pieces has been found, but the goal 
   ** configuration has not been reached (position error is not 
   ** small with respect to the distance), an error message is 
   ** written if @c ISEEML_CHECK_DLIKE_PATH_POSTCOND is defined 
   ** (see CompilerFlags.h). 
   **
   ** @see   getConnection, setNoPiece, 
   **        @ref forwardDefinePieces "the other definePieces".
   **/
  void definePieces(const iSeeML::rob::OrPtConfig& start,
		    const iSeeML::rob::OrPtConfig& goal, 
		    const iSeeML::geom::Point 
		    centers[nbTurningCircles]); 
protected:
  /** Modification method, setting pieces of a non-valid Dubins' 
   ** like path from a starting configuration (pieces array will 
   ** only contain a zero length line segment). 
   **
   ** @param start    the starting configuration. 
   **
   ** @pre       memory should not be full.
   ** If memory is full and @c ISEEML_CHECK_NIL_POINTER is defined 
   ** (see CompilerFlags.h), an error message is generated and 
   ** the program exits. 
   **/
  void setNoPiece(const iSeeML::rob::OrPtConfig& start) {
    reset( 1, iSeeML::rob::LinCurvPath(iSeeML::rob::CurvConfig
				       (start, 0), 0, 0) ); } 

  /** @anchor forwardDefinePieces
   ** Modification method, defining pieces of a Dubins-like path 
   ** from a starting configuration and deflections (change 
   ** of orientation, for turns) or length (for straight lines) 
   ** of each part. 
   **
   ** @warning 
   ** @ref type and @ref maxCurv "maximum curvature" (at least) 
   ** of the current path are used. 
   **
   ** @param start    the starting configuration, 
   ** @param defl1    the deflection of the first part of the path, 
   ** @param lenDefl  the length or deflection of the second part, 
   ** @param defl3    the deflection of the third part of the path. 
   **
   ** @pre       memory should not be full.
   ** If memory is full and @c ISEEML_CHECK_NIL_POINTER is defined 
   ** (see CompilerFlags.h), an error message is generated (in 
   ** this method or in @ref setNoPiece) and the program exits. 
   **
   ** @post
   ** Number of pieces needed is estimated for memory allocation.
   ** Pieces array is then filled using @ref setNoPiece, @ref 
   ** addTurn and @ref addPiece. 
   ** Coherence between estimated and used number of pieces is 
   ** verified if @c ISEEML_CHECK_DLIKE_PATH_POSTCOND is defined 
   ** (see CompilerFlags.h). 
   **
   ** @see   setNoPiece, addTurn, addPiece. 
   **/
  void definePieces(const iSeeML::rob::OrPtConfig& start, 
		    const double& defl1, const double& lenDefl, 
		    const double& defl3); 


  /** Modification method, adding to the pieces of a path a turn 
   ** starting at a given configuration, with a given curvature 
   ** sign and length. 
   ** 
   ** Index @p index of the first undefined piece in the pieces 
   ** array is modified by this method, to remain the index of 
   ** the first undefined piece in the array.
   ** Adress @p start of the next starting configuration is also 
   ** modified along this method. 
   ** Maximum curvature of the current Dubins-like path is used 
   ** in this method. 
   ** 
   ** @param  index     the first free index in the pieces array
   **                   (pieces will be added at this index and 
   **                    after, and the index will be incremented), 
   ** @param  start     a reference to the reference of 
   **                   the starting configuration (it will be 
   **                    changed to a ref. to the ref. of next 
   **                    starting config.), 
   ** @param  defl      the turn's deflection 
   **                   (change of orientation), 
   **                   giving the curvature sign. 
   **
   ** @pre  This method only works if the maximum curvature is not 
   **       zero (otherwise, it writes an error on @c stderr). 
   **/
  virtual void addTurn(int& index, 
		       const iSeeML::rob::OrPtConfig **start, 
		       const double& defl) = 0; 

  /** Modification method, adding to the pieces of a path a piece 
   ** starting at a given configuration (with curvature), with 
   ** a given curvature's derivative and length. 
   ** 
   ** Index @p index of the first undefined piece in the pieces 
   ** array is incremented by one in this method, to remain 
   ** the index of the first undefined piece in the array.
   ** Configuration @p start remains constant. 
   **
   ** @param  index      the first free index in the pieces array
   **                    (@p index will be added at this index, 
   **                     which will be incremented), 
   ** @param  start      the starting configuration, 
   ** @param  curvDeriv  the turn's curvature's derivative, 
   ** @param  length     the turn's length. 
   **/
  void addPiece(int& index, const iSeeML::rob::CurvConfig& start, 
		const double& curvDeriv, const double& length) {
    // add the new piece and change next piece's index
    lcPiece(++index) = LinCurvPath( start, curvDeriv, 
				    fabs(length) );  } 

public:
  /** Modification method, defining type and pieces of a Dubins' 
   ** like path in order to connect two given configurations with 
   ** the shortest length (between @ref nbPossiblePaths 
   ** possibilities).
   **
   ** Maximum curvature of the current Dubins-like path is used. 
   **
   ** @param start  the starting configuration, 
   ** @param goal   the goal configuration. 
   **/
  void connect(const iSeeML::rob::OrPtConfig& start, 
	       const iSeeML::rob::OrPtConfig& goal); 


  // === Operators ================================================

  /** Copy operator (default one is not correct). 
   **
   ** @param other  the path to copy. 
   **
   ** @pre       memory should not be full.
   ** If memory is full and @c ISEEML_CHECK_NULL_POINTER is defined 
   ** (see CompilerFlags.h), an error message is generated and 
   ** the program exits. 
   **
   ** @return       the current path, after modification.
   **/
  DubinsLikePath& operator=(const DubinsLikePath& other) { 
    NbPieces = other.nbPieces();   PathType = other.type(); 
    // free previous LinCurvPath array
    if (LcPieces != NULL)  delete[] LcPieces; 
    if (NbPieces) { 
      LcPieces = new LinCurvPath[NbPieces]; 
#ifdef ISEEML_CHECK_NULL_POINTER
      if (LcPieces == NULL) { 
	cerr << ClassName << "operator=: out of memory!!!" 
	     << endl;  exit(-1); }
#endif
      ISEEML_ROB_COMPATH_forall_pieces(i) 
	lcPiece(i) = other.lcPiece(i);  } 
    else
      LcPieces = NULL;
    MaximumCurvature = other.maxCurv();  return *this; }


  // === Various Methods ==========================================

  /** Various method, computing the set of circle centers 
   ** used to turn from starting configuration of the current path, 
   ** or to turn to reach the given goal configuration. 
   **
   ** @param  start    the starting configuration, 
   ** @param  goal     the configuration to reach, 
   ** @param  centers  the set of @ref nbTurningCircles circle 
   **                  centers computed. 
   **
   ** @pre  This method only works if the maximum curvature is not 
   **       zero (zero curvature case is to be treated separately 
   **       in constructors). 
   **/
  virtual void computeCenters(const iSeeML::rob::OrPtConfig& start,
			      const iSeeML::rob::OrPtConfig& goal, 
			      iSeeML::geom::Point 
			      centers[nbTurningCircles]) const 
    = 0; 

protected:
  // cf. CompoundPath
  iSeeML::rob::BasicPath& _piece(const int index) const {
    // cout << this << "->DLP::_piece(" << index << ")" << endl; 
    return LcPieces[index - 1]; }


  /** Various method, returning an array 
   ** of @ref nbPossiblePaths Dubins-like paths, initialised 
   ** as clones of the current path. 
   **
   ** @return  the array of Dubins-like paths. 
   **/
  virtual DubinsLikePath* connectArray() const = 0;

  /** Various method, getting in an array of Dubins-like paths 
   ** the element of given index. 
   **
   ** This method is virtual as the @b real type of the paths 
   ** in the first parameter array is the same as the real type 
   ** of the current Dubins-like path 
   ** (either @ref iSeeML::rob::DubinsPath "Dubins"' or 
   **  @ref iSeeML::rob::FscPath "FSC" path).
   ** Size of these paths' representation being different, access 
   ** in an array needs different computations.
   **
   ** @param   paths  the array of Dubins-like paths,
   ** @param   index  the index of the searched path. 
   **
   ** @pre     first parameter should be an array of paths 
   **          obtained using @ref connectArray. 
   ** @pre     second parameter should be a correct index 
   **          for first parameter array 
   **          (between 0 and @ref nbPossiblePaths - 1). 
   ** 
   ** @return  the array of Dubins-like paths. 
   **
   ** @see     connectArray.
   **/
  virtual DubinsLikePath& 
  getSolution(DubinsLikePath* paths, const int index) const = 0;

private:
  /** Various method, selecting the index of the best (shortest) 
   ** path from a given array, corresponding to every possible 
   ** types. 
   **
   ** @param   paths  the array of @ref nbPossiblePaths paths. 
   **
   ** @pre     parameter should be an array of paths obtained 
   **          using @ref connectArray. 
   **
   ** @return  the index of the shortest one. 
   **
   ** @see     connectArray, getConnection, Path::operator<. 
   **/
  int bestPathIndex(DubinsLikePath* paths) const {
    // Compare lsl and lsr paths and get the shortest's index
    bool test = 
      getSolution(paths, lsl) < getSolution(paths, lsr);
    const int ls   = test ? lsl : lsr; 
    // Compare rsl and rsr pathss and get the shortest's index
    test = getSolution(paths, rsl) < getSolution(paths, rsr);
    const int rs   = test ? rsl : rsr; 
    // Compare lrl and rlr pathss and get the shortest's index
    test = getSolution(paths, lrl) < getSolution(paths, rlr);
    const int ccc  = test ? lrl : rlr;
    // Compare ls* and rs* pathss and get the shortest's index
    test = getSolution(paths, ls) < getSolution(paths, rs);
    const int csc  = test ? ls  : rs; 
    // get the index of the shortest between all
    test = getSolution(paths, csc) < getSolution(paths, ccc);
    const int best = test ? csc : ccc;
    return best; } // end of int bestPathIndex(DubinsLikeArray&) 

}; // end of class iSeeML::rob::DubinsLikePath

#endif  // end of definition 

