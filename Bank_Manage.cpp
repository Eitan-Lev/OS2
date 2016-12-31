/*
 * Bank_Manage.c
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */
#include "Bank_Manage.h"

/*
 * ****openNewAccount****
 * Open a new account.
 * Parameters: the account number, the account password, the account balance.
 * Error Values:
 * 		AccountNumberAlreadyExistsException if account already exists in map.
 * 		InvalidPasswordException if password is not 4 digits.
 * Print Requirements: On account already exists, on account open success.
 */
void openNewAccount(BankMap bankAccountsMap, unsigned int accountNumber, int accountPass, unsigned int balance) {
	if (isAccountInMap(accountNumber) == true) {
		throw AccountNumberAlreadyExistsException();
	} else {
		//assert password is 4 digits, throw otherwise. FIXME should never happen
		//throw InvalidPasswordException;
	}
	bankAccount newAccount(accountNumber, accountPass, balance);
	Pair newPair(accountNumber, newAccount);
	bankAccountsMap.insert(newPair);
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
unsigned int getAccountBalance(BankMap bankAccountsMap, unsigned int accountNumber, int accountPass) {
	if (isAccountInMap(accountNumber) == false) {
		throw AccountDoesntExistException();
	}
	bankAccount tmpAccount((bankAccountsMap.find(accountNumber))->second);
	if (accountPass != tmpAccount.getPassword()) {
		throw WrongPasswordException();
	}
	return tmpAccount.getBalance();
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
void freezeAccount(unsigned int accountNumber, int accountPass) {

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
void unFreezeAccount(unsigned int accountNumber, int accountPass) {

}

/*
 * ****depositToAccont****
 * Deposit sum of money to a given account.
 * Parameters: the account number, the account password, deposit sum.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		BalanceOverflowException if deposit causes unsigned int balance overflow.
 * Print Requirements: On wrong password, on account doesn't exist, on successful deposit.
 */
bool depositToAccont(unsigned int accountNumber, int accountPass, unsigned int depositSum) {

	return true;
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
bool withrawFromAccont(unsigned int accountNumber, int accountPass, unsigned int withrawSum) {

	return true;
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
void transferMoney(unsigned int srcAccountNumber, int srcAccountPass, unsigned int DestAccountNumber, unsigned int amount) {

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
unsigned int takeComission(unsigned int accountNumber) {

	return 0;
}

/*
 * ****getPassword****
 * Get a given account password.
 * Parameters: the account number.
 * Error Values:
 * 		AccountNumberAlreadyExistsException if account already exists in map.
 * 		InvalidPasswordException if password is not 4 digits.
 * Return Values: account password.
 * --------Note: Helper function, isn't required.--------
 */
int getPassword(unsigned int accountNumber, int accountPass) {

	return 0;
}

/*
 * ****isAccountInMap****
 * Check if given account exists in map.
 * Parameters: the account number.
 * Return Values: true if exists, false if doesn't.
 * --------Note: Helper function, isn't required.--------
 */
bool isAccountInMap(unsigned int accountNumber) {

	return true;
}

