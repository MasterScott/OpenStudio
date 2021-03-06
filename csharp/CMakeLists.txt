
install(DIRECTORY "examples"
  DESTINATION "CSharp/"
  COMPONENT "CSharpAPI"
  FILES_MATCHING PATTERN "*.cs"
  PATTERN "*.csproj"
  PATTERN "*.sln"
  PATTERN "*.resx"
  PATTERN "*.settings"
  PATTERN "*.docx"
)

file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp" CSHARP_LIBRARY_OUTPUT_DIRECTORY)
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR)
file(TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR)

set(CSHARP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CSHARP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CSHARP_VERSION_PATCH ${PROJECT_VERSION_PATCH_DIGIT})
set(CSHARP_VERSION_BUILD 0)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CSHARP_PLATFORM "x64")
else()
  set(CSHARP_PLATFORM "x86")
endif()

set(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_PATCH}.${CSHARP_VERSION_BUILD}")

set(CSHARP_TOOLS_VERSION "4.5")

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/csharp_wrapper/generated_sources")
if(MSVC)
  configure_file("${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")
else()
  configure_file("${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj_dotnet.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")
endif()

# Trying to get rid of this by placing more attributes in csproj instead
if (MSVC)
  configure_file("${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/Properties/AssemblyVersion.cs.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/generated_sources/AssemblyVersion.cs")
endif()

#This is generating odd errors about needing to convert the included project file, but it seems like it would be a good
#way to move forward, if it worked. Leaving here as a note.
#include_external_msproject(OpenStudio "${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj" ${CSHARP_LIBRARIES})
#in lieu of the above, we are using the following:

# the main OpenStudio.dll generated

if (MSVC)
  # TODO: Remove
  # set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")

  if(NINJA)
    set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.dll")
    set(CONFIG_ARGS "/p:Configuration=${CMAKE_BUILD_TYPE}")
  else()
    # **Regular case, pure MSVC**
    set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.dll")
    set(CONFIG_ARGS "/p:Configuration=$(ConfigurationName)")
  endif()

  # ignore deprecated warnings since we will export deprecated methods
  add_definitions(/wd4996 /bigobj)

  # custom command to make OPENSTUDIO_CSHARP_DLL
  add_custom_command(
    OUTPUT ${OPENSTUDIO_CSHARP_DLL}
    COMMAND "MSBuild" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" "${CONFIG_ARGS}"
    DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
  )

  if(NINJA)
    install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
  else()
    # **Regular case, pure MSVC**
    install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
    install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")
  endif()

else()
  # Rely on dotnet Core (open source)
  find_program(DOTNET dotnet)
  if(NOT DOTNET)
    message(FATAL_ERROR "C# generation outside of MSVC requires .NET Core (dotnet)")
  endif()

  # TODO: there are probably adjustments to make to OpenStudio.csproj_dotnet.in
  # The Release/Debug can be stripped probably
  # Also probably don't need to use multiple `<TargetFrameworks>` which is the reason there's an extra netcoreapp3.0/ level
  set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/netcoreapp3.0/OpenStudio.dll")

  # ignore deprecated warnings since we will export deprecated methods
  add_definitions(-Wno-deprecated-declarations)

  # custom command to make OPENSTUDIO_CSHARP_DLL
  add_custom_command(
    OUTPUT ${OPENSTUDIO_CSHARP_DLL}
    COMMAND "${DOTNET}" "build" "-c" "$<CONFIGURATION>" "-f" "netcoreapp3.0" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj"
    DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
  )

  install(FILES "${OPENSTUDIO_CSHARP_DLL}" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
endif()

# keep the following lists aligned with translator_names in ProjectMacros.cmake
set(translator_wrappers
  csharp_OpenStudioAirflow_wrap.cxx
  csharp_OpenStudioEnergyPlus_wrap.cxx
  csharp_OpenStudioGBXML_wrap.cxx
  csharp_OpenStudioISOModel_wrap.cxx
  csharp_OpenStudioRadiance_wrap.cxx
  csharp_OpenStudioSDD_wrap.cxx
)

set(model_wrappers
  csharp_OpenStudioMeasure_wrap.cxx
  csharp_OpenStudioModel_wrap.cxx
  csharp_OpenStudioModelAirflow_wrap.cxx
  csharp_OpenStudioModelAvailabilityManager_wrap.cxx
  csharp_OpenStudioModelCore_wrap.cxx
  csharp_OpenStudioModelGenerators_wrap.cxx
  csharp_OpenStudioModelGeometry_wrap.cxx
  csharp_OpenStudioModelHVAC_wrap.cxx
  csharp_OpenStudioModelPlantEquipmentOperationScheme_wrap.cxx
  csharp_OpenStudioModelRefrigeration_wrap.cxx
  csharp_OpenStudioModelResources_wrap.cxx
  csharp_OpenStudioModelSetpointManager
  csharp_OpenStudioModelSimulation_wrap.cxx
  csharp_OpenStudioModelStraightComponent_wrap.cxx
  csharp_OpenStudioModelZoneHVAC_wrap.cxx
  csharp_OpenStudioOSVersion_wrap.cxx
)

set(utilities_wrappers
  csharp_OpenStudioEnergyPlus_wrap.cxx
  csharp_OpenStudioRadiance_wrap.cxx
  csharp_OpenStudioGBXML_wrap.cxx
  csharp_OpenStudioAirflow_wrap.cxx
  csharp_OpenStudioISOModel_wrap.cxx
  csharp_OpenStudioSDD_wrap.cxx
)

foreach(wrapper_file IN LISTS ALL_CSHARP_WRAPPER_FILES)
  get_filename_component(wrapper_name ${wrapper_file} NAME)

  set(notfound TRUE)

  foreach(model_wrapper IN LISTS model_wrappers)
    if( wrapper_name STREQUAL model_wrapper )
      list(APPEND model_wrapper_files ${wrapper_file})
      set(notfound FALSE)
      break()
    endif()
  endforeach()

  foreach(translator_wrapper IN LISTS translator_wrappers)
    if( wrapper_name STREQUAL translator_wrapper )
      list(APPEND translator_wrapper_files ${wrapper_file})
      set(notfound FALSE)
      break()
    endif()
  endforeach()

  if( notfound )
    list(APPEND core_wrapper_files ${wrapper_file})
  endif()
endforeach()

#message("model_wrapper_files = ${model_wrapper_files}")
#message("translator_wrapper_files = ${translator_wrapper_files}")
#message("core_wrapper_files = ${core_wrapper_files}")

set_source_files_properties(${ALL_CSHARP_WRAPPER_FILES} PROPERTIES GENERATED TRUE)

add_library(
  openstudio_csharp
  SHARED # Was "MODULE" on Windows before
  ${core_wrapper_files}
)
add_dependencies(openstudio_csharp ${ALL_CSHARP_WRAPPER_TARGETS})


# TODO: Remove
# DLM: currently there are some multiply defined symbols, force them for now, investigate later

# Note JM 2019-10-07:
# eg: multiple definition of `CSharp_OpenStudio_LeftShift__SWIG_1'
# This is due to the ostream << operators

# Couldn't make it work with target_link_options, nor with generator expressions in target_link_libraries.
# Only thing that worked was explicit `target_link_libraries(openstudio_XXX PUBLIC "-Wl,--allow-multiple-definition" ...)` for each of the three libraries

#target_link_options(openstudio_csharp
#   PUBLIC
#   $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:"LINKER:--allow-multiple-definition">
#   $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:"-Wl,--allow-multiple-definition">
#   $<$<CXX_COMPILER_ID:MSVC>:"/FORCE:MULTIPLE">
#)

target_link_libraries(openstudio_csharp
 openstudiolib
)
install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

add_library(
  openstudio_model_csharp
  SHARED # Was "MODULE" on Windows before
  ${model_wrapper_files}
)
add_dependencies(openstudio_model_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
target_link_libraries(openstudio_model_csharp
  openstudiolib
)
install(TARGETS openstudio_model_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_model_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

add_library(
  openstudio_translators_csharp
  SHARED # Was "MODULE" on Windows before
  ${translator_wrapper_files}
)
add_dependencies(openstudio_translators_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
target_link_libraries(openstudio_translators_csharp
  openstudiolib
)
install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

# convenience target to build the SDK
add_custom_target(csharp_sdk ALL
  DEPENDS ${OPENSTUDIO_CSHARP_DLL} openstudio_csharp openstudio_model_csharp openstudio_translators_csharp
)

if( BUILD_NUGET_PACKAGE )
  if(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bit
    message(FATAL_ERROR "Nuget package cannot be built using the 32 bit generator")
  endif()

  if (MSVC)
    include(ExternalProject)

    ExternalProject_Add(OS-32
      SOURCE_DIR ${PROJECT_SOURCE_DIR}
      BINARY_DIR ${PROJECT_BINARY_DIR}/OS-32
      BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release
      CMAKE_GENERATOR
        "Visual Studio 16 2019"
      CMAKE_ARGS
        -A Win32
        -DCMAKE_BUILD_TYPE=Release
      CMAKE_CACHE_ARGS
        -DBUILD_CSHARP_BINDINGS:BOOL=ON
        -DBUILD_NUGET_PACKAGE:BOOL=OFF
      INSTALL_COMMAND ""
    )

    # This is used in the OpenStudio.nuspec template
    # One problem. What if 64 bit is configured with ninja,
    # but 32 bit is configured without ninja?
    if(NINJA)
      set(OPENSTUDIO_CONFIGURATION_TYPE "")
    else()
      # Regular case: pure MSVC
      set(OPENSTUDIO_CONFIGURATION_TYPE "$CONFIGURATION$")
    endif()

    configure_file( ${PROJECT_SOURCE_DIR}/csharp/OpenStudio.nuspec.in ${PROJECT_BINARY_DIR}/OpenStudio.nuspec )

    add_custom_target(nuget_package
      COMMAND NuGet pack OpenStudio.nuspec -BasePath "${PROJECT_BINARY_DIR}/" -properties Configuration=$<CONFIGURATION>
      DEPENDS csharp_sdk
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    )
  else()

    set(OPENSTUDIO_CSHARP_DLLNAME "Products/csharp/$CONFIGURATION$/netcoreapp3.0/OpenStudio.dll")
    set(OPENSTUDIO_CSHARP_LIBNAME "Products/libopenstudio_csharp.so")
    set(OPENSTUDIO_MODEL_CSHARP_LIBNAME "Products/libopenstudio_model_csharp.so")
    set(OPENSTUDIO_TRANSLATORS_CSHARP_LIBNAME "Products/libopenstudio_translators_csharp.so")

    # TODO: Remove altogether? For dotnet I'm trying to use the nuget properties metadata directly in csproj, but not managing to do it
    configure_file( ${PROJECT_SOURCE_DIR}/csharp/OpenStudio.nuspec.in ${PROJECT_BINARY_DIR}/OpenStudio.nuspec )

    #dotnet pack OpenStudio.csproj --configuration $<CONFIG> -p:NuspecFile="${PROJECT_BINARY_DIR}/OpenStudio.nuspec" -p:NuspectProperties="Configuration=$<CONFIG>"
    # dotnet pack <path to .csproj file> -p:NuspecFile=<path to nuspec file> -p:NuspecProperties=<> -p:NuspecBasePath=<Base path>
    add_custom_target(nuget_package
      COMMAND ${DOTNET} pack --configuration $<CONFIG> -p:TargetFrameworks=netcoreapp3.0 -p:Configuration=$<CONFIG> -p:NuspecProperties="Configuration=$<CONFIG>" ${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj
      DEPENDS csharp_sdk
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/csharp_wrapper/"
    )
  endif()

endif()
