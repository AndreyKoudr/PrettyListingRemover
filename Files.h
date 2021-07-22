#pragma once
                              // server is defined for char * only
#undef UNICODE

#include <string>
#include <vector>

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
                              // to include into DLL if necessary
#define FILES_API

namespace MyProjects1 { namespace Common {

                              // file exists?
FILES_API bool FileExists(std::string &path);
                              // get file size
FILES_API size_t FileSize(std::string &path);
FILES_API size_t FileSize(FILE *file);
                              // get file list of a directory (wildcard);
void GetFileList(char *mask, bool removeextension, bool fullpath, std::vector<std::string> &list);

 
}}