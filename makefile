#Grupo 12
#Daniel Santos 44887
#Luís Costa 47082
#Marcus Dias 44901

BIN = bin
INC = include
OBJ = obj
SRC = src
CFLAGS = -I include

#OBJFILES_DATA = $(OBJ)/data.o $(OBJ)/entry.o $(OBJ)/list.o $(OBJ)/test_data.o
#OBJFILES_ENTRY = $(OBJ)/data.o $(OBJ)/entry.o $(OBJ)/list.o $(OBJ)/test_entry.o
#OBJFILES_LIST = $(OBJ)/data.o $(OBJ)/entry.o $(OBJ)/list.o $(OBJ)/test_list.o
OBJFILES_TABLE = $(OBJ)/data.o $(OBJ)/entry.o $(OBJ)/list.o $(OBJ)/test_table.o
#OBJFILES_MESSAGE = $(OBJ)/data.o $(OBJ)/entry.o $(OBJ)/list.o $(OBJ)/test_message.o

#all: test_list test_entry test_data

#test_message: $(OBJFILES_MESSAGE)
	gcc -o test_message $(OBJFILES_MESSAGE)

test_table: $(OBJFILES_TABLE)
	gcc -o test_table $(OBJFILES_TABLE)

#test_list: $(OBJFILES_LIST)
#	gcc -o test_list $(OBJFILES_LIST)

#test_entry: $(OBJFILES_ENTRY)
#	gcc -o test_entry $(OBJFILES_ENTRY)

#test_data: $(OBJFILES_DATA)
#	gcc -o test_data $(OBJFILES_DATA)

#$(OBJ)/test_message.o: test_message.c $(INC)/message-private.h
#	gcc $(CFLAGS) -c test_message.c -o $(OBJ)/test_message.o
	
$(OBJ)/test_table.o: test_table.c $(INC)/table-private.h
	gcc $(CFLAGS) -c test_table.c -o $(OBJ)/test_table.o

#$(OBJ)/test_list.o: test_list.c $(INC)/list-private.h
#	gcc $(CFLAGS) -c test_list.c -o $(OBJ)/test_list.o	

#$(OBJ)/test_entry.o: test_entry.c $(INC)/entry.h $(INC)/data.h
#	gcc $(CFLAGS) -c test_entry.c -o $(OBJ)/test_entry.o
	
#$(OBJ)/test_data.o: test_data.c $(INC)/data.h
#	gcc $(CFLAGS) -c test_data.c -o $(OBJ)/test_data.o

#$(OBJ)/message.o: $(SRC)/message.c $(INC)/message.h $(INC)/message-private.h
#	gcc -c $(SRC)/message.c -o $(OBJ)/message.o

$(OBJ)/table.o: $(SRC)/table.c $(INC)/table.h $(INC)/table-private.h
	gcc -c $(SRC)/table.c -o $(OBJ)/table.o

$(OBJ)/list.o: $(SRC)/list.c $(INC)/list.h $(INC)/list-private.h $(INC)/entry.h
	gcc -c $(SRC)/list.c -o $(OBJ)/list.o
	
$(OBJ)/entry.o: $(SRC)/entry.c $(INC)/entry.h $(INC)/data.h
	gcc -c $(SRC)/entry.c -o $(OBJ)/entry.o
	
$(OBJ)/data.o: $(SRC)/data.c $(INC)/data.h
	gcc -c $(SRC)/data.c -o $(OBJ)/data.o
			
clean:
	rm -f obj/*.o
	rm -f test_data test_entry test_list
	rm -f *~
	
.PHONY: clean
