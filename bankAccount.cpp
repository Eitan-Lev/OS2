/*
 * bankAccount.cpp
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */
#include "bankAccount.h"
#include <iostream>
#include <assert.h>
#include "MacroBank.h"

using std::cout;
using std::endl;

int bankAccount::getNumber() {//No need for locks, once account is created, number never changes.
	return this->_id;
}

int bankAccount::getPassword() {//No need for locks, once account is created, password never changes.
	return this->_password;
}

int bankAccount::getBalance() {
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter++;//Indicate 1 more thread is reading
	if (readBalanceCounter == 1) {//First reader
		pthread_mutex_lock(&write_balance_lock);
	}
	sleep(1);
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	int currentBalance = _balance;//No need to be locked, all readers can pull together. Writers are locked.
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter--;//Indicate 1 less thread is reading
	if (readBalanceCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_balance_lock);
	}
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	NOT_TRANSFER_FUNC_END();
	return currentBalance;
}

int bankAccount::getBalanceNoSleep() {
	NOT_TRANSFER_FUNC_INIT();
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
	NOT_TRANSFER_FUNC_END();
	return currentBalance;
}

bool bankAccount::isAccountFrozen() {
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter++;//Indicate 1 more thread is reading
	if (readFreezeCounter == 1) {//First reader
		pthread_mutex_lock(&write_freeze_lock);
	}
	pthread_mutex_unlock(&read_freeze_lock);//Unlock readers
	bool currentFreezeStatus = _isFrozen;//No need to be locked, all readers can pull together. Writers are locked.
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter--;//Indicate 1 less thread is reading
	if (readFreezeCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_freeze_lock);
	}
	pthread_mutex_unlock(&read_freeze_lock);//Unlock readers
	NOT_TRANSFER_FUNC_END();
	return currentFreezeStatus;
}

bool bankAccount::freeze() {
	bool resFreeze;
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&write_freeze_lock);
	if (this->_isFrozen == true) {
		resFreeze = false;
	} else {
		this->_isFrozen = true;
	}
	pthread_mutex_unlock(&write_freeze_lock);
	NOT_TRANSFER_FUNC_END();
	return resFreeze;
}
bool bankAccount::unFreeze() {
	bool resFreeze;
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&write_freeze_lock);
	if (this->_isFrozen == false) {
		resFreeze = false;
	} else {
		this->_isFrozen = false;
	}
	pthread_mutex_unlock(&write_freeze_lock);
	NOT_TRANSFER_FUNC_END();
	return resFreeze;
}

int bankAccount::withrawMoney(int withrawSum) {
	int tmpBalance, resVar;
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter++;//Indicate 1 more thread is reading
	if (readFreezeCounter == 1) {//First reader
		pthread_mutex_lock(&write_freeze_lock);
	}
	sleep(1);
	pthread_mutex_unlock(&read_freeze_lock);
	if (this->_isFrozen == true) {
		resVar = ACCOUNT_FROZEN;
	} else {
		pthread_mutex_lock(&write_balance_lock);
		tmpBalance = this->_balance;
		if (withrawSum > tmpBalance) {
			resVar = ACCOUNT_NOT_ENOUGH_MONEY;
		} else {
			this->_balance -= withrawSum;
			resVar = ACCOUNT_SUCCESS;
		}
		//this->_balance -= withrawSum;
		pthread_mutex_unlock(&write_balance_lock);
		//resVar = ACCOUNT_SUCCESS;
	}
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter--;//Indicate 1 less thread is reading
	if (readFreezeCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_freeze_lock);
	}
	pthread_mutex_unlock(&read_freeze_lock);//Unlock readers
	NOT_TRANSFER_FUNC_END();
	return resVar;
}

int bankAccount::withrawMoneyForCommission(int percentage) {
	NOT_TRANSFER_FUNC_INIT();
	int commission;
	pthread_mutex_lock(&write_balance_lock);
	commission = (this->_balance * percentage)/100;
	if (commission > this->_balance) {//Should never happen. For debugging.
		exit(ERROR_VALUE);
	}
	this->_balance -= commission;
	pthread_mutex_unlock(&write_balance_lock);
	NOT_TRANSFER_FUNC_END();
	return commission;
}

int bankAccount::depositMoney(int depositSum) {
	int tmpBalance, resVar;
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter++;//Indicate 1 more thread is reading
	if (readFreezeCounter == 1) {//First reader
		pthread_mutex_lock(&write_freeze_lock);
	}
	sleep(1);
	pthread_mutex_unlock(&read_freeze_lock);
	if (this->_isFrozen == true) {
		resVar = ACCOUNT_FROZEN;
	} else {
		pthread_mutex_lock(&write_balance_lock);
		tmpBalance = this->_balance;
		if ((depositSum + tmpBalance) < tmpBalance) {//int overflow, Should never happen
			resVar = ACCOUNT_BALANCE_OVERFLOW;//Overflow Failure
		} else {
			this->_balance += depositSum;
			resVar = ACCOUNT_SUCCESS;
		}
		//this->_balance += depositSum;
		pthread_mutex_unlock(&write_balance_lock);
		//resVar = ACCOUNT_SUCCESS;
	}
	pthread_mutex_lock(&read_freeze_lock);//Lock readers
	readFreezeCounter--;//Indicate 1 less thread is reading
	if (readFreezeCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_freeze_lock);
	}
	pthread_mutex_unlock(&read_freeze_lock);//Unlock readers
	NOT_TRANSFER_FUNC_END();
	return resVar;
}

void bankAccount::printAccount() {
	NOT_TRANSFER_FUNC_INIT();
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter++;//Indicate 1 more thread is reading
	if (readBalanceCounter == 1) {//First reader
		pthread_mutex_lock(&write_balance_lock);
	}
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	int currentBalance = this->_balance;//No need to be locked, all readers can pull together. Writers are locked.
	pthread_mutex_lock(&read_balance_lock);//Lock readers
	readBalanceCounter--;//Indicate 1 less thread is reading
	if (readBalanceCounter == 0) {//Last to read
		pthread_mutex_unlock(&write_balance_lock);
	}
	pthread_mutex_unlock(&read_balance_lock);//Unlock readers
	cout << "Account " << this->_id << ": Balance - " << currentBalance <<
			"$ , Account Password - " << this->_password << endl;
	NOT_TRANSFER_FUNC_END();
}

void bankAccount::lockAccount() {
	pthread_mutex_lock(&write_account_lock);
}

void bankAccount::unLockAccount() {
	pthread_mutex_unlock(&write_account_lock);
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
	if ((depositSum + this->_balance) < (this->_balance)) {//int overflow, Should never happen, for debugging.
		std::cout << "Overflow" << std::endl;
		exit(ERROR_VALUE);
	} else {
		this->_balance += depositSum;
		return true;
	}
}

int bankAccount::transferCheckBalance() {
	return this->_balance;
}

bool bankAccount::transferIsFrozen() {
	return this->_isFrozen;
}

bankAccount::~bankAccount() {
	pthread_mutex_destroy(&read_balance_lock);
	pthread_mutex_destroy(&write_balance_lock);
	pthread_mutex_destroy(&read_freeze_lock);
	pthread_mutex_destroy(&write_freeze_lock);
	pthread_mutex_destroy(&read_account_lock);
	pthread_mutex_destroy(&write_account_lock);
}
