workspace "First"
    configurations { "Debug", "Release" }

    targetdir "%{wks.location}/Resource/Build"
	objdir "%{wks.location}/Resource/Intermediates"

group "Scripts"
	include "Assets/Scripts"
group ""

group "Core"
    include "../../Nigozi-CSharpCore/premake5-dotnet.lua"
group ""
