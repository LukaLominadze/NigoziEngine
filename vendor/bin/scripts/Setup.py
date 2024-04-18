import subprocess
import platform
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
solution_dir = os.path.join(current_dir, '../../../')

batch_file = os.path.join(current_dir, 'premake5.bat')

if platform.system() == 'Windows':
    try:
        subprocess.call([batch_file], cwd=current_dir)

        vcxproj_ng = os.path.join(solution_dir, 'NigoziEngine/NigoziEngine.vcxproj')
        vcxproj_sb = os.path.join(solution_dir, 'Sandbox/Sandbox.vcxproj')

        insert_count = 0
        ng_guid = ''

        print('\nAttempting to retrieve "NigoziEngine" GUID...')
        with open(vcxproj_ng, "r") as f:
            lines = f.readlines()

            for i, line in enumerate(lines):    
                if "<ProjectGuid>" in line:
                    ng_guid = line.strip().lstrip('<ProjectGuid>{').rstrip('}</ProjectGuid>').lower()
                    insert_count += 1
                    print('Success! -> ' + ng_guid)
                    insert_count += 1
                if '<ClCompile Include="src\\ngpch.cpp"' in line:
                    lines.insert(i + 1, "\t\t\t<PrecompiledHeader Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Create</PrecompiledHeader>\n")
                    insert_count += 1
                if insert_count == 2:
                    break
        
        f.close()

        print('\nAttempting to set reference to "NigoziEngine" in "Sandbox"...')
        with open(vcxproj_sb, "r") as f1:
            lines1 = f1.readlines()

            for j, line in enumerate(lines1):
                if '<ClCompile Include="src\\main.cpp"' in line:
                    lines1.insert(j + 2, '\t<ItemGroup>\n\t\t<ProjectReference Include="..\\NigoziEngine\\NigoziEngine.vcxproj">\n\t\t\t<Project>{' + ng_guid + '}</Project>\n\t\t</ProjectReference>\n\t</ItemGroup>\n')
                    break
        
        with open(vcxproj_sb, "w") as f1:
            f1.writelines(lines1)

        print('Success!\n')
    except subprocess.CalledProcessError as e:
        print("Script failed! --- ", e)

input('Press press ENTER to end setup...')
