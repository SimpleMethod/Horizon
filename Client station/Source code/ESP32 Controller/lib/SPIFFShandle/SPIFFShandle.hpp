/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include <memory>
#include <Arduino.h>
#include "SPIFFS.h"
#include "FS.h"

#ifndef _SPIFFSHANDLE_HPP

#define _SPIFFSHANDLE_HPP

using namespace fs;

class spiffsfs_handle : public SPIFFSFS
{

public:
    spiffsfs_handle() : SPIFFSFS{SPIFFS} {};
    String readFile(const char *path);
    boolean checkFile(const char *path);
    boolean writeFile(const char *path, const char *message);
    boolean appendFile(const char *path, const char *text);
    boolean renameFile(const char *path1, const char *path2);
    boolean deleteFile(const char *path);
};

#endif