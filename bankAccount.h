/*
 * bankAccount.h
 *
 *  Created on: 30 ���� 2016
 *      Author: Eitan Levin
 */

#ifndef BANKACCOUNT_H_
#define BANKACCOUNT_H_

/*
 * Here will be all the simplest of functions.
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
	//bankAccount(const bankAccount &obj);  // copy constructor
	//bankAccount(const bankAccount &obj) : this->_id(obj.getAccountNumber()), this->_password(obj.getAccountPassword()), this->_balance(obj.getAccountBalance()), this->_isFrozen(obj.isAccountFrozen()) {}
	//bankAccount(const bankAccount& obj) : _id(obj.getAccountNumber()), _password(obj.getAccountPassword()), _balance(obj.getAccountBalance()), _isFrozen(obj.isAccountFrozen()) {};
	unsigned int getAccountNumber();
	int getAccountPassword();
	unsigned int getAccountBalance();
	bool isAccountFrozen();//True if frozen, false if not frozen
	bool freeze();//False if already frozen, true otherwise
	bool unFreeze();//False if already not frozen, true otherwise
	bool withrawMoney(unsigned int withrawSum);//False if not enough money, true otherwise
	bool depositMoney(unsigned int depositSum);//False if sum causes unsigned int overflow, true otherwise
	void printAccount();
};



#endif /* BANKACCOUNT_H_ */
