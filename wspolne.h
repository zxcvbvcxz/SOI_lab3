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
#define SEM_CZYT_1 "/sem-czyt-1"
#define SEM_PIS_1 "/sem-pis-1"
#define SEM_ZAJETE_1 "/sem-zajete-1"
#define SEM_WOLNE_1 "/sem-wolne-1"
#define SEM_MUTEX_1 "/sem-mutex-1"
#define SHARED_MEM_NAME_1 "/dzielona_pamiec-1"
#define SEM_CZYT_2 "/sem-czyt-2"
#define SEM_PIS_2 "/sem-pis-2"
#define SEM_ZAJETE_2 "/sem-zajete-2"
#define SEM_WOLNE_2 "/sem-wolne-2"
#define SEM_MUTEX_2 "/sem-mutex-2"
#define SHARED_MEM_NAME_2 "/dzielona_pamiec-2"
#define ROZMIAR_KOLEJKI 4

sem_t *czyt_1;
sem_t *pis_1;
sem_t *zajete_1;
sem_t *wolne_1;
sem_t *mutex_1;
sem_t *czyt_2;
sem_t *pis_2;
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
	int czekaCzyt;
	int czekaPis;
	int czytelnicy;
	int pisarze;



	int max_rozmiar;;
	int glowa;
	int licznik;
	wiadomosc kolejka[ROZMIAR_KOLEJKI];

	int dodaj(int nr_kolejki, wiadomosc msg)
	{
		if(nr_kolejki==1){
			sem_wait(wolne_1);
			poczPis(1);
//			sem_wait(mutex_1);
			int koniec = (glowa + licznik) % max_rozmiar;
			kolejka[koniec] = msg;
			licznik++;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Dodalem do 1 kolejki\n\n";
//			sem_post(mutex_1);
			konPis(1);
			sem_post(zajete_1);

		return 0;
		}else
		{	
			sem_wait(wolne_2);
			poczPis(2);
//			sem_wait(mutex_2);
			int koniec = (glowa + licznik) % max_rozmiar;
			kolejka[koniec] = msg;
			licznik++;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Dodalem do 2 kolejki\n\n";
//			sem_post(mutex_2);
			konPis(2);
			sem_post(zajete_2);
			return 0;
		}
	}

	int wyjmij( int nr_kolejki)
	{
		if(nr_kolejki==1)
		{
			sem_wait(zajete_1);
			poczPis(1);
//			sem_wait(mutex_1);
			glowa = (glowa == max_rozmiar - 1) ? 0 : glowa + 1;
			licznik--;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Wyjalem z 1 kolejki\n\n";
//			sem_post(mutex_1);
			konPis(1);
			sem_post(wolne_1);
			return 0;
		} else
		{
			sem_wait(zajete_2);
			poczPis(2);
//			sem_wait(mutex_2);
			glowa = (glowa == max_rozmiar - 1) ? 0 : glowa + 1;
			licznik--;
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "Wyjalem z 2 kolejki\n\n";
//			sem_post(mutex_2);
			konPis(2);
			sem_post(wolne_2);
			return 0;
		}
	}

	int czytaj(int nr_kolejki)
	{
		if(nr_kolejki==1)
		{
			sem_wait(zajete_1);
			poczCzyt(1);
//			sem_wait(mutex_1);
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "id_kolejki: " << kolejka[glowa].id_kolejki << endl;
			cout << "id_producenta: " << kolejka[glowa].id_producenta << endl;
			cout << "id_wiadomosci: " << kolejka[glowa].id_wiadomosci << endl << endl;
//			sem_post(mutex_1);
			konCzyt(1);
			sem_post(zajete_1);
		} else
		{
			sem_wait(zajete_2);
			poczCzyt(2);
//			sem_wait(mutex_2);
			std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
			cout << std::ctime(&end_time);
			cout << "id_kolejki: " << kolejka[glowa].id_kolejki << endl;
			cout << "id_producenta: " << kolejka[glowa].id_producenta << endl;
			cout << "id_wiadomosci: " << kolejka[glowa].id_wiadomosci << endl << endl;
//			sem_post(mutex_2);
			konCzyt(2);
			sem_post(zajete_2);
		}
	}

	void budzProces(int nr_kolejki)
	{
	if(nr_kolejki == 1){
		if(pisarze == 0 and czekaCzyt > 0)
		{
			czekaCzyt--;
			sem_post(czyt_1);
		}else if (czytelnicy == 0 and pisarze == 0 and czekaPis >0)
		{
			czekaPis--;
			sem_post(pis_1);
		}else
		{
			sem_post(mutex_1);
		}
	}else
	{
		if(pisarze == 0 and czekaCzyt > 0)
		{
			czekaCzyt--;
			sem_post(czyt_2);
		}else if (czytelnicy == 0 and pisarze == 0 and czekaPis >0)
		{
			czekaPis--;
			sem_post(pis_2);
		}else
		{
			sem_post(mutex_2);
		}
	}
	}

	void poczCzyt(int nr_kolejki)
	{
	if (nr_kolejki==1)
	{
		sem_wait(mutex_1);
		if(pisarze >0)
		{
			czekaCzyt++;
			sem_post(mutex_1);
			sem_wait(czyt_1);	
		}
		czytelnicy++;
		budzProces(1);
	} else
	{
		sem_wait(mutex_2);
		if(pisarze > 0)
		{
			czekaCzyt++;
			sem_post(mutex_2);
			sem_wait(czyt_2);	
		}
		czytelnicy++;
		budzProces(2);
	}
	}

	void konCzyt(int nr_kolejki)
	{
	if (nr_kolejki==1)
	{
		sem_wait(mutex_1);
		czytelnicy--;
		budzProces(1);
	} else
	{
		sem_wait(mutex_2);
		czytelnicy--;
		budzProces(2);
	}
	}
	
	void poczPis(int nr_kolejki)
	{
	if (nr_kolejki==1)
	{	
		sem_wait(mutex_1);
		if (pisarze > 0 or czytelnicy >0)
		{
			czekaPis++;
			sem_post(mutex_1);
			sem_wait(pis_1);
		}
		pisarze++;
		budzProces(1);
	} else
	{
		sem_wait(mutex_2);
		if (pisarze > 0 or czytelnicy >0)
		{
			czekaPis++;
			sem_post(mutex_2);
			sem_wait(pis_2);
		}
		pisarze++;
		budzProces(2);
	}
	}

	void konPis(int nr_kolejki)
	{
	if (nr_kolejki==1)
	{	
		sem_wait(mutex_1);
		pisarze--;
		budzProces(1);
	} else
	{
		sem_wait(mutex_2);
		pisarze--;
		budzProces(2);
	}
	}

	int inicjalizuj()
	{
		max_rozmiar = ROZMIAR_KOLEJKI;
		glowa = 0;
		licznik = 0;
		czekaCzyt = 0;
		czekaPis = 0;
		czytelnicy = 0;
		pisarze = 0;
	}
};

