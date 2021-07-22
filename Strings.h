#pragma once
                              // server is defined for char * only
#undef UNICODE

//#include "stdafx.h"
                              // STL
#include <string>
                              // math
#include <math.h>
                              // from windows, e.g. SYSTEMTIME
#ifdef x64
  #pragma pack(push,8)
    #define _WINSOCKAPI_        // stops windows.h including winsock.h  
    #define NOMINMAX
    #include <windows.h>
  #pragma pack(pop)
#else
    #define _WINSOCKAPI_        // stops windows.h including winsock.h  
    #define NOMINMAX
    #include <windows.h>
#endif


namespace MyProjects1 { namespace Common {

                              // to include into DLL if necessary
#define STRINGS_API


#define NOT_DEFINED -1
#define NOT_FOUND NOT_DEFINED


                              // integer to string
STRINGS_API std::string to_string(int i);
                              // float to string
STRINGS_API std::string to_string(float r);
                              // double to string
STRINGS_API std::string to_string(double r);
STRINGS_API std::string to_string(double d, int len, int len_after_dot);
                              // pad string from left by chars
STRINGS_API std::string PadFromLeft(std::string s, unsigned int len, char ch = ' ');
                              // get string of specified length
                              // with leading zeroes
STRINGS_API std::string to_string(int i, int numdigits);

                              // convert to upper case
STRINGS_API std::string UpCase(std::string str);
                              // convert to lower case
STRINGS_API std::string LowerCase(std::string str);



                              // delete characters (" \n\r\t" in the worst case)
                              // from string head
STRINGS_API std::string TrimLeft(std::string str, std::string chars = " ");
                              // delete characters (" \n\r\t" in the worst case)
                              // from string tail
STRINGS_API std::string TrimRight(std::string str, std::string chars = " ");
                              // delete characters (" \n\r\t" in the worst case)
                              // from both string tail and head
STRINGS_API std::string Trim(std::string str, std::string chars = " ");
                              // replace characters
STRINGS_API std::string Replace(std::string str, char from, char to);
                              // replace substrings
STRINGS_API std::string Replace(std::string str, std::string from, std::string to);


                              // get string like 04/06/2013 13:04:58,
                              // id st is NULL, it is obtained within the function
STRINGS_API std::string GetDateTimeString(SYSTEMTIME *pst = NULL, bool date = true, 
  bool time = true, bool millisec = false, char datedivider = '/', char timedivider = ':', 
  char datetimedivider = ' ');
                              // find first in string, return position,
                              // -1 if not found
STRINGS_API int Find(std::string str, char ch);
STRINGS_API int Find(std::string str, std::string substr);
                              // get substring form pos0 to pos1
STRINGS_API std::string GetSubString(std::string str, int pos0, int pos1);
                              // delete num characters from pos0
STRINGS_API void Delete(std::string &str, int pos0, int numchars);
                              // parse string by divider; returns number of extracted words;
                              // pos1 and pos2 on exit contain starting and ending positions
                              // of each word; maxcount is dimension of pos1,pos2 arrays;
                              // positions are 0-based (?)
STRINGS_API int ParseWords(std::string str, char divider, int pos1[], int pos2[], int maxcount);
															// get date/time file name (without extension), like
                              // 20130627_114113 whic means 27 June 2013 at 11:41 and 13 sec
STRINGS_API std::string GetDateTimeFilename();
															// get module file name
STRINGS_API std::string GetOwnModuleFileName(HMODULE hModule);
															// get module directory (with ending backshash)
STRINGS_API std::string GetModuleDirectory(HMODULE hModule);
                              // return just file name
STRINGS_API std::string JustFileName(std::string &path);
                              // return just file directory name with ending backslash
STRINGS_API std::string JustDirectory(std::string &path);
															// get name (all string till dot)
STRINGS_API std::string GetName(std::string s);
															// force extension
STRINGS_API std::string ForceExtension(std::string s, std::string ext);
                              // get extension if any, otherwise returns empty string
STRINGS_API std::string GetExtension(std::string s);
															// cut file name from path
STRINGS_API std::string GetFileDirectory(std::string s);

                              // convert data bytes to string
STRINGS_API std::string DataToCString(unsigned char *data, int size, bool inversebytes);
                              // convert GUID to string
STRINGS_API std::string GUIDToString(GUID *guid);

                              // add backslash to directory name
STRINGS_API std::string AddBackslash(std::string directory);

                              // get own computer name
STRINGS_API std::string GetOwnComputerName();
STRINGS_API std::wstring GetOwnComputerNameW();
                              // swap four bytes
STRINGS_API DWORD SwapBytes(DWORD i);
                              // inet_ntoa
STRINGS_API std::string GetIPString(unsigned int ip);
                              // convert dotted string to IP, returns 0 in failure
STRINGS_API unsigned long GetStringIP(std::string &ip);
                              // string contains only chars from to
STRINGS_API bool ContainsOnlyChars(std::string &s, char from, char to);
                              // count chars in the range
STRINGS_API int GetCharCount(std::string &s, unsigned char from, unsigned char to);
                              // temporary file name based of GetTickCount()
STRINGS_API std::string TempFileName(std::string prefix);

}}