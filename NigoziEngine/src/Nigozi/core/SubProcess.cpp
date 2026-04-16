#include "ngpch.h"
#ifdef NG_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <ShObjIdl_core.h>
#endif
#include "SubProcess.h"

namespace Nigozi
{
    bool SubProcess::Run(std::string command)
    {
#ifdef NG_PLATFORM_WINDOWS
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		std::wstring wsysCommand = std::filesystem::path(command).wstring();

		INT flags = CREATE_UNICODE_ENVIRONMENT;
		#ifdef DISTRIBUTION
			flags |= CREATE_NO_WINDOW;
		#endif
		// Start the child process. 
		if (!CreateProcess(NULL,   // No module name (use command line)
			wsysCommand.data(),    // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			TRUE,          // Set handle inheritance to TRUE
			flags, // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return false;
		}

		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
#else
		std::system(sysCommand.c_str());
		return true;
#endif
    }
}
