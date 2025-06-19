# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-src"
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-build"
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix"
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix/tmp"
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix/src/highway-populate-stamp"
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix/src"
  "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix/src/highway-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix/src/highway-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/clang/_deps/highway-subbuild/highway-populate-prefix/src/highway-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
