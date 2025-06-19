#pragma once


#include <SDL.h>
//#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <set>
#include <array>
#include <string_view>
#include <cstring>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdint.h>
#include <type_traits>  // Для std::void_t, std::false_type, std::true_type
#include <tuple>
#include <utility>
#include <cstdio>
#include <cstddef>
#include <iomanip>
#include <memory.h>
#include <malloc.h>
#include <random>
#include <thread>



//#include <windows.h>
//cpuid
#include <bitset>
//#include <intrin.h>


#include <xmmintrin.h>//__m128
#include <immintrin.h>//__m256 __m256d __m256i __m512 __m512d __m512i
//#include <emmintrin.h>//__m128d __m128i
//#include <wmmintrin.h>//AES-инструкции(шифрование) PCLMULQDQ

//#include "cpuinfo.cpp"

//#include "headers/button.hpp"
//#include "headers/multikey_input_checker.hpp"
#include "core/timers.hpp"

#include "core/random/randoms.hpp"

#include "headers/auto_click.hpp"

#include "core/memory/aligned_alloc_impl.hpp"
//#include "core/memory/const_memory_vectorization64.hpp"

#include "core/Neuron32/utils.hpp"

//#include "core/Neuron32/dynamic_neural_network_object.hpp"
//#include "core/Neuron32/dynamic_neural_network_object_pull.hpp"
#include "core/Neuron32/Objects/static_neural_network_object.hpp"
//#include "headers/Neuron64/neural_object.hpp" сделать

#include "headers/color_RGBA_computing_accelerator.hpp"

#include "headers/rooms/sell/cell.hpp"
//#include "core/device_info.hpp"

//cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL2_INCLUDE_DIR=C:/Users/user/Desktop/Programming/lib/SDL-release-2.30.11/include -DSDL2_LIBRARY=C:/Users/user/Desktop/Programming/lib/SDL-release-2.30.11/build/libSDL2.a -DFREETYPE_INCLUDE_DIRS=C:/Users/user/Desktop/Programming/lib/freetype-2.12.0/include -DFREETYPE_LIBRARY=C:/Users/user/Desktop/Programming/lib/freetype-2.12.0/build/libfreetype.a
/*

//#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define BOOST_NO_CXX11_SCOPED_ENUMS #include <boost/filesystem.hpp> #undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/sinks/text_file_backend.hpp>
/**/
