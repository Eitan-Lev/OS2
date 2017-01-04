/*
 * bankMap.h
 *
 *
 *      Author: Eitan Levin
 */

#ifndef BANK_MAP_H_
#define BANK_MAP_H_

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
typedef map<int, bankAccount> AccountsMap;
typedef AccountsMap::value_type Pair;
typedef AccountsMap::iterator Iterator;
typedef AccountsMap::const_iterator IteratorConst;

class bankMap {
private:
	AccountsMap _innerMap;
	IteratorConst accountIter;
public:

	void openNewAccount(int accountNumber, int accountPass, int balance);
	int getAccountBalance(int accountNumber, int accountPass);
	void freezeAccount(int accountNumber, int accountPass);
	void unFreezeAccount(int accountNumber, int accountPass);
	void depositToAccount(int accountNumber, int accountPass, int depositSum);//FIXME frozen
	void withrawFromAccount(int accountNumber, int accountPass, int withrawSum);//FIXME frozen
	void transferMoney(int srcAccountNumber, int srcAccountPass, int destAccountNumber, int amount);//FIXME frozen
	int takeComission(int accountNumber, int percentage);
	int getPassword(int accountNumber);
	bool checkPassword(int accountNumber, int accountPass);
	bool isAccountInMap(int accountNumber);
	int transferGetBalance(int accountNumber);//Use only for money transfer!//TODO
	void printAccountInMap(int accountNumber);//FIXME
	IteratorConst begin();//FIXME
	IteratorConst end();//FIXME
};



#endif /* BANK_MAP_H_ */
