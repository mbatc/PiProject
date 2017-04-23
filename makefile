_SOURCES= CInput.cpp CPyFile.cpp Commands.cpp CSystem.cpp CGPIOManager.cpp CLog.cpp CGPIOPin.cpp CIO.cpp main.cpp CPyManager.cpp COutput.cpp
_HEADERS= CPyManager.h CMDHelp.h CGPIOManager.h CSystem.h CIO.h Commands.h CGPIOPin.h CPyFile.h CLog.h COutput.h main.h CInput.h

CC=g++
CFLAGS=-c -std=c++11 -Werror -I/usr/include/python3.4m -I/usr/include/python3.4m  -Wno-unused-result -g -fstack-protector-strong -DNDEBUG -g -fwrapv -O3
INCLUDE = source
OBJ = obj
LDFLAGS = -L/usr/lib/python3.4/config-3.4m-arm-linux-gnueabihf -L/usr/lib -lpython3.4m -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
HEADERS = $(patsubst %,$(INCLUDE)/%,$(_HEADERS))
SOURCES = $(patsubst %,$(INCLUDE)/%,$(_SOURCES))
OBJECTS=$(patsubst %,$(OBJ)/%,$(_SOURCES:.cpp=.o))
OUTPUT=output
EXECUTABLE=debug
all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(OUTPUT)/$(EXECUTABLE)
$(OBJ)/%.o: $(INCLUDE)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm $(OBJ)/*.o $(OUTPUT)/$(EXECUTABLE)

