if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
  # using Clang
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  #set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
endif()


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# common library for openstudio.so and openstudio.exe

add_library( init_openstudio OBJECT
  init_openstudio.cpp
  RubyAPI.hpp
)
target_include_directories(init_openstudio PRIVATE ${RUBY_INCLUDE_DIRS})

if(MSVC)
  # TODO: JM 2019-01-04: @macumber it seems like all three targets use the same COMPILE_FLAGS,
  # so instead of defining that three times, perhaps modify CMAKE_CXX_FLAGS?
  # wd4996=no deprecated warnings ; wd5033=register
  set_target_properties(init_openstudio PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
elseif (UNIX)
  # Disable register warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=register -Wno-register")
endif()


# openstudio ruby bindings to load into normal ruby

add_library(
  openstudio_rb
  MODULE
  RubyAPI.hpp
  openstudio_rb.cpp
)
set_target_properties(openstudio_rb PROPERTIES PREFIX "")
set_target_properties(openstudio_rb PROPERTIES OUTPUT_NAME openstudio)
target_include_directories(openstudio_rb PRIVATE ${RUBY_INCLUDE_DIRS})

# change output directory so the openstudio.pdb file does not conflict between this target and the CLI
set_target_properties(openstudio_rb PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ruby/")

if(APPLE)
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".bundle" )
else()
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".so" )
endif()

if(MSVC)
  set_target_properties(openstudio_rb PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
endif()

add_custom_command(TARGET openstudio_rb
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb $<TARGET_FILE_DIR:openstudio_rb>/openstudio.rb
)

file(GLOB_RECURSE OPENSTUDIO_FILES FOLLOW_SYMLINKS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/" "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/**/*.*")
foreach(OPENSTUDIO_FILE ${OPENSTUDIO_FILES})
  add_custom_command(TARGET openstudio_rb
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/${OPENSTUDIO_FILE}" "$<TARGET_FILE_DIR:openstudio_rb>/openstudio/${OPENSTUDIO_FILE}"
  )
endforeach()


target_link_libraries(
  openstudio_rb
  PRIVATE
  init_openstudio
  ${ALL_RUBY_BINDING_TARGETS}
)

if( WIN32 )
  # Dynamically link to openstudiolib.dll on Windows only
  target_link_libraries(
    openstudio_rb
    PUBLIC
    openstudiolib
  )
  target_link_libraries(openstudio_rb PRIVATE ${RUBY_MINGW_STUB_LIB})
else()
  # statically link everywhere else
  target_link_libraries(
    openstudio_rb
    PRIVATE
    openstudio_utilities
    openstudio_airflow
    openstudio_model
    openstudio_energyplus
    openstudio_measure
    openstudio_osversion
    openstudio_sdd
    openstudio_isomodel
    openstudio_gbxml
    openstudio_radiance
  )
endif()

if (APPLE)
  target_link_libraries(openstudio_rb
    PRIVATE
    ${OPENSSL_CRYPTO_LIBRARY}
    ${OPENSSL_SSL_LIBRARY}
  )
endif()


install(TARGETS openstudio_rb
  EXPORT openstudio
  DESTINATION Ruby
  COMPONENT "RubyAPI"
  INCLUDES DESTINATION include include/openstudio
)
install(FILES openstudio.rb DESTINATION Ruby COMPONENT "RubyAPI")
install(DIRECTORY openstudio DESTINATION Ruby COMPONENT "RubyAPI")


###############################################################################
#                        T E S T I N G:   C T E S T S                         #
###############################################################################

# find all tests
file(GLOB RUBY_TEST_SRC "test/*.rb")

# TODO: It doesn't not work with this executable that's in build/Ruby-install/bin
# message("RUBY_EXECUTABLE=${RUBY_EXECUTABLE}")

set(_RUBY_POSSIBLE_EXECUTABLE_NAMES
  ruby
  ruby2.5
  ruby2.5.5
  ruby2.5.1
  ruby25
  ruby255
  ruby251)

# TODO: this isn't great but I haven't found a better way to locate the system ruby (and avoid the one in build/Ruby-install/)
find_program(SYSTEM_RUBY_EXECUTABLE NAMES ${_RUBY_POSSIBLE_EXECUTABLE_NAMES}
  HINTS
    "/usr/share/rvm/rubies/ruby-2.5.5/bin/"
    "$ENV{HOME}/.rvm/rubies/ruby-2.5.5/bin/"
    "/usr/local/rvm/rubies/ruby-2.5.5/bin/ruby"
    "C:/Ruby25-x64/bin/"

    "/usr/share/rvm/rubies/ruby-2.5.4/bin/"
    "/usr/share/rvm/rubies/ruby-2.5.3/bin/"
    "/usr/share/rvm/rubies/ruby-2.5.2/bin/"
    "/usr/share/rvm/rubies/ruby-2.5.1/bin/"

    "$ENV{HOME}/.rvm/rubies/ruby-2.5.4/bin/"
    "$ENV{HOME}/.rvm/rubies/ruby-2.5.3/bin/"
    "$ENV{HOME}/.rvm/rubies/ruby-2.5.2/bin/"
    "$ENV{HOME}/.rvm/rubies/ruby-2.5.1/bin/"

    "/usr/local/rvm/rubies/ruby-2.5.4/bin/ruby"
    "/usr/local/rvm/rubies/ruby-2.5.3/bin/ruby"
    "/usr/local/rvm/rubies/ruby-2.5.2/bin/ruby"
    "/usr/local/rvm/rubies/ruby-2.5.1/bin/ruby"

    "/usr/local/bin/"
    "/usr/bin/"

  NO_DEFAULT_PATH)

if(NOT SYSTEM_RUBY_EXECUTABLE)
  message(WARNING "Your system ruby wasn't found, you won't be able to run the `ctest -R RubyTest` command and the tests won't be created at all.")
else()
  message(STATUS "Found SYSTEM_RUBY_EXECUTABLE=${SYSTEM_RUBY_EXECUTABLE}")

  # add a test for each unit test
  set(RUBY_TEST_REQUIRES "#include test files")
  foreach(f ${RUBY_TEST_SRC})

    file(READ "${f}" CONTENTS)
    string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})

    foreach(HIT ${FOUND_TESTS})
      string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
      string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
      string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})
      if(BUILD_TESTING)

        # Call with Ruby itself
        add_test(NAME "RubyTest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "${SYSTEM_RUBY_EXECUTABLE}" "-I" "$<TARGET_FILE_DIR:openstudio_rb>" "${f}" "--name=test_${TEST_NAME}"
        )

        set_tests_properties("RubyTest-${FILE_NAME}-${TEST_NAME}" PROPERTIES TIMEOUT 660 )
      endif()
    endforeach()
  endforeach()

endif()
