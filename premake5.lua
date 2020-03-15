workspace "filerenamer"
    architecture "x64"
    startproject "filerenamer"

    configurations
    {
        "Debug",
        "Release"
    }

project "filerenamer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "build/bin/%{cfg.buildcfg}"
    objdir "build/bin-int/%{cfg.buildcfg}"

     files
     {
        "src/**.h",
        "src/**.cpp"
     }

     includedirs
     {
        "lib/"
     }

     filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

     filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"
