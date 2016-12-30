/*
 * bankAccount.h
 *
 *  Created on: 30 בדצמ 2016
 *      Author: Eitan Levin
 */

#ifndef BANKACCOUNT_H_
#define BANKACCOUNT_H_

/*
 * This Data Structure assumes trust between it and the user.
 * Only parameters validation is required.
 */

class bankAccount {
private:
	unsigned int _id;
	int _password;
	unsigned int _balance;
	bool _isFrozen;
public:
	bankAccount(unsigned int accountNumber, int accountPass, unsigned int balance, bool isFrozen = false) :
		_id(accountNumber), _password(accountPass), _balance (balance), _isFrozen(isFrozen) {}
	unsigned int accountNumber();
	int accountPassword();
	unsigned int accountBalance();
	bool isAccountFrozen();
	bool freeze();
	bool unFreeze();
	bool withrawMoney(unsigned int withrawSum);
	bool depositMoney(unsigned int depositSum);
	void printAccount();
};



#endif /* BANKACCOUNT_H_ */
