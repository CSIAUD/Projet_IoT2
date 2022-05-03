/**
 * \file Projet_IoT2.ino
 * \page projet Projet IoT B2
 * \brief Projet IoT Contact-tracer
 * 
 */

#define DEBUG 1

#include "Debug.h"
#include "mySPIFFS.h"

/**
 * \brief Initialisation des fonctionnalités
 * 
 */
void setup() {
  setupDebug();
  testSpiffs();
}

void loop() {
}
