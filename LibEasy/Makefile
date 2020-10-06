OBJS_WITHOUT_MAIN  = AsmLib.o String.o Console.o Math.o Integer.o StringBuffer.o Float.o FileInputStream.o \
						FileOutputStream.o FileReader.o FileWriter.o Date.o File.o LXML.o ANSIFileInputStream.o \
						EasyRes.o System.o IOUtility.o Exception.o Character.o Regex.o EasyC.o UInteger.o \
						Thread.o
						
OBJS    = main.o $(OBJS_WITHOUT_MAIN)
INCLUDE = -I./include
FLAGS = -Wall $(INCLUDE)
#FLAGS   =  $(INCLUDE)

CC = g++
MAKE = make

default:
	$(MAKE) easy.exe
	
EasyRes.o: EasyRes.rc
	windres EasyRes.rc EasyRes.o

main.o: ./src/main.cpp 
	$(CC) $(FLAGS) $< -c

easy.exe: test.cpp $(OBJS)
	$(CC) $(FLAGS) $< $(OBJS) -o easy.exe

%.o: ./src/%.cpp ./include/%.hpp ./include/Exception.hpp
	$(CC) $(FLAGS) $< -c

Regex.o: ./src/Regex.cpp ./include/Regex/Regex.hpp ./include/Exception.hpp
	$(CC) $(FLAGS) $< -c
	
AsmLib.o: ./src/AsmLib.asm ./include/AsmLib.hpp
	nasm $< -f win64 -o AsmLib.o


#要先指定库文件名, 添加 main.o 到 libeasy.a
libeasy: $(OBJS_WITHOUT_MAIN) main.o
	$(CC) -shared $(OBJS_WITHOUT_MAIN) -o libeasy.dll -Wl,--out-implib,libeasy.a
	ar q libeasy.a main.o 
	ranlib libeasy.a
	@$(MAKE) static
	
static: $(OBJS)
	ar r libeasystatic.a $(OBJS)
	ranlib libeasystatic.a

setuplib:
	cp --top .\include D:\MinGW64\x86_64-w64-mingw32\include\Easy
	cp .\libeasy.a D:\MinGW64\x86_64-w64-mingw32\lib
	cp .\libeasystatic.a D:\MinGW64\x86_64-w64-mingw32\lib
	cp .\libeasy.dll E:\dllLib

clean:
	rm *.o
