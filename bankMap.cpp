/*
 * bankMap.c
 *
 *      Author: Eitan Levin
 */
#include "bankMap.h"
#include <ctime>
#include <cstdlib>

#include <unistd.h>//FIXME for testing
#include <stdio.h>//FIXME testing

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
	return (this->_innerMap[accountNumber]).getBalance();
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
		//Undefined according to pdf, so we do nothing, as this is "success".
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
		//According to pdf do nothing. This is success.
	}
}

/*
 * ****depositToAccont****
 * Deposit sum of money to a given account.
 * Parameters: the account number, the account password, deposit sum.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		BalanceOverflowException if deposit causes int balance overflow.
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
	if (depositResult == ACCOUNT_FROZEN) {//Account is frozen
		throw WrongPasswordException();
	} else if (depositResult == ACCOUNT_BALANCE_OVERFLOW) {//Balance overflow
		throw BalanceOverflowException();
	}
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
	int resWithdraw = this->_innerMap[accountNumber].withrawMoney(withrawSum);
	if(resWithdraw == ACCOUNT_NOT_ENOUGH_MONEY) {//Not enough money
		throw NotEnoughMoneyException();
	} else if(resWithdraw == ACCOUNT_FROZEN) {//Frozen
		throw WrongPasswordException();
	}
}

/*
 * ****transferMoneyAndSaveBalances****
 * Transfer money between given accounts.
 * Parameters: 	source account number, source account password, destination account number, amount to transfer,
 * 				source account balance and destination account balance (to return),
 * 				which of the accounts is frozen, if any (to return).
 * Error Values:
 * 		AccountDoesntExistException if either account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		NotEnoughMoneyException if balance not enough for transfer.
 * Print Requirements: On wrong password, on account doesn't exist, on not enough balance, on successful deposit.
 */
int bankMap::transferMoneyAndSaveBalances(int srcAccountNumber, int srcAccountPass,
		int dstAccountNumber, int amount, int* srcBalance, int* dstBalance, int* frozenAccount) {
	if (this->isAccountInMap(srcAccountNumber) == false || this->isAccountInMap(dstAccountNumber) == false) {
		throw AccountDoesntExistException();
	} else if (this->checkPassword(srcAccountNumber, srcAccountPass) == false) {
		throw WrongPasswordException();
	}
	int firstAccount = (srcAccountNumber >= dstAccountNumber) ? dstAccountNumber : srcAccountNumber;//To prevent deadlocks
	int secondAccount = (srcAccountNumber >= dstAccountNumber) ? srcAccountNumber : dstAccountNumber;//To prevent deadlocks
	this->_innerMap[firstAccount].lockAccount();
	printf("first account: %d \n", firstAccount);//TODO
	sleep(3);//FIXME for testing
	if(this->_innerMap[firstAccount].transferIsFrozen()) {
		*frozenAccount = firstAccount;
		this->_innerMap[firstAccount].unLockAccount();
		printf("unlocked src account\n");//TODO
		return FIRST_ACCOUNT_FROZEN; //first is frozen
	}
	if (srcAccountNumber == dstAccountNumber) {//Definitely not frozen
		*srcBalance = this->_innerMap[firstAccount].transferCheckBalance();
		*dstBalance = *srcBalance;
		this->_innerMap[firstAccount].unLockAccount();
		return SAME_ACCOUNT;//Same account. Legal, but acknowledge this
	}
	this->_innerMap[secondAccount].lockAccount();
	printf("secondAccount: %d \n", firstAccount);//TODO
	sleep(5);//FIXME for testing
	if(this->_innerMap[secondAccount].transferIsFrozen()) {
		this->_innerMap[firstAccount].unLockAccount();
		this->_innerMap[secondAccount].unLockAccount();
		printf("unlocked dst account\n");//TODO
		*frozenAccount = secondAccount;
		return SECOND_ACCOUNT_FROZEN; //second is frozen
	}
	this->_innerMap[srcAccountNumber].transferWithdraw(amount);//FIXME check enough money
	this->_innerMap[dstAccountNumber].transferDeposit(amount);
	*srcBalance = this->_innerMap[srcAccountNumber].transferCheckBalance();
	*dstBalance = this->_innerMap[dstAccountNumber].transferCheckBalance();
	this->_innerMap[secondAccount].unLockAccount();
	printf("secondAccount unlocked: %d \n", firstAccount);//TODO
	this->_innerMap[firstAccount].unLockAccount();
	printf("firstAccount unlocked: %d \n", firstAccount);//TODO
	return TRANSFER_SUCCESSFUL;
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

IteratorConst bankMap::begin() {
	return (this->_innerMap).begin();
}

IteratorConst bankMap::end() {
	return (this->_innerMap).end();
}

void bankMap::printAccountInMap(int accountNumber) {
	this->_innerMap[accountNumber].printAccount();
}

