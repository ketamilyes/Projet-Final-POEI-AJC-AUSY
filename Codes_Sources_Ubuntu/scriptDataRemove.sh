#!/bin/sh



# Vérification de l'existence du fichier
if [ -e weather.db ]; then
  # Suppression du fichier weather.db 
 rm -f weather.db
fi



