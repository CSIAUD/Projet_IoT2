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

#include "SPIFFS.h"
#include <ArduinoJson.h>   

String strConfigFile("/config.txt");
DynamicJsonDocument doc(512);

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
 * @brief Permet de sauvegarder une valeur
 * Crée un nouveau path dans le fichier s'il n'existe pas
 * 
 * @param sPath L'endroit dans lequel stocker l'objet
 * @param newObj L'objet à stocker
 */
void spiffsSet(String sPath, JsonObject newObj){
  if(SPIFFS.begin(true)){
    File file = SPIFFS.open(strConfigFile, "w+");

    if (file){
      JsonObject obj;
      DeserializationError error = deserializeJson(doc, file);
      if (error) {
          MYDEBUG_PRINT("deserializeJson() failed: ");
          MYDEBUG_PRINTLN(error.c_str());
          return;
      }
      if(doc.containsKey(sPath)){
          obj = doc[sPath];
      }else{
          obj = doc.createNestedObject(sPath);
      }
      
      for(JsonPair elem : newObj){
        obj[elem.key().c_str()] = elem.value().as<const char*>();
      }
      doc.add(obj);

      display(doc["config"]);
      if(serializeJson(doc, file) == 0){
          MYDEBUG_PRINTLN("Erreur SerializeJson Set");
      }
    }else{
      MYDEBUG_PRINTLN("Ereur Ouverture");
    }
  }
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
  const size_t CAPACITY = JSON_OBJECT_SIZE(1);
  StaticJsonDocument<CAPACITY> doc;

  // create an object
  JsonObject config = doc.to<JsonObject>();
  config["ssid"] = "IoTeam";
  config["pwd"] = "29052022";

  spiffsSet("config", config);
}

/**
 * @brief Permet la récupération d'un Json Object en fonction du sPath.
 * 
 * @param sPath Chemin d'acces dans le spiffs
 * @return JsonObject (vide si non trouvé)
 */
JsonObject spiffsGet(String sPath){
  JsonObject obj;

  if(SPIFFS.begin(true)){
    if (SPIFFS.exists(strConfigFile)) { 
      File file = SPIFFS.open(strConfigFile, "r");

      if (file){
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            MYDEBUG_PRINT("deserializeJson() Get failed: ");
            MYDEBUG_PRINTLN(error.c_str());
            file.close();
            setDefault();
            return obj;
        }
        if(doc.containsKey(sPath)){
            obj = doc[sPath];
        }
      }else{
        MYDEBUG_PRINTLN("Ereur Get Ouverture");
      }
    } else {
      MYDEBUG_PRINTLN("Ereur Get Fichier inexistant");
      SPIFFS.end();
      setDefault();
    }
    SPIFFS.end();
  }
  return obj;
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
 * @brief Simple fonction de test
 * 
 */
void testSpiffs(){
  setDefault();
  display(spiffsGet("config"));
}