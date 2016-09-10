cmake_minimum_required (VERSION 3.1.0)
project (hdftest)

find_package (HDF5)
#find_package (HDF5 NAMES hdf5 required)
#find_package (HDF5) # Find non-cmake built HDF5

message(STATUS ${HDF5_LIBRARIES}  )
message(STATUS ${HDF5_C_STATIC_LIBRARY})
message(STATUS ${HDF5_CXX_LIBRARIES})
message(STATUS ${HDF5_CXX_EXPORT_LIBRARIES})
message(INFO ${HDF5_INCLUDE_DIR})

INCLUDE_DIRECTORIES (${HDF5_INCLUDE_DIR})

#link_directories("C:/Program Files/HDF_Group/HDF5/1.10.0/lib")
file( GLOB SRCS *.cpp *.h)
add_executable(hdftest ${SRCS} )
#target_link_libraries (hdftest libhdf5_cpp libhdf5 zlib szip)
target_link_libraries (hdftest hdf5_cpp-static)




