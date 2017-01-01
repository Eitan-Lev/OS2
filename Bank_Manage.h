/*
 * Bank_Manage.h
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */

#ifndef BANK_MANAGE_H_
#define BANK_MANAGE_H_

#include <map>
#include <stdlib.h>
#include "bankAccount.h"
#include "Exceptions.h"

/*
 * In this file all the top level functions will be.
 * Any validation of password or otherwise is done here.
 * All actions are implemented here.
 */

using std::map;
typedef map<int, bankAccount>BankMap;
typedef BankMap::value_type Pair;
typedef BankMap::iterator Iterator;

//This is the map for the bank accounts:
//BankMap bankAccountsMap;//TODO beware of duplication

#define SRC_ACCOUNT 1
#define DST_ACCOUNT 2


/*
 * ****openNewAccount****
 * Open a new account.
 * Parameters: the account number, the account password, the account balance.
 * Error Values:
 * 		AccountNumberAlreadyExistsException if account already exists in map.
 * 		InvalidPasswordException if password is not 4 digits.
 * Print Requirements: On account already exists, on account open success.
 */
void openNewAccount(BankMap bankAccountsMap, unsigned int accountNumber, int accountPass, unsigned int balance);

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
unsigned int getAccountBalance(BankMap bankAccountsMap, unsigned int accountNumber, int accountPass);

/*
 * ****freezeAccount****
 * Freeze a given account.
 * Parameters: the account number, the account password.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * Print Requirements: On wrong password, on account doesn't exist.
 */
void freezeAccount(unsigned int accountNumber, int accountPass);

/*
 * ****unFreezeAccount****
 * UnFreeze a given account.
 * Parameters: the account number, the account password.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * Print Requirements: On wrong password, on account doesn't exist.
 */
void unFreezeAccount(unsigned int accountNumber, int accountPass);

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
bool depositToAccont(unsigned int accountNumber, int accountPass, unsigned int depositSum);

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
bool withrawFromAccont(unsigned int accountNumber, int accountPass, unsigned int withrawSum);

/*
 * ****transferMoney****
 * Transfer money between given accounts.
 * Parameters: source account number, source account password, destination account number, amount to transfer.
 * Error Values:
 * 		AccountDoesntExistException if either account doesn't exist in map.
 * 		WrongPasswordException if password is not the correct one.
 * 		NotEnoughMoneyException if balance not enough for transfer.
 * Print Requirements: On wrong password, on account doesn't exist, on not enough balance, on successful deposit.
 * --------Note: if Dest account if frozen, should return wrong password of the frozen one message--------//TODO
 */
void transferMoney(unsigned int srcAccountNumber, int srcAccountPass, unsigned int DestAccountNumber, unsigned int amount);

/*
 * ****takeComission****
 * Take commission from given account.
 * Parameters: the account number.
 * Error Values:
 * 		AccountDoesntExistException if account doesn't exist in map.
 * Print Requirements: if account dosen't exist.
 * Return Values: the commission taken.
 */
unsigned int takeComission(unsigned int accountNumber);

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
int getPassword(unsigned int accountNumber, int accountPass);

/*
 * ****isAccountInMap****
 * Check if given account exists in map.
 * Parameters: the account number.
 * Return Values: true if exists, false if doesn't.
 * --------Note: Helper function, isn't required.--------
 */
bool isAccountInMap(unsigned int accountNumber);

#endif /* BANK_MANAGE_H_ */





/*
 * Open a new account
 * Parameters: the account number, the account password, the account balance.
 * Return Value:
 */





























