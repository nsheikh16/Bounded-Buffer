#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N){
	//TODO: constructor to initiliaze all the varibales declared in BoundedBuffer.h

	pthread_mutex_init(&buffer_lock,NULL);
	pthread_cond_init(&buffer_full, NULL);
	pthread_cond_init(&buffer_empty,NULL);
	buffer_size = N;
	buffer = new int[buffer_size];
	buffer_cnt = 0;
	buffer_last =0;
}

void BoundedBuffer::append(int data){
	//TODO: append a data item to the circular buffer
	pthread_mutex_lock(&buffer_lock);

	while (buffer_size == buffer_cnt){

		pthread_cond_wait(&buffer_full, &buffer_lock);
	}
	//if (buffer_cnt == 1){
	//	pthread_cond_signal(&buffer_empty);
	//}
	buffer[buffer_last]= data;
	buffer_cnt++;
	buffer_last = (buffer_last +1 ) % buffer_size;
	
	pthread_cond_signal(&buffer_empty);
	
	pthread_mutex_unlock(&buffer_lock);

}

int BoundedBuffer::remove(){
	//TODO: remove and return a data item from the circular buffer
	//set lock 
	//if count is zero , wait 
	pthread_mutex_lock(&buffer_lock);

	while (buffer_cnt == 0){
		pthread_cond_wait(&buffer_empty, &buffer_lock);
	}
	int place = (buffer_last - buffer_cnt) % buffer_size;
	if (place<0){
		place= buffer_last-buffer_cnt + buffer_size;
	}
	int data = buffer[place];
	buffer_cnt--;
	pthread_cond_signal(&buffer_full);
	pthread_mutex_unlock(&buffer_lock);

	return data;
}

bool BoundedBuffer::isEmpty(){
	//TODO: check is the buffer is empty
	pthread_mutex_lock(&buffer_lock);
	if (buffer_cnt == 0){
		pthread_mutex_unlock(&buffer_lock);
		return true;}
	else {
		pthread_mutex_unlock(&buffer_lock);
		return false;
	}
}
