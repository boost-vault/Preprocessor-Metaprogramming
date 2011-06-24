//Purpose:
//  Prototype whether typelist can ease
//  the forwarding function problem.
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <iostream>
using namespace boost::mpl;
struct target
{
    target
      ( int
      )
    {
        std::cout<<"target(int)\n";
    }
    
    target
      ( int
      , float
      )
    {
        std::cout<<"target(int,float)\n";
    }
    
    target
      ( int
      , float
      , target const&
      )
    {
        std::cout<<"target(int,float,target const&)\n";
    }
    
    target
      ( target const&
      )
    {
        std::cout<<"target(target const&)\n";
    }
};

struct source
{
      target
    my_target
    ;
    template<typename VecOfTypes>
    source
    ( VecOfTypes const&
    , typename at_c<VecOfTypes,0>::type a0
    )
    : my_target
      ( a0
      )
    {}
    template<typename VecOfTypes>
    source
    ( VecOfTypes const&
    , typename at_c<VecOfTypes,0>::type a0
    , typename at_c<VecOfTypes,1>::type a1
    )
    : my_target
      ( a0
      , a1
      )
    {}
    template
    < typename T0
    , typename T1
    , typename T2
    >
    source
    ( vector<T0,T1,T2> const&
    , int& a0
    , double& a1
    , T2 a2
    )
    : my_target
      ( a0
      , a1
      , a2
      )
    {}
};

int main(void)
{
    typedef vector<int,double> int_double_typelist;
    BOOST_STATIC_ASSERT((boost::is_same<at_c<int_double_typelist,0>::type,int>::value));
    BOOST_STATIC_ASSERT((boost::is_same<at_c<int_double_typelist,1>::type,double>::value));
    int i=0;
    double d=2.5;
    source s0(int_double_typelist(), i);
    source s1(int_double_typelist(), i, d);
    //source s2(int_double_typelist(), 0, 2.5, s0.my_target);//fails compile (nargs > typelist size).
    typedef vector<int,double,target const&> int_double_targetConstRef_typelist;
    //source s3(int_double_targetConstRef_typelist(), i, d, s0.my_target);//fails compile (no at_c<,2>::type )
    return 0;
}    
