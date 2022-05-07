/**
 * @file myWebServer.h
 * @page server Serveur WEB
 * @brief Génération des pages + gestion des routes
 * 
 */

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
  out +="<li><a href=\"wifi\"> Scanner le WiFi</a></li>";
  out +="<li><a href=\"ble\"> Scanner les Bluetooth</a></li>";
  out +="<li><a href=\"config\"> Configuration manuelle du WiFi </a></li>";
  out +="<li><a href=\"reset\"> Rétablissement des paramètres d'usine </a></li></ul>";
  if(WiFi.status() == WL_CONNECTED){
    out += "<h2>Connecté à " + WiFi.SSID() + "</h>";
  }
  out += "<p>Nom réseau : " + String(ap_ssid) + "<br>Mot de passe : " + String(ap_pwd) + "</p>";
  out += "</body></html>";
  webServer.send(200, "text/html", out);
}

/**
 * @brief 
 * 
 */
void handleWifiScan(){
  MYDEBUG_PRINTLN("Serveur => req scan");

  int n = WiFi.scanNetworks();

  String out = "";
  out += "<html><head>";
  out += "<meta http-equiv='refresh' content='5'/>";
  out += "<title>Projet IoT - Scan</title>";
  out += "<meta charset = 'utf-8'>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  out += "</head><body>";
  out += "<h1>Page de scan</h1><br>";

  if(n == 0){
    MYDEBUG_PRINTLN("Aucun Wifi trouvé");
    out += "<h3>Aucun Wifi détecté</h3>";

  } else {
    out += "<ul>";
    for (int i = 0; i < n; ++i) {
      out += "<li><a href='/config?ssid="+ WiFi.SSID(i) +"'>"+ WiFi.SSID(i) +"</a></li>";
    }
    out += "</ul>";
  }
  out += "</body></html>";
  
  webServer.send(200, "text/html", out);
}

/**
 * @brief Lance la remise à zéro du SPIFFS
 * 
 */
void handleReset(){
  MYDEBUG_PRINTLN("Serveur => req RESET");
  reset();
}

/**
 * @brief Affichage et configuration du Wifi
 * */
void handleConfig() {
  MYDEBUG_PRINTLN("Serveur => req config");

  String out = "";
  out += "<html><head><meta http-equiv='refresh' content='30'/>";
  out += "<title>Projet IoT - Config</title>";
  out += "<meta charset = 'utf-8'>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  out += "</head><body>";
  out += "<h1>Page de config</h1>";
  out += "<form action='/connect' style='display: flex; flex-direction: column; width: 150px;'>";
  out += "<input name='ssid' type='text' placeholder='SSID' value='";
  if(webServer.arg("ssid") != ""){
    out += webServer.arg("ssid");
  }
  out += "'>";
  out += "<input name='pwd' type='text' placeholder='Mot de Passe'>";
  out += "<input value='Connexion' type='submit'>";
  out += "</form>";
  
  if(webServer.arg("connected") == "false"){
    out += "<h1 style='color: red;'>Connexion échouée</h1>";
  }
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
  webServer.on("/reset", handleReset);
  webServer.on("/wifi", handleWifiScan);
  webServer.on("/config", handleConfig);

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