ROOT_PATH=$(shell pwd)

COMM_PATH=$(ROOT_PATH)/util
DATA_POOL_PATH=$(ROOT_PATH)/data_pool
LIB_PATH=$(ROOT_PATH)/lib
LOG_PATH=$(ROOT_PATH)/log
SERVER_PATH=$(ROOT_PATH)/server
USER_MANAGE_PATH=$(ROOT_PATH)/user_manage

CC=g++
CFLAGS=-g -Wall -std=c++11
LIB= -lpthread -ljsoncpp
LDFLAGS=#-static
INCLUDE=-I$(COMM_PATH) -I$(DATA_POOL_PATH) \
		-I$(LIB_PATH) -I$(LOG_PATH) -I$(SERVER_PATH) -I$(USER_MANAGE_PATH)

SRC=$(shell ls -R | egrep '*.cpp')
SER_OBJ=$(SRC:.cpp=.o)
SER_BIN=chat_server

all:$(SER_BIN)

$(SER_BIN):$(SER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIB)

%.o:$(USER_MANAGE_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE)  
%.o:$(COMM_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE)  
%.o:$(DATA_POOL_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE)  
%.o:$(SERVER_PATH)/%.cpp
	$(CC) $(CFLAGS) -c $< $(INCLUDE)  


clean:
	rm -rf *.o $(CLI_BIN) $(SER_BIN) output

