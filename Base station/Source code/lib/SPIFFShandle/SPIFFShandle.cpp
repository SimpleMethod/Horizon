/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include "SPIFFShandle.hpp"

String spiffsfs_handle::readFile(const char *path)
{
  File file = SPIFFS.open(path);
  if (!file || file.isDirectory())
  {
    return "NULL";
  }
  while (file.available())
  {
    return file.readString();
    file.close();
  }
  return "NULL";
}

boolean spiffsfs_handle::checkFile(const char *path)
{
  File file = SPIFFS.open(path);
  if (!file || file.isDirectory())
  {
      file.close();
    return false;
  }
  else
  {
      file.close();
    return true;
  }
}

boolean spiffsfs_handle::writeFile(const char *path, const char *message)
{
  File file = SPIFFS.open(path, FILE_WRITE);
  if (!file)
  {
      file.close();
      return false;
  }
  if (file.print(message))
  {
      file.close();
    return true;
  }
  else
  {
      file.close();
   return false;
  }
}

boolean spiffsfs_handle::appendFile( const char *path, const char *text)
{
  File file = SPIFFS.open(path, FILE_APPEND);
  if (!file)
  {
      file.close();
     return false;
  }
  if (file.print(text))
  {
      file.close();
     return true;
  }
  else
  {
      file.close();
     return false;
  }
}

boolean spiffsfs_handle::renameFile( const char *path1, const char *path2)
{
  if (SPIFFS.rename(path1, path2))
  {
    return true;
  }
  else
  {
   return false;
  }
}

boolean spiffsfs_handle::deleteFile(const char *path)
{
  if (SPIFFS.remove(path))
  {
    return true;
  }
  else
  {
   return false;
  }
}

