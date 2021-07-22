#include "defines.h"
                              // disable exception warning
#pragma warning(disable: 4530)
                              // class header
#include "Strings.h"
#include <regex>
                              // time
#include <time.h>
#include <algorithm>
#include <assert.h>
                              // memory leaks
#include "memoryleaktrace.h"

namespace MyProjects1 { namespace Common {

std::string to_string(int i)
{
  char buf[1024];
  sprintf(buf,"%d",i);
  return std::string(buf);
}

std::string to_string(float r)
{
  char buf[1024];
  sprintf(buf,"%f",r);
  return std::string(buf);
}

std::string to_string(double r)
{
  char buf[1024];
  sprintf(buf,"%f",r);
  return std::string(buf);
}

std::string to_string(double d, int len, int len_after_dot)
{
  assert(len > len_after_dot);

  char buf[1024];
  std::string format = std::string("%") + to_string(len) + '.' + to_string(len_after_dot) + 'f';
  sprintf(buf,format.c_str(),d);
  return std::string(buf);
}

std::string PadFromLeft(std::string s, unsigned int len, char ch)
{
  int n = len - static_cast<int>(s.length());
  if (n > 0) s = s.insert(0,n,ch);

  return s;
}

std::string to_string(int i, int numdigits)
{
  std::string s = to_string(i);
  std::string res = PadFromLeft(s,numdigits,'0');
  return res;
}

std::string UpCase(std::string str)
{
  std::transform(str.begin(),str.end(),str.begin(),::toupper);
  return str;
}

std::string LowerCase(std::string str)
{
  std::transform(str.begin(),str.end(),str.begin(),::tolower);
  return str;
}

std::string TrimLeft(std::string str, std::string chars)
{
  str.erase(0,str.find_first_not_of(chars));
  return str;
}

std::string TrimRight(std::string str, std::string chars)
{
  str.erase(str.find_last_not_of(chars)+1);
  return str;
}

std::string Trim(std::string str, std::string chars)
{
  return TrimRight(TrimLeft(str,chars),chars);
}

std::string Replace(std::string str, char from, char to)
{
  std::string s = str;
  std::replace(s.begin(),s.end(),from,to);
  return s;
}

static void replaceAll(std::string &s, const std::string &search, const std::string &replace ) {
  for (size_t pos = 0; ; pos += replace.length()) {
    // Locate the substring to replace
    pos = s.find( search, pos );
    if( pos == std::string::npos ) break;
    // Replace by erasing and inserting
    s.erase( pos, search.length() );
    s.insert( pos, replace );
  }
}

std::string Replace(std::string str, std::string from, std::string to)
{
  std::string s = str;
  replaceAll(s,from,to);
  return s;
}

std::string GetDateTimeString(SYSTEMTIME *pst, bool date, bool time, 
  bool millisec, char datedivider, char timedivider, char datetimedivider)
{
                              // get current time
  SYSTEMTIME st;
  if (pst != NULL)
  {
    memmove(&st,pst,sizeof(st));
  } else
  {
    GetLocalTime(&st);
  } 

  std::string str;

  if (date)
  {
    std::string datestr = to_string(st.wDay,2) + datedivider +
      to_string(st.wMonth,2) + datedivider +
      to_string(st.wYear,4);

    str = str + datestr;
  }

  if (time)
  {
    std::string timestr = to_string(st.wHour,2) + timedivider +
      to_string(st.wMinute,2) + timedivider +
      to_string(st.wSecond,2);

    if (str.length()) str = str + datetimedivider;
    str = str + timestr;
  }

  if (millisec)
  {
    std::string msecstr = to_string(st.wMilliseconds,3);
    if (str.length()) str = str + timedivider;
    str = str + msecstr;
  }

  return str;
}

int Find(std::string str, char ch)
{
  size_t pos = str.find(ch);
  if (pos == std::string::npos)
  {
    return NOT_FOUND;
  } else
  {
    return static_cast<int>(pos);
  }
}

int Find(std::string str, std::string substr)
{
  size_t pos = str.find(substr);
  if (pos == std::string::npos)
  {
    return NOT_FOUND;
  } else
  {
    return static_cast<int>(pos);
  }
}

std::string GetSubString(std::string str, int pos0, int pos1)
{
  std::string res = str.substr(pos0,(pos1 - pos0 + 1));
  return res;
}

void Delete(std::string &str, int pos0, int numchars)
{
  str.erase(pos0,numchars);
}

int ParseWords(std::string str, char divider, int pos1[], int pos2[], int maxcount)
{
  int numwords = 0;
  int len = static_cast<int>(str.length());
  if (len > 0)
  {
		int curpos1 = NOT_DEFINED;
		if (str[0] != divider) curpos1 = 0;
		for (int i = 0; i < len; i++)
		{
			if (str[i] == divider)
      {
				if (curpos1 != NOT_DEFINED)
				{
          if (numwords < maxcount)
          {
            pos1[numwords] = curpos1;
            pos2[numwords] = i - 1;
					  curpos1 = NOT_DEFINED;
					  numwords++;
          } else
          {
            break;
          }
				}
			} else
			{
				if (curpos1 == NOT_DEFINED)
				{
          curpos1 = i;
				}
			}
		}

		if (curpos1 != NOT_DEFINED)
		{
      if (numwords < maxcount)
      {
        pos1[numwords] = curpos1;
        pos2[numwords] = len;
			  numwords++;
      }
		}
  }

  return numwords;
}

std::string GetDateTimeFilename()
{
                              // get current time
  SYSTEMTIME st;
  GetLocalTime(&st); 

  std::string str = to_string(st.wYear,4) +
    to_string(st.wMonth,2) +
    to_string(st.wDay,2) + "_" +
    to_string(st.wHour,2) +
    to_string(st.wMinute,2) +
    to_string(st.wSecond,2);

  return str;
}

std::string GetOwnModuleFileName(HMODULE hModule)
{
	#define DIR_SIZE 2048
	char buffer[DIR_SIZE] = {0};

	int res = GetModuleFileNameA(hModule,buffer,DIR_SIZE);

	return std::string(buffer);
}

std::string GetModuleDirectory(HMODULE hModule)
{
	std::string name = GetOwnModuleFileName(hModule);

  name.erase(name.find_last_of('\\') + 1);

	return name;
}

std::string JustFileName(std::string &path)
{
  std::string name = path;
  name.erase(0,path.find_last_of("\\/") + 1);

	return name;
}

std::string JustDirectory(std::string &path)
{
  std::string name = path;
  name.erase(path.find_last_of("\\/") + 1);

	return name;
}

std::string GetName(std::string s)
{
  std::size_t pos = s.rfind('.');
	if (pos != std::string::npos)
	{
		return s.substr(0,pos);
	} else
	{
		return std::string("");
	}
}

std::string ForceExtension(std::string s, std::string ext)
{
  std::size_t pos = s.rfind('.');
	if (pos != std::string::npos)
	{
		if (ext.length())
		{
			return s.substr(0,pos + 1) + ext;
		} else
		{
			return s.substr(0,pos);
		}
	} else
	{
		if (ext.length())
		{
			return s + "." + ext;
		} else
		{
			return s;
		}
	}
}

std::string GetExtension(std::string s)
{
  std::string ext;
  std::size_t pos = s.rfind('.');
	if (pos != std::string::npos)
	{
    ext = s.substr(pos + 1);
	}
  return ext;
}

std::string GetFileDirectory(std::string s)
{
  std::size_t pos = s.rfind('\\');
	if (pos != std::string::npos)
  {
		return s.substr(0,pos);
	} else
	{
		return std::string("");
	}
}

std::string DataToCString(unsigned char *data, int size, bool inversebytes)
{
  std::string result;
  result.resize(size * 2);
  char buffer[128];
  int count = 0;
  if (inversebytes)
  {
    for (int i = size - 1; i >= 0; i--)
    {
      sprintf(buffer,"%02X",data[i]);
      result[count++] = buffer[0];
      result[count++] = buffer[1];
    }
  } else
  {
    for (int i = 0; i < size; i++)
    {
      sprintf(buffer,"%02X",data[i]);
      result[count++] = buffer[0];
      result[count++] = buffer[1];
    }
  }

  return result;
}

std::string GUIDToString(GUID *guid)
{
  unsigned char *buffer = (unsigned char *) guid;
  std::string res = DataToCString(buffer,4,true) + '-' +
    DataToCString(buffer + 4,2,true) + '-' +
    DataToCString(buffer + 6,2,true) + '-' +
    DataToCString(buffer + 8,2,false) + '-' +
    DataToCString(buffer + 10,6,false);
  return res;
}

std::string AddBackslash(std::string directory)
{
  if (directory.length() == 0)
  {
    return std::string("");
  } else 
  {
    if (directory[directory.length() - 1] != '\\')
    {
      return directory + "\\";
    } else
    {
      return directory;
    }
  }
}

std::string GetOwnComputerName()
{
  #define SIZE 1024
  char buffer[SIZE];
  DWORD size = SIZE;
  ::GetComputerNameA(buffer,&size);
  return std::string(buffer);
}

std::wstring GetOwnComputerNameW()
{
  #define SIZE 1024
  wchar_t buffer[SIZE];
  DWORD size = SIZE;
  ::GetComputerNameW(buffer,&size);
  return std::wstring(buffer);
}

DWORD SwapBytes(DWORD i)
{
  DWORD res = (((i & 0x000000FF) << 24) | ((i & 0x0000FF00) << 8) | 
    ((i & 0x00FF0000) >> 8) | ((i & 0xFF000000) >> 24));
  return res;
}

std::string GetIPString(unsigned int ip)
{
  unsigned char *bytes = (unsigned char *) &ip;
  return to_string(bytes[0]) + "." + to_string(bytes[1]) + "." +
    to_string(bytes[2]) + "." + to_string(bytes[3]);
}

unsigned long GetStringIP(std::string &ip)
{
  unsigned long result = 0;
  unsigned char *bytes = (unsigned char *) &result;

  int pos1[100],pos2[100];
  int n = ParseWords(ip,'.',pos1,pos2,100);
  if (n == 4)
  {
    for (int i = 0; i < n; i++)
    {
      std::string s = ip.substr(pos1[i],pos2[i] - pos1[i] + 1);
      if (s.length() > 3) return 0;
      if (!ContainsOnlyChars(s,'0','9')) return 0;
      int b = atoi(s.c_str());
      if (b > 255) return 0;
      bytes[i] = b;
    }

    return result;
  } else
  {
    return 0;
  }
}

bool ContainsOnlyChars(std::string &s, char from, char to)
{
  for (size_t i = 0; i < s.length(); i++)
  {
    if (s[i] < from || s[i] > to) return false;
  }

  return true;
}

std::string TempFileName(std::string prefix)
{
  std::string name = prefix + to_string(static_cast<int>(GetTickCount()));
  return name;
}

// count chars in the range
int GetCharCount(std::string &s, unsigned char from, unsigned char to)
{
  int count = 0;

  for (int i = 0; i < s.length(); i++)
  {
    if ((s[i] >= from) && (s[i] <= to))
    {
      count++;
    }
  }

  return count;
}


}}