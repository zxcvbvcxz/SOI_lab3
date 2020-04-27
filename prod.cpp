#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>  
using namespace std;

#define SEM_ZAJETE_1 "/sem-zajete-1"
#define SEM_WOLNE_1 "/sem-wolne-1"
#define SEM_MUTEX_1 "/sem-mutex-1"
#define SHARED_MEM_NAME_1 "/dzielona_pamiec-1"
#define SEM_ZAJETE_2 "/sem-zajete-2"
#define SEM_WOLNE_2 "/sem-wolne-2"
#define SEM_MUTEX_2 "/sem-mutex-2"
#define SHARED_MEM_NAME_2 "/dzielona_pamiec-2"
#define ROZMIAR_KOLEJKI 4

sem_t *zajete_1;
sem_t *wolne_1;
sem_t *mutex_1;
sem_t *zajete_2;
sem_t *wolne_2;
sem_t *mutex_2;


class wiadomosc{
public:
	int id_kolejki;
	int id_producenta;
	int id_wiadomosci;

	wiadomosc(int id_k,int id_p,int id_w)
	{
		id_kolejki = id_k;
		id_producenta = id_p;
		id_wiadomosci = id_w;
	}
	wiadomosc()
	{
		id_kolejki = 0;
		id_producenta = 0;
		id_wiadomosci = 0;
	}
};


class k_kolejka{
public:
	int max_rozmiar;;
	int glowa;
	int licznik;
	wiadomosc kolejka[ROZMIAR_KOLEJKI];

	int dodaj(int nr_kolejki, wiadomosc msg)
	{
		if(nr_kolejki==1){
			sem_wait(wolne_1);
			sem_wait(mutex_1);
			int koniec = (glowa + licznik) % max_rozmiar;
			kolejka[koniec] = msg;
			licznik++;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Dodalem do 1 kolejki\n\n";
			sem_post(mutex_1);
			sem_post(zajete_1);
		return 0;
		}else
		{	
			sem_wait(wolne_2);
			sem_wait(mutex_2);
			int koniec = (glowa + licznik) % max_rozmiar;
			kolejka[koniec] = msg;
			licznik++;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Dodalem do 2 kolejki\n\n";
			sem_post(mutex_2);
			sem_post(zajete_2);
			return 0;
		}
	}

	int wyjmij( int nr_kolejki)
	{
		if(nr_kolejki==1)
		{
			sem_wait(zajete_1);
			sem_wait(mutex_1);
			glowa = (glowa == max_rozmiar - 1) ? 0 : glowa + 1;
			licznik--;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Wyjalem z 1 kolejki\n\n";
			sem_post(mutex_1);
			sem_post(wolne_1);
			return 0;
		} else
		{
			sem_wait(zajete_2);
			sem_wait(mutex_2);
			glowa = (glowa == max_rozmiar - 1) ? 0 : glowa + 1;
			licznik--;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Wyjalem z 2 kolejki\n\n";
			sem_post(mutex_2);
			sem_post(wolne_2);
			return 0;
		}
	}

	int czytaj(int nr_kolejki)
	{
		if(nr_kolejki==1)
		{
			sem_wait(zajete_1);
			sem_wait(mutex_1);
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "id_kolejki: " << kolejka[glowa].id_kolejki << endl;
			cout << "id_producenta: " << kolejka[glowa].id_producenta << endl;
			cout << "id_wiadomosci: " << kolejka[glowa].id_wiadomosci << endl << endl;
			sem_post(mutex_1);
			sem_post(zajete_1);
		} else
		{
			sem_wait(zajete_2);
			sem_wait(mutex_2);
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "id_kolejki: " << kolejka[glowa].id_kolejki << endl;
			cout << "id_producenta: " << kolejka[glowa].id_producenta << endl;
			cout << "id_wiadomosci: " << kolejka[glowa].id_wiadomosci << endl << endl;
			sem_post(mutex_2);
			sem_post(zajete_2);
		}
	}

	int inicjalizuj()
	{
		max_rozmiar = ROZMIAR_KOLEJKI;
		glowa = 0;
		licznik = 0;
	}
};








int main(int argc, char** argv){
	if(argc<3)
	{
		cout << "Podaj id kolejki i id producenta\n";
		return 0;
	}
	if(atoi(argv[1]) != 1 and atoi(argv[1]) != 2)
	{
		cout << "Podaj liczbę 1 lub 2\n";
		return 0;
	}
	int wybor = atoi(argv[1]);
	int id_p = atoi(argv[2]);

	srand(unsigned (time(0)));

	int 	fd_shm_1, fd_shm_2; //file descriptor shared memory

	k_kolejka* kolejka_1;
	k_kolejka* kolejka_2;

	if ((fd_shm_1 = shm_open (SHARED_MEM_NAME_1, O_RDWR, 0660)) == -1)
        cout << "shm_open" << endl;

	if ((kolejka_1 = (k_kolejka*)mmap (NULL, sizeof (k_kolejka), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm_1, 0)) == MAP_FAILED)
       cout << "mmap" << endl;
	
	if ((fd_shm_2 = shm_open (SHARED_MEM_NAME_2, O_RDWR, 0660)) == -1)
        cout << "shm_open" << endl;

	if ((kolejka_2 = (k_kolejka*)mmap (NULL, sizeof (k_kolejka), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm_2, 0)) == MAP_FAILED)
       cout << "mmap" << endl;

	if ((zajete_1 = sem_open (SEM_ZAJETE_1, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((wolne_1 = sem_open (SEM_WOLNE_1, O_RDWR, 0660, ROZMIAR_KOLEJKI)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((mutex_1 = sem_open (SEM_MUTEX_1, O_RDWR, 0660, 1)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}

	if ((zajete_2 = sem_open (SEM_ZAJETE_2, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((wolne_2 = sem_open (SEM_WOLNE_2, O_RDWR, 0660, ROZMIAR_KOLEJKI)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((mutex_2 = sem_open (SEM_MUTEX_2, O_RDWR, 0660, 1)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}
	
	int id_w=1;

	wiadomosc msg(1,id_p,id_w);
	double sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	while(true){
	if(wybor == 1)
	{
		msg.id_wiadomosci = id_w;
		msg.id_kolejki=1;
		kolejka_1->dodaj(1,msg);
		id_w++;
	} else
	{
		msg.id_wiadomosci = id_w;
		msg.id_kolejki=2;
		kolejka_2->dodaj(2,msg);
		id_w++;
	}
	usleep(sekundy  * 1000000);
	sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	}

	cout << "prod.cpp!\n";
	return 0;
}
