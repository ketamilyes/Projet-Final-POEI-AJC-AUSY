#include <stdio.h>
#include <sqlite3.h>
#include <gd.h>
#include <gdfontt.h>
#include <time.h>

int main(int argc, char** argv) {
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char query[150];
sqlite3_stmt *stmt;

// Ouvrir la base de données SQlite3
rc = sqlite3_open("weather.db", &db);
if( rc ) {
fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
sqlite3_close(db);
return 1;
}

// Interroger la base de données et sélectionner le contenu de la dernière ligne de la table
sprintf(query, "SELECT description, temperature, humidity, pressure, wind_speed, temps FROM donnees_meteo ORDER BY temps DESC LIMIT 1");
rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
if (rc != SQLITE_OK) {
fprintf(stderr, "Erreur d'ouverture de la base de données: %s\n", zErrMsg);
sqlite3_free(zErrMsg);
sqlite3_close(db);
return 1;
}
rc = sqlite3_step(stmt);
if (rc != SQLITE_ROW) {
fprintf(stderr, "Aucune donnée trouvée\n");
sqlite3_close(db);
return 1;
}

// Récupération des données de la base de données
const char* description = (const char*)sqlite3_column_text(stmt, 0);
double temperature = sqlite3_column_double(stmt, 1);
double humidity = sqlite3_column_double(stmt, 2);
double pressure = sqlite3_column_double(stmt, 3)/10; //diviser la pression par 10
double wind_speed = sqlite3_column_double(stmt, 4);
int temps = sqlite3_column_int(stmt, 5);

// Initialisation de l'image
gdImagePtr im = gdImageCreate(550, 480);
int BlackPearl = gdImageColorAllocate(im, 4, 16, 20);
int GreenWhite = gdImageColorAllocate(im, 238, 238, 228);
int red = gdImageColorAllocate(im, 190, 77, 37);
int blue = gdImageColorAllocate(im, 37, 150, 190);
int GoldenDream = gdImageColorAllocate(im, 232, 236, 45);
int Lochinvar = gdImageColorAllocate(im, 47, 154, 129);
int gray_tram = gdImageColorAllocate(im, 30, 30, 28);

//Ajouter la graduation à intervalle de 5 pour l'axe y
int y = 400;
for (int i = 0; i <= 150; i += 5) {
gdImageLine(im, 45, y, 500, y, gray_tram);
char graduation[5];
sprintf(graduation, "%d", i);

gdImageLine(im, 50, 600, 450, 450, BlackPearl); // Couleur du fond
gdImageLine(im, 50, 400, 400, 400, GreenWhite); // axe x

gdImageString(im, gdFontGetTiny(), 5, y - 5, (unsigned char*)graduation, BlackPearl);
y -= 5;
}

// Dessiner les histogrammes
gdImageFilledRectangle(im, 100, 400-(temperature), 150, 400, red);
gdImageFilledRectangle(im, 200, 400-(humidity), 250, 400, blue);
gdImageFilledRectangle(im, 300, 400-(pressure),350, 400, GoldenDream);
gdImageFilledRectangle(im, 400, 400-(wind_speed), 450, 400, Lochinvar);

// Ajouter des étiquettes pour les données

gdImageString(im, gdFontGetTiny(), 100, 410, (unsigned char*) "TEMPERATURE", red);
gdImageString(im, gdFontGetTiny(), 200, 410, (unsigned char*) "HUMIDITE", blue);
gdImageString(im, gdFontGetTiny(), 300, 410, (unsigned char*) "PRESSION", GoldenDream);
gdImageString(im, gdFontGetTiny(), 400, 410, (unsigned char*) "VITESSE DU VENT", Lochinvar);

// Ajouter les valeurs pour les données
char temp[15];
sprintf(temp, "%.2f(C)", temperature);
gdImageString(im, gdFontGetTiny(), 105, 390-(temperature), (unsigned char*)temp, GreenWhite);

char hum[15];
sprintf(hum, "%.2f%%", humidity);
gdImageString(im, gdFontGetTiny(), 210, 415-(humidity), (unsigned char*)hum, GreenWhite);

char press[15];
sprintf(press, "%.2fhPa", pressure);
gdImageString(im, gdFontGetTiny(), 305, 450-(pressure), (unsigned char*)press, BlackPearl);

char wind[15];
sprintf(wind, "%.2fm/s", wind_speed);
gdImageString(im, gdFontGetTiny(), 405, 390-(wind_speed), (unsigned char*)wind, GreenWhite);

// Ajouter une légende
gdImageString(im, gdFontGetTiny(), 200, 440, (unsigned char*) "DERNIER JEU DE DONNEES RECUPERE", GreenWhite);
gdImageString(im, gdFontGetTiny(), 200, 450, (unsigned char*) "HISTOGRAMMES DU           A", GreenWhite);

// Ajouter l'heure actuelle sur l'image
time_t now = time(0);
struct tm *time_struct = localtime(&now);
char str_time[20];
strftime(str_time, sizeof(str_time), "%H:%M", (const struct tm *) time_struct);
gdImageString(im, gdFontGetTiny(), 338, 450, (unsigned char *) str_time, GreenWhite);

 // Obtenir la date actuelle
  time_t current_time;
  time(&current_time);
  struct tm *local_time = localtime(&current_time);
  
  // Formater la date
  char date_string[20];
  strftime(date_string, sizeof(date_string), "%d/%m/%Y", local_time);
  
  // Dessiner la date sur l'image
  gdImageString(im, gdFontGetTiny(), 277, 450, (unsigned char*)date_string, GreenWhite);


// Sauvegarder l'image dans un fichier PNG
FILE *pngout = fopen("histogramme.png", "wb");
gdImagePng(im, pngout);
fclose(pngout);
gdImageDestroy(im);

// Fermer la base de données
sqlite3_finalize(stmt);
sqlite3_close(db);

return 0;
}
