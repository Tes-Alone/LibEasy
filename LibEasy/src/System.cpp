
/*
 * File name: System.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月04日 14时59分04秒 周5
 * End Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include <windows.h>

#include <stdlib.h>

#include "System.hpp"
#include "Exception.hpp"
#include "String.hpp"


namespace Easy {

e_void System::exec(const String& cmd) {
	if (cmd.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	CharArray buf{MAX_PATH};
	for (e_int i=0; i<cmd.length()&&i<buf.length(); i++) {
		buf[i] = cmd[i];
	}
	
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process. 
    if( !CreateProcessW( NULL,   // No module name (use command line)
        buf.toCArray(),      // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        throw ProcessCreateException{__func__, __LINE__, cmd};
    }

    // Wait until child process exits.
    //WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

e_void System::shell(const String& cmd) {
	if (cmd.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	if (system((const char*)cmd.getBytes().toCArray()) == -1) {
		throw ProcessCreateException{__func__, __LINE__, cmd};
	}
}

} // Easy