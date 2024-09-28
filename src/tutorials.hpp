#pragma once


#include <embree3/rtcore_device.h>
#include <string>

void error_handler( void * user_ptr, const RTCError code, const char * str = nullptr );

int tutorial_1( const char * config = "threads=0,verbose=3" );
int tutorial_2();
int tutorial_3( const std::string file_name, const char * config = "threads=0,verbose=0" );
