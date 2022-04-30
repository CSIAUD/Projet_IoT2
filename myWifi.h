/**
 * @file myWifi.h
 * @page wifi Wifi
 * @brief Configuration de l'access point
 * Uilisation de la biliothèque <WiFi.h>
 * Uilisation de la biliothèque <WiFiMulti.h>
 * Fonction de connexion à un wifi
 * Fonction de sauvegarde d'un wifi
 * 
 */

#include <WiFi.h>
#include <WiFiMulti.h>

char ap_ssid[32]; // SSID du AP pour la config
char ap_pwd[64]; // MDP du AP pour la config

void setupWifi(){
    JsonObject obj = spiffsGet("config");
    // String ssid = obj["ssid"];
    // ssid.toCharArray(ap_ssid,sizeof(ssid)+1);
    // String pwd = obj["pwd"];
    // pwd.toCharArray(ap_pwd,sizeof(pwd)+1);

    WiFi.mode(WIFI_AP_STA);
    // WiFi.softAP(ap_ssid, ap_pwd);
    WiFi.softAP(obj["ssid"], obj["pwd"]);

    MYDEBUG_PRINT("@IP de la carte : ");
    MYDEBUG_PRINTLN(WiFi.softAPIP());
}