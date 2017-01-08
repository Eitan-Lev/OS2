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
#include <unistd.h>

#include "bankMap.h"
#include "bankAccount.h"
#include "Exceptions.h"
#include "MacroBank.h"


#define ERROR_VALUE (-1)
#define SUCCESS_VALUE (0)

using std::stringstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;
using std::getline;

pthread_mutex_t map_lock;
pthread_mutex_t bank_balance_lock;
pthread_mutex_t log_file_lock;
//pthread_mutex_t read_transfer_lock;//TODO
//pthread_mutex_t write_transfer_lock;//TODO
//int readTransferCounter = 0;//TODO
//pthread_mutex_t tranfer_accounts_lock;//TODO
ofstream log_file;

class ATM {
public:
	int _id;
	char* commandsList;
};

//Functions used by the threads:
void* Continuous_Print_Run(void* cmds);
void* Commission_Run(void* cmds);
void* ATM_Run(void* cmds);
int BankBalance;
bool atmThreadRunning;
bool commissionThreadRunning;
//int srcAccountTransfer;//TODO
//int dstAccountTransfer;//TODO

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
bankMap bankAccountsMap;


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
	//cout << "Finished checking arguments " << __LINE__ << endl;
	//================================================================
	//Initialize locks:
	pthread_mutex_init(&map_lock, NULL);
	pthread_mutex_init(&bank_balance_lock, NULL);
	pthread_mutex_init(&log_file_lock, NULL);
	//pthread_mutex_init(&read_transfer_lock, NULL);//TODO
	//pthread_mutex_init(&write_transfer_lock, NULL);//TODO
	//readTransferCounter = 0;//TODO
	//================================================================
	//Preparing all threads and data:
	atmThreadRunning = false;
	commissionThreadRunning = false;
	BankBalance = 0;
	//srcAccountTransfer = (-1);//TODO
	//dstAccountTransfer = (-1);//TODO
	log_file.open("log.txt");
 	pthread_t threads_ATM[ATMs_NUMBER];
	pthread_t thread_Comission;
	pthread_t thread_Continuous_Print;
	ATM* ATM_Manage = new ATM[ATMs_NUMBER];
	int cmdsCounter = 2;
	int atmCounter = 1;
	for (int i = 0; i < ATMs_NUMBER; i++) {
		ATM_Manage[i]._id = atmCounter++;
		//ATM_Manage[i].commandsList = string(argv[cmdsCounter++]);
		ATM_Manage[i].commandsList = argv[cmdsCounter++];
	}
	//cout << "Finished init " << __LINE__ << endl;
	//================================================================
	//Create threads for ATM, commission and printing status:
	int threadRes;
	atmThreadRunning = true;
	threadRes = pthread_create(&thread_Comission, NULL, Commission_Run, NULL);
	if (threadRes != SUCCESS_VALUE) {
		fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
		exit (ERROR_VALUE);
	}
	for (int i = 0; i < ATMs_NUMBER; i++) {
		threadRes = pthread_create(&threads_ATM[i], NULL, ATM_Run, &ATM_Manage[i]);
		if (threadRes != SUCCESS_VALUE) {
			fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
			exit (ERROR_VALUE);
		}
	}
	threadRes = pthread_create(&thread_Continuous_Print, NULL, Continuous_Print_Run, NULL);
	if (threadRes != SUCCESS_VALUE) {
		fprintf(stderr, "error: pthread_create, threadRes: %d\n", threadRes);
		exit (ERROR_VALUE);
	}
	//cout << "Finished creating threads " << __LINE__ << endl;
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
	atmThreadRunning = false;
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
	//cout << "Finished joining threads " << __LINE__ << endl;
	//================================================================
	//Destroy locks, free pointers and close file:
	log_file.close();
	pthread_mutex_destroy(&map_lock);
	pthread_mutex_destroy(&bank_balance_lock);
	pthread_mutex_destroy(&log_file_lock);
	//pthread_mutex_destroy(&read_transfer_lock);//TODO
	//pthread_mutex_destroy(&write_transfer_lock);//TODO
	delete[] ATM_Manage;
	//cout << "Finished destroying threads " << __LINE__ << endl;
	return 0;
}

void* ATM_Run(void* cmds) {
	//================================================================
	//Initialize variables:
	ATM* atm = (ATM*)cmds;
	//todo changed to ifstream to open file
	/*std::ifstream file(atm->commandsList);
	stringstream cmdList;
	cmdList << file.rdbuf();
    file.close();*/
    std::ifstream cmdList(atm->commandsList);
	//stringstream cmdList(atm->commandsList);
	ASSERT_VALID(cmdList, "File not found");
	ASSERT_VALID(cmdList.good(), "File not found");
	string cmdLine;
	int accountNumber, password, atmNumber, balance, sum, dstAccountNumber;
	char opCode;
	bool isAccountCurrentlyInMap;
	//================================================================
	while (getline(cmdList, cmdLine)) {
		std::istringstream command(cmdLine);
		//cout << "string cmdLine is " << cmdLine << " " << __LINE__ << endl;
		//cout << "Entered command reading " << __LINE__ << endl;
		//================================================================
		//Define variables and initialize checks
		ASSERT_VALID(command >> opCode >> accountNumber >> password, "Error in line format");//Add all arguments to command stream.
		if (opCode == 'O' || opCode == 'D' || opCode == 'W') {
			//cout << "Entered first if in getline" << __LINE__ << endl;
			ASSERT_VALID(command >> sum, "Error in line format");//Cmd is open, deposit, or withdraw so add sum to command stream.
		}
		atmNumber = atm->_id;
		pthread_mutex_lock(&map_lock);//FIXME for testing
		isAccountCurrentlyInMap = bankAccountsMap.isAccountInMap(accountNumber);//FIXME what if we add later here?
		pthread_mutex_unlock(&map_lock);
		if (opCode != 'O' && isAccountCurrentlyInMap == true) {//Then Open command is not expected
			//cout << "Entered second if in getline" << __LINE__ << endl;
			if (!(bankAccountsMap.checkPassword(accountNumber, password))) {//If not Open command, password needs to be correct
				//cout << "Entered internal if in if" << __LINE__ << endl;
				//cout << "password is " << bankAccountsMap.getPassword(accountNumber) << endl;
				LOG_WRONG_PASSWORD(atmNumber, accountNumber);
				continue;
			}
		} else if (opCode != 'O' && isAccountCurrentlyInMap == false) {
			//cout << "Entered else if getline. opcode is " << opCode << __LINE__ << endl;
			LOG_ACCOUNT_DOESNT_EXIST(atmNumber, accountNumber);
			continue;
		}
		//cout << "Finished preliminary checks " << __LINE__ << endl;
		//================================================================
		//Deal with each command:
		if (opCode == 'O') {//Open Account
			//cout << "Entered Open" << __LINE__ << endl;
			if (isAccountCurrentlyInMap == true) {
				LOG_ACCOUNT_ALREADY_EXISTS(atmNumber);
				//continue;//TODO don't skip usleep
			} else {
				pthread_mutex_lock(&map_lock); // not allowing two ATMs to add the same account - atomic check&add
				sleep(1);
				try {
					bankAccountsMap.openNewAccount(accountNumber, password, sum);
					LOG_OPEN_NEW_ACCOUNT(atmNumber, accountNumber, password, sum);
				} catch (AccountNumberAlreadyExistsException&) {
					pthread_mutex_unlock(&map_lock);
					LOG_ACCOUNT_ALREADY_EXISTS(atmNumber);
				} catch (...) {
					pthread_mutex_unlock(&map_lock);
					UNEXPECTED_EXCEPTION();
				}
				pthread_mutex_unlock(&map_lock);
			}
		}  else if (opCode == 'T') {//Transfer money between accounts
			ASSERT_VALID(command >> dstAccountNumber >> sum, "Error in line format");//Cmd is transfer, deposit, or withdraw so add sum to command stream.
			//pthread_mutex_lock(&map_lock);//FIXME for testing
			sleep(1);
			bool isDstCurrentlyInMap = bankAccountsMap.isAccountInMap(dstAccountNumber);//FIXME what if we add later here?
			//pthread_mutex_unlock(&map_lock);
			if (isDstCurrentlyInMap == false) {
				LOG_ACCOUNT_DOESNT_EXIST(atmNumber, dstAccountNumber);
				continue;
			}
			try	{
				int srcNewBalance = 0, dstNewBalance = 0, frozenAccount = (-1);
				//pthread_mutex_lock(&write_transfer_lock);//FIXME
				int resTransfer = bankAccountsMap.transferMoneyAndSaveBalances(accountNumber, password,
						dstAccountNumber, sum, &srcNewBalance, &dstNewBalance, &frozenAccount);
				//pthread_mutex_unlock(&write_transfer_lock);
				if (resTransfer != 0) {
					LOG_WRONG_PASSWORD(atmNumber, frozenAccount);
				} else {
					LOG_TRANSFER(atmNumber, sum, accountNumber, dstAccountNumber, srcNewBalance, dstNewBalance);
				}
			} catch (WrongPasswordException&) {
				WRONG_PASSWORD_ILLEGALY();
			} catch (AccountDoesntExistException&) {
				ACCOUNT_DOESNT_EXIST_ILLEGALY();
			} catch (...) {
				UNEXPECTED_EXCEPTION();
			}
		} else {
			//MAP_NOT_TRANSFER_INIT();//FIXME
		}
		if (opCode == 'L') {//Freeze account
			sleep(1);
			try {
				bankAccountsMap.freezeAccount(accountNumber, password);
			} catch (WrongPasswordException&) {
				WRONG_PASSWORD_ILLEGALY();
			} catch (AccountDoesntExistException&) {
				ACCOUNT_DOESNT_EXIST_ILLEGALY();
			} catch (...) {
				UNEXPECTED_EXCEPTION();
			}
		} else if (opCode == 'U') {//UnFreeze account
			sleep(1);
			try {
				bankAccountsMap.unFreezeAccount(accountNumber, password);
			} catch (WrongPasswordException&) {
				WRONG_PASSWORD_ILLEGALY();
			} catch (AccountDoesntExistException&) {
				ACCOUNT_DOESNT_EXIST_ILLEGALY();
			} catch (...) {
				UNEXPECTED_EXCEPTION();
			}
		} else if (opCode == 'D') {//Deposit
			sleep(1);
			try	{
				bankAccountsMap.depositToAccount(accountNumber, password, sum);
				LOG_DEPOSIT(atmNumber, accountNumber, bankAccountsMap.getAccountBalance(accountNumber,password), sum);
			} catch (WrongPasswordException&) {
				//WRONG_PASSWORD(atmNumber, accountNumber);//FIXME illegally
				WRONG_PASSWORD_ILLEGALY();//FIXME illegally
				//continue;
			} catch (AccountDoesntExistException&) {
				ACCOUNT_DOESNT_EXIST_ILLEGALY();
			} catch (BalanceOverflowException&) {
				BALANCE_OVERFLOW_ILLEGALY();
			} catch (...) {
				UNEXPECTED_EXCEPTION();
			}
		} else if (opCode == 'W') {//Withdraw
			sleep(1);
			try	{
				bankAccountsMap.withrawFromAccount(accountNumber, password, sum);
				LOG_WITHDRAW(atmNumber, accountNumber, bankAccountsMap.getAccountBalance(accountNumber,password), sum);
			} catch (WrongPasswordException&) {
				//WRONG_PASSWORD(atmNumber, accountNumber);//FIXME illegally
				WRONG_PASSWORD_ILLEGALY();//FIXME illegally
				//continue;
			} catch (AccountDoesntExistException&) {
				ACCOUNT_DOESNT_EXIST_ILLEGALY();
			} catch (NotEnoughMoneyException&) {
				LOG_NOT_ENOUGH_MONEY(atmNumber, accountNumber, sum);
			} catch (...) {
				UNEXPECTED_EXCEPTION();
			}
		} else if (opCode == 'B') {//Check Balance
			sleep(1);
			try	{
				balance = bankAccountsMap.getAccountBalance(accountNumber, password);
				LOG_BALANCE(atmNumber, accountNumber, balance);
			} catch (WrongPasswordException&) {
				//WRONG_PASSWORD(atmNumber, accountNumber);//FIXME illegally
				WRONG_PASSWORD_ILLEGALY();//FIXME illegally
				//continue;
			} catch (AccountDoesntExistException&) {
				ACCOUNT_DOESNT_EXIST_ILLEGALY();
			} catch (...) {
				UNEXPECTED_EXCEPTION();
			}
		} /*else {
			usleep(100000);//Open or Transfer
			continue;
		}*///FIXME not needed if no readers/writers on transfer
		//MAP_NOT_TRANSFER_END();//Not Open nor Transfer//FIXME
		usleep(100000);
	}
	pthread_exit(NULL);//FIXME why? from Lior
	return NULL;
}

void* Commission_Run(void* cmds) {
	commissionThreadRunning = true;
	int commission, accountNumber, percentage;
	IteratorConst account;
	while(atmThreadRunning) {
		percentage = ((((rand())%(2)) + 2));//y = rand, x = (y(mod2)+2)/100
		for (account = bankAccountsMap.begin(); account != bankAccountsMap.end(); account++) {
			accountNumber = account->first;
			commission = bankAccountsMap.takeComission(accountNumber, percentage);
			pthread_mutex_lock(&bank_balance_lock);
			BankBalance += commission;
			pthread_mutex_unlock(&bank_balance_lock);
			LOG_COMMISSION(percentage, commission, accountNumber);
		}
		sleep(3); //Every 3 seconds commission is taken from all
	}
	commissionThreadRunning = false;
	pthread_exit(NULL);//FIXME why? Lior
	return NULL;
}

void* Continuous_Print_Run(void* cmds) {
	int accountNumber;
	IteratorConst account;
	while(true) {//Continuous printing
		if (atmThreadRunning == false && commissionThreadRunning == false) {
			break;//ATMs threads are done and so is the bank commission thread
		}
		printf("\033[2J");
		printf("\033[1;1H");
		cout << "Current Bank Status" << endl;
		for (account = bankAccountsMap.begin(); account != bankAccountsMap.end(); account++) {
			accountNumber = account->first;
			bankAccountsMap.printAccountInMap(accountNumber);
		}
		pthread_mutex_lock(&bank_balance_lock); // no need for readers/writers because only 1 thread reading and only 1 thread writing
		cout << "The Bank has " << BankBalance << " $" << endl;
		pthread_mutex_unlock(&bank_balance_lock);
		usleep(500000);
	}
	pthread_exit(NULL);//FIXME why? Lior
	return NULL;
}






