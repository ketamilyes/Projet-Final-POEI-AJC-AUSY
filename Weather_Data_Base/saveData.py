#!/usr/bin/python3

import urllib3
import json
import sqlite3
import datetime

# Initialisation de la connexion à l'API OpenWeatherMap
http = urllib3.PoolManager()
url = 'http://api.openweathermap.org/data/2.5/weather?q=Paris,fr&appid=0d07d9ee6a76a3403f724c3cc8b9e623&units=metric&lang=fr'

# Récupération des données
response = http.request('GET', url)
data = json.loads(response.data.decode('utf-8'))

# Extraction des données requises

description = data['weather'][0]['description']
temperature = data['main']['temp']
humidity = data['main']['humidity']
pressure = data['main']['pressure']
wind_speed = data['wind']['speed']

# Connexion à la base de données SQLite3

conn = sqlite3.connect('weather.db')

# Récupérer le temps actuel

now = datetime.datetime.now()
temps_actuel = now.strftime("%Y-%m-%d %H:%M")

# Creation de la table Sqlite3 si elle n'est pas existante

c = conn.cursor()
c.execute("""CREATE TABLE IF NOT EXISTS donnees_meteo 
                 (temps text, description text, temperature real, humidity real, pressure real, wind_speed real)
              """)

# Stocker les données dans la base de données Sqlite3
c.execute("INSERT INTO donnees_meteo (temps, description, temperature, humidity, pressure, wind_speed) VALUES (?,?,?,?,?,?)", 
	(temps_actuel, description, temperature, humidity, pressure, wind_speed))

# Enregistrement des modifications
conn.commit()

# Fermeture de la connexion à la base de données
conn.close()

