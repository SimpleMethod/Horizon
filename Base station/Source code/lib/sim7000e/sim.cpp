/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include "sim.hpp"

bool Sim::reconectToNetwork()
{
  delay(8);
  Serial2.print(F("AT+CGACT=1, 1\r\n"));

  if (getAttachedToNetwork() == "Not found")
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Sim::sim7000eLaunch()
{
  //AT+IPR=57600
  Serial2.print(F("AT\r\n"));
  delay(5);
  Serial2.print(F("AT+CPIN?\r\n"));
  delay(5);
  Serial2.print(F("ATE1V1\r\n"));
  delay(5);
  //Serial2.print(F("AT+CGMM\r\n"));
  //delay(5);
  //Serial2.print(F("AT+CGMI\r\n"));
  delay(10);
  Serial2.print(F("AT+COPS=4,2,\"26002\"\r\n"));
  delay(10);
  Serial2.print(F("AT+CMEE=1\r\n"));
  delay(5);
  Serial2.print(F("AT+CGATT=1\r\n"));
  delay(8);
  Serial2.print(F("AT+CGACT=1, 1\r\n"));
  delay(8);

  if (getAttachedToNetwork() == "Not found")
  {
    return false;
  }
  else
  {
    return true;
  }
}

void Sim::executeCommand(String command)
{
  Serial2.flush();
  Serial2.print(command);
  sim7000Stack.push_back(parseAT(Serial2.readString()));
}

string Sim::getLocalNetwork()
{
  Serial2.flush();
  Serial2.print(F("AT+COPS?\r\n"));

  smatch match;
  regex reg{"\"(.*?)\""};
  string temp = parseAT(Serial2.readString());
  if (regex_search(temp, match, reg))
  {
    return match.str(1);
  }
  else
  {
    return "Not found";
  }
}

string Sim::getSignalStrenght()
{
  Serial2.flush();
  Serial2.print(F("AT+CSQ\r\n"));

  smatch match;
  regex reg{" (.*?),"};
  string temp = parseAT(Serial2.readString());
  if (regex_search(temp, match, reg))
  {
    return match.str(1);
  }
  else
  {
    return "Not found";
  }
}

string Sim::getIPAddress()
{

try
{
  Serial2.flush();
  Serial2.print(F("AT+CGPADDR= 1\r\n"));

  smatch match;
  regex reg{"\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"};
  string temp = parseAT(Serial2.readString());
  if (regex_search(temp, match, reg))
  {
    return match.str(0);
  }
  else
  {
    return "Not found";
  }
}
catch(const std::exception& e)
{
  Serial.println( e.what());
  return "Not found";
}

return "Not found";
  
}

string Sim::getAttachedToNetwork()
{
  try
  {
      Serial2.flush();
  Serial2.print(F("AT+CGATT?\r\n"));

  smatch match;
  regex reg{"[0-9]"};
  string temp = parseAT(Serial2.readString());
  if (regex_search(temp, match, reg))
  {
    return match.str(0);
  }
  else
  {
    return "Not found";
  }
  }
  catch(const std::exception& e)
  {
    Serial.println(e.what());
  }
   return "Not found";

}

string Sim::parseAT(String command)
{
  char tempArray[90];
  //string tempString;
  regex reg{"\r\r\n(.+)\r\n\r\n"};
  smatch match;
  command.toCharArray(tempArray, 90);
  string s(tempArray);

  if (regex_search(s, match, reg))
  {
    //tempString = match.str(1);
    return match.str(1);
  }
  else
  {
    return "Not found";
  }

  /*
  Tylko jeśli chcemy wrócić do formatu String Arduino
  */
  //int n = tempString.length();
  //char char_array[n + 1];
  //strcpy(char_array, tempString.c_str());
  //Serial.println(char_array);

  //return tempString;
}

boolean Sim::safeLaunch()
{
  while (panicLaunchSim7000e <= 20)
  {
    delay(2000);
    if (sim7000eLaunch() == false)
    {
      delay(500);
      panicLaunchSim7000e++;
      Serial.printf(".");
    }
    else
    {
      panicLaunchSim7000e = 0;
      return true;
    }
    if (panicLaunchSim7000e == 5 || panicLaunchSim7000e == 10)
    {
      delay(2000);
    }
    if (panicLaunchSim7000e == 15)
    {
      delay(2000);
    }
  }
  return false;
}

boolean Sim::sendpost(string url, string data)
{
try
{

  
  // url
  string templateURL = "AT+HTTPPARA=\"URL\",\"STR\"\r\n";
  templateURL = stringReplace(templateURL, "STR", url);
  const char *httpurl = templateURL.data();
  //
  // Lenght
  string templateAT = "AT+HTTPDATA=320,1000\r\n";
  const char *c = data.data();
  int datalenght = data.size();
  string out_string;
  stringstream ss;
  ss << datalenght;
  out_string = ss.str();
  templateAT = stringReplace(templateAT, "320", out_string);
  const char *httpdata = templateAT.data();
  //

  Serial2.print(F("AT+SAPBR=2,1\r\n"));
  delay(30);
  Serial2.print(F("AT+SAPBR=1,1\r\n"));
  delay(10);
  Serial2.print(F("AT+HTTPTERM\r\n"));
  delay(20);
  Serial2.print(F("AT+HTTPINIT\r\n"));
  delay(10);
  Serial2.print(httpurl);
  delay(30);
  Serial2.print(F("AT+HTTPPARA=\"CID\",1\r\n"));
  delay(10);
  Serial2.print(httpdata);
  //Serial2.print("AT+HTTPDATA=354,1000\r\n");
  delay(400);
  Serial2.print(c);
  delay(1200);
  Serial2.print(F("AT+HTTPACTION=1\r\n")); //1=>0
  delay(2000);
  return true;
}
catch(const std::exception& e)
{
  Serial.println(e.what());
  return false;
}

    
  //Serial2.print(F("AT+HTTPREAD\r\n"));
  //delay(2000);
  //Serial2.print(F("AT+HTTPTERM\r\n"));
  //delay(100);
  //Serial2.flush();
  //Serial2.print(F("AT+HTTPREAD\r\n"));

  //Serial.println(Serial2.readString());

  //Serial.println(Serial2.readString());
  //string respond = parseAT("AT+HTTPREAD");

  //const char *read_data = respond.data();
  //Serial.printf(read_data);

  //Serial.println("koniec");
  //Serial.println("Dlugosc:");
  //Serial.println(httpurl);
  //Serial.println("url:");
  //Serial.println(httpdata);
}
string Sim::stringReplace(string text, string oldtext, string newtext)
{
  while (text.find(oldtext) != string::npos)
  {
    text.replace(text.find(oldtext), 3, newtext);
  }

  return text;
}