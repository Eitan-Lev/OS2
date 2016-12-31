# Makefile for the HW2
CC = g++
CFLAGS = -g -Wall 
CCLINK = $(CC)
OBJS = Bank.o bankAccount.o Bank_Manage.o Exceptions.o 
RM = rm -f
# Creating the  executable
Bank: $(OBJS) 
	$(CCLINK) -o Bank $(OBJS)
# Creating the object files
Bank_Manage.o: Bank_Manage.cpp Bank_Manage.h bankAccount.h 
bankAccount.o: bankAccount.cpp bankAccount.h
Bank.o: Bank.cpp Bank_Manage.h
Exceptions.o: Exceptions.cpp Exceptions.h
# Cleaning old files before new make
clean: 
	$(RM) Bank *.o *~ "#"* core.*


