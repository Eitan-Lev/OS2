/*
 * Bank.cpp
 *
 *  Created on: Dec 31, 2016
 *      Author: eitanlev
 */
/*#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>*/

#include "Bank_Manage.h"
#include "bankAccount.h"
#include "Exceptions.h"

/*
 * In this file the threads will be managed- Init thread, ATM thread, Commission thread and Bank Status thread.
 * All functions called from here must be sent with the accounts map.
 */

//This is the map for the bank accounts:
BankMap bankAccountsMap;//TODO beware of duplication

int main(int argc, char* argv[]) {
	/*bankAccount new_account(1,2,3);
	Pair dataPair(1, new_account);
	bankAccountsMap.insert(dataPair);*/
	return 0;
}


