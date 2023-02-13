#!/usr/bin/python3

"""
Fichier: grapheData.c
Descroption: Application en python qui intérroge une base de données météorologiques vi une API et qui stocke les données dans un fichier SQlite.  
Auteur: Lyes KETAMI
Version: 1.0
Date: 14/02/2023
"""


import requests
import sqlite3
import datetime


# Définition de la clée API, de la langue et les unités de mesure. 
CLE_API = "0d07d9ee6a76a3403f724c3cc8b9e623"
langue = "fr"
unites = "metric"

# Appel de l'API pour OpenWeatherMap 

URL = "http://api.openweathermap.org/data/2.5/weather?"

# Fonction pour recevoir les données de l'API et les stocker dans la base de donnée
def recuperer_donnees_meteo():
    
    
    # Récupérer l'heure actuelle
    """
       now : méthode qui qui utilise la classe "datetime".
       class: datetime Récupère  l'heure et la date actuelle.
       now.strftime("%Y-%m-%d %H:%M"): utilise la méthode "strftime" pour formater la date et l'heure renvoyées par datetime.datetime.now().
       temps_actuel : C'est une variable qui stocke le résultat final.
    """
    """
       url_complete : Concaténation de toutes les variables pour avoir l'adresse complète de la base de donnée.
       requests.get(url_complete): Envoie une requête GET à l'URL définie dans la variable "url_complete" en utilisant la méthode "get" du module "requests".
       response.json(): Stocke la réponse HTTP sous forme de dictionnaire Python en utilisant la méthode "json()" sur la réponse obtenue.
       La variable "donnees" contiendra ces informations sous forme de dictionnaire..
    """
    """
       donnees["weather"][0]["description"] : Utilise l'indexation pour accéder à la valeur associée à la clé "weather" dans le dictionnaire "donnees", 
       puis à la première entrée de cette liste (index 0) et enfin à la description associée à la clé "description"
       Les trois dernières lignes utilisent l'indexation pour accéder aux valeurs associées aux clés "temp", "humidity" et "pressure" dans le dictionnaire "main".
    """
    """
       sqlite3.connect("weather.db") : Connexion à la base de données "weather.db" avec le module SQlite3.
       conn.cursor(): Exécute une requêtes SQL pour récupérer les données. 
    """
    """
      c.execute : Utilise le module SQLite pour créer une table appelée "donnees_meteo" si elle n'existe pas déjà.
      La table "donnees_meteo" aura cinq colonnes.
    """
    """
      INSERT INTO : Permet d'insérer les données récupérée dans la table "donnees_meteo" en utilisant une instruction SQL préparée avec des valeurs de remplacement.
    """
    """
      conn.commit() : Signifie que toutes les modifications apportées à la base de données sont enregistrées et permanentes.
      conn.close() : signifie que la connexion à la base de données est fermée.
    """
    now = datetime.datetime.now()
    temps_actuel = now.strftime("%Y-%m-%d %H:%M")
    
    # Faire la requête depuis l'API
    
    nom_ville = "Paris,FR"
    url_complete = URL + "appid=" + CLE_API + "&q=" + nom_ville + "&units=" + unites + "&lang=" + langue
    response = requests.get(url_complete)
    donnees = response.json()
    
    # Extraire les données depuis la réponse de l'API
    
    description = donnees["weather"][0]["description"]
    temperature = donnees["main"]["temp"]
    humidity = donnees["main"]["humidity"]
    pressure = donnees["main"]["pressure"]
    wind_speed = donnees["wind"]["speed"]
    
    # Se connecter à la base de données

    conn = sqlite3.connect("weather.db")
    c = conn.cursor()
    
    # Creation de la table Sqlite3 si elle n'est pas existante
     
    c.execute("""CREATE TABLE IF NOT EXISTS donnees_meteo 
                 (temps text, description text, temperature real, humidity real, pressure real, wind_speed real)
              """)
    
    # Stocker les données dans la base de données Sqlite3
    
    c.execute("INSERT INTO donnees_meteo (temps, description, temperature, humidity, pressure, wind_speed) VALUES (?,?,?,?,?,?)", 
              (temps_actuel, description, temperature, humidity, pressure, wind_speed))
    
    # Valider les modifications et fermer la connexion
    
    conn.commit()
    conn.close()
    
# Fonction d'appel pour récupérer les données
recuperer_donnees_meteo()
