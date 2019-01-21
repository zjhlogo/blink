macro(group_sources SOURCE_FILES)
	foreach(FILE ${SOURCE_FILES})
		get_filename_component(PARENT_DIR "${FILE}" PATH)

		# skip source and changes /'s to \\'s
		string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")
		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endmacro()

macro(configure_library PROJ_NAME FOLDER_NAME)
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJ_NAME}_D)
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJ_NAME})
	if (NOT ${FOLDER_NAME} STREQUAL "")
		set_target_properties(${PROJ_NAME} PROPERTIES FOLDER ${FOLDER_NAME})
	endif()
endMacro()

macro(configure_runtime PROJ_NAME OUTPUT_PATH FOLDER_NAME)
	find_package(glfw3 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
	find_package(glm PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
	find_package(fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
	find_package(tinyxml2 PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
	find_package(entityx PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

	if(CMAKE_SYSTEM_NAME MATCHES "Windows")
		add_executable(${PROJ_NAME} WIN32 ${SOURCE_FILES})
		set_target_properties(${PROJ_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
	endif()

	target_include_directories(${PROJ_NAME} PRIVATE
		$<BUILD_INTERFACE:$<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>>
		$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
		$<BUILD_INTERFACE:$<TARGET_PROPERTY:entityx,INTERFACE_INCLUDE_DIRECTORIES>>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/glad/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/imgui>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libfoundation>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libblink>
	)

	set(ALL_LIBRARIES
	glfw
	fmt::fmt
	tinyxml2::tinyxml2
	entityx
	libglad
	libimgui
	libfoundation
	libblink
	)

	if (CMAKE_SYSTEM_NAME MATCHES "Windows")
		set(ALL_LIBRARIES ${ALL_LIBRARIES} opengl32)
	endif ()
	target_link_libraries(${PROJ_NAME} ${ALL_LIBRARIES})

	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJ_NAME}_D)
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJ_NAME})
	set_target_properties(${PROJ_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_PATH})
	if (NOT ${FOLDER_NAME} STREQUAL "")
		set_target_properties(${PROJ_NAME} PROPERTIES FOLDER ${FOLDER_NAME})
	endif()
endMacro()

macro(configure_test PROJ_NAME OUTPUT_PATH FOLDER_NAME)
	find_package (GTest PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)
	find_package (fmt PATHS ${CMAKE_INSTALL_PREFIX} NO_DEFAULT_PATH REQUIRED)

	add_executable(${PROJ_NAME} ${SOURCE_FILES})

	target_include_directories(${PROJ_NAME} PRIVATE
		$<BUILD_INTERFACE:$<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>>
		$<BUILD_INTERFACE:$<TARGET_PROPERTY:GTest::gtest,INTERFACE_INCLUDE_DIRECTORIES>>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../libfoundation>
	)

	set(ALL_LIBRARIES
	fmt::fmt
	GTest::gtest
	libfoundation
	)
	target_link_libraries(${PROJ_NAME} ${ALL_LIBRARIES})

	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJ_NAME}_D)
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJ_NAME})
	set_target_properties(${PROJ_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_PATH})
	if (NOT ${FOLDER_NAME} STREQUAL "")
		set_target_properties(${PROJ_NAME} PROPERTIES FOLDER ${FOLDER_NAME})
	endif()
endMacro()
