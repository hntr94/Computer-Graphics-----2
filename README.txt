Nume: Vinatoru Andrei-Ioan, 335CB

============================================================================================================

Pentru utilizarea aplicatiei, se va apasa mai intai click dreapta in interiorul aplicatie mai intai, iar apoi:

- pentru deplasarea personajului se vor folosi W, A, S, D;
- pentru schimbarea armei se folosesc 1,2,3 (fiecare tasta aferenta unui tip de arme);
- pentru schimbarea vectorului up al camerei si apasa E si Q, in functie de sens;
- pentru a trage cu arma sau pentru a plasa turnuri se apasa click in modul aferent;
- pentru a misca personajul si camera se va folosi mouse-ul;
- pentru schimbarea in modul de build se apasa G.

Pentru implementare am folosit si concepte de OOP. Astfel, am clasa GameObject care este extinsa de toate celelalte clase Player, Enemy, Bullet etc;

Fiecare clasa contie date specifice de control pentru fiecare obiect in parte. Ex:
- cand se se spawneze
- in ce interval sa faca animatie
- daca este un personaj mort

Traiectoria inamicilor este hrdcodata, acestia parcurgand in Z harta.
Daca ajung la sfarsit, vor scadea pe rand cate o viata de la player.
Cand playerul ajunge la 0 vieti, va cadea intr-o parte si va disparea, camera putandu-se misca in continuare
Vietile sunt randate fiecare pe cate o axa de rotatie, in jurul capului (specific lui Khael'Thas din Warcraft)
Pentru first person se modifica field of view. Astfel cu o pozitie corecta se va vedea chiar din punctul de vedere al personajului. 
Bolta de la lansatorul de grenade este facuta folosind principiile mecanicii fara rezistenta din partea aerului.
Ghiulelele de la tunuri dau damage standard ca si pusca.
Pistolul si grenade launcherul dau 2, respectiv 5.
Fiecare arma are o viteza si un timp de tragere specific: timpul in care poti sa tragi cu pistolul este de 1
secunda, timpul de tragere cu pusca de 0.25 si cu grenade launcher 3s.

Probleme aparute pe parcursul temei:
- greseli asupra calculului de unghiuri
- am lasat minimapul in partea stanga, pare mai natural in partea stanga

In interiorul arhivei se gasesc toate sursele si resursele, si de asemenea in root, si acest README.

Status implementare functionalitati:
 - am implementat toate feature-urile din baseline;
 - bonus, am implementat modul de construire turnuri
