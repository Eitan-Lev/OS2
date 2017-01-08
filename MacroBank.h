/*
 * MacroBank.h
 *
 *  Created on: 3 ���� 2017
 *      Author: Eitan Levin
 */

#ifndef MACROBANK_H_
#define MACROBANK_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
#include <pthread.h>

#define ERROR_VALUE (-1)
#define SUCCESS_VALUE (0)

using std::stringstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;
using std::getline;

extern pthread_mutex_t map_lock;
extern pthread_mutex_t bank_balance_lock;
extern pthread_mutex_t log_file_lock;
extern ofstream log_file;

//Eitan macro FIXME
//Macro:
#define ASSERT_VALID(a, b) do {\
	if (!(a)) { \
		cout << b << endl; \
		continue; \
	} \
	} while (0)

#define PRINT_DETAILS_ILLEGALLY() do {\
		cout << "account number: " << accountNumber << "cmdLine: " << cmdLine << endl; \
	} while (0)

#define UNEXPECTED_EXCEPTION() do {\
		cout << "Unknown exception was thrown, no known handling at this point. " << "atmThread: " << atmNumber << endl; \
		PRINT_DETAILS_ILLEGALLY(); \
		exit(ERROR_VALUE); \
	} while (0)

#define ACCOUNT_DOESNT_EXIST_ILLEGALY() do {\
		cout << "AccountDoesntExistException has been thrown, no known handling at this point. " << "atmThread: " << atmNumber << endl; \
		PRINT_DETAILS_ILLEGALLY(); \
		exit(ERROR_VALUE); \
	} while (0)

#define ACCOUNT_ALREADY_EXISTS_ILLEGALY() do {\
		cout << "AccountNumberAlreadyExistsException has been thrown, no known handling at this point. " << "atmThread: " << atmNumber << endl; \
		PRINT_DETAILS_ILLEGALLY(); \
		exit(ERROR_VALUE); \
	} while (0)

#define WRONG_PASSWORD_ILLEGALY() do {\
		cout << "WrongPasswordException has been thrown, no known handling at this point. " << "atmThread: " << atmNumber << endl; \
		PRINT_DETAILS_ILLEGALLY(); \
		exit(ERROR_VALUE); \
	} while (0)

#define BALANCE_OVERFLOW_ILLEGALY() do {\
		cout << "BalanceOverflowException has been thrown, no known handling at this point. " << "atmThread: " << atmNumber << endl; \
		PRINT_DETAILS_ILLEGALLY(); \
		exit(ERROR_VALUE); \
	} while (0)

#define LOG_NOT_ENOUGH_MONEY(a, b, c) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << "Error " << a << ": Your transaction failed - account id "<< b << " balance is lower than " << c << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_WRONG_PASSWORD(a, b) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << "Error " << a << ": Your transaction failed - password for account id " << b << " is incorrect" << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_ACCOUNT_ALREADY_EXISTS(a) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << "Error " << a <<": Your transaction failed - account with the same id exists" << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_ACCOUNT_DOESNT_EXIST(a, b) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << "Error " << a << ": Your transaction failed - account id " << b << " does not exist" << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_BALANCE(a, b, c) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << a << ": Account " << b << " balance is " << c << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_OPEN_NEW_ACCOUNT(a, b, c, d) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << a << ": New account id is " << b << " with password " << c << " and initial balance " << d << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_DEPOSIT(a, b, c, d) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << a << ": Account " << b << " new balance is " << c << " after " << d << " $ was deposited" << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_WITHDRAW(a, b, c, d) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << a << ": Account " << b << " new balance is " << c << " after " << d << " $ was withdrew" << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_TRANSFER(a, b, c, d, e, f) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << a << ": Transfer " << b << " from account " << c << " to account " << d << " new account" << endl; \
		log_file << "balance is " << e << " new target account balance is " << f << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_COMMISSION(a, b, c) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << "Bank: commissions of " << a << " % were charged, the bank gained " << b << " $ from account " << c << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define LOG_STATUS(a, b, c, d) do {\
		pthread_mutex_lock(&log_file_lock); \
		log_file << a << ": New account id is " << b << " with password " << c << " and initial balance " << d << endl; \
		pthread_mutex_unlock(&log_file_lock); \
	} while (0)

#define NOT_TRANSFER_FUNC_INIT() do {\
		pthread_mutex_lock(&read_account_lock); \
		readAccountCounter++; \
		if (readAccountCounter == 1) { \
			pthread_mutex_lock(&write_account_lock); \
		} \
		pthread_mutex_unlock(&read_account_lock); \
	} while (0)

#define NOT_TRANSFER_FUNC_END() do {\
		pthread_mutex_lock(&read_account_lock); \
		readAccountCounter--; \
		if (readAccountCounter == 0) { \
			pthread_mutex_unlock(&write_account_lock); \
		} \
		pthread_mutex_unlock(&read_account_lock); \
	} while (0)

#define MAP_NOT_TRANSFER_INIT() do {\
		pthread_mutex_lock(&read_transfer_lock); \
		readTransferCounter++; \
		if (readTransferCounter == 1) { \
			pthread_mutex_lock(&write_transfer_lock); \
		} \
		pthread_mutex_unlock(&read_transfer_lock); \
	} while (0)

#define MAP_NOT_TRANSFER_END() do {\
		pthread_mutex_lock(&read_transfer_lock); \
		readTransferCounter--; \
		if (readTransferCounter == 0) { \
			pthread_mutex_unlock(&write_transfer_lock); \
		} \
		pthread_mutex_unlock(&read_transfer_lock); \
	} while (0)

#endif /* MACROBANK_H_ */
