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
	unsigned int _id;
	int _password;
	unsigned int _balance;
	bool _isFrozen;
	int readBalanceCounter;
	int readFreezeCounter;
	pthread_mutex_t read_balance_lock;
	pthread_mutex_t write_balance_lock;
	pthread_mutex_t read_freeze_lock;
	pthread_mutex_t write_freeze_lock;
public:
	bankAccount(unsigned int accountNumber, int accountPass, unsigned int balance, bool isFrozen = false) :
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
	unsigned int getNumber();
	int getPassword();
	unsigned int getBalance();
	bool isAccountFrozen();//True if frozen, false if not frozen
	bool freeze();//False if already frozen, true otherwise
	bool unFreeze();//False if already not frozen, true otherwise
	bool withrawMoney(unsigned int withrawSum);//False if not enough money, true otherwise
	bool depositMoney(unsigned int depositSum);//False if sum causes unsigned int overflow, true otherwise
	void printAccount();
	~bankAccount();
};



#endif /* BANKACCOUNT_H_ */
