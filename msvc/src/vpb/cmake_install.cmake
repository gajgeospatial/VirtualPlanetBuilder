# Install script for directory: C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/src/vpb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/VirtualPlanetBuilder")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/Debug/vpbd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/Release/vpb.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/MinSizeRel/vpb.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/RelWithDebInfo/vpb.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/Debug/vpbd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/Release/vpb.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/MinSizeRel/vpb.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/msvc/lib/RelWithDebInfo/vpb.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/vpb" TYPE FILE FILES
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/BlockOperation"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/BuildLog"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/BuildOperation"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/BuildOptions"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Commandline"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/DatabaseBuilder"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/DataSet"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Date"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Destination"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Export"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/ExtrudeVisitor"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/FileCache"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/FileDetails"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/FileUtils"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/FilePathManager"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/GeospatialDataset"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/HeightFieldMapper"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/MachinePool"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/ObjectPlacer"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/PropertyFile"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/ShapeFilePlacer"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Source"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/SourceData"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/SpatialProperties"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/System"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/TextureUtils"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Task"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/TaskManager"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/ThreadPool"
    "C:/Development/op3d_active/VirtualPlanetBuilder-1-1-0/include/vpb/Version"
    )
endif()

