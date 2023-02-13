#include <stdio.h>
#include <sqlite3.h>
#include <gdfonts.h>
#include <gd.h>
#include <time.h>

int main(int argc, char *argv[]) {
sqlite3 *db;
sqlite3_stmt *stmt;
int rc;
int i, n;
double temps[1000], temperature[1000], humidity[1000], pressure[1000], wind_speed[1000];
rc = sqlite3_open("weather.db", &db);
if (rc) {
    printf("Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
    return 1;
}

rc = sqlite3_prepare_v2(db, "SELECT temps, temperature, humidity, pressure, wind_speed FROM donnees_meteo", -1, &stmt, NULL);
if (rc) {
    printf("Impossible de préparer la requête: %s\n", sqlite3_errmsg(db));
    return 1;
}

n = 0;
while (sqlite3_step(stmt) == SQLITE_ROW) {
    temps[n] = sqlite3_column_double(stmt, 0);
    temperature[n] = sqlite3_column_double(stmt, 1);
    humidity[n] = sqlite3_column_double(stmt, 2);
    pressure[n] = sqlite3_column_double(stmt, 3) / 10;
    wind_speed[n] = sqlite3_column_double(stmt, 4);
    n++;
}

sqlite3_finalize(stmt);
sqlite3_close(db);

gdImagePtr im;
int BlackPearl,  GreenWhite, red, blue, RoyalBlue, GoldenDream, lightGray;
FILE *out;

im = gdImageCreate(670, 550);
BlackPearl = gdImageColorAllocate(im, 4, 16, 20);       // Font d'écran
GreenWhite = gdImageColorAllocate(im, 238, 238, 228);  // Couleur des axes
red = gdImageColorAllocate(im, 190, 77, 37);          // Températures
blue = gdImageColorAllocate(im, 37, 150, 190);       // humidité
GoldenDream = gdImageColorAllocate(im, 232, 236, 45);  // Pression 47, 154, 129
RoyalBlue = gdImageColorAllocate(im, 110, 49, 234); // Vent
lightGray = gdImageColorAllocate(im, 46, 44, 49); // Trame de fond

// Ajouter des graduations à intervalle de 5 sur l'axe x et à intervalle de 5 sur l'axe y
for (int j = 0; j <= 100; j+=5) {
    int x1, y1, x2, y2;
    x1 = 650;
    y1 = 460 - j * 4.4;
    x2 = 20 + 5;
    y2 = 460 - j * 4.4;
    gdImageLine(im, x1, y1, x2, y2, lightGray);
    char graduation[10];
    sprintf(graduation, "%d",j);
gdImageString(im, gdFontSmall, 0, y1 - 5, (unsigned char*) graduation, GreenWhite);
}

for (int j = 0; j <= 24; j++) {
    int x1, y1, x2, y2;
    x1 = 20 + j * 26;
    y1 = 460;
    x2 = 20 + j * 26;
    y2 = 0;
gdImageLine(im, x1, y1, x2, y2, lightGray);
char graduation[10];
sprintf(graduation, "%d", j);
gdImageString(im, gdFontSmall, x1 - 5, 470, (unsigned char*) graduation, GreenWhite);
}
// Création des axes

gdImageLine(im, 20, 20, 20, 460, GreenWhite);
gdImageLine(im, 20, 460, 650, 460, GreenWhite);

gdImageSetThickness(im, 3);// définir la largeur de la ligne à 2 pixels
for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + temps[i] * 26;
y1 = 460 - temperature[i] * 4.4;
if (i < n - 1) {
x2 = 20 + temps[i + 1] * 26;
y2 = 460 - temperature[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, red);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "T", red);
}

for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + temps[i] * 26;
y1 = 460 - humidity[i] * 4.4;
if (i < n - 1) {
x2 = 20 + temps[i + 1] * 26;
y2 = 460 - humidity[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, blue);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "H", blue);
}

for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + temps[i] * 26;
y1 = 460 - pressure[i] * 4.4;
if (i < n - 1) {
x2 = 20 + temps[i + 1] * 26;
y2 = 460 - pressure[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, GoldenDream);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "P", GoldenDream);
}

for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + temps[i] * 26;
y1 = 460 - wind_speed[i] * 4.4;
if (i < n - 1) {
x2 = 20 + temps[i + 1] * 26;
y2 = 460 - wind_speed[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, RoyalBlue);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "W", RoyalBlue);
}
// Dessiner les quatre graphes
for (i = 0; i < n-1; i++) {
    int x1, y1, x2, y2;
    x1 = 20 + i * 600 / n;
    y1 = 460 - temperature[i] * 440 / 100;
    x2 = 20 + (i + 1) * 600 / n;
    y2 = 460 - temperature[i + 1] * 440 / 100;
    gdImageLine(im, x1, y1, x2, y2, red);

x1 = 20 + i * 600 / n;
y1 = 460 - humidity[i] * 440 / 100;
x2 = 20 + (i + 1) * 600 / n;
y2 = 460 - humidity[i + 1] * 440 / 100;
gdImageLine(im, x1, y1, x2, y2, blue);

x1 = 20 + i * 600 / n;
y1 = 460 - pressure[i] * 440 / 100;
x2 = 20 + (i + 1) * 600 / n;
y2 = 460 - pressure[i + 1] * 440 / 100;
gdImageLine(im, x1, y1, x2, y2, GoldenDream);

x1 = 20 + i * 600 / n;
y1 = 460 - wind_speed[i] * 440 / 100;
x2 = 20 + (i + 1) * 600 / n;
y2 = 460 - wind_speed[i + 1] * 440 / 100;
gdImageLine(im, x1, y1, x2, y2, RoyalBlue);
}

gdFont *font = gdFontGetSmall(); 

// Ajouter une légende pour la température
char *temp_legend = "TEMPERATURE (C)";
gdImageString(im, font, 500, 400, temp_legend, red);

// Ajouter une légende pour l'humidité
char *humid_legend = "HUMIDITE (%)";
gdImageString(im, font, 500, 30, humid_legend, blue);

// Ajouter une légende pour la pression
char *pressure_legend = "PRESSION (hPa)";
gdImageString(im, font, 500, 10, pressure_legend, GoldenDream);

// Ajouter une légende pour la vitesse du vent
char *wind_legend = "VITESSE DU VENT (m/s)";
gdImageString(im, font, 355, 415, wind_legend, RoyalBlue);

// Ajouter une légende

gdImageString(im, gdFontGetSmall(), 200, 505, (unsigned char*) "SUIVI DES DIFFERENTS PARAMETRES DANS LE TEMPS.", GreenWhite);
gdImageString(im, gdFontGetSmall(), 250, 520, (unsigned char*) "GRAPHIQUES DU            A", GreenWhite);

// Ajouter le code pour afficher l'heure actuelle sur l'image
time_t rawtime;
struct tm * timeinfo;
char buffer[80];

time(&rawtime);
timeinfo = localtime(&rawtime);

strftime(buffer, 80, "%H:%M", timeinfo);
gdImageString(im, gdFontSmall, 410, 520, (unsigned char*) buffer, GreenWhite);

//Ajout de date

time_t now = time(0);
struct tm tstruct;
char buf[80];
tstruct = *localtime(&now);
strftime(buf, sizeof(buf), "%d/%m/%Y", &tstruct);

gdImageString(im, gdFontSmall, 333, 520, (unsigned char*) buf, GreenWhite);

out = fopen("grapheData.png", "wb");
if (out == NULL) {
printf("Impossible de créer l'image\n");
return 1;
}

gdImagePng(im, out);
fclose(out);

gdImageDestroy(im);

return 0;
}
