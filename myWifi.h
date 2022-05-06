/**
 * @file myWifi.h
 * @page wifi Wifi
 * @brief Configuration de l'access point
 * Uilisation de la biliothèque <WiFi.h>
 * Uilisation de la biliothèque <WiFiMulti.h>
 * 
 * Fonction de connexion à un wifi
 * Fonction de sauvegarde d'un wifi
 * 
 */

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti; 

/**
 * @brief Tente la connexion en WifiMulti.
 * Get l'objet wifis dans le fichier de SPIFFS
 * Si au moins 1 wifis de sauvegardé tentative de connexion
 */
void multiConnect(){
    JsonObject wifis = spiffsGet("wifis");
    if(wifis.size() > 0){
        for(JsonPair wifi : wifis){
            wifiMulti.addAP(
                    wifi.key().c_str(), // Le SSID
                    wifi.value().as<const char*>() // Le MDP
                );
        }

        if(wifiMulti.run() == WL_CONNECTED) { // Connexion
            MYDEBUG_PRINTLN("");
            MYDEBUG_PRINTLN("WiFi connecté");
            MYDEBUG_PRINTLN("@IP : ");
            MYDEBUG_PRINTLN(WiFi.localIP());
        }else{
            MYDEBUG_PRINTLN("Aucun réseau connu à proximité");
        }
    }
}

/**
 * @brief Démarrage d'une nouvelle connexion
 * 
 * @param sta_ssid1 Le nouveau SSID 
 * @param sta_pwd1 Le nouveau MDP
 * @return true 
 * @return false 
 */
bool newConnection(char sta_ssid[], char sta_pwd[]){
  
//   char sta_ssid[30];
//   char sta_pwd[30];
//   sta_ssid1.toCharArray(sta_ssid,sizeof(sta_ssid1)+1);
//   sta_pwd1.toCharArray(sta_pwd,sizeof(sta_pwd1)+1);

  int connectionCount = 0;
  
  MYDEBUG_PRINTLN("===== Connexion =====");
  
  // Démarrage du mode Station
  WiFi.begin(sta_ssid, sta_pwd);
  MYDEBUG_PRINT("WIFI =>  Connexion au réseau : ");
  MYDEBUG_PRINTLN(sta_ssid);
  // Tentative de connexion
  while (WiFi.status() != WL_CONNECTED && connectionCount <= 15*2) {
    delay(500);
    connectionCount++;
    MYDEBUG_PRINT(".");
  }

  if(WiFi.status() != WL_CONNECTED){
    return false;
  }else{
    MYDEBUG_PRINTLN(WiFi.SSID());
    MYDEBUG_PRINTLN("== Connecté =========");
    // saveWifi(sta_ssid, sta_pwd);
    return true;
  }
}

/**
 * @brief Démarre l'access point à partir du fichier SPIFFS
 * 
 * 
 */
void setupWifi(){
    JsonObject obj = spiffsGet("config");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(obj["ssid"], obj["pwd"]);

    MYDEBUG_PRINT("@IP de la carte : ");
    MYDEBUG_PRINTLN(WiFi.softAPIP());

    multiConnect();
}