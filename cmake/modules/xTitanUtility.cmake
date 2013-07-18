# Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>

# groups source files with directory
function(group_sources root)
	function(__group_sources__ root prefix)
		file(GLOB entries RELATIVE "${root}" "${root}/*")
		foreach(entry ${entries})
			set(abspath "${root}/${entry}")
			if(IS_DIRECTORY "${abspath}")
				__group_sources__("${abspath}" "${prefix}/${entry}")
			else()
				list(APPEND group "${abspath}")
			endif()
		endforeach()
		string(REPLACE "/" "\\" prefix "${prefix}")
		source_group("${prefix}" FILES ${group})
	endfunction()

	get_filename_component(prefix "${root}" NAME)
	__group_sources__("${CMAKE_CURRENT_SOURCE_DIR}/${root}" "${prefix}")
endfunction()

# get path from env
function(path_from_env native_path)
	file(TO_CMAKE_PATH "$ENV{${native_path}}" tmp)
	set(${native_path} "${tmp}" PARENT_SCOPE)
endfunction()
