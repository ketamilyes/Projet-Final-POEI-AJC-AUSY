/**
 * @file imageWeather.c
 * @brief Application en C qui crée des histogrammes à partir du dernier jeu de données météorologiques stockées dans une base de données SQLite appelée weather.db.  
 * @author Lyes KETAMI
 * @version 1.0
 * @date 14/02/2023
 */
#include <stdio.h>
#include <sqlite3.h>
#include <gd.h>
#include <gdfontt.h>


/**
 * @brief Fonction principale du programme.
 * **sqlite *db** C'est une variable qui représente une instance de la bibliothèque SQLite3.  
 * **char *zErrMsg** C'est une variable qui enregistre les erreurs qui se produisent lors de l'exécution de requêtes SQL.
 * **int rc** C'est une variable pour stocker le code de retour d'une opération SQLite. 
 * **char query[150]** C'est un tableau de caractères qui stocke la requête SQL à exécuter.
 * **sqlite3_stmt *stmt** C'est une variable qui représente une requête SQlite.
 */


/**
 *int main(int argc, char** argv) {
 *sqlite3 *db;
 *char *zErrMsg = 0;
 *int rc;
 *char query[150];
 *sqlite3_stmt *stmt;
 */

/**
 * @brief C'est une procédure qui permet d'accéder à une base de données nommée "weather.db".
 * Elle récupère le dernier jeu de données météorologiques récupéré. 
 * Elle affiche un messsage d'erreur si la base de données n'a pas pu être ouverte ou si la requête a échoué. 
 * **sqlite3_prepare_v2** Fonction qui prépare la requête et vérifie si elle s'exécute correctement.
 * **sqlite3_free(zErrMsg)** Cette instruction libère la mémoire qui a été allouée pour la variable "zErrMsg"
 * **sqlite3_close(db)** ferme la connexion à la base de données SQLite spécifiée par la variable "db".
 * @return code 1  termine le programme en renvoyant un code de retour de 1, si il y a une erreur.
 */

// Ouvrir la base de données SQlite3
rc = sqlite3_open("weather.db", &db);
if( rc ) {
fprintf(stderr, "Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
sqlite3_close(db);
return 1;
}

// Interroger la base de données et sélectionner le contenu de la dernière ligne de la table
sprintf(query, "SELECT description, temperature, humidity, pressure, wind_speed, time FROM donnees_meteo ORDER BY time DESC LIMIT 1");
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

/**
 * @brief Procédure qui récupère et les varibales des paramètres **time, temperature, humidity, pressure, wind-speed**.
 *  **sqlite3_step(stmt)** La boucle s'exécute tant que la fonction **sqlite3_step** renvoie la constante **SQLITE_ROW**
 *  **qlite3_column_double(stmt)** Sert à récupérer les valeurs des différentes colonnes pour les stocker dans des variables.
 * @details Pour une raison de confort visuel sur les graphes, j'ai utilisé l'unité **hPa** plutôt que **kPa** fournie par l'API\n
 * (1 kilo Pascal = 10 hecto Pascal) d'où la division de pressure par 10 avant de stocker la variable **pressure**.  
 */
 
// Récupération des données de la base de données
const char* description = (const char*)sqlite3_column_text(stmt, 0);
double temperature = sqlite3_column_double(stmt, 1);
double humidity = sqlite3_column_double(stmt, 2);
double pressure = sqlite3_column_double(stmt, 3)/10; //diviser la pression par 10
double wind_speed = sqlite3_column_double(stmt, 4);
int time = sqlite3_column_int(stmt, 5);

/**
 * @brief Fonction qui crée une nouvelle image de taille 550x480 pixels.
 * **gdImageCreate** Permet de créer l'image de 550x480.
 * @details **gdImageColorAllocate** Permet l'allocation des différentes couleurs pour l'image, les histogrames, le fond d'écran,...etc.  
 */
 
// Initialisation de l'image
gdImagePtr im = gdImageCreate(550, 480);
int BlackPearl = gdImageColorAllocate(im, 4, 16, 20);
int GreenWhite = gdImageColorAllocate(im, 238, 238, 228);
int red = gdImageColorAllocate(im, 190, 77, 37);
int blue = gdImageColorAllocate(im, 37, 150, 190);
int purple = gdImageColorAllocate(im, 108, 37, 190);
int Lochinvar = gdImageColorAllocate(im, 47, 154, 129);
int gray_tram = gdImageColorAllocate(im, 30, 30, 28);

/**
 * @brief Boucle qui crée des valeurs de graduations, des étiquettes et une trame de fond pour les histogrammes.
 * **la boucle for** s'exécute 150 fois en incrémentant la variable i de 5 afin de dessiner la trame de fond.
 * La fonction **gdImageLine** permet ensuite de dessiner les lignes de la tame de fond.
 * **sprintf** formate la valeur de "i" en tant que chaîne de caractères et la stocke dans une variable appelée "graduation".
 * **gdImageLine** permet aussi de dessiner l'axe x.
 * **gdImageString** et **gdFontGetTiny()** permet d'écrire la chaine de caractère sur l'image.
 */

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

/**
 * @brief Ces procédures permettent de dessiner les réctangles des histogrammes .
 * **lgdImageFilledRectangle** permet de créer un réctangle dans les positions indiquées et remplir les réctangles avec la couleur appropriée.
 */
 
// Dessiner les histogrammes
gdImageFilledRectangle(im, 100, 400-(temperature), 150, 400, red);
gdImageFilledRectangle(im, 200, 400-(humidity), 250, 400, blue);
gdImageFilledRectangle(im, 300, 400-(pressure),350, 400, purple);
gdImageFilledRectangle(im, 400, 400-(wind_speed), 450, 400, Lochinvar);

/**
 * @brief Ces procédures permettent d'insérer du texte à côté des histogrammes .
 * **gdImageString** permet d'insérer du texte
 */

// Ajouter des étiquettes pour les données

gdImageString(im, gdFontGetTiny(), 100, 410, (unsigned char*) "TEMPERATURE", red);
gdImageString(im, gdFontGetTiny(), 200, 410, (unsigned char*) "HUMIDITE", blue);
gdImageString(im, gdFontGetTiny(), 300, 410, (unsigned char*) "PRESSION", purple);
gdImageString(im, gdFontGetTiny(), 400, 410, (unsigned char*) "VITESSE DU VENT", Lochinvar);

/**
 * @brief Ces fonctions permettent d'insérer les valeurs des paramètre récupérées dans les histogrammes.
 * **char temp[15]** Tableau de caractères "temp" avec une taille de 15 éléments.
 * **sprintf** formate la valeur de la variable "temperature" en une chaîne de caractères stockée dans "temp". 
 * La valeur est formatée avec seulement 2 décimales et le symbole de degré Celsius "°C" est ajouté à la fin.
 * **gdImageString** dessine le txte stocké dans "temp". Elle prend les paramètres de l'objet de l'image "im", de position, de la police et de couleur.
 */

// Ajouter les valeurs pour les données
char temp[15];
sprintf(temp, "%.2f°C", temperature);
gdImageString(im, gdFontGetTiny(), 105, 400-(temperature), (unsigned char*)temp, GreenWhite);

char hum[15];
sprintf(hum, "%.2f%%", humidity);
gdImageString(im, gdFontGetTiny(), 210, 415-(humidity), (unsigned char*)hum, GreenWhite);

char press[15];
sprintf(press, "%.2fhPa", pressure);
gdImageString(im, gdFontGetTiny(), 305, 450-(pressure), (unsigned char*)press, GreenWhite);

char wind[15];
sprintf(wind, "%.2fm/s", wind_speed);
gdImageString(im, gdFontGetTiny(), 405, 390-(wind_speed), (unsigned char*)wind, GreenWhite);

// Ajouter une légende

gdImageString(im, gdFontGetTiny(), 200, 450, (unsigned char*) "DERNIER JEU DE DONNEES RECUPERE", GreenWhite);
gdImageString(im, gdFontGetTiny(), 200, 450, (unsigned char*) "HISTOGRAMMES CONSTRUITS A", GreenWhite);

// Ajouter l'heure actuelle sur l'image
time_t now = time(0);
struct tm *time_struct = localtime(&now);
char str_time[20];
strftime(str_time, sizeof(str_time), "%H:%M", (const struct tm *) time_struct);
gdImageString(im, gdFontGetTiny(), 330, 450, (unsigned char *) str_time, GreenWhite);

/**
 * @brief Enregistrement de l'image et libératuion de la mémoire. 
 * **out** est un pointeur de fichier déclaré pour stocker la sortie de l'image PNG. **fopen("grapheData.png", "wb")** ouvre un fichier nommé "grapheData.png" en mode **wb**   (écriture binaire).
**if (out == NULL)** vérifie si le fichier **grapheData.png** peut être ouvert. Si ce n'est pas le cas, le message "Impossible de créer l'image" est affiché et la fonction retourne 1.
**gdImagePng(im, out)** utilise la fonction gdImagePng pour enregistrer l'image "im" dans le fichier "out".
**fclose(out)" ferme le fichier **out**.
**gdImageDestroy(im)** détruit l'image "im" en libérant la mémoire allouée pour cette image.
 * @return 0.
 */

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
