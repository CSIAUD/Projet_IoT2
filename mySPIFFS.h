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
 * 
 * @param obj Le Json Object à afficher
 */
void display(JsonObject obj){
  for(JsonPair elem : obj){
    MYDEBUG_PRINT("SPIFFS key : ");
    MYDEBUG_PRINT(elem.key().c_str());
    MYDEBUG_PRINT(" || value => ");
    MYDEBUG_PRINTLN(elem.value().as<char*>())
  }
}

/**
 * @brief Reset fichier de configuration
 * Attribution des valeurs par défaut pour l'AP
 */
void setDefault(){
  if(SPIFFS.begin(true)){
    if (SPIFFS.exists(strConfigFile)) { 
      File file = SPIFFS.open(strConfigFile, "w+");

      if (file){
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            MYDEBUG_PRINT("deserializeJson() Set failed: ");
            MYDEBUG_PRINTLN(error.c_str());
            return;
        }
        JsonObject config = doc.createNestedObject("config");
        config["ssid"] = "IoTeam";
        config["pwd"] = "29052022";

        doc.add(config);
        if(serializeJson(doc, file) == 0){
            MYDEBUG_PRINTLN("Erreur Set SerializeJson Default");
        }
      }else{
        MYDEBUG_PRINTLN("Ereur Set Ouverture");
      }
    }
  }
}

/**
 * @brief Permet la récupération d'un Json Object en fonction du sPath.
 * 
 * @param sPath chemin d'acces dans le spiffs
 * @return JsonObject vide si sPath pas trouvé
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
      setDefault();
    }
  }
  return obj;
}

/**
 * @brief Permet de sauvegarder une valeur
 * 
 * @param sPath L'endroit dans lequel stocker l'objet
 * @param newObj L'objet à stocker
 */
void spiffsSet(String sPath, JsonObject newObj){
    if(SPIFFS.begin(true)){
    if (SPIFFS.exists(strConfigFile)) { 
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
        obj.add(newObj);
        doc.add(obj);

        if(serializeJson(doc, file) == 0){
            MYDEBUG_PRINTLN("Erreur SerializeJson Set");
        }
      }else{
        MYDEBUG_PRINTLN("Ereur Ouverture");
      }
    }
  }
}

/**
 * @brief Remet à zéro les fichiers de SPIFFS
 * Puis redirection sur la page d'accueil
 */
void reset(){
    SPIFFS.format();
    setDefault();
    // webServer.sendHeader("Location", "/",true);
    // webServer.send(302, "text/plane",""); 
}
