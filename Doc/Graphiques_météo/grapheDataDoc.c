/**
 * @file grapheData.c
 * @brief Application en C qui crée des graphiques à partir de données météorologiques stockées dans une base de données SQLite appelée weather.db.  
 * @author Lyes KETAMI
 * @version 1.0
 * @date 14/02/2023
 */
#include <stdio.h>
#include <sqlite3.h>
#include <gdfonts.h>
#include <gd.h>

/**
 * @brief Fonction principale du programme.
 *
 * **sqlite *db** C'est une variable qui représente une instance de la bibliothèque SQLite3.  
 * **sqlite3_stmt *stmt** C'est une autre variable qui représente une requête SQlite.
 */

int main(int argc, char *argv[]) {
 *sqlite3 *db;
 *sqlite3_stmt *stmt;**


/**
 * @brief C'est une procédure qui permet d'accéder à une base de données nommée "weather.db".
 * Elle affiche un messsage d'erreur si la base de données n'a pas pu être ouverte ou si la requête a échoué. 
 * **int rc** déclare une variable **rc** de type entier qui sera utilisée pour stocker le code de retour d'une fonction appelée plus tard.
 * **int i, n** déclare deux variables **i** et **n** de type entier qui seront utilisées pour les boucles et les opérations sur les tableaux.
 * **double time[1000], temperature[1000], humidity[1000], pressure[1000], wind_speed[1000]** Déclare cinq tableaux de type flottant.
 * **rc = sqlite3_open("weather.db", &db)** appelle la fonction **sqlite3_open** pour ouvrir la base de données et stocker le résultat dans la variable **rc**
 * @return code 1  termine le programme en renvoyant un code de retour de 1, indiquant une erreur.
 */
 
int rc;
int i, n;
double time[1000], temperature[1000], humidity[1000], pressure[1000], wind_speed[1000];
rc = sqlite3_open("weather.db", &db);
if (rc) {
    printf("Impossible d'ouvrir la base de données: %s\n", sqlite3_errmsg(db));
    return 1;
}

rc = sqlite3_prepare_v2(db, "SELECT time, temperature, humidity, pressure, wind_speed FROM donnees_meteo", -1, &stmt, NULL);
if (rc) {
    printf("Impossible de préparer la requête: %s\n", sqlite3_errmsg(db));
    return 1;
}

/**
 * @brief Boucle qui récupère les lignes de la base de données pour les stocker dans des tableaux de paramètres récupérés.
 *  **sqlite3_step(stmt)** La boucle s'exécute tant que la fonction **sqlite3_step** renvoie la constante **SQLITE_ROW**
 *  **qlite3_column_double(stmt)** Sert à récupérer les valeurs des différentes colonnes pour les stocker dans les tableaux\n
 * **n++** Incrémente la variable n de 1 pour refléter l'ajout d'une nouvelle ligne de données dans les tableaux\n
 * @details Pour une raison de confort visuel sur les graphes, j'ai utilisé l'unité **hPa** plutôt que **kPa** fournie par l'API\n
 * (1 kilo Pascal = 10 hecto Pascal) d'où la division de pressure par 10 avant de l'insérer dans le tableau. 
 * **sqlite3_finalize(stmt)** Libérer les ressources de la requête pour éviter les fuites de mémoire.
 * **sqlite3_close(db)** Ferme la base de donnée pour éviter les conflits de connexion et garantir la sécurité de la base de données.  
 */
n = 0;
while (sqlite3_step(stmt) == SQLITE_ROW) {
    time[n] = sqlite3_column_double(stmt, 0);
    temperature[n] = sqlite3_column_double(stmt, 1);
    humidity[n] = sqlite3_column_double(stmt, 2);
    pressure[n] = sqlite3_column_double(stmt, 3) / 10;
    wind_speed[n] = sqlite3_column_double(stmt, 4);
    n++;
}

sqlite3_finalize(stmt);
sqlite3_close(db);

/**
 * @brief Fonction qui crée une nouvelle image de taille 670x550 pixels.
 * **gdImageCreate** Permet de créer l'image de 670x550.
 * @details **gdImageColorAllocate** Permet l'allocation des différentes couleurs pour l'image, les graphes, le fond d'écran,...etc.  
 */
 
// initialisation de l'image et allocation des couleurs
gdImagePtr im;
  *int BlackPearl,  GreenWhite, red, blue, RoyalBlue, SeaGreen, lightGray;
  *FILE *out;

  *im = gdImageCreate(670, 550);
  *BlackPearl = gdImageColorAllocate(im, 4, 16, 20);       // Font d'écran
  *GreenWhite = gdImageColorAllocate(im, 238, 238, 228);  // Couleur des axes
  *red = gdImageColorAllocate(im, 190, 77, 37);          // Températures
  *blue = gdImageColorAllocate(im, 37, 150, 190);       // humidité
  *SeaGreen = gdImageColorAllocate(im, 42, 148, 62);  // Pression 47, 154, 129
  *RoyalBlue = gdImageColorAllocate(im, 110, 49, 234); // Vent
  *lightGray = gdImageColorAllocate(im, 46, 44, 49); // Trame de fond
  
/**
 * @brief Boucle qui crée des valeurs de graduations, des étiquettes et une trame de fond pour les graphes.
 * Les coordonnées de deux points **(x1, y1)** et **(x2, y2)** sont calculées en utilisant la valeur de **j**.
 * La fonction **gdImageLine** permet ensuite de connecter ces points.
 * La valeur de **j** est convertie en chaîne de caractères à l'aide de la fonction **sprintf** \n 
 * La couleur de la chaîne est définie sur **GreenWhite**.
 * **gdImageString** et **gdFontSmall** sont des foctions qui permettent de créer et de manipuler les images.
 */
 
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

/**
 * @brief Création des axes pour les graphes.
 * **gdImageLine** est une fonction de la **libgd** qui permet de tracer des lignes.
 */
 
// Création des axes

gdImageLine(im, 20, 20, 20, 460, GreenWhite);
gdImageLine(im, 20, 460, 650, 460, GreenWhite);

/**
 * @brief Insertion des courbes dans l'image.
 * **gdImageSetThickness** est une fonction de la **libgd** qui permet de définir l'épaisseur des lignes des graphes.
 * La boucle **for** parcourt un tableau d'entiers nommé **time** et un autre tableau d'entiers nommé **temperature**.
 * Elle utilise la bibliothèque **gd** pour dessiner une ligne entre des points (x1, y1) et (x2, y2) à chaque itération.
 * @details La boucle utilise un compteur "i" qui commence à zéro et se termine lorsque "i" est égal à "n". À chaque itération, les variables x1, y1, x2, y2 sont définies en utilisant les valeurs de "time" et "temperature". La fonction "gdImageLine" dessine une ligne entre les points **(x1, y1)** et **(x2, y2)** en utilisant la couleur "red". Si "i" est inférieur à "n-1", ce qui signifie qu'il reste encore des points à traiter, alors les valeurs de x2 et y2 sont déterminées pour la prochaine itération de la boucle. Enfin, la fonction "gdImageString" affiche la lettre "T" en utilisant la police "gdFontSmall" près du point **(x1, y1)** en utilisant la couleur "red".
 * Cela se répète pour les différents paramètres **d'humidité**, de **pression atmosphérique**, et de la **vitesse du vent**.
 */
gdImageSetThickness(im, 3);// définir la largeur de la ligne à 2 pixels
for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + time[i] * 26;
y1 = 460 - temperature[i] * 4.4;
if (i < n - 1) {
x2 = 20 + time[i + 1] * 26;
y2 = 460 - temperature[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, red);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "T", red);
}

for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + time[i] * 26;
y1 = 460 - humidity[i] * 4.4;
if (i < n - 1) {
x2 = 20 + time[i + 1] * 26;
y2 = 460 - humidity[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, blue);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "H", blue);
}

for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + time[i] * 26;
y1 = 460 - pressure[i] * 4.4;
if (i < n - 1) {
x2 = 20 + time[i + 1] * 26;
y2 = 460 - pressure[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, SeaGreen);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "P", SeaGreen);
}

for (i = 0; i < n; i++) {
int x1, y1, x2, y2;
x1 = 20 + time[i] * 26;
y1 = 460 - wind_speed[i] * 4.4;
if (i < n - 1) {
x2 = 20 + time[i + 1] * 26;
y2 = 460 - wind_speed[i + 1] * 4.4;
gdImageLine(im, x1, y1, x2, y2, RoyalBlue);
}
gdImageString(im, gdFontSmall, x1 - 5, y1 - 5, (unsigned char*) "W", RoyalBlue);
}

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
gdImageLine(im, x1, y1, x2, y2, SeaGreen);

x1 = 20 + i * 600 / n;
y1 = 460 - wind_speed[i] * 440 / 100;
x2 = 20 + (i + 1) * 600 / n;
y2 = 460 - wind_speed[i + 1] * 440 / 100;
gdImageLine(im, x1, y1, x2, y2, RoyalBlue);
}

/**
 * @brief Ajouter une légende pour chaque paramètre **température**, **humidité**, **pression atmosphérique** et **vitesse du vent**.
 * **gdFont *font = gdFontGetSmall()** détermine la police utilisée.
 * **gdImageString** permet d'inscrire le texte dans l'image.
 * @details La première ligne déclare une variable de pointeur de caractère nommée **temp_legend**. Elle est affectée à la chaîne de caractères **TEMPERATURE (°C)**.
 * La deuxième ligne appelle une fonction appelée **gdImageString**. Cette fonction permet de dessiner du texte sur une image en utilisant la bibliothèque GD.
 * Les arguments de cette fonction sont **im** est une image précédemment créée avec la bibliothèque GD.
 * **font** est une police de caractères définie précédemment dans le programme.
 * "500" et "400" sont les coordonnées **X** et **Y** respectivement où le texte sera dessiné sur l'image.
 * **temp_legend** est la variable de pointeur de caractères qui contient la chaîne de caractères à dessiner.
 * **red** est la couleur dans laquelle le texte sera dessiné, définie précédemment dans le programme.
 * Le résultat final sera un texte **TEMPERATURE (°C)** dessiné sur l'image à partir des coordonnées 500, 400 et en rouge.
 * La même procédure est définie pour les différents paramètres.
 */

// Ajouter une légende pour la température

gdFont *font = gdFontGetSmall(); 

char *temp_legend = "TEMPERATURE (°C)";
gdImageString(im, font, 500, 400, temp_legend, red);

// Ajouter une légende pour l'humidité
char *humid_legend = "HUMIDITE (%)";
gdImageString(im, font, 500, 40, humid_legend, blue);

// Ajouter une légende pour la pression
char *pressure_legend = "PRESSION (hPa)/Time";
gdImageString(im, font, 500, 10, pressure_legend, SeaGreen);

// Ajouter une légende pour la vitesse du vent
char *wind_legend = "VITESSE DU VENT (m/s)";
gdImageString(im, font, 500, 445, wind_legend, RoyalBlue);

/**
 * @brief Création d'une légende pour les graphes générés.
 * **gdImageString** est une fonction de la **libgd** qui permet d'insérer du texte.
 */
 
// Ajouter une légende

gdImageString(im, gdFontGetSmall(), 200, 500, (unsigned char*) "SUIVI DES DIFFERENTS PARAMETRES DANS LE TEMPS", GreenWhite);
gdImageString(im, gdFontGetSmall(), 250, 520, (unsigned char*) "GRAPHIQUES CONSTRUITS A:", GreenWhite);

// Ajouter le code pour afficher l'heure actuelle sur l'image
time_t rawtime;
struct tm * timeinfo;
char buffer[80];

time(&rawtime);
timeinfo = localtime(&rawtime);

strftime(buffer, 80, "%H:%M", timeinfo);
gdImageString(im, gdFontSmall, 400, 520, (unsigned char*) buffer, GreenWhite);


/**
 * @brief Enregistrement de l'image et libératuion de la mémoire. 
 * **out** est un pointeur de fichier déclaré pour stocker la sortie de l'image PNG. **fopen("grapheData.png", "wb")** ouvre un fichier nommé "grapheData.png" en mode **wb**   (écriture binaire).
**if (out == NULL)** vérifie si le fichier **grapheData.png** peut être ouvert. Si ce n'est pas le cas, le message "Impossible de créer l'image" est affiché et la fonction retourne 1.
**gdImagePng(im, out)** utilise la fonction gdImagePng pour enregistrer l'image "im" dans le fichier "out".
**fclose(out)" ferme le fichier **out**.
**gdImageDestroy(im)** détruit l'image "im" en libérant la mémoire allouée pour cette image.
 * @return 0.
 */
 
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
