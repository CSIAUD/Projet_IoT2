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
        for(JsonVariant wifi : wifis){
            wifiMulti.add(
                    wifi.key().c-str(), // Le SSID
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