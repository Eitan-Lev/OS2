/*
 * bankAccount.cpp
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */
#include "bankAccount.h"
#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;

unsigned int bankAccount::getNumber() {//No need for locks, once account is created, number never changes.
	return this->_id;
}

int bankAccount::getPassword() {//No need for locks, once account is created, password never changes.
	return this->_password;
}

unsigned int bankAccount::getBalance() {
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter++;//Indicate 1 more thread is reading
	if (readBalanceCounter == 1) {//First reader
		pthread_mutex_lock(&write_balance_lock);
	}
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	unsigned int currentBalance = _balance;//No need to be locked, all readers can pull together. Writers are locked.
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter--;//Indicate 1 less thread is reading
	if (readBalanceCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_balance_lock);
	}
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	return currentBalance;
}

bool bankAccount::isAccountFrozen() {
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter++;//Indicate 1 more thread is reading
	if (readFreezeCounter == 1) {//First reader
		pthread_mutex_lock(&write_freeze_lock);
	}
	pthread_mutex_unlock(&read_freeze_lock);//Unlock rseaders
	bool currentFreezeStatus = _isFrozen;//No need to be locked, all readers can pull together. Writers are locked.
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter--;//Indicate 1 less thread is reading
	if (readFreezeCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_freeze_lock);
	}
	pthread_mutex_unlock(&read_freeze_lock);//Unlock readers
	return currentFreezeStatus;
}

bool bankAccount::freeze() {
	if (isAccountFrozen() == true) {
		return false;
	} else {
		pthread_mutex_lock(&write_freeze_lock);
		this->_isFrozen = true;
		pthread_mutex_unlock(&write_freeze_lock);
		return true;
	}
	/*if (this->_isFrozen == true) {
		return false;
	} else {
		this->_isFrozen = true;
		return true;
	}*/
	//TODO
}

bool bankAccount::unFreeze() {
	if (isAccountFrozen() == false) {
		return false;
	} else {
		pthread_mutex_lock(&write_freeze_lock);
		this->_isFrozen = true;
		pthread_mutex_unlock(&write_freeze_lock);
		return true;
	}
	/*if (this->_isFrozen == false) {
		return false;
	} else {
		this->_isFrozen = false;
		return true;
	}*/
	//TODO
}

bool bankAccount::withrawMoney(unsigned int withrawSum) {
	if (withrawSum > (this->getBalance())) {
		return false;
	} else {
		pthread_mutex_lock(&write_balance_lock);
		this->_balance -= withrawSum;
		pthread_mutex_unlock(&write_balance_lock);
		return true;
	}
	/*if (withrawSum > this->_balance) {
		return false;
	} else {
		this->_balance -= withrawSum;
		return true;
	}*/
	//TODO
}

bool bankAccount::depositMoney(unsigned int depositSum) {
	assert((depositSum + this->_balance) > (this->getBalance()));//make sure no overflow
	if ((depositSum + this->_balance) < (this->getBalance())) {//unsigned int overflow, Should never happen
		return false;
	} else {
		pthread_mutex_lock(&write_balance_lock);
		this->_balance += depositSum;
		pthread_mutex_unlock(&write_balance_lock);
		return true;
	}
	/*if ((depositSum + this->_balance) < (this->_balance)) {//unsigned int overflow
		return false;
	} else {
		this->_balance += depositSum;
		return true;
	}*/
	//TODO
}

void bankAccount::printAccount() {
	cout << "Account " << this->_id << ": Balance - " << this->getBalance() <<
			"$ , Account Password - " << this->_password << endl;
}

bankAccount::~bankAccount() {
	pthread_mutex_destroy(&read_balance_lock);
	pthread_mutex_destroy(&write_balance_lock);
	pthread_mutex_destroy(&read_freeze_lock);
	pthread_mutex_destroy(&write_freeze_lock);
}
