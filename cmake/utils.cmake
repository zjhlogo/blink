macro(group_sources SOURCE_FILES)
	foreach(FILE ${SOURCE_FILES})
		get_filename_component(PARENT_DIR "${FILE}" PATH)

		# skip source and changes /'s to \\'s
		string(REGEX REPLACE "(\\./)?(source)/?" "" GROUP "${PARENT_DIR}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")
		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endmacro()

macro(configure_project PROJ_NAME OUTPUT_PATH)
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_DEBUG ${PROJ_NAME}_D)
	set_target_properties(${PROJ_NAME} PROPERTIES OUTPUT_NAME_RELEASE ${PROJ_NAME})
	set_target_properties(${PROJ_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_PATH})
endMacro()
