#!/bin/sh



# Vérification de l'existence des fichiers
if [ -e grapheData.png ] && [ -e histogramme.png ] ; then
  # Copier les deux fichiers
 cp grapheData.png histogramme.png ../
fi

