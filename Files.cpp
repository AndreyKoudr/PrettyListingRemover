#include "defines.h"
                              // disable exception warning
#pragma warning(disable: 4530)

#include <sys/stat.h>
                              // strings
#include "Strings.h"
                              // class header
#include "Files.h"

                              // memory leaks
#include "memoryleaktrace.h"

namespace MyProjects1 { namespace Common {

                               // file exists?
bool FileExists(std::string &path)
{
  char buffer[1024] = {0};
  strcpy(buffer,path.c_str());
  FILE *file = fopen(buffer,"rb");
  if (file != NULL)
  {
    fclose(file);
    return true;
  } else
  {
    return false;
  };
};

size_t FileSize(std::string &path)
{
  struct stat st;
  stat(path.c_str(), &st);
  size_t size = st.st_size;
  return size;
};

size_t FileSize(FILE *file)
{
  long oldpos = ftell(file);
  fseek(file, 0, SEEK_END); 
  size_t size = ftell(file);
  fseek(file, oldpos, SEEK_SET);
  return size;
};

void GetFileList(char *mask, bool removeextension, bool fullpath, std::vector<std::string> &list)
{
	HANDLE hFind;
	BOOL bContinue = true;
	WIN32_FIND_DATAA data;
	std::string s;
  std::string directory = GetFileDirectory(std::string(mask));

	hFind = FindFirstFileA(mask,&data); 

  if ((hFind != NULL) && ((int) hFind != -1))
  {
	  while (hFind && bContinue) 
	  {
		  if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		  {
				s = data.cFileName;
				if (removeextension) s = ForceExtension(s,"");
        if (fullpath) s = AddBackslash(directory) + s;
        list.push_back(s);
		  }; 
		  bContinue = FindNextFileA(hFind,&data);
	  };
	  FindClose(hFind);
  };
};

}}
