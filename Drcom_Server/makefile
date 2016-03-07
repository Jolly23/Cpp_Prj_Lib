ROOT=/home/Cpp_Prj_Lib/Drcom_Server
OBJ=$(ROOT)/obj
BIN=$(ROOT)/bin

OUT=g++

$(BIN)/Drcom_Server.exe:$(OBJ)/main.o $(OBJ)/SockJolly.o
	$(OUT) $^ -o $@ -lpthread
$(OBJ)/main.o:main.cpp
	$(OUT) -c $^ -o $@
$(OBJ)/SockJolly.o:SockJolly.cpp
	$(OUT) -c $^ -o $@
clean:
	$(RM) $(OBJ)/*.o $(BIN)/*.exe
rebuild:clean $(BIN)/Drcom_Server.exe