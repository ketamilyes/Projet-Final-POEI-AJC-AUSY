#!/bin/sh



# Vérification de l'existence de l'application
if [ -e weather.db ]; then
  # Lancer l'application 
 rm -f weather.db
fi



