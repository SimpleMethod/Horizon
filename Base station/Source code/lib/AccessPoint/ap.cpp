/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
#include "ap.hpp"

void AccessPoint::getConnectedList()
{
  connectedList.clear();
  prettyConnectedList.clear();
  JsonObject root = connectedList.to<JsonObject>();
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;
 
  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
 
  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
 
JsonArray levels = root.createNestedArray("devices");

  for (int i = 0; i < adapter_sta_list.num; i++) {
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
    levels.add(ip4addr_ntoa(&(station.ip)));
  }
  serializeJson(root, prettyConnectedList);
}