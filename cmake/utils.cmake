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
	if(CMAKE_SYSTEM_NAME MATCHES "Windows")
		add_executable(${PROJ_NAME} WIN32 ${SOURCE_FILES})
		set_target_properties(${PROJ_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
	endif()

	target_include_directories(${PROJ_NAME} PRIVATE
		$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
		$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
	)

	set(ALL_LIBRARIES
	libflecs
	libfmt
	libglad
	libglfw3
	libglm
	libimgui
	libfoundation
    librender_vulkan
	libblink
	)

	if (CMAKE_SYSTEM_NAME MATCHES "Windows")
		set(ALL_LIBRARIES ${ALL_LIBRARIES} opengl32)
        
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(ALL_LIBRARIES ${ALL_LIBRARIES} ${PROJECT_ROOT}/external/vulkan/lib/x64/vulkan-1.lib)
        elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(ALL_LIBRARIES ${ALL_LIBRARIES} ${PROJECT_ROOT}/external/vulkan/lib/x86/vulkan-1.lib)
        endif ()
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
	add_executable(${PROJ_NAME} ${SOURCE_FILES})

	target_include_directories(${PROJ_NAME} PRIVATE
		$<BUILD_INTERFACE:${PROJECT_ROOT}/external>
		$<BUILD_INTERFACE:${PROJECT_ROOT}/source>
	)

	set(ALL_LIBRARIES
	libfmt
	libgtest
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
