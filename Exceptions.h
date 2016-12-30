/*
 * Exceptions.h
 *
 *  Created on: 30 בדצמ 2016
 *      Author: Eitan Levin
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

class OSException {};

class BankAccountException : public OSException {};
class WrongPasswordException : public BankAccountException {};
class NotEnoughMoneyException : public BankAccountException {};





#endif /* EXCEPTIONS_H_ */
