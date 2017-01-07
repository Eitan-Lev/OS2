/*
 * bankMap.c
 *
 *      Author: Eitan Levin
 */
#include "bankMap.h"
#include <ctime>
#include <cstdlib>

#include <stdio.h>//FIXME testing

//TODO: Add a Macro for checking if the account exists and if the password is correct.

/*
 * ****openNewAccount****
 * Open a new account.
 * Parameters: the account number, the account password, the account balance.
 * Error Values:
 * 		AccountNumberAlreadyExistsException if account already exists in map.
 * 		InvalidPasswordException if password is not 4 digits.
 * Print Requirements: On account already exists, on account open success.
 */
void bankMap::openNewAccount(int accountNumber, int accountPass, int balance) {
	if (this->isAccountInMap(accountNumber) == true) {
		throw AccountNumberAlreadyExistsException();
	}
	bankAccount newAccount(accountNumber, accountPass, balance);
	Pair newPair(accountNumber, newAccount);
	this->_innerMap.insert(newPair);
}

/*
 * ****getAccountBalance****
 * Return the account balance.
 * Parameters: the account number, the account password.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * Return Values: the account balance.
 * Print Requirements: On wrong password, on account doesn't exist, on success.
 */
int bankMap::getAccountBalance(int accountNumber, int accountPass) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	if(this->checkPassword(accountNumber, accountPass == false)) {
		throw WrongPasswordException();
	}
	return (this->_innerMap[accountNumber]).getBalance();//FIXME Amit: why?
}

/*
 * ****freezeAccount****
 * Freeze a given account.
 * Parameters: the account number, the account password.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * Print Requirements: On wrong password, on account doesn't exist.
 */
void bankMap::freezeAccount(int accountNumber, int accountPass) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	if(this->checkPassword(accountNumber, accountPass) == false) {
		throw WrongPasswordException();
	}
	bool result = this->_innerMap[accountNumber].freeze();
	if(result == false) {
		//Undefined according to pdf. We should decide TODO
	}
}

/*
 * ****unFreezeAccount****
 * UnFreeze a given account.
 * Parameters: the account number, the account password.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * Print Requirements: On wrong password, on account doesn't exist.
 */
void bankMap::unFreezeAccount(int accountNumber, int accountPass) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	if(this->checkPassword(accountNumber, accountPass) == false) {
		throw WrongPasswordException();
	}
	bool result = this->_innerMap[accountNumber].unFreeze();
	if(result == false) {
		//According to pdf do nothing. Do we want/need to know it wasn't frozen? TODO
	}
}

/*
 * ****depositToAccont****
 * Deposit sum of money to a given account.
 * Parameters: the account number, the account password, deposit sum.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		BalanceOverflowException if deposit causes int balance overflow. //TODO do you really think it is possible?
 * Print Requirements: On wrong password, on account doesn't exist, on successful deposit.
 */
void bankMap::depositToAccount(int accountNumber, int accountPass, int depositSum) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	if(this->checkPassword(accountNumber, accountPass) == false || this->_innerMap[accountNumber].isAccountFrozen()) {
		throw WrongPasswordException();
	}
	int depositResult = this->_innerMap[accountNumber].depositMoney(depositSum);
	if (depositResult == 0) {//Account is frozen
		throw WrongPasswordException();
	} else if (depositResult == 2) {//Balance overflow
		throw BalanceOverflowException();
	}
	/*if(this->_innerMap[accountNumber].depositMoney(depositSum) == false) {
		throw BalanceOverflowException();
	}*/
}

/*
 * ****withrawFromAccont****
 * Withdraw sum of money from a given account.
 * Parameters: the account number, the account password, withdraw sum.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		NotEnoughMoneyException if balance not enough for withdrawal.
 * Print Requirements: On wrong password, on account doesn't exist, on not enough balance, on successful deposit.
 */
void bankMap::withrawFromAccount(int accountNumber, int accountPass, int withrawSum) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	if(this->checkPassword(accountNumber, accountPass) == false || this->_innerMap[accountNumber].isAccountFrozen()) {
		throw WrongPasswordException();
	}
	if(this->_innerMap[accountNumber].withrawMoney(withrawSum) == false) {
		throw NotEnoughMoneyException();
	}
}

/*
 * ****transferMoney****
 * Transfer money between given accounts.
 * Parameters: source account number, source account password, destination account number, amount to transfer.
 * Error Values:
 * 		AccountDoesntExistException if either account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		NotEnoughMoneyException if balance not enough for transfer.
 * Print Requirements: On wrong password, on account doesn't exist, on not enough balance, on successful deposit.
 */
int bankMap::transferMoney(int srcAccountNumber, int srcAccountPass, int destAccountNumber, int amount) {
	if (this->isAccountInMap(srcAccountNumber) == false || this->isAccountInMap(destAccountNumber) == false) {
		throw AccountDoesntExistException();
	}
	if(this->checkPassword(srcAccountNumber, srcAccountPass) == false) {
		throw WrongPasswordException();
	}
	if (srcAccountNumber == destAccountNumber) {
		return 4;
	}
	//this->_innerMap[srcAccountNumber].transferWithdraw(amount);//FIXME testing
	//this->_innerMap[destAccountNumber].transferDeposit(amount);//FIXME testing
	this->_innerMap[srcAccountNumber].lockAccount();
	if(this->_innerMap[srcAccountNumber].isAccountFrozen()) {
		this->_innerMap[srcAccountNumber].unLockAccount();
		return 1; //SRC is frozen
	}
	this->_innerMap[destAccountNumber].lockAccount();
	if(this->_innerMap[destAccountNumber].isAccountFrozen()) {
		this->_innerMap[srcAccountNumber].unLockAccount();
		this->_innerMap[destAccountNumber].unLockAccount();
		return 2; //DST is frozen
	}
	printf("check transfer\n");//FIXME testing
	this->_innerMap[srcAccountNumber].transferWithdraw(amount);//FIXME check enough money
	this->_innerMap[destAccountNumber].transferDeposit(amount);
	this->_innerMap[destAccountNumber].unLockAccount();
	this->_innerMap[srcAccountNumber].unLockAccount();
	return 0;
}

/*
 * ****takeComission****
 * Take commission from given account.
 * Parameters: the account number.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * Print Requirements: if account dosen't exist.
 * Return Values: the commission taken.
 */
int bankMap::takeComission(int accountNumber, int percentage) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	//NEEDS TO BE ATOMIC IN bankAccount.cpp Note the percentage should be decided in bank.cpp and pass
	//As an argument to this function, as well as to the bankAccount function TODO
	int currBalance = this->_innerMap[accountNumber].getBalance();
	int commission = (currBalance * percentage)/100;
	this->_innerMap[accountNumber].withrawMoney(commission);
	return commission;
}

/*
 * ****getPassword****
 * Get a given account password.
 * Parameters: the account number.
 * Error Values:
 * 		AccountDoesntExistException if the account doesn't exist in map.
 * Return Values: account password.
 */
int bankMap::getPassword(int accountNumber) {
	if (this->isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	return this->_innerMap[accountNumber].getPassword();
}

/*
 * ****checkPassword****
 * Check if the account with "accountNumber" has the password "accountPass"
 * Parameters: the account number, the password.
 * Return Values: true- if password matches.
 *				  false- otherwise.
 */
bool bankMap::checkPassword(int accountNumber, int accountPass) {
	int realPass = this->getPassword(accountNumber);
	return (realPass == accountPass) ? true : false;
}

/*
 * ****isAccountInMap****
 * Check if given account exists in map.
 * Parameters: the account number.
 * Return Values: true if exists, false if doesn't.
 */
bool bankMap::isAccountInMap(int accountNumber) {
	if(this->_innerMap.find(accountNumber) == this->_innerMap.end()) {
		return false;
	}
	return true;
}

int bankMap::transferGetBalance(int accountNumber) {//Use only for money transfer!//TODO
	return this->_innerMap[accountNumber].transferCheckBalance();
}

IteratorConst bankMap::begin() {
	return (this->_innerMap).begin();
}

IteratorConst bankMap::end() {
	return (this->_innerMap).end();
}

void bankMap::printAccountInMap(int accountNumber) {
	this->_innerMap[accountNumber].printAccount();
}

