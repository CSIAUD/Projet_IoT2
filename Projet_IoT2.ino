/**
 * @file Projet_IoT2.ino
 * @page projet Projet IoT B2
 * @brief Projet IoT Contact-tracer
 * 
 */

#define DEBUG 1

#include <WebServer.h>

WebServer webServer(80);

#include "Debug.h"
#include "mySPIFFS.h"
#include "myWebServer.h"
#include "myWifi.h"

/**
 * \brief Initialisation des fonctionnalités
 * 
 */
void setup() {
  setupDebug();
  setupWifi();
  setupWebServer();
}

void loop() {
  loopWebServer();
  delay(5);
}
