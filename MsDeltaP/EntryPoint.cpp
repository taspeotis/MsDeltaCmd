
#define UNICODE
#define WINVER 0x0600

#include <sdkddkver.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <shellapi.h>
#include <msdelta.h>

#include <iostream>

int main()
{
	const auto szCommandLine = GetCommandLine();
	
	if (szCommandLine != nullptr)
	{
		int nArguments;
		const auto pszArguments = CommandLineToArgvW(szCommandLine, &nArguments);

		if (pszArguments != nullptr)
		{
			if (nArguments == 3)
			{
				// do msdelta
			}
			else
			{
				// print usage
			}

			return 0;
		}
	}

	// TODO: show a descriptive message
	return GetLastError();
}