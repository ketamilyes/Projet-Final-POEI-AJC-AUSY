#!/bin/sh



# Vérification de l'existence de l'application
if [ -e grapheData ] && [ -e imageWeather ]; then
  # Lancer l'application 
 ./grapheData
 ./imageWeather
fi



