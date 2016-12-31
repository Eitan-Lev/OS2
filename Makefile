# Makefile for the HW2
CC = g++
CFLAGS = -g -Wall 
CCLINK = $(CC)
OBJS = Bank.o bankAccount.o Bank_Manage.o Exceptions.o 
RM = rm -f
# Creating the  executable
bank: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
Bank.o: Bank.cpp Bank_Manage.h
bankAccount.o: bankAccount.cpp bankAccount.h
Bank_Manage.o: Bank_Manage.cpp Bank_Manage.h bankAccount.h 
Exceptions.o: Exceptions.h
# Cleaning old files before new make
clean:
$(RM) bank *.o *~ "#"* core.*
