// Author: Adam Orcholski, www.tath.eu

#include <Windows.h>
#include <string>
#include <iostream>

#include "Shlwapi.h" // PathMatchSpec
#pragma comment(lib,"shlwapi.lib")

using namespace std;

string GetLastErrorAsString(void); // stackoverflow
void SearchAllDirectories(const string & start_path, const string & search_phrase, const string & type);

void OpenFile(const string & filename, const string & text)
{
    HANDLE file = INVALID_HANDLE_VALUE;
    
    file = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (INVALID_HANDLE_VALUE != file)
    {
        char buffer[1028] = { 0 };
        DWORD bytesRead = 0;
        size_t total_size = 0;

        BOOL readok = 0;
        string context;
        do
        {
            readok = ReadFile(file, &buffer, sizeof(buffer), &bytesRead, 0);

            if (readok && bytesRead != 0)
            {
                context += string(buffer, (size_t)bytesRead);
                total_size += bytesRead;
            }
        } while (readok && bytesRead != 0);

        size_t found = 0;
        size_t offset = 0;
        bool once = false;

        do
        {
            found = context.find(text, offset);

            if (string::npos != found)
            {
                if (!once++) { cout << "file: " << filename.c_str() << endl; }
                
                cout << "found \"" << text << "\" at pos: " << found << endl;
                offset = found + text.length();
            }
        } while (string::npos != found);
    }
    else
    {
        cout << "cant open file: " << filename.c_str() << endl;
        cout << GetLastErrorAsString() << endl;
    }

    CloseHandle(file);
}


void CheckSearchResults(const WIN32_FIND_DATA & result, const string start_path, const string search_phrase, const string type)
{
    if (!(result.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && result.cFileName[0] != '.')
    {
        if (result.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            SearchAllDirectories(start_path + "\\" + string(result.cFileName), search_phrase, type);
        }
        else
        {
            if (PathMatchSpec(result.cFileName, type.c_str()))
            {
                OpenFile((start_path + "\\" + string(result.cFileName)).c_str(), search_phrase);
            }
            else
            {
                // skip
            }
        }
    }
}

void SearchAllDirectories(const string & start_path, const string & search_phrase, const string & type)
{
    WIN32_FIND_DATA result = { 0 };
    HANDLE          file = INVALID_HANDLE_VALUE; 
    
    file = FindFirstFile((start_path + "\\*").c_str(), &result);

    if (INVALID_HANDLE_VALUE != file)
    {
        CheckSearchResults(result, start_path, search_phrase, type);

        BOOL foundNextFile = FALSE;

        do
        {
            foundNextFile = FindNextFile(file, &result);

            if (foundNextFile)
            {
                CheckSearchResults(result, start_path, search_phrase, type);
            }
            else
            {
                if (ERROR_NO_MORE_FILES != GetLastError())
                {
                    cout << GetLastErrorAsString() << endl;
                }
            }
        } while (foundNextFile);
    }
    else
    {
        cout << GetLastErrorAsString() << endl;
    }

    FindClose(file);
}

int main()
{
    const string dir = "C:\\Projects\\CAN\\cantact-fw-master" ;
    const string search_phrase = "STM32";
    const string type_filter = "*.c";  // *.c;*.h

    SearchAllDirectories(dir, search_phrase, type_filter);

    system("pause");
    return 0;
}

string GetLastErrorAsString(void)
{
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) { return string(); }
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    string message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
}
