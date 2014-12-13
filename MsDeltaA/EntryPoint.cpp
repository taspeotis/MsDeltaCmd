
#pragma comment(lib, "msdelta.lib")

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#define WINVER 0x0600

#include <sdkddkver.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <shellapi.h>
#include <msdelta.h>

#include <iostream>
#include <iomanip>

int main()
{
	const auto szCommandLine = GetCommandLine();

	if (szCommandLine != nullptr)
	{
		int nArguments;
		const auto pszArguments = CommandLineToArgvW(szCommandLine, &nArguments);

		if (pszArguments != nullptr)
		{
			if (nArguments == 4)
			{
				const auto &szSourcePath(pszArguments[1]);
				const auto &szDeltaPath(pszArguments[2]);
				const auto &szTargetPath(pszArguments[3]);

				if (ApplyDelta(DELTA_APPLY_FLAG_ALLOW_PA19, szSourcePath, szDeltaPath, szTargetPath))
				{
					return 0;
				}
			}
			else
			{
				const auto &szExecutable(pszArguments[0]);

				std::wcout << L"Use the specified delta and source files " << std::endl;
				std::wcout << L"to create a new copy of the target file." << std::endl;
				std::wcout << std::endl;
				std::wcout << szExecutable << " [source] [delta] [target]" << std::endl;
				std::wcout << std::endl;

				SetLastError(ERROR_INVALID_DATA);
			}
		}
	}

	const auto &dwLastError = GetLastError();
	LPWSTR szMessage;

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&szMessage), 0, nullptr))
	{
		std::wcout << L"0x" << std::hex << std::setw(8) << std::setfill(L'0') << dwLastError << L" " << szMessage << std::endl;
	}

	return dwLastError;
}