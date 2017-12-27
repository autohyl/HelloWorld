CC = g++
FLAG = -g -DHAS_SYSV_IPC -DTEST_SEM
BASE=../
SM=$(BASE)shared_memory/
INCLUDE = -I./ -I./include -I$(SM) -I$(SM)include -I$(BASE)os_include
LIBDIR =
LIB = 
BIN = 
TARGET = SV_Semaphore_Test
SRCS = SV_Semaphore_Test.cpp \
		SV_Semaphore_Simple.cpp \
		SV_Semaphore_Complex.cpp \
		$(SM)Shared_Memory.cpp \
		$(SM)Shared_Memory_SV.cpp \
		$(SM)SV_Shared_Memory.cpp \
		$(BASE)os_include/crc3.cpp

$(TARGET):$(SRCS:.cpp=.o)
	$(CC) $(FLAG) -o $@ $^ $(LIBDIR) $(LIB)

%.o:%.cpp
	$(CC) $(FLAG) $(INCLUDE) -c -o $@ $<

clean:
	-rm -f *.o
	-rm -f $(SM)*.o
	-rm -f $(TARGET)
