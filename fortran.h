#ifndef MYTOOLS_FORTRAN_H_
#define MYTOOLS_FORTRAN_H_



/** \file
 *  \brief Set of macros to ease portable declarations of external Fortran 
 *         routines. 
 *  
 *  The functionality is for mixed language call FROM C++ to Fortran, not vice 
 *  versa. 
 *    
 *  The idea of stub code generation is due to 
 *  http://arnholm.org/software/cppf77/cppf77.htm
 *
 *  \par Example:
 *   
 *  \code
 *   
 *  #include <mytools/fortran.h>
 *  
 *  #define FUNC MYTOOLS_FORTRAN_FUNCTION
 *  #define SUB  MYTOOLS_FORTRAN_SUBROUTINE
 *  #define CHAR MYTOOLS_FORTRAN_CHAR   
 *  
 *  FUNC (int, foo, FOO, 3, (CHAR, double*, int&) );
 *  SUB  (bar, BAR, 4, (CHAR, const double&, CHAR, int*) );
 *    
 *  \endcode
 *  
 *  The above will \e effectively expand to:   
 *  
 *  \code
 *  
 *  #ifdef unix
 *  extern "C" int foo_ (const char*, double*, int&, size_t);
 *  inline int FOO( internal_character_type v0,  double* v1, int& v2) 
 *  {
 *     return foo_(v0.p, v1, v2, v0.n); 
 *  }   
 *      
 * 
 *  extern "C" void bar_ (const char*, const double&, const char*, int*, size_t, size_t);
 *  inline void BAR ( 
 *     internal_character_type v0,  
 *     double* v1,
 *     internal_character_type v2,
 *     int& v3
 *     ) 
 *  {
 *     bar_(v0.p, v1, v2.p, v3, v0.n, v2.n); 
 *  }   
 *  
 *  #else // in MS compilers 
 *    
 *  extern "C" int  __stdcall FOO( internal_character_type, size_t, double*, int&);
 *  extern "C" void __stdcall BAR( internal_character_type, const double&, internal_character_type, int*);
 *     
 *  #endif  
 *  
 *  \endcode
 *  Here \a internal_character is an unspecified implementation type referring 
 *  to a structure, that can be implicitly constructed from a \a std::string or 
 *  \a const \a char*. 
 * 
 */
 
   
/** \def MYTOOLS_FORTRAN_SUBROUTINE(x, X, n, args) 
 *  \brief Generates portable prototype for a Fortran subroutine.
 *
 *  Preprocessor macro that portably declares an external Fortran subroutine in 
 *  the caller scope with all-caps name. 
 *  \param x  Small case name of the Fortran subroutine 
 *  \param X  Capital case name of the Fortran subroutine 
 *  \param n  Number of arguments           
 *  \param args  Tuple of argument declerations WITHOUT the formal name. That is 
 *             specify <tt>(int*, double&)</tt> and NOT 
 *             <tt>(int* i, double& d)</tt>. For zero arity subroutines 
 *             specify \a n as 0 and \a args as ().   
 */
  
/** \def MYTOOLS_FORTRAN_FUNCTION(type, x, X, n, args)
 *  \brief Generates portable prototype for a Fortran function.
 *
 *  Preprocessor macro that portably declares an external Fortran function in  
 *  the caller scope with all-caps name. 
 *  \param type  Return type of the Fortran function
 *  \param x     Small case name of the Fortran function 
 *  \param X     Capital case name of the Fortran function 
 *  \param n     Number of arguments           
 *  \param args  Tuple of argument declerations WITHOUT the formal name. That 
 *                is specify (int*, double&) and NOT (int* i1, double& d1). For 
 *                zero arity functions specify \a n as 0 and \a args as ().   
 */  
  

/** \def MYTOOLS_FORTRAN_PROTOTYPE(cname, fname, FNAME, not_void, rtype, n,args)
 *  \brief Generates portable prototype for a Fortran routine. 
 *
 * This is the lowest level macro, and recommended to use only if renaming of 
 * the external routine is required. Otherwise consider using 
 * MYTOOLS_FORTRAN_FUNCTION or MYTOOLS_FORTRAN_SUBROUTINE.  
 *   
 * \param  cname Name of the routine inside the caller scope. This is an arbitrary
 *               name except it can not be the same as decorated function name. 
 *               Since name decoration is trivial for most platforms and yields the
 *               same thing as \a FNAME, it is recommended to use this macro only 
 *               when \a cname differs from \a FNAME more than letter casing.   
 * \param  fname Small case undecorated name of the Fortran routine. 
 * \param  FNAME Capital case undecorated name of the Fortran routine. 
 * \param  not_void Specify 1 if the routine is a Fortran function and has 
 *                  non-void return. Specify 0 otherwise.	   
 * \param  rtype Return type of the Fortran routine (ignored if not_void is 0)
 * \param  n     Number of arguments
 * \param  args  Tuple of argument decleration WITHOUT the formal name. 
 *
 */	





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <boost/preprocessor/detail/is_unary.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <string>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//==============================================================================
// CONFIGURATION MACROS 
//    Canonical definition of portability macros based on platform.
//==============================================================================
#ifdef unix
#   define MYTOOLS_FORTRAN_split_character
#   define MYTOOLS_FORTRAN_decorate(x,X) x##_
#   undef  MYTOOLS_FORTRAN_decoration_allcaps
#   define MYTOOLS_FORTRAN_call_conv 
#else 
#   undef  MYTOOLS_FORTRAN_split_character
#   define MYTOOLS_FORTRAN_decorate(x,X) X
#   define MYTOOLS_FORTRAN_decoration_allcaps
#   define MYTOOLS_FORTRAN_call_conv __stdcall
#endif


#define MYTOOLS_FORTRAN_char_p    const char*
#define MYTOOLS_FORTRAN_char_size_t size_t
#define MYTOOLS_FORTRAN_INTEGER   int
#define MYTOOLS_FORTRAN_DOUBLE    double
#define MYTOOLS_FORTRAN_REAL      float


//==============================================================================
// CHAR_PROXY   Proxy struct to enable splitting character content and length
//==============================================================================
namespace mytools {
namespace detail  {
struct char_proxy
{
  char_proxy( const char* c, std::size_t sz ) 
	  : p(c)
		, n(static_cast<MYTOOLS_FORTRAN_char_size_t>(sz))
  {}
  char_proxy( const std::string& s  ) 
	  : p(s.c_str())
		, n(static_cast<MYTOOLS_FORTRAN_char_size_t>(s.size())) 
	{}
	//
  MYTOOLS_FORTRAN_char_p p;
  MYTOOLS_FORTRAN_char_size_t n;
};
}} 

namespace mytools {
namespace f90 {
typedef ::mytools::detail::char_proxy character;
}}


//==============================================================================
//  CHAR  Part of the interface, to be used to prototype character 
//        arguments.
//
//  The character inside the parenthesis is arbitrary. See discussion here 
//  http://tinyurl.com/2u8dwt .
//
//==============================================================================
#ifdef MYTOOLS_FORTRAN_split_character 
#  define MYTOOLS_FORTRAN_CHAR (~)
#  define MYTOOLS_FORTRAN_char_extern_type MYTOOLS_FORTRAN_char_p
#else 
#  define MYTOOLS_FORTRAN_CHAR ::mytools::detail::char_proxy
#  define MYTOOLS_FORTRAN_char_extern_type ::mytools::detail::char_proxy
#endif


//==============================================================================
//  IS_CHAR(x)  will return 1 if x is FORTRAN_CHAR, 0 otherwise. 
// 
//  This works since x is supposed to be MYTOOLS_FORTRAN_CHAR or some type 
//  declaration which never comes in the form of a tuple.
// 
//  If CHARACTER arguments need not be split, this will always return 0, which 
//  is OK.     
//==============================================================================
#define MYTOOLS_FORTRAN_is_char BOOST_PP_IS_UNARY



//==============================================================================
// extern_arg_type  Convert `i'th element of the formal argument type array 
//                  `args' to argument type in extern declaration.      
//==============================================================================
#define MYTOOLS_FORTRAN_extern_arg_type(z,i,args) \
   BOOST_PP_IIF( MYTOOLS_FORTRAN_is_char(  BOOST_PP_ARRAY_ELEM(i,args)  ) , \
   MYTOOLS_FORTRAN_char_extern_type , \
   BOOST_PP_ARRAY_ELEM(i,args) \
	 )

//==============================================================================
// extra_arg_type  Generate formal declaration of extra integer argument if 
//                 `i'th element of `args' array is MYTOOLS_FORTRAN_CHAR
//==============================================================================
#define MYTOOLS_FORTRAN_extra_arg_type(z,i,args) \
  BOOST_PP_COMMA_IF( MYTOOLS_FORTRAN_is_char(BOOST_PP_ARRAY_ELEM(i,args)) ) \
  BOOST_PP_EXPR_IIF( MYTOOLS_FORTRAN_is_char(BOOST_PP_ARRAY_ELEM(i,args)) , \
	MYTOOLS_FORTRAN_char_size_t )

//==============================================================================
// actual_arg  Create identifier name for actual argument corresponding to `i'th
//             formal argument of call to extern routine.  
//==============================================================================
#define MYTOOLS_FORTRAN_actual_arg(z,i,args) \
   BOOST_PP_IIF( MYTOOLS_FORTRAN_is_char( BOOST_PP_ARRAY_ELEM(i,args)  ),  \
   v##i.p , v##i )

//==============================================================================
// formal_arg  Generate formal argument type and identifier for `i'th element of
//             `args' array. 
//==============================================================================
#define MYTOOLS_FORTRAN_formal_arg(z,i,args) \
   BOOST_PP_IIF( MYTOOLS_FORTRAN_is_char(  BOOST_PP_ARRAY_ELEM(i,args)  ), \
   ::mytools::detail::char_proxy, BOOST_PP_ARRAY_ELEM(i,args) )  v##i 


//==============================================================================
// extra_arg  Generate actual argument corresponding to extra integer arguments.
//==============================================================================
#define MYTOOLS_FORTRAN_extra_arg(z,i,args) \
  BOOST_PP_COMMA_IF( MYTOOLS_FORTRAN_is_char(BOOST_PP_ARRAY_ELEM(i,args)) ) \
  BOOST_PP_EXPR_IIF( MYTOOLS_FORTRAN_is_char(BOOST_PP_ARRAY_ELEM(i,args)) , v##i.n )



//==============================================================================
// enum_args  Enumerate `args' calling macro `f'
// loop_args  Repeatedly call `f' with `args'  
//==============================================================================
#define MYTOOLS_FORTRAN_enum_args(n,args,f)   BOOST_PP_ENUM(n, f, (n,args) )
#define MYTOOLS_FORTRAN_loop_args(n,args,f)   BOOST_PP_REPEAT(n, f, (n,args) )


//==============================================================================
// extern_decl   Generate declaration of the extern (actual) routine. 
//==============================================================================
#define MYTOOLS_FORTRAN_extern_decl(n,args) \
   MYTOOLS_FORTRAN_enum_args(n,args,MYTOOLS_FORTRAN_extern_arg_type) \
	 MYTOOLS_FORTRAN_loop_args(n,args,MYTOOLS_FORTRAN_extra_arg_type)

//==============================================================================
// inline_decl  Generate declaration of the inline stub function.  
//==============================================================================
#define MYTOOLS_FORTRAN_inline_decl(n,args) \
   MYTOOLS_FORTRAN_enum_args(n,args,MYTOOLS_FORTRAN_formal_arg)

//==============================================================================
// extern_call   Generate a call to the extern routine. 
//==============================================================================
#define MYTOOLS_FORTRAN_extern_call(n,args) \
   MYTOOLS_FORTRAN_enum_args(n,args,MYTOOLS_FORTRAN_actual_arg) \
	 MYTOOLS_FORTRAN_loop_args(n,args,MYTOOLS_FORTRAN_extra_arg)






//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  MACROS provided as a part of the library interface:  
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------




//==============================================================================
// PROTOTYPE  Generic prototyping macro. Part of the library interface.
//==============================================================================
#define MYTOOLS_FORTRAN_PROTOTYPE(cname, fname, FNAME, not_void, rtype, n,args) \
   extern "C" BOOST_PP_IIF(not_void, rtype, void) \
	 MYTOOLS_FORTRAN_call_conv \
	 MYTOOLS_FORTRAN_decorate(fname,FNAME) \
	 (MYTOOLS_FORTRAN_extern_decl(n,args)) \
	 ; \
   inline BOOST_PP_IIF(not_void, rtype, void) \
	 cname (MYTOOLS_FORTRAN_inline_decl(n,args)) { \
	 BOOST_PP_EXPR_IIF(not_void, return) \
	 MYTOOLS_FORTRAN_decorate(fname,FNAME) \
	 (MYTOOLS_FORTRAN_extern_call(n,args)) \
	 ;}

//==============================================================================
// FUNCTION Convenience call to PROTOTYPE. Part of the library interface. 
//==============================================================================
#ifdef MYTOOLS_FORTRAN_decoration_allcaps
#define MYTOOLS_FORTRAN_FUNCTION(type, x, X, n, args) \
   extern "C" type MYTOOLS_FORTRAN_call_conv X  \
	 (MYTOOLS_FORTRAN_extern_decl(n,args))
#else 
#define MYTOOLS_FORTRAN_FUNCTION(type, x, X, n, args ) \
   MYTOOLS_FORTRAN_PROTOTYPE( X, x, X, 1, type, n, args )
#endif
   


//==============================================================================
// SUBROUTINE Convenience call to PROTOTYPE. Part of the library interface. 
//==============================================================================
#ifdef MYTOOLS_FORTRAN_decoration_allcaps
#define MYTOOLS_FORTRAN_SUBROUTINE(x, X, n, args) \
   extern "C" void MYTOOLS_FORTRAN_call_conv X  \
	 (MYTOOLS_FORTRAN_extern_decl(n,args))
#else 
#define MYTOOLS_FORTRAN_SUBROUTINE(x, X, n, args ) \
   MYTOOLS_FORTRAN_PROTOTYPE( X, x, X, 0, ~, n, args )
#endif





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // MYTOOLS_FORTRAN_H_
