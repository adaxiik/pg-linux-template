#include "tutorials.hpp"
#include <iostream>

#include <xmmintrin.h>
#include <pmmintrin.h>



int main(int argc, char** argv)
{

    _MM_SET_FLUSH_ZERO_MODE( _MM_FLUSH_ZERO_ON );
    _MM_SET_DENORMALS_ZERO_MODE( _MM_DENORMALS_ZERO_ON );


    return tutorial_3( "data/6887_allied_avenger.obj" );
}
