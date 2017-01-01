/*
 * Bank.cpp
 *
 *  Created on: Dec 31, 2016
 *      Author: eitanlev
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include <pthread.h>

#include "bankMap.h"
#include "bankAccount.h"
#include "Exceptions.h"



/*#include <math.h>
#include <iostream>
#include <string>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <sstream>
#include <exception>
using std::map;
using std::istringstream;
using std::ifstream;
using std::string;
using std::getline;
using std::cout;
using std::endl;
using std::FILE;
using std::stringstream;
using std::ostream;
using std::ofstream;
using std::pair;
using std::exception;*/


#define ERROR_VALUE (-1)
#define SUCCESS_VALUE (0)

using std::stringstream;
using std::ofstream;
using std::string;

pthread_mutex_t map_lock;
pthread_mutex_t bank_balance_lock;
pthread_mutex_t log_file_lock;
ofstream log_file;

class ATM {//TODO move to another file
public:
	int _id;
	string commandsList;
};

//Functions used by the threads:
void* Continuous_Print_Run(void* cmds);
void* Commission_Run(void* cmds);
void* ATM_Run(void* cmds);


/*
 * In this file the threads will be managed- Init thread, ATM thread, Commission thread and Bank Status thread.
 * All functions called from here must be sent with the accounts map.
 */

//TODO
/*
 * The way things currently are implemented, first reader (from something of bank account) might get an updated value,
 * inconsistent with the value he originally meant. This is due to the writers lock implementation.
 * Technically there is not problem with that.
 */

//This is the map for the bank accounts:
bankMap bankAccountsMap;//TODO beware of duplication


int main(int argc, char* argv[]) {
	//================================================================
	//First check all parameters are OK:
	if (argc <= 2) { //Check at least 3 arguments- File, Number of ATMs, Input File.
		fprintf(stderr, "illegal arguments\n");
		exit(ERROR_VALUE);
	}

	int ATMs_NUMBER = atoi(argv[1]);
	if(argc != ATMs_NUMBER + 2) {
		fprintf(stderr,"Given number of ATMs doesn't match the actual amount of ATMs as arguments\n");
		exit(ERROR_VALUE);
	}
	for(int i = 2; i < argc; ++i ){ //checking ATM files
		if ((stringstream(argv[2])).good() == false) {
			fprintf(stderr, "At least one of the files doesn't exist or isn't readable\n");
			exit(ERROR_VALUE);
		}
	}
	//================================================================
	//Initialize locks:
	pthread_mutex_init(&map_lock, NULL);
	pthread_mutex_init(&bank_balance_lock, NULL);
	pthread_mutex_init(&log_file_lock, NULL);
	//================================================================
	//Preparing all threads and data:
	log_file.open("log.txt");
 	pthread_t threads_ATM[ATMs_NUMBER];
	pthread_t thread_Comission;
	pthread_t thread_Continuous_Print;
	ATM* ATM_Manage = new ATM[ATMs_NUMBER];
	int cmdsCounter = 2;
	int atmCounter = 1;
	for (int i = 0; i < ATMs_NUMBER; i++) {
		ATM_Manage[i]._id = atmCounter++;
		ATM_Manage[i].commandsList = string(argv[cmdsCounter++]);
	}
	//================================================================
	//Create threads for ATM, commission and printing status:
	int threadRes;
	for (int i = 0; i < ATMs_NUMBER; i++) {
		threadRes = pthread_create(&threads_ATM[i], NULL, ATM_Run, &ATM_Manage[i]);
		if (threadRes != SUCCESS_VALUE) {
			fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
			exit (ERROR_VALUE);
		}
	}
	threadRes = pthread_create(&thread_Comission, NULL, Commission_Run, NULL);
	if (threadRes != SUCCESS_VALUE) {
		fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
		exit (ERROR_VALUE);
	}
	threadRes = pthread_create(&thread_Continuous_Print, NULL, Continuous_Print_Run, NULL);
	if (threadRes != SUCCESS_VALUE) {
		fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
		exit (ERROR_VALUE);
	}
	//================================================================
	//Join all ATM's:
	for (int i = 0; i < ATMs_NUMBER; i++) {
		threadRes = pthread_join(threads_ATM[i], NULL);
		if (threadRes != SUCCESS_VALUE) {//TODO only for testing
			printf("pthread_join failure:\n");
			printf("threads_ATM[%d]:\n", i);
			printf("threadRes = %d \n", threadRes);
		}
		assert(threadRes == SUCCESS_VALUE);
	}
	threadRes = pthread_join(thread_Comission, NULL);
	if (threadRes != SUCCESS_VALUE) {//TODO only for testing
		printf("pthread_join failure:\n");
		printf("thread_Comission, ");
		printf("threadRes = %d \n", threadRes);
	}
	assert(threadRes == SUCCESS_VALUE);
	threadRes = pthread_join(thread_Continuous_Print, NULL);
	if (threadRes != SUCCESS_VALUE) {//TODO only for testing
		printf("pthread_join failure:\n");
		printf("thread_Continuous_Print, ");
		printf("threadRes = %d \n", threadRes);
	}
	assert(threadRes == SUCCESS_VALUE);
	//================================================================
	//Destroy locks, free pointers and close file:
	log_file.close();
	pthread_mutex_destroy(&map_lock);
	pthread_mutex_destroy(&bank_balance_lock);
	pthread_mutex_destroy(&log_file_lock);
	delete[] ATM_Manage;
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	//================================================================
	/*bankAccount new_account(1,2,3);
	Pair dataPair(1, new_account);
	bankAccountsMap.insert(dataPair);*/
	return 0;
}

void* ATM_Run(void* cmds) {
	return NULL;
}

void* Commission_Run(void* cmds) {
	return NULL;
}

void* Continuous_Print_Run(void* cmds) {
	return NULL;
}

//Eyal main:
/*int main(int argc, char* argv[]) {
	if (argc <= 2) { //Check at least 3 arguments- File, Number of ATMs, Input File.
		fprintf(stderr, "illegal arguments\n");
		exit(ERROR_VALUE);
	}

	int ATMs_NUMBER = atoi(argv[1]);
	if(argc != ATMs_NUMBER + 2) {
		fprintf(stderr,"Given number of ATMs doesn't match the actual amount of ATMs as arguments\n");
		exit(ERROR_VALUE);
	}
	for(int i = 2; i < argc; ++i ){ //checking ATM files
		if ((stringstream(argv[2])).good() == false) {
			fprintf(stderr, "At least one of the files doesn't exist or isn't readable\n");
			exit(ERROR_VALUE);
		}
	}
	//init locks
	pthread_mutex_init(&listMutex, NULL);
	pthread_mutex_init(&log_write, NULL);
	pthread_mutex_init(&BankMoney_lock, NULL);

	//open log file
	logFile.open("log.txt");

	//declaring threads
	pthread_t atms_tr[ATMs_NUMBER];
	pthread_t commission_tr;
	pthread_t print_status_tr;

	//making data for threads
	pATM atm_structs = new ATM[ATMs_NUMBER];
	int i;
	for (i = 0; i < ATMs_NUMBER; ++i)
	{
		atm_structs[i].atm_id = i + 1;
		atm_structs[i].list_of_commands = argv[i + 2];
	}

	//creating ATM threads
	int threadRes;
	for (i = 0; i < ATMs_NUMBER; i++) {
		threadRes = pthread_create(&atms_tr[i], NULL, atm, &atm_structs[i]);
		if (threadRes != SUCCESS_VALUE) {
			fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
			exit (ERROR_VALUE);
		}
	}

	//creating status and commission threads

	InProcess = true;
	threadRes = pthread_create(&commission_tr, NULL, commission, NULL);
	if (threadRes != SUCCESS_VALUE) {
		fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
		exit (ERROR_VALUE);
	}
	threadRes = pthread_create(&print_status_tr, NULL, print_bank_status, NULL);
	if (threadRes != SUCCESS_VALUE) {
		fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
		exit (ERROR_VALUE);
	}

	// waiting for all ATMs to finish

	for (i = 0; i < ATMs_NUMBER; ++i) {
		pthread_join(atms_tr[i], NULL);
	}
	InProcess = false; // ATM threads are done

	//now wait for other threads

	pthread_join(commission_tr, NULL);
	pthread_join(print_status_tr, NULL);

	//clean up

	pthread_mutex_destroy(&listMutex);
	pthread_mutex_destroy(&log_write);
	pthread_mutex_destroy(&BankMoney_lock);

	logFile.close();
	delete[] atm_structs;
	return 0;
}*/


//==============================================================
//Lior:
/*
pthread_mutex_t listMutex;
pthread_mutex_t log_write;
pthread_mutex_t BankMoney_lock;

int BankMoney=0;
bool InProcess = false;
bool com_flag = false;
ofstream logFile;

typedef struct _ATM {
  int atm_id;
  char* list_of_commands;
} ATM;

typedef ATM* pATM;

void *print_bank_status(void* data);
void *commission(void* data);
void *atm(void* data);
*/
//==============================================================
