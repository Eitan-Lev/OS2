/*
 * bankAccount.h
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */

#ifndef BANKACCOUNT_H_
#define BANKACCOUNT_H_

#include <pthread.h>

/*
 * Here will be all the simplest of functions.
 * This Data Structure assumes trust between it and the user.
 * Only parameters validation is required.
 */

class bankAccount {
private:
	int _id;
	int _password;
	int _balance;
	bool _isFrozen;
	/*pthread_mutex_t read_lock;
	pthread_mutex_t write_lock;*/
public:
	int readBalanceCounter;
	int readFreezeCounter;
	int readAccountCounter;
	pthread_mutex_t read_balance_lock;//FIXME is it ok to make it public?
	pthread_mutex_t write_balance_lock;
	pthread_mutex_t read_freeze_lock;
	pthread_mutex_t write_freeze_lock;
	pthread_mutex_t read_account_lock;
	pthread_mutex_t write_account_lock;
	//Empty Constructor:
	bankAccount() : _id(0), _password(0), _balance (0),
		_isFrozen(false), readBalanceCounter(0), readFreezeCounter(0) , readAccountCounter(0){
		pthread_mutex_init(&read_balance_lock, NULL); //mutex is initialized as unlocked
		pthread_mutex_init(&write_balance_lock, NULL);
		pthread_mutex_init(&read_freeze_lock, NULL);
		pthread_mutex_init(&write_freeze_lock, NULL);
		pthread_mutex_init(&read_account_lock, NULL);
		pthread_mutex_init(&write_account_lock, NULL);
	}
	//Constructor:
	bankAccount(int accountNumber, int accountPass, int balance, bool isFrozen = false) :
		_id(accountNumber), _password(accountPass), _balance (balance), _isFrozen(isFrozen),
		readBalanceCounter(0), readFreezeCounter(0), readAccountCounter(0) {
		/*pthread_mutex_init(&read_lock, NULL); //mutex is initialized as unlocked
		pthread_mutex_init(&write_lock, NULL);*/
		pthread_mutex_init(&read_balance_lock, NULL); //mutex is initialized as unlocked
		pthread_mutex_init(&write_balance_lock, NULL);
		pthread_mutex_init(&read_freeze_lock, NULL);
		pthread_mutex_init(&write_freeze_lock, NULL);
		pthread_mutex_init(&read_account_lock, NULL);
		pthread_mutex_init(&write_account_lock, NULL);
	}
	//Copy Constructor:
	/*bankAccount(const bankAccount& obj) {
		this->_id = obj._id;
		this->_password = obj._password;
		this->_balance = obj._balance;
		this->_isFrozen = obj._isFrozen;
		this->readBalanceCounter = obj.readBalanceCounter;
		this->readFreezeCounter = obj.readFreezeCounter;
		pthread_mutex_init(&read_balance_lock, NULL); //mutex is initialized as unlocked
		pthread_mutex_init(&write_balance_lock, NULL);
		pthread_mutex_init(&read_freeze_lock, NULL);
		pthread_mutex_init(&write_freeze_lock, NULL);
	}*/
	int getNumber();
	int getPassword();
	int getBalance();
	bool isAccountFrozen();//True if frozen, false if not frozen
	bool freeze();//False if already frozen, true otherwise
	bool unFreeze();//False if already not frozen, true otherwise
	int withrawMoney(int withrawSum);//False if not enough money or account frozen, true otherwise
	int depositMoney(int depositSum);//False if sum causes unsigned int overflow or account frozen, true otherwise
	void printAccount();//Is it possible it won't be printed continuously? //TODO
	void lockAccount();//Use only for money transfer!
	void unLockAccount();//Use only for money transfer!
	bool transferWithdraw(int withrawSum);//Use only for money transfer and only after lockAccount!
	bool transferDeposit(int depositSum);//Use only for money transfer and only after lockAccount!
	int transferCheckBalance();//Use only for money transfer and only after lockAccount!
	bool transferIsFrozen();//TODO
	~bankAccount();
};



#endif /* BANKACCOUNT_H_ */
