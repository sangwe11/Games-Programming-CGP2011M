solution "Games-Programming-CGP2011M"
	configurations { "Debug", "Release" }

	configuration { "Debug" }
		targetdir "bin/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration { "Release" }
		targetdir "bin/Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	configuration { "windows" }
		defines { "WINDOWS" }
	configuration { "linux" }
		defines { "LINUX" }
	configuration {}

	project "Engine"
		language "C++"
		kind "StaticLib"

		-- C++ on linux
		configuration { "linux" }
			buildoptions "-std=c++11"
		configuration {}

		files { "./Engine/**.h", "./Engine/**.cpp" }
		excludes { "./includes/**", "./libraries/**" }
		
		includedirs { "./includes/" }
		libdirs { "./libraries/" }
		
		configuration "windows"
			links { "SDL/SDL2", "SDL/SDL2main", "SDL/SDL2_image", "ASSIMP/assimp", "opengl32", "GLEW/glew32", "freetype/freetype" }
		configuration "linux"
			links { "SDL2", "SDL2main", "SDL2_image", "assimp", "GL", "GLEW", "freetype" }
		configuration {}
		
	project "ZombieSurvival"
		language "C++"
		kind "ConsoleApp"

		-- C++ on linux
		configuration { "linux" }
			buildoptions "-std=c++11"
		configuration {}

		files { "./ZombieSurvival/**.h", "./ZombieSurvival/**.cpp" }
		excludes { "./includes/**", "./libraries/**" }

		includedirs { "./includes/", "./Engine/" }
		libdirs { "./libraries/" }
		
		-- Link to Engine static library
		links { "Engine" }
		
		configuration "windows"
			links { "SDL/SDL2", "SDL/SDL2main", "SDL/SDL2_image", "ASSIMP/assimp", "opengl32", "GLEW/glew32", "freetype/freetype" }
		configuration "linux"
			links { "SDL2", "SDL2main", "SDL2_image", "assimp", "GL", "GLEW", "freetype" }
		configuration {}