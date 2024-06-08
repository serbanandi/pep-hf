/**
 * @file main.c
 * @brief Ez a fájl a BANANA TREE játék Linux oldali kódját tartalmazza.
 *
 * Az applikáció soros porton keresztül kommunikál a mikrovezérlővel.
 *    A mikrokontroller 's' karaktert küld, ha a játék elindul.
 *    A játékos ekkor a nevét adhatja meg, majd elkezdheti beállítani a nehézséget.
 *       A nehézség beállítása a 'W' és 'S' karakterekkel, valamint a számokkal (1-8) lehetséges.
 *    A játék indtására a 'Space' gombbal van lehetőség.
 *    A játék során a játékos a 'W' és 'D' gombokkal tudja irányítani a kosarat.
 *       A játék 25 banánt dob ki, amiket a játékosnak el kell kapnia, ezután véget ér.
 *    A játék végeztével a mikrokontroller 'e' karakterrel kezdődő üzentet küld, amely tartalmazza a játékos pontszámát és a beállított nehézséget.
 *    A játékos a pontszámát és a ranglistát megtekintheti, majd eldöntheti, hogy menti-e a pontszámát.
 * 
 * Az applikációból kilépésre a 'q' vagy 'x' karakterekkel van lehetőség.
 */

#include <stdio.h>      // standard I/O
#include <stdlib.h>     // standard library
#include <stdbool.h>    // bool típus
#include <unistd.h>     // UNIX standard függvények
#include <fcntl.h>      // file control
#include <termios.h>    // UART kezeles
#include <string.h>     // string kezeles miatt kell
#include <poll.h>       // poll() miatt kell

#define BUFLEN (32)                 // buffer hossza
#define PFDSLEN (2)                 // poll file descriptorok száma
#define MAX_SCORES (1000)           // maximum tárolható pontszámok száma
#define MAX_NAME_LENGTH (BUFLEN-1)  // maximum játékosnév hossza


typedef enum eGameState { // játék állapotai
   initGame,   // kezdeti állapot - még nem érkezett start jel
   startGame,  // játék kezdete - játékos nevének bekérése
   playGame    // játék folyamatban - játékos irányítása 'e' jel érkezéséig, majd pontszám kiírása és mentési lehetőség
} eGameState;

typedef struct Player { // játékosok adatai (pontszám kiírásához)
    char name[MAX_NAME_LENGTH];  
    int score;
} Player;

/**
 * @brief Két játékos összehasonlítása pontszám alapján. (qsort használja)
 * @param a pointer az első játékosra.
 * @param b pointer a második játékosra.
 * @return Negatív szám, ha a pontszám a második játékosnál nagyobb, pozitív szám, ha az első játékosnál nagyobb, 0, ha egyenlő.
 */
int compare(const void * a, const void * b) {
   Player *playerA = (Player *)a;
   Player *playerB = (Player *)b;
   return (playerB->score - playerA->score);
}

/**
 * @brief Beállítja a terminál attribútumokat, hogy kikapcsolja a kanonikus módot és az echo-t.
 * @return void
 */
void set_input_mode() {
   struct termios tattr;
   tcgetattr(STDIN_FILENO, &tattr);
   tattr.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

/**
 * @brief Visszaállítja az alapértelmezett terminál attribútumokat: bekapcsolja a kanonikus módot és az echo-t.
 * @return void
 */
void reset_input_mode() {
    struct termios tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

/**
 * @brief Beállítja a soros portot: 115200 baud, 8 bites keretméret, nincs paritás, 1 stop bit.
 * @param tty_fd a soros port file descriptora.
 * @return void
*/
void setup_tty(int tty_fd) {
    struct termios serial;
    memset(&serial, 0, sizeof(struct termios));

    serial.c_cflag = CS8 | CREAD | CLOCAL; // 8-bites keretméret, vétel engedélyezése, modem control tiltása
    serial.c_cc[VMIN] = 1; // karakterenkénti olvasás engedélyezése
    serial.c_cc[VTIME] = 5; // nem-kanonikus olvasás időlimitje tizedmásodpercben
    cfsetospeed(&serial, B115200); // adó sebességének beállítása
    cfsetispeed(&serial, B115200); // vevő sebességének beállítása

    // beállítások alkalmazása
    tcsetattr(tty_fd, TCSANOW, &serial);
}


/**
 * @brief A játékos pontszámának és nevének megjelnítése a ranglistán, valamint a pontszám mentésének lehetősége.
 * @param player_name a játékos neve.
 * @param score a játékos pontszáma.
 * @param difficulty a játék nehézsége.
 * @return void
 */
void record_score(char* player_name, int score, int difficulty){
   char answer;   // a játékos válasza a pontszám mentésére
   FILE* file;    // a pontszámokat tartalmazó fájl
   Player players[MAX_SCORES]; // a pontszámokat tartalmazó tömb
   int prev_highscore = 0;       // a játékos legmagasabb pontszáma
   int i = 0;

   printf("Player name: %s\tScore: %d\n", player_name, score*difficulty);

   file = fopen("scores.txt", "r"); // pontszámok fájljának megnyitása a ranglista megjelenítéséhez
   if (file == NULL) {
      read(STDERR_FILENO, "Error opening file\n", 19);
      return;
   }
   
   
   // a fájlban tárolt játékosok beolvasása
   while (fscanf(file, "%s\t%d", players[i].name, &players[i].score) == 2) {
      if(strcmp(player_name, players[i].name) == 0) {
         prev_highscore = players[i].score; // a játékos eddigi legmagasabb pontszámának meghatározása
         players[i].score = score*difficulty; // játékos pontszámának frissítése
      }
      i++;
   }
   if(prev_highscore == 0){
       prev_highscore = score*difficulty; // ha a játékos még nem szerepelt a ranglistán, akkor a jelenlegi pontszám lesz a legmagasabb
      //jelenlegi játékos felvétele a listára
      players[i].score = score*difficulty;
      strcpy(players[i].name, player_name);
      i++;
   }
   fclose(file);

   int num_players = i;
   qsort(players, num_players, sizeof(Player), compare); // rangsorolás

   //ranglista megjelenítése
   printf("High scores:\n");
   printf("Rank\tName\tScore\n");
   for (i = 0; i < num_players; i++) {
      if(strcmp(player_name, players[i].name) == 0)
         printf("%d\t%s\t%d **** YOU NOW\n", i+1, players[i].name,  players[i].score);
      else printf("%d\t%s\t%d\n", i+1, players[i].name, players[i].score);
   }
   
   // pontszám mentésének lehetősége
   if (prev_highscore <= score*difficulty){
      printf("High score!\n");
      printf("Save score? (y/n)\n");
      read(STDIN_FILENO, &answer, 1);
   } else {
      printf("High score: %d\n", prev_highscore);
   }
   
   if (answer == 'y') { // új ranglista mentése
      file = fopen("scores.txt", "w");
      if (file == NULL) {
         read(STDERR_FILENO, "Error opening file\n", 19);
         return;
      }
      for (i = 0; i < num_players; i++) {
         fprintf(file, "%s\t%d\n", players[i].name, players[i].score);
      }
      fclose(file);
   }

}

int main(int argc, char** argv)
{
   eGameState state = initGame;        //kezdeti állapot beállítása
   int serial_fd;                      // soros port file descriptora
   int score=0, difficulty=0;          // játékos pontszáma és nehézsége
   char player_name[MAX_NAME_LENGTH];  // játékos neve

   set_input_mode(); // terminál beállítása nem-kanonikus módba

   serial_fd = open("/dev/ttyACM0",O_RDWR); // soros port megnyitása
   setup_tty(serial_fd);                  // soros port beállítása

   bool exit_loop = false; // program bezárása, ha true
   char linebuf[BUFLEN];   // buffer kiolvasáshoz

   // FD-ok tömbje a poll() számára
   struct pollfd pfds[PFDSLEN];
   // STDIN hozzáadása
   pfds[0].fd = STDIN_FILENO;
   pfds[0].events = POLLIN; // beérkező adatok figyelése
   // soros port hozzáadása
   pfds[1].fd = serial_fd;
   pfds[1].events = POLLIN; // beérkező adatok figyelése

   // timeout meghatározása
   int timeout = 50; // 50 ms
   while (!exit_loop) {
      
      int ret = poll(pfds, PFDSLEN, timeout); // poll hívás

      if (ret == -1) { // hiba történt
         write(STDERR_FILENO, "Error polling\n", 15);
         exit_loop = true;
      } else if (ret > 0) { // bekövetkezett legalább egy esemény, amire vártunk
      
         // adat van a standard inputon         
         if (pfds[0].revents & POLLIN) { 
            if (state == startGame){ // játékos nevének bekérése
               int i=0;
               ssize_t n=read(STDIN_FILENO, linebuf, BUFLEN);  // név beolvasása
               if (n>0){ 
                  linebuf[n-1] = '\0'; // sorvége jel levágása
                  strcpy(player_name, linebuf); // név mentése
                  write(STDOUT_FILENO, "Player name: ", 13);
                  write(STDOUT_FILENO, player_name, strlen(player_name));
                  write(STDOUT_FILENO, "\n", 1);
                  write(STDOUT_FILENO, "Set difficulty using W and S or numbers 1-8, press Enter to start game\n", 72);
                  state = playGame; //játék indítása
                  set_input_mode(); //terminál beállítása nem-kanonikus módba karaktertovábbításhoz
               } else {
                  write(STDERR_FILENO, "Error reading from STDIN\n", 26);
               }
            } else { // játék folyamatban vagy kezdeti állapot
               ssize_t n = read(STDIN_FILENO, linebuf, 1); // karakter beolvasása a STDIN-ről
               if (n==1){
                  switch (linebuf[0]) {
                     case 'x': // kilépés
                     case 'q':
                        exit_loop = true;
                        break;
                     
                     default: // karakterek továbbítása a soros portra
                        write(serial_fd, linebuf, 1); 
                  }
               } else {
                  write(STDERR_FILENO, "Error reading from STDIN\n", 26);
               }
            }
            
         }

         // bejövő adat van a soros porton
         if (pfds[1].revents & POLLIN) { 
            ssize_t n = read(serial_fd, linebuf, BUFLEN); // max. BUFLEN mennyiségű karakter olvasása az STDIN-ről
            if(n > 0){
               linebuf[n] = '\0'; // Sorvége jel hozzáadása
               switch (linebuf[0]) {
                  case 's':   // indítás jel
                     state = startGame; 
                     write(STDOUT_FILENO, "Welcome to BANANA TREE!\nCatch as many bananas as you can!\n", 59);
                     write(STDOUT_FILENO, "Enter your name and press Enter to start the game\n", 50);
                     reset_input_mode(); // terminál beállítása kanonikus módba név beolvasásához
                     break;
                  case 'e':   // játék vége
                     sscanf(linebuf, "e%dd%d", &score, &difficulty); // pontszám és nehézség beolvasása
                     write(STDOUT_FILENO, "Game over!\n", 11);
                     reset_input_mode();  // terminál beállítása kanonikus módba válasz beolvasásához
                     record_score(player_name, score, difficulty); // ranglista megjelenítése és pontszám mentésének lehetősége
                     set_input_mode();    // terminál beállítása nem-kanonikus módba karaktertovábbításhoz
                     state = initGame;    // kezdeti állapotba visszatérés
                     break;
                  default: break;
               }
            } else {
               write(STDERR_FILENO, "Error reading from serial port\n", 32);
            }
         }

         }
      }

   // soros port bezására
   close(serial_fd);

   return 0;
}
