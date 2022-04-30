/**
 * @file Projet_IoT2.ino
 * @page projet Projet IoT B2
 * @brief Projet IoT Contact-tracer
 * 
 */

#include "Debug.h"
#include "mySPIFFS.h"
#include "myWifi.h"

/**
 * \brief Initialisation des fonctionnalités
 * 
 */
void setup() {
  setupDebug();
  setupWifi();
  display(spiffsGet("wifis"));
}

void loop() {
}
