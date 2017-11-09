#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
using namespace std;

sem_t mutex1, mutex2;

void *thr_fn(void *p) {
	int32_t epoch = 1;
	sleep(1);
	while (true) {
		sem_post(&mutex2);
		sem_wait (&mutex1);
		cerr << " thread " << epoch++ << endl;
		sleep(3);
		cerr << "done\n";
		// retrieve the analysis result
	}
}


int main() {
	sem_init(&mutex1, 0, 0);
	sem_init(&mutex2, 0, 0);
	// while (true) {
	// 	int cnt = sem_trywait(&mutex2);
	// 	if(0 == cnt){
	// 		cerr << "succeed\n";
	// 	} else{
	// 		cerr << "blocked\n";
	// 	}
	// 	sleep(1);
	// }

	pthread_t task;
	pthread_create(&task, NULL, &thr_fn, NULL);
	while (true) {
		int cnt = 0;
		if(0 == sem_trywait(&mutex2)) {
			// cv thrd is idle
			cerr << "prepare task for thread\n";
			// prepare the task
			sem_post(&mutex1);

		} else {
			// use previous resu
			cerr << "thread is busy, do sth else\n";
		}

		sleep(rand()%8);
	}


}