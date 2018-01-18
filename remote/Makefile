all:remote.exe
remote.exe: common.h main.cpp
	g++ -g -Wall common.h main.cpp -o remote.exe -lws2_32
clean: remote.exe
	-rm remote.exe
run: remote.exe
	./remote.exe 127.0.0.1 8083
