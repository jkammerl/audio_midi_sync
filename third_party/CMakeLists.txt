message("dwdinc_dirs =") 

add_subdirectory(kissfft)
add_subdirectory(portaudio)
add_subdirectory(portmidi)

include_directories(BTrack/src)
add_library (BTrack STATIC
  BTrack/src/BTrack.cpp
  BTrack/src/BTrack.h
  BTrack/src/SpectralAnalysis.cpp
  BTrack/src/SpectralAnalysis.h
  BTrack/src/CircularBuffer.h
  BTrack/src/OnsetDetectionFunction.cpp
  BTrack/src/OnsetDetectionFunction.h
  )
target_compile_definitions(BTrack PRIVATE USE_KISS_FFT=1)
target_include_directories(BTrack PUBLIC BTrack/src/)
target_link_libraries (BTrack LINK_PUBLIC kissfft)

include_directories(seriallib/lib)
add_library (serialib STATIC
   serialib/lib/serialib.cpp
)
target_include_directories(serialib PUBLIC serialib/lib/)
