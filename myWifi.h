/**
 * @file myWifi.h
 * @page wifi Wifi
 * @brief Configuration de l'access point
 * Uilisation de la biliothèque WiFi.h
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
        MYDEBUG_PRINT("Connecté à ");
        MYDEBUG_PRINTLN(WiFI.SSID());
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
    saveWifi(sta_ssid, sta_pwd);
    return true;
  }
}

/**
 * @brief Démarre l'access point à partir du fichier SPIFFS
 * 
 * 
 */
void setupWifi(){
  
  char ap_ssid[30]; //SSID du réseau AP pour la config
  char ap_pwd[30]; //Mot de passe du réseau AP pour la config

  MYDEBUG_PRINTLN("=== Setup WIFI ===");
  JsonObject obj = spiffsGet("config");
  String ssid = obj["ssid"];
  String pwd = obj["pwd"];
  ssid.toCharArray(ap_ssid,sizeof(ssid)+1);
  pwd.toCharArray(ap_pwd,sizeof(pwd)+1);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ap_ssid); // Connexion sans mot de passe
  // WiFi.softAP(ap_ssid, ap_pwd); // Connexion avec mot de passe

  MYDEBUG_PRINT("@IP de la carte : ");
  MYDEBUG_PRINTLN(WiFi.softAPIP());

  multiConnect();
}