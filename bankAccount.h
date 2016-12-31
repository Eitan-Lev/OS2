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
	int readBalanceCounter;
	int readFreezeCounter;
	pthread_mutex_t read_balance_lock;
	pthread_mutex_t write_balance_lock;
	pthread_mutex_t read_freeze_lock;
	pthread_mutex_t write_freeze_lock;
public:
	bankAccount(int accountNumber, int accountPass, int balance, bool isFrozen = false) :
		_id(accountNumber), _password(accountPass), _balance (balance), _isFrozen(isFrozen),
		readBalanceCounter(0) , readFreezeCounter(0) {
		pthread_mutex_init(&read_balance_lock, NULL); //mutex is initialized as unlocked
		pthread_mutex_init(&write_balance_lock, NULL);
		pthread_mutex_init(&read_freeze_lock, NULL);
		pthread_mutex_init(&write_freeze_lock, NULL);
	}
	//bankAccount(const bankAccount &obj);  // copy constructor
	//bankAccount(const bankAccount &obj) : this->_id(obj.getAccountNumber()), this->_password(obj.getAccountPassword()), this->_balance(obj.getAccountBalance()), this->_isFrozen(obj.isAccountFrozen()) {}
	//bankAccount(const bankAccount& obj) : _id(obj.getAccountNumber()), _password(obj.getAccountPassword()), _balance(obj.getAccountBalance()), _isFrozen(obj.isAccountFrozen()) {};
	int getNumber();
	int getPassword();
	int getBalance();
	bool isAccountFrozen();//True if frozen, false if not frozen
	bool freeze();//False if already frozen, true otherwise
	bool unFreeze();//False if already not frozen, true otherwise
	bool withrawMoney(int withrawSum);//False if not enough money, true otherwise
	bool depositMoney(int depositSum);//False if sum causes int overflow, true otherwise
	void printAccount();
	~bankAccount();
};



#endif /* BANKACCOUNT_H_ */
