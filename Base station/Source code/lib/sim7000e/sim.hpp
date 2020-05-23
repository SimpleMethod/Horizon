/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#ifndef SIM_HPP
#define SIM_HPP

#include <Arduino.h>
#include <string>
#include <iostream>
#include <regex>
#include <sstream>


using namespace std;
class Sim
{
private:
  int panicLaunchSim7000e = 0;

public:
  vector<string> sim7000Stack;
  bool sim7000eLaunch();
  bool reconectToNetwork();
  void executeCommand(String command);
  string getLocalNetwork();
  string getSignalStrenght();
  string getIPAddress();
  string getAttachedToNetwork();
  string parseAT(String command);
  boolean safeLaunch();
  boolean sendpost(string url, string data);
  string stringReplace(string text, string oldtext, string newtext);
};

#endif