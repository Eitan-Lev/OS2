/*
 * bankAccount.cpp
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */
#include "bankAccount.h"
#include <iostream>

using std::cout;
using std::endl;

/*bankAccount bankAccount::bankAccount(const bankAccount &obj) {
	bankAccount newAccount(obj._id, obj._password, obj._balance);
	return newAccount;
}*/

unsigned int bankAccount::getAccountNumber() {
	return this->_id;
}

int bankAccount::getAccountPassword() {
	return this->_password;
}

unsigned int bankAccount::getAccountBalance() {
	return this->_balance;
}

bool bankAccount::isAccountFrozen() {
	return this->_isFrozen;
}

bool bankAccount::freeze() {
	if (this->_isFrozen == true) {
		return false;
	} else {
		this->_isFrozen = true;
		return true;
	}
}

bool bankAccount::unFreeze() {
	if (this->_isFrozen == false) {
		return false;
	} else {
		this->_isFrozen = false;
		return true;
	}
}

bool bankAccount::withrawMoney(unsigned int withrawSum) {
	if (withrawSum > this->_balance) {
		return false;
	} else {
		this->_balance -= withrawSum;
		return true;
	}
}

bool bankAccount::depositMoney(unsigned int depositSum) {
	if ((depositSum + this->_balance) < (this->_balance)) {//unsigned int overflow
		return false;
	} else {
		this->_balance += depositSum;
		return true;
	}
}

void bankAccount::printAccount() {
	cout << "Account " << this->_id << ": Balance - " << this->_balance << "$ , Account Password - " << this->_password << endl;
}
