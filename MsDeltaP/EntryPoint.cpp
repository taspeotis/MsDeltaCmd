
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

bool ParseCommandLineArguments(int nArguments, LPWSTR *pszArguments,
	bool &bVersion2, LPWSTR &szSourcePath, LPWSTR &szTargetPath, LPWSTR &szDeltaPath)
{
	if (nArguments == 5) {
		if (!wcscmp(pszArguments[1], L"-v2")) {
			bVersion2 = true;
			szSourcePath = pszArguments[2];
			szTargetPath = pszArguments[3];
			szDeltaPath = pszArguments[4];

			return true;
		}
	} else if (nArguments == 4) {
		bVersion2 = false;
		szSourcePath = pszArguments[1];
		szTargetPath = pszArguments[2];
		szDeltaPath = pszArguments[3];

		return true;
	}

	return false;
}

int main()
{
	const auto szCommandLine = GetCommandLine();

	if (szCommandLine != nullptr)
	{
		int nArguments;
		const auto pszArguments = CommandLineToArgvW(szCommandLine, &nArguments);

		if (pszArguments != nullptr)
		{
			bool bVersion2;
			LPWSTR szSourcePath;
			LPWSTR szTargetPath;
			LPWSTR szDeltaPath;

			if (ParseCommandLineArguments(nArguments, pszArguments, bVersion2, szSourcePath, szTargetPath, szDeltaPath))
			{
				const auto qwFileTypeSet = bVersion2
					? DELTA_FILE_TYPE_SET_EXECUTABLES_2
					: DELTA_FILE_TYPE_SET_EXECUTABLES_1;

				if (CreateDelta(qwFileTypeSet, DELTA_FLAG_IGNORE_FILE_SIZE_LIMIT, 0,
					szSourcePath, szTargetPath, nullptr, nullptr, DELTA_INPUT(), nullptr, 32, szDeltaPath))
				{
					return 0;
				}
			}
			else
			{
				const auto &szExecutable(pszArguments[0]);

				std::wcout << L"Create a delta from the specified source and target files" << std::endl;
				std::wcout << L"and write the output delta to the designated file name." << std::endl;
				std::wcout << std::endl;
				std::wcout << szExecutable << " (-v2) [source] [target] [delta]" << std::endl;
				std::wcout << std::endl;
				std::wcout << L"-v2 is an optional parameter that will invoke CreateDelta" << std::endl;
				std::wcout << L" with a file type set of DELTA_FILE_TYPE_SET_EXECUTABLES_2." << std::endl;
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