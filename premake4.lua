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
			links { "SDL/SDL2", "SDL/SDL2main", "SDL/SDL2_image", "ASSIMP/assimp", "opengl32", "GLEW/glew32", "freetype/freetype", "irrKlang-1.5.0/irrKlang" }
		configuration "linux"
			links { "SDL2", "SDL2main", "SDL2_image", "assimp", "GL", "GLEW", "freetype", "irrKlang" }
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
			links { "SDL/SDL2", "SDL/SDL2main", "SDL/SDL2_image", "ASSIMP/assimp", "opengl32", "GLEW/glew32", "freetype/freetype", "irrKlang-1.5.0/irrKlang" }
		configuration "linux"
			links { "SDL2", "SDL2main", "SDL2_image", "assimp", "GL", "GLEW", "freetype", "irrKlang" }
		configuration {}
		
		-- Post build commands
		configuration { "Debug", "windows" }
			postbuildcommands { "xcopy models bin\\Debug\\models /s /e /h /i /Y", "xcopy shaders bin\\Debug\\shaders /s /e /h /i /Y", "xcopy textures bin\\Debug\\textures /s /e /h /i /Y", "xcopy sounds bin\\Debug\\sounds /s /e /h /i /Y", "xcopy fonts bin\\Debug\\fonts /s /e /h /i /Y", "xcopy windows\\dlls bin\\Debug /s /e /h /i /Y" }
		configuration { "Release", "windows" }
			postbuildcommands { "xcopy models bin\\Release\\models /s /e /h /i /Y", "xcopy shaders bin\\Release\\shaders /s /e /h /i /Y", "xcopy textures bin\\Release\\textures /s /e /h /i /Y", "xcopy sounds bin\\Release\\sounds /s /e /h /i /Y", "xcopy fonts bin\\Release\\fonts /s /e /h /i /Y", "xcopy windows\\dlls bin\\Release /s /e /h /i /Y" }
		configuration { "Debug", "linux" }
			postbuildcommands { "cp models bin/Debug/models -r", "cp shaders bin/Debug/shaders -r", "cp textures bin/Debug/textures -r", "cp sounds bin/Debug/sounds -r", "cp fonts bin/Debug/fonts -r" }
		configuration { "Release", "linux" }
			postbuildcommands { "cp models bin/Release/models -r", "cp shaders bin/Release/shaders -r", "cp textures bin/Release/textures -r", "cp sounds bin/Release/sounds -r", "cp fonts bin/Release/fonts -r" }
		configuration {}