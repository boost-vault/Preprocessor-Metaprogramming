// Copyright (C) 2006 Aristid Breitkreuz
// Copyright (C) 2006 Tobias Schwinger
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/replace.hpp>

// binary sequence to sequence of binary tuples
#define TUPLEIZE_BIN_SEQ_I(x, y) ((x, y)) TUPLEIZE_BIN_SEQ_II
#define TUPLEIZE_BIN_SEQ_II(x, y) ((x, y)) TUPLEIZE_BIN_SEQ_I
#define TUPLEIZE_BIN_SEQ_IT
#define TUPLEIZE_BIN_SEQ_IIT
#define TUPLEIZE_BIN_SEQ(bs) BOOST_PP_CAT(TUPLEIZE_BIN_SEQ_I bs, T)

// apply the operations encoded in the param descriptors
#define APPLY_PARAM(args,index,macro,value) \
    BOOST_PP_SEQ_REPLACE(args,index,macro(BOOST_PP_SEQ_ELEM(index,args),value))

#define APPLY_PARAM_ARGS(prefix,args,name,value) \
    (args, BOOST_PP_CAT(BOOST_PP_CAT(prefix, __),name), value)

#define FOLD_OP(s,state,elem) ( BOOST_PP_TUPLE_ELEM(2,0,state) \
    , APPLY_PARAM BOOST_PP_EXPAND( APPLY_PARAM_ARGS BOOST_PP_EXPAND(( \
            BOOST_PP_TUPLE_REM(2) state, BOOST_PP_TUPLE_REM(2) elem)) )  )

// expand the main macro with the unfolded arguments
#define APPLY_PARAMS(macro, args_seq) macro BOOST_PP_SEQ_TO_TUPLE(args_seq)

// ?! interface ?! it's awkward to combine named and ordinary parameters
#define PARAM_INVOKE(macro,bseq) \
    BOOST_PP_EXPAND( APPLY_PARAMS BOOST_PP_SEQ_FOLD_LEFT( FOLD_OP, \
        (macro, BOOST_PP_CAT(macro,__DEFAULTS)), TUPLEIZE_BIN_SEQ(bseq) ) \
    ) 

// helper to replace previous argument
// ?! error checking to detect duplicate clobber - implementable ?!
#define REPLACE(x,y) y

---------------------
Example:

#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/array/push_back.hpp>

#define xPARAM__DEFAULTS ("") ((abc)) ((0, ()))
#define xPARAM__replacing_param   0, REPLACE
#define xPARAM__seq_param         1, BOOST_PP_SEQ_PUSH_BACK
#define xPARAM__arr_param         2, BOOST_PP_ARRAY_PUSH_BACK

#define xPARAM(a, b, c) \
  A = a; \
  B = b; \
  C = c; \

#define MACRO(x) PARAM_INVOKE(xPARAM, x)

MACRO(
  (replacing_param,dead)
  (replacing_param,alive)
  (seq_param,foo)
  (seq_param,bar)
  (seq_param,baz)
  (arr_param,sn)
  (arr_param,afu)
)

