/**
 * @file myWebServer.h
 * @page server Serveur WEB
 * @brief Génération des pages + gestion des routes
 * 
 */
#include <WebServer.h>

WebServer webServer(80);

/**
 * @brief Affichage de la page d'accueil
 * 
 */
void handleRoot(){
    MYDEBUG_PRINTLN("Serveur => req root");
    JsonObject wifi = spiffsGet("config");
    String ap_ssid = wifi["ssid"];
    String ap_pwd = wifi["pwd"];

    String out = "";
    out += "<html><head><meta http-equiv='refresh' content='30'/>";
    out += "<title>Projet IoT - Root</title>";
    out += "<meta charset = 'utf-8'>";
    out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
    out += "</head><body>";
    out += "<h1>Bienvenue</h1><br>";
    out += "Depuis cette page, vous pouvez<br><ul>";
    out +="<li><a href=\"scan\"> Scanner le WiFi</a></li>";
    out +="<li><a href=\"ble\"> Scanner les Bluetooth</a></li>";
    out +="<li><a href=\"config\"> Configurer le WiFi</a></li></ul>";
    if(WiFi.status() == WL_CONNECTED){
    out += "<h2>Connecté à " + WiFi.SSID() + "</h>";
    }
    out += "<p>Nom réseau : " + String(ap_ssid) + "<br>Mot de passe : " + String(ap_pwd) + "</p>";
    out += "</body></html>";
    webServer.send(200, "text/html", out);
}

/**
 * @brief Setup du serveur
 * */
void setupWebServer(){
  MYDEBUG_PRINTLN("Serveur => Démarrage");

  //Configuration de mon serveur web
  webServer.on("/", handleRoot);

  //Démarrage du serveur
  webServer.begin();                                  
  MYDEBUG_PRINTLN("Serveur => Serveur Web démarré");
  MYDEBUG_PRINTLN();
}

/**
 * @brief Fonction pour utilisation du webServer
 * 
 */
void loopWebServer() {
    webServer.handleClient();
}