#include "ProducerConsumer.h"

//TODO: add BoundedBuffer, locks and any global variables here

BoundedBuffer *buff  = new BoundedBuffer(1000);
int i, ps,cs;
//void *producer (void *param);
///void *consumer (void *param);
ofstream file;
int count=0;
pthread_mutex_t lock;
int time1=0;
void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
	ps = psleep;
	cs = csleep; 
	i = items;
	pthread_t producers[p];
	pthread_t consumers[c];
	pthread_mutex_init(&lock,NULL);
	file.open("output.txt");


	for (int j=0; j < c ; j++){
		pthread_create(&consumers[j], NULL, consumer,reinterpret_cast<void*>(j));
	}
	for (int n=0; n < p ; n++){
		pthread_create(&producers[n], NULL, producer,reinterpret_cast<void*>(n));
	}

	for (int i =0; i<c; i++){
		pthread_join(consumers[i],NULL);
	}

	for (int i =0; i<p; i++){
		pthread_join(producers[i],NULL);
	}


	file.close();
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
	 //variable how many global items produced
	int id = reinterpret_cast<long>(threadID);
	while(count<i){
		sleep(ps);
		int a = rand() % 10;

		pthread_mutex_lock(&lock);
		if (count ==i){
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);

		}
		pthread_mutex_unlock(&lock);

		buff->append(a);
		
		pthread_mutex_lock(&lock);
		count++;
		time1+=ps;
		file << "Producer #" << id << ", time =" <<time1<<", producing data item #" <<count<<", item value="<<a<<endl;
		pthread_mutex_unlock(&lock);
	}

	pthread_exit(NULL);
	

}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	 int id = reinterpret_cast<long>(threadID);
	while(count<i || !buff->isEmpty()){
		sleep(cs);
		if (!buff->isEmpty()){
		int a =buff->remove();
		pthread_mutex_lock(&lock);
		time1+=cs;
		file << "Consumer #" << id << ", time =" <<time1<<", consuming data item with value="<<a<<endl;
		pthread_mutex_unlock(&lock);
	}
	}
	pthread_exit(NULL);
}
