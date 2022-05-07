/**
 * @file mySPIFFS.h
 * @page spiffs SPIFFS
 * @brief Stockage des fichiers
 * 
 * Utilisation de la Bibliothèque ArduinoJson crée par Benoit Blanchon : https://github.com/bblanchon/ArduinoJson
 * 
 * Utilisation de la fonction spiffsGet :
 * Pour le Json suivant :
 * @code
 * {
 *  "config" : [
 *    "ssid" : "IoTeam",
 *    "pwd" : "29052022",
 *    ],
 *  "wifis" : [
 *    "Creanics" : "moimoimoi",
 *    "Superman" : "ClarkKent"
 *    ]
 * }
 * @endcode
 * 
 * Pour accéder à la liste des wifis, il suffira de mettre "wifis" pour le sPath et vous aurez la liste des wifis
 * 
 */

#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>   

String strConfigFile("/config.json");

/**
 * @brief Affichage d'un JsonObject
 * key => value
 * 
 * @param obj Le Json Object à afficher
 */
void display(JsonObject obj){
  for(JsonPair elem : obj){
    MYDEBUG_PRINT("SPIFFS key : ");
    MYDEBUG_PRINT(elem.key().c_str());
    MYDEBUG_PRINT(" || value => ");
    MYDEBUG_PRINTLN(elem.value().as<const char*>());
  }
}

/**
 * @brief Permet la récupération d'un Json Object en fonction du sPath.
 * 
 * @param sPath Chemin d'acces dans le spiffs
 * @return JsonObject (vide si non trouvé)
 */
JsonObject spiffsGet(String sPath){
  MYDEBUG_PRINTLN("GET");
  JsonObject obj;

  if(SPIFFS.begin(true)){
    File configFile = SPIFFS.open(strConfigFile, "r");
    if (configFile) {
      DynamicJsonDocument jsonDocument(512);
      
      DeserializationError error = deserializeJson(jsonDocument, configFile);
      if (error){
        MYDEBUG_PRINTLN("-SPIFFS : Impossible de parser le JSON");  
        return obj;        
      }
      if(jsonDocument.containsKey(sPath)){
          obj = jsonDocument[sPath];
      }
      configFile.close();
    }else{
      MYDEBUG_PRINTLN("Ereur Get Ouverture");
    }
    SPIFFS.end();
  }else{
    MYDEBUG_PRINTLN("Ereur Get SPIFFS");
  }
  return obj;
}

/**
 * @brief Implémentation des valeurs par défaut du fichier de config :
 * @code
 * doc
 * -> config
 *   -> ssid = IoTeam
 *   -> pwd = 29052022
 * @endcode
 */
void setDefault(){
  MYDEBUG_PRINTLN("default");

  if (SPIFFS.begin(true)) {
    MYDEBUG_PRINTLN("-SPIFFS : MONTE");
    File configFile = SPIFFS.open(strConfigFile, "w");
    if (configFile) {
      MYDEBUG_PRINTLN("-SPIFFS: Fichier créé");
      DynamicJsonDocument jsonDocument(512);

      JsonObject config = jsonDocument.createNestedObject("config");
      JsonObject wifis = jsonDocument.createNestedObject("wifis");

      config["ssid"] = "IoTeam";
      config["pwd"] = "29052022";
      jsonDocument.add(config);
      jsonDocument.add(wifis);

      if (serializeJson(jsonDocument, configFile) == 0) {
        MYDEBUG_PRINTLN("-SPIFFS : Impossible d'écrire le JSON dans le fichier de configuration");
      }
      // Fermeture du fichier
      configFile.close();
      MYDEBUG_PRINTLN("-SPIFFS : Fichier fermé");
    } else{
      MYDEBUG_PRINTLN("-SPIFFS : Impossible d'ouvrir le fichier en ecriture");
    }
    SPIFFS.end();  
  } else {
    MYDEBUG_PRINT("-SPIFFS : Impossible de monter le système de fichier");
  }







  // if(SPIFFS.begin()){
  //   File file = SPIFFS.open(strConfigFile, "w");
  //   if (file){
  //     DynamicJsonDocument doc(512);
  //     JsonArray base = doc.to<JsonArray>();

  //     JsonObject config;
  //     config["ssid"] = "IoTeam";
  //     config["pwd"] = "29052022";
  //     JsonObject wifis;
  //     wifis["Creanics"] = "moimoimoi";

  //     base.add(config);
  //     base.add(wifis);

  //     MYDEBUG_PRINTLN(">>>>>>>>>>>>>>>>>>>>");
  //     display(doc["config"]);
  //     MYDEBUG_PRINTLN("----------");
  //     display(doc["wifis"]);
  //     MYDEBUG_PRINTLN("<<<<<<<<<<<<<<<<<<<<");

  //     if(serializeJson(doc, file) == 0){
  //       MYDEBUG_PRINTLN("Erreur SerializeJson default");
  //     }else{
  //       MYDEBUG_PRINTLN("VVVVV Test apres serialisation VVVVV");
  //       display(spiffsGet("config"));
  //     }
  //   }else{
  //     MYDEBUG_PRINTLN("Ereur default Ouverture");
  //   }
  //   SPIFFS.end();
  // }
}

/**
 * @brief Appel de la fonction @code setDefault() @endcode Pour la remise à zéro du fichier de SPIFFS
 * Puis redirection sur la page d'accueil
 */
void reset(){
  SPIFFS.format();
  setDefault();
  // webServer.sendHeader("Location", "/",true);
  // webServer.send(302, "text/plane",""); 
}

/**
 * @brief Sauvegarde un wifi dans le fichier de SPIFFS
 * 
 * @param ssid Nouveau SSID
 * @param pwd Nouveau PWD
 */
void saveWifi(String ssid, String pwd){
  JsonObject obj;
  obj[ssid] = pwd;
  // spiffsSet("wifis", obj);
}