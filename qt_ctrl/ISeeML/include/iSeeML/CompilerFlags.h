/** @file CompilerFlags.h
 **
 ** Definition of relations between ISeeML compilation flags.
 **
 ** ISeeML compilation flags are hierarchicaly defined: for example, 
 ** definition of @c ISEEML_CHECKS activates all ISeeML checking flags.
 ** They can be activated by adding in makefile a @c -D option for 
 ** the compiler. 
 ** Possible flags are:
 **
 **  - @c ISEEML_CHECKS (checking everything)
 **    - @c ISEEML_CHECK_PRECOND (checking preconditions)
 **      - @c ISEEML_CHECK_BASIC_PRECOND 
 **        (checking preconditions for 
 **         @ref basics "basic objects")
 **        - @c ISEEML_CHECK_OBJECT_PRECOND 
 **          (checking preconditions for 
 **           @ref iSeeML::Object "ISeeML' objects")
 **      - @c ISEEML_CHECK_GEOM_PRECOND 
 **        (checking preconditions for 
 **         @ref geometrics "geometric module")
 **        - @c ISEEML_CHECK_GEOM_VECT_PRECOND
 **          (checking preconditions for 
 **           @ref iSeeML::geom::Vector "vectors")
 **        - @c ISEEML_CHECK_GEOM_POINT_PRECOND 
 **          (checking preconditions for 
 **           @ref iSeeML::geom::Point "points")
 **      - @c ISEEML_CHECK_ROB_PRECOND 
 **        (checking preconditions for 
 **         @ref robotics "robotic module")
 **        - @c ISEEML_CHECK_CONFIG_PRECOND 
 **          (checking preconditions for configurations)
 **          - @c ISEEML_CHECK_ORPT_CONFIG_PRECOND 
 **            (checking preconditions for 
 **             @ref iSeeML::rob::OrPtConfig "standard configurations")
 **          - @c ISEEML_CHECK_CURV_CONFIG_PRECOND 
 **            (checking preconditions for 
 **             @ref iSeeML::rob::CurvConfig "curvature configurations")
 **        - @c ISEEML_CHECK_PATH_PRECOND 
 **          (checking preconditions for 
 **           @ref iSeeML::rob::Path "paths")
 **          - @c ISEEML_CHECK_LIN_PATH_PRECOND  
 **            (checking preconditions for 
 **             @ref iSeeML::rob::LinCurvPath "linear basic paths")
 **          - @c ISEEML_CHECK_COMP_PATH_PRECOND
 **            (checking preconditions for 
 **             @ref iSeeML::rob::CompoundPath "compound paths")
 **            - @c ISEEML_CHECK_DLIKE_PATH_PRECOND
 **              (checking preconditions for 
 **               @ref iSeeML::rob::DubinsLikePath "Dubins-like paths")
 **              - @c ISEEML_CHECK_DUBINS_PATH_PRECOND
 **                (checking preconditions for 
 **                 @ref iSeeML::rob::DubinsPath "Dubins' paths")
 **              - @c ISEEML_CHECK_FSC_PATH_PRECOND
 **                (checking preconditions for 
 **                 @ref iSeeML::rob::FscPath "FSC paths")
 **    - @c ISEEML_CHECK_POSTCOND (checking postconditions)
 **      - @c ISEEML_CHECK_PATH_POSTCOND
 **        (checking postconditions for 
 **         @ref iSeeML::rob::Path "robotic paths")
 **        - @c ISEEML_CHECK_COMP_PATH_POSTCOND
 **          (checking postconditions for 
 **           @ref iSeeML::rob::CompoundPath "compound paths")
 **          - @c ISEEML_CHECK_DLIKE_PATH_POSTCOND
 **            (checking postconditions for 
 **             @ref iSeeML::rob::DubinsLikePath "Dubins-like paths")
 **            - @c ISEEML_CHECK_DUBINS_PATH_POSTCOND
 **              (checking postconditions for 
 **               @ref iSeeML::rob::DubinsPath "Dubins' paths")
 **            - @c ISEEML_CHECK_FSC_PATH_POSTCOND 
 **              (checking postconditions for 
 **               @ref iSeeML::rob::FscPath "FSC paths")
 **    - @c ISEEML_CHECK_POINTERS (checking pointer manipulations)
 **      - @c ISEEML_CHECK_NULL_POINTER 
 **           (checking for invalid reference)
 **      - @c ISEEML_CHECK_ARRAY_ELEMT 
 **           (checking for incorrect array's index)
 **/
 
/* Checking everything */
#ifdef ISEEML_CHECKS                       /* all checks include:     */
#define ISEEML_CHECK_PRECOND               /* preconditions           */
#define ISEEML_CHECK_POSTCOND              /* postconditions          */
#define ISEEML_CHECK_POINTERS              /* pointers                */
#endif

/* Preconditions */
#ifdef ISEEML_CHECK_PRECOND                /* Precond. checks are     */
#define ISEEML_CHECK_BASIC_PRECOND         /* basic's                 */
#define ISEEML_CHECK_GEOM_PRECOND          /* geometric's             */
#define ISEEML_CHECK_ROB_PRECOND           /* robotic's               */
#endif

/* Basic preconditions */
#ifdef ISEEML_CHECK_BASIC_PRECOND          /* Basic precond. are      */
#define ISEEML_CHECK_OBJECT_PRECOND        /* object's                */
#define ISEEML_CHECK_ARRAY_PRECOND         /* array's                 */
#endif

/* Geometric preconditions */
#ifdef ISEEML_CHECK_GEOM_PRECOND           /* Geometric precond. are  */
#define ISEEML_CHECK_GEOM_VECT_PRECOND     /* vector's                */
#define ISEEML_CHECK_GEOM_POINT_PRECOND    /* point's                 */
#endif

/* Robotic preconditions */
#ifdef ISEEML_CHECK_ROB_PRECOND           /* Robotic precond. are    */
#define ISEEML_CHECK_CONFIG_PRECOND       /* configuration's         */
#define ISEEML_CHECK_PATH_PRECOND         /* path's                  */
#endif

/* Configurations preconditions */
#ifdef ISEEML_CHECK_CONFIG_PRECOND           /* config. precond. are  */
#define ISEEML_CHECK_ORPT_CONFIG_PRECOND     /* standard's            */
#define ISEEML_CHECK_CURV_CONFIG_PRECOND     /* curvature's           */
#endif

/* Paths preconditions */
#ifdef ISEEML_CHECK_PATH_PRECOND           /* Path precond. are       */
#define ISEEML_CHECK_LIN_PATH_PRECOND      /* lin. elem. path's       */
#define ISEEML_CHECK_COMP_PATH_PRECOND     /* compound path's         */
#endif

/* Compound paths preconditions */
#ifdef ISEEML_CHECK_COMP_PATH_PRECOND      /* Comp. path precond.     */
#define ISEEML_CHECK_DLIKE_PATH_PRECOND    /* are Dubins' like path's */
#endif

/* DL paths preconditions */
#ifdef ISEEML_CHECK_DLIKE_PATH_PRECOND     /* DL Path precond. are    */
#define ISEEML_CHECK_DUBINS_PATH_PRECOND   /* Dubins' path's          */
#define ISEEML_CHECK_FSC_PATH_PRECOND      /* fwd sub-opt c-c path's  */
#endif

/* Postconditions */
#ifdef ISEEML_CHECK_POSTCOND               /* Postcond. checks are    */
#define ISEEML_CHECK_PATH_POSTCOND         /* path's                  */
#endif

/* Paths postconditions */
#ifdef ISEEML_CHECK_PATH_POSTCOND          /* Path postcond. are      */
#define ISEEML_CHECK_COMP_PATH_POSTCOND    /* compound path's         */
#endif

/* Compound paths postconditions */
#ifdef ISEEML_CHECK_COMP_PATH_POSTCOND     /* Comp. path postcond.    */
#define ISEEML_CHECK_DLIKE_PATH_POSTCOND   /* are Dubins' like path's */
#endif

/* DL paths postconditions */
#ifdef ISEEML_CHECK_DLIKE_PATH_POSTCOND    /* DL Path postcond. are   */
#define ISEEML_CHECK_DUBINS_PATH_POSTCOND  /* Dubins' path's          */
#define ISEEML_CHECK_FSC_PATH_POSTCOND     /* fwd sub-opt c-c path's  */
#endif

/* Validity of pointers' value */
#ifdef ISEEML_CHECK_POINTERS               /* Pointers checks are     */
#define ISEEML_CHECK_NULL_POINTER          /* nil pointer detect.     */
#define ISEEML_CHECK_ARRAY_ELEMT           /* index verification      */
#endif

