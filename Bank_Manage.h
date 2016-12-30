/*
 * Bank_Manage.h
 *
 *  Created on: 30 בדצמ 2016
 *      Author: Eitan Levin
 */

#ifndef BANK_MANAGE_H_
#define BANK_MANAGE_H_

#include <map.h>
#include <stdlib.h>
#include "bankAccount.h"

/*
 * Here all the top level functions will be.
 * Any validation of password or otherwise is done here.
 */
using std::map;
typedef map<int, bankAccount>BankMap;
typedef BankMap::value_type Pair;
typedef BankMap::iterator Iterator;

BankMap bankAccounts;

int getPassword(unsigned int accountNumber, int accountPass);
bool openNewAccount(unsigned int accountNumber, int accountPass, unsigned int balance);
bool freezeAccount(unsigned int accountNumber, int accountPass);
bool unFreezeAccount(unsigned int accountNumber, int accountPass);
bool withrawFromAccont(unsigned int accountNumber, int accountPass, unsigned int withrawSum);
bool depositToAccont(unsigned int accountNumber, int accountPass, unsigned int depositSum);
unsigned int getAccountBalance(unsigned int accountNumber, int accountPass);
bool transferMoney(unsigned int srcAccountNumber, int srcAccountPass, unsigned int DestAccountNumber, unsigned int amount);
bool takeComission();

#endif /* BANK_MANAGE_H_ */
