cmake_minimum_required (VERSION 3.1.0)
project (hdfdb_prototype)

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
file( GLOB SRCS hdf_db.cpp main.cpp Img_object.h hdf_db.h)
add_executable(hdfdb_prototype ${SRCS} )
#target_link_libraries (hdfdb_prototype libhdf5_cpp libhdf5 zlib szip)
target_link_libraries (hdfdb_prototype hdf5_cpp-static)




