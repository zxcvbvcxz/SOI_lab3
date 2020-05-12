#include "wspolne.h"






int main(){
	int 	fd_shm_1, fd_shm_2; //file descriptor shared memory

	k_kolejka* kolejka_1;
	k_kolejka* kolejka_2;

	if ((fd_shm_1 = shm_open (SHARED_MEM_NAME_1, O_RDWR | O_CREAT, 0660)) == -1)
        cout << "shm_open" << endl;

	if (ftruncate (fd_shm_1, sizeof (k_kolejka)) == -1)
       cout << "ftruncate" << endl;
    
	if ((kolejka_1 = (k_kolejka*)mmap (NULL, sizeof (k_kolejka), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm_1, 0)) == MAP_FAILED)
       cout << "mmap" << endl;

	if ((fd_shm_2 = shm_open (SHARED_MEM_NAME_2, O_RDWR | O_CREAT, 0660)) == -1)
        cout << "shm_open" << endl;

	if (ftruncate (fd_shm_2, sizeof (k_kolejka)) == -1)
       cout << "ftruncate" << endl;
    
	if ((kolejka_2 = (k_kolejka*)mmap (NULL, sizeof (k_kolejka), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm_2, 0)) == MAP_FAILED)
       cout << "mmap" << endl;
	
	kolejka_1->inicjalizuj();
	kolejka_2->inicjalizuj();
	if ((czyt_1 = sem_open (SEM_CZYT_1, O_CREAT, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((pis_1 = sem_open (SEM_PIS_1, O_CREAT, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((zajete_1 = sem_open (SEM_ZAJETE_1, O_CREAT, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((wolne_1 = sem_open (SEM_WOLNE_1, O_CREAT, 0660, ROZMIAR_KOLEJKI)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((mutex_1 = sem_open (SEM_MUTEX_1, O_CREAT, 0660, 1)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	

	if ((czyt_2 = sem_open (SEM_CZYT_2, O_CREAT, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}
	if ((pis_2 = sem_open (SEM_PIS_2, O_CREAT, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}
	if ((zajete_2 = sem_open (SEM_ZAJETE_2, O_CREAT, 0660, 0)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((wolne_2 = sem_open (SEM_WOLNE_2, O_CREAT, 0660, ROZMIAR_KOLEJKI)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}	
	if ((mutex_2 = sem_open (SEM_MUTEX_2, O_CREAT, 0660, 1)) == SEM_FAILED) {
     	cout << "sem_open" << endl;
	}

	int i;
	cin >> i;

	shm_unlink(SHARED_MEM_NAME_1);
	sem_unlink(SEM_CZYT_1);
	sem_unlink(SEM_PIS_1);
	sem_unlink(SEM_ZAJETE_1);
	sem_unlink(SEM_WOLNE_1);
	sem_unlink(SEM_MUTEX_1);
	shm_unlink(SHARED_MEM_NAME_2);
	sem_unlink(SEM_CZYT_2);
	sem_unlink(SEM_PIS_2);
	sem_unlink(SEM_ZAJETE_2);
	sem_unlink(SEM_WOLNE_2);
	sem_unlink(SEM_MUTEX_2);


	cout << "kolejki.cpp!\n";
	return 0;
}
