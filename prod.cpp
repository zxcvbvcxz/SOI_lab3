#include "wspolne.h"



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

	if ((czyt_1 = sem_open (SEM_CZYT_1, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}

	if ((pis_1 = sem_open (SEM_PIS_1, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	

	if ((mutex_1 = sem_open (SEM_MUTEX_1, O_RDWR, 0660, 1)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}

	if ((zajete_1 = sem_open (SEM_ZAJETE_1, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}

	if ((wolne_1 = sem_open (SEM_WOLNE_1, O_RDWR, 0660, ROZMIAR_KOLEJKI)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	

	if ((czyt_2 = sem_open (SEM_CZYT_2, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}

	if ((pis_2 = sem_open (SEM_PIS_2, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	

	if ((mutex_2 = sem_open (SEM_MUTEX_2, O_RDWR, 0660, 1)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}
	
	if ((zajete_2 = sem_open (SEM_ZAJETE_2, O_RDWR, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	

	if ((wolne_2 = sem_open (SEM_WOLNE_2, O_RDWR, 0660, ROZMIAR_KOLEJKI)) == SEM_FAILED) {
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
