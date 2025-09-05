import subprocess
import platform
import os
import re

current_dir = os.path.dirname(os.path.abspath(__file__))
solution_dir = os.path.join(current_dir, '../../../')

batch_file = os.path.join(current_dir, 'premake5.bat')

if platform.system() == 'Windows':
    try:
        subprocess.call([batch_file], cwd=current_dir)
        
        vcxproj_nigozi = os.path.join(solution_dir, 'NigoziEngine/NigoziEngine.vcxproj')
        vcxproj_sandbox = os.path.join(solution_dir, 'Sandbox/Sandbox.vcxproj')

        insert_count = 0
        glcore_guid = ''

        print('Attempting to set pch source as ngpch.cpp...')
        with open(vcxproj_nigozi, "r") as f2:
            lines2 = f2.readlines()
            for j, line in enumerate(lines2):
                if '<ClCompile Include="src\\ngpch.cpp" />' in line:
                    del lines2[j]
                    lines2.insert(j, '\t\t<ClCompile Include="src\\ngpch.cpp">\n' +
                                  "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Create</PrecompiledHeader>\n" +
                                  "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Distribution|x64'\">Create</PrecompiledHeader>\n" +
                                  "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">Create</PrecompiledHeader>\n" +
                                  "\t\t</ClCompile>\n")
                    print('Success! -> set pch source file!')
                    break
        with open(vcxproj_nigozi, "r") as f2:
            for j, line in enumerate(lines2):
                if '<ClCompile Include="src\\Benchmarking\\Timer.cpp" />' in line:
                    del lines2[j]
                    lines2.insert(j, '\t\t<ClCompile Include="src\\utils\Timer.cpp">\n' +
                                  "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">NotUsing</PrecompiledHeader>\n" +
                                  "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Distribution|x64'\">NotUsing</PrecompiledHeader>\n" +
                                  "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">NotUsing</PrecompiledHeader>\n" +
                                  "\t\t</ClCompile>\n")
                    print('Success! -> configured pch settings in other source files!\n')
                    break

        with open(vcxproj_nigozi, "w") as f2:
            f2.writelines(lines2)



    except subprocess.CalledProcessError as e:
        print("Script failed! --- ", e)
