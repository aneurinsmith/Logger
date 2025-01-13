
#include "terminal_sink.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef win32
#include <Windows.h>
#endif

namespace LOG
{
	TerminalSink::TerminalSink()
	{
	#ifdef win32

		std::string name = "cmdpipe";
		name += std::to_string(std::rand() % 101);
		std::cout << name << std::endl;

		std::string pipe = "\\\\.\\pipe\\" + name;
		std::cout << pipe << std::endl;

		DWORD dwWritten;
		m_pipe = CreateNamedPipeA(
			pipe.c_str(),
			PIPE_ACCESS_OUTBOUND,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			1,
			1024 * 16,
			1024 * 16,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL);

		if (m_pipe != INVALID_HANDLE_VALUE) {
			STARTUPINFO si = { sizeof(si) };
			si.lpTitle = (LPSTR)"Debugger";
			PROCESS_INFORMATION pi;

			std::string command = "/c powershell -Command \""
				"$pipe = new-object System.IO.Pipes.NamedPipeClientStream('.','" + name + "','In'); "
				"$pipe.Connect(); "
				"$sr = new-object System.IO.StreamReader $pipe; "
				"while (($data = $sr.ReadLine()) -ne $null) { \\\"$data\\\" } "
				"\"";

			if (CreateProcess("C:\\Windows\\System32\\cmd.exe", (LPSTR)command.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
				ConnectNamedPipe(m_pipe, NULL);
			}
		}

	#endif
	}

	void TerminalSink::write(std::string msg)
	{
	#ifdef win32
		msg += "\n";
		DWORD bytesWritten;
		WriteFile(m_pipe, msg.c_str(), msg.size(), &bytesWritten, NULL);
	#endif
	}
}
