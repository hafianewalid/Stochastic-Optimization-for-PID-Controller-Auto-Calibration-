//
// Definition of all ISeeML' namespaces. 
//


// To only include this definition once
#ifndef ISEEML_NAMESPACES_HH
#define ISEEML_NAMESPACES_HH


/** All ISeeML' objects are defined in a single namespace.
 ** This namespace contains @ref basics "basic files" and 
 ** two sub-namespaces, geom and rob. 
 **/
namespace iSeeML {
  class Object; 

  /** ISeeML' geometric objects are gathered in this namespace. 
   ** These objects are sorted in a more logical (hierarchical) way 
   ** in @ref geometrics "the corresponding module". 
   **/
  namespace geom { 
    class Object; 
    class BasicObject; 
    class Vector; 
    class Point; 
  } // end of namespace geom 

  /** ISeeML' robotic objects are gathered in this namespace. 
   ** These objects are sorted in a more logical (hierarchical) way 
   ** in @ref robotics "the corresponding module". 
   **/
  namespace rob { 
    class Object; 
    class OrPtConfig; 
    class CurvConfig; 
    class Path; 
    class BasicPath; 
    class LinCurvPath; 
    class CompoundPath; 
    class ArrayPaths; 
    class DubinsLikePath; 
    class DubinsPath; 
    class FscPath; 
  } // end of namespace rob
} // end of namespace iSeeML 

#endif  // end of definition 

