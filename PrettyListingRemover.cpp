

#include <iostream>
#include <fstream>
#include <algorithm>

#include "Strings.h"
#include "Files.h"

using namespace MyProjects1::Common;

// File open modes
const std::ios_base::openmode oMode(std::ios::in | std::ios::binary);
const std::ios_base::openmode wMode(std::ios::out | std::ios::binary);

// Load file into lines
bool loadLines(std::string filename, std::vector<std::string> &lines)
{
  if (!FileExists(filename))
  {
    printf("ERROR : file %s not found\n\n",filename.c_str());
    return false;
  }

  std::ifstream file;
  file.open(filename.c_str(), oMode);

  if (!file.is_open())
  {
    printf("ERROR : cannot open file %s\n\n", filename.c_str());
    return false;
  }

  while (!file.eof())
  {
    std::string s;
    std::getline(file, s);

    // last line might well be without ending (CR)/LF
    if (file.fail()) {
      break;
    }

    // trim line
    s = Trim(s, "\r");

    lines.push_back(s);
  }

  file.close();

  return true;
}

// Save lines into file
bool saveLines(std::string filename, std::vector<std::string> &lines)
{
  std::ofstream file;

  file.open(filename.c_str(), wMode);

  for (std::string s : lines)
  {
    file << s << "\r\n";
  }

  file.close();

  return true;
}

// Is it a comment starting from "//" ?
bool isComment(std::string &s)
{
  std::string ss = Trim(s," \n\r\t");
  if (ss.length() >= 2 && ss[0] == '/' && ss[1] == '/')
  {
    return true;
  } else
  {
    return false;
  }
}

int main(int argc, char* argv[])
{
  printf("\n  Remover of \"pretty listing\" code\n");
  printf(">PrettyListingRemover source_file or\n");
  printf(">PrettyListingRemover full_path_file_mask\n\n");

  if (argc != 2) 
    return 1;

  std::string cppfile = argv[1];
  std::string destdir = AddBackslash(GetFileDirectory(cppfile));

  std::vector<std::string> fileList;
                              // file name or mask?
  bool isFileMask = (Find(cppfile,'*') != -1);

  if (isFileMask)
  {
    GetFileList((char *) cppfile.c_str(),false,true,fileList);
  } else
  {
    fileList.push_back(cppfile);
  }

  for (auto fileName : fileList)
  {
    printf("\n  File : %s\n", fileName.c_str());

    if (!FileExists(fileName)) 
    {
      printf("this file not found\n");
      continue;
    }
                                // load all cpp lines
    std::vector<std::string> allLines;
    if (!loadLines(fileName, allLines))
      return 1;
                                // save backup file
    saveLines(fileName + ".bak", allLines);
                                // go through all header lines
    std::vector<int> linesToDelete;
                                // number of corrections
    size_t numCorrections = 0;
                                // parse lines
    for (int i = 0; i < allLines.size() - 2; i++)
    {
#if 1
      {
        // Replacement of
        //
        // } 
        // else 
        // {
        //
        // by
        // 
        // } else (maybe else if...)
        // {
                                  // take three successive lines
        auto s0 = allLines[i];
        auto s1 = allLines[i + 1];
        auto s2 = allLines[i + 2];
                                  // we look for "}", then "else ...", then "{"
        auto s0clean = Trim(s0);
        auto s1clean = Trim(s1);
        auto s2clean = Trim(s2);

        if (s0clean == "}" && s1clean.substr(0,4) == "else" && s2clean == "{")
        {
          allLines[i] = allLines[i] + " " + TrimLeft(allLines[i + 1]);
          linesToDelete.push_back(i + 1);

          numCorrections++;
        }
      }
#endif

#if 1
      {
        // Replacement of "if(" by "if ("

        auto s0 = allLines[i];
        int pos = Find(s0,"if(");
        if (pos != -1)
        {
          allLines[i].insert(pos + 2," ");

          numCorrections++;
        }
      }
#endif

      // This may remove "};" at initailiser lists or class declarations!!!
#if 1
      {
        // Replacement of "};" by "}"

        auto s0 = allLines[i];
        auto s0clean = Trim(s0);

        if (s0clean == "};")
        {
          int pos = Find(s0,"};");
          if (pos != -1)
          {
            allLines[i].erase(pos + 1);

            numCorrections++;
          }
        }
      }
#endif

#if 1
      {
        // Insert blank line before "//" comment

        auto s0 = allLines[i];
        auto s1 = allLines[i + 1];
        auto s0clean = Trim(s0);

        if ((!isComment(s0) && s0.length() > 0 && s0clean != "{") && isComment(s1))
        {
          allLines[i + 1].insert(0,"\r\n");

          numCorrections++;
        }
      }
#endif

#if 1
      {
        // Remove blanks after every comma in function calls, like 
        // replace
        // memmove(&surface->cpoints[0], &lower->cpoints[0], sizeof(CXVector) * linesize);
        // by
        // memmove(&surface->cpoints[0],&lower->cpoints[0],sizeof(CXVector) * linesize);
        //
        // Avoid function declarations

        auto s0 = allLines[i];
        auto s1 = allLines[i + 1];
        auto s2 = allLines[i + 2];

        // avoid function declarations, take just function calls
        int count0 = GetCharCount(s0,'(','(');
        int count1 = GetCharCount(s0,')',')') + GetCharCount(s1,')',')');
        int count2 = GetCharCount(s0,'{','{') + GetCharCount(s1,'{','{') + GetCharCount(s2,'{','{');

        bool funccall = (count0 == 1 && count1 == 1 && count2 == 0);

        if (funccall)
        {
          allLines[i] = Replace(allLines[i],", ",",");

          numCorrections++;
        }
      }
#endif

    }

    printf("total %zd corrections made\n",numCorrections);
                                // now delete all h lines marked for deletion
    for (int i = static_cast<int>(linesToDelete.size()) - 1; i >= 0; i--)
      allLines.erase(allLines.begin() + linesToDelete[i]);
                                // save modified h and cpp files stripped of classes
    saveLines(fileName, allLines);
  }

  return 0;
}
