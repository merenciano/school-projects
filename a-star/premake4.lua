-- ---------------------------------
-- Toni Barella
-- Inteligencia Artificial 3�
-- ESAT 2016-2019
-- Genie Project Configuration.
-- ---------------------------------

solution ("3IA_Solution" .. _ACTION)
  configurations { "Debug", "Release" }
  platforms { "native", "x32", "x64" }
  location ("build")
	language "C++"
	kind "ConsoleApp"

	projects = { "PR0_Base", "PR1_AStar" }

	for i, prj in ipairs(projects) do 
		project (prj)
		targetname (prj)

		prj_path = "./build/" .. prj

		location (prj_path .. "/" .. _ACTION)
		
		includedirs {
			"./include/",			
			"./deps/include",
			"./deps/include/SDL2",
		}

		files {
			"./deps/include/*",
		}

    defines {
      "SDL_MAIN_HANDLED",
    }
	
    flags { "ExtraWarnings" }

		configuration "vs2015"
		  windowstargetplatformversion "8.1"
	    defines { "_CRT_SECURE_NO_WARNINGS" }
		  links { "opengl32","user32","shell32","gdi32", "./deps/lib/SDL2", "./deps/lib/SDL2_image"}

		configuration "vs2017"
		--windowstargetplatformversion "10.0.15063.0"
		  windowstargetplatformversion "10.0.16299.0"
		--windowstargetplatformversion "10.0.17134.0"
		--windowstargetplatformversion "10.0.17134.471"
	    defines { "_CRT_SECURE_NO_WARNINGS" }
		  links { "opengl32","user32","shell32","gdi32", "./deps/lib/SDL2", "./deps/lib/SDL2_image"}

    configuration "gmake" 
      buildoptions_cpp {
        "-x c++",
        "-std=c++14",
      }
      defines {
        
      }

      links {
        "SDL2", "SDL2_image"
      }


    configuration "Debug"
        defines { "DEBUG" }
        targetdir ("bin/Debug")
        targetsuffix "_d"
        objdir ("build/" .. "Debug")
        flags { "Symbols" }

    configuration "Release"
        targetdir ("bin/Release")
        objdir ("build/" .. "Release")
        flags { "Optimize" }

	end

	project "PR0_Base"
		files {
      "include/gamemanager.h",
      "include/path.h",
      "include/agent.h",
      "include/entity.h",
			"src/gamemanager.cc",
      "src/main.cc",
      "src/path.cc",
      "src/agent.cc",
      "src/entity.cc",
	}

	project "PR1_AStar"
		files {
      "include/astarnode.h",
      "include/path.h",
      "include/agent.h",
      "include/entity.h",
      "include/gamemanager.h",
      "include/levelmap.h",
      "src/mainastar.cc",
      "src/path.cc",
      "src/agent.cc",
      "src/entity.cc",
			"src/gamemanager.cc",
			"src/levelmap.cc",
			"src/astarnode.cc"
	}

  
