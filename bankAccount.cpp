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

int bankAccount::getNumber() {//No need for locks, once account is created, number never changes.
	return this->_id;
}

int bankAccount::getPassword() {//No need for locks, once account is created, password never changes.
	return this->_password;
}

int bankAccount::getBalance() {
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter++;//Indicate 1 more thread is reading
	if (readBalanceCounter == 1) {//First reader
		pthread_mutex_lock(&write_balance_lock);
	}
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	int currentBalance = _balance;//No need to be locked, all readers can pull together. Writers are locked.
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
}

bool bankAccount::withrawMoney(int withrawSum) {
	assert(isAccountFrozen() == false);
	if (withrawSum > (this->getBalance())) {
		return false;
	} else {
		pthread_mutex_lock(&write_balance_lock);
		this->_balance -= withrawSum;
		pthread_mutex_unlock(&write_balance_lock);
		return true;
	}
}

bool bankAccount::depositMoney(int depositSum) {
	assert(((depositSum + this->_balance) > (this->getBalance()))  && (isAccountFrozen() == false));//make sure no overflow
	if (((depositSum + this->_balance) < (this->getBalance())) || (isAccountFrozen() == true)) {//int overflow, Should never happen
		return false;
	} else {
		pthread_mutex_lock(&write_balance_lock);
		this->_balance += depositSum;
		pthread_mutex_unlock(&write_balance_lock);
		return true;
	}
}

void bankAccount::printAccount() {
	cout << "Account " << this->_id << ": Balance - " << this->getBalance() <<
			"$ , Account Password - " << this->_password << endl;
}

void bankAccount::lockAccount() {
	pthread_mutex_lock(&write_balance_lock);
	pthread_mutex_lock(&read_balance_lock);//Lock readers
}

void bankAccount::unLockAccount() {
	pthread_mutex_unlock(&read_balance_lock);//Lock readers
	pthread_mutex_unlock(&write_balance_lock);
}
bool bankAccount::transferWithdraw(int withrawSum) {
	if (withrawSum > (this->_balance)) {
		return false;
	} else {
		this->_balance -= withrawSum;
		return true;
	}
}

bool bankAccount::transferDeposit(int depositSum) {
	assert((depositSum + this->_balance) > (this->getBalance()));//make sure no overflow
	if ((depositSum + this->_balance) < (this->getBalance())) {//int overflow, Should never happen
		return false;
	} else {
		this->_balance += depositSum;
		return true;
	}
}

int bankAccount::transferCheckBalance() {
	return this->_balance;
}

bankAccount::~bankAccount() {
	pthread_mutex_destroy(&read_balance_lock);
	pthread_mutex_destroy(&write_balance_lock);
	pthread_mutex_destroy(&read_freeze_lock);
	pthread_mutex_destroy(&write_freeze_lock);
}
