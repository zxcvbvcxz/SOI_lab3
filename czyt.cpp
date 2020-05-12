#include "wspolne.h"







int main(int argc, char** argv){
	if(argc<2)
	{
		cout << "Podaj id kolejki z której chcesz czytać\n";
		return 0;
	}
	if(atoi(argv[1]) != 1 and atoi(argv[1]) != 2)
	{
		cout << "Podaj liczbę 1 lub 2\n";
		return 0;
	}
	int wybor = atoi(argv[1]);

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

	double sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	while(true){
	if(wybor==1)
	{
		kolejka_1->czytaj(1);
	} else
	{
		kolejka_2->czytaj(2);
	}
	usleep(sekundy  * 1000000);
	sekundy = 1 + (rand()%4) / (rand()%10 + 1);// od 1 do 5
	}

	cout << "czyt.cpp!\n";
	return 0;
}
