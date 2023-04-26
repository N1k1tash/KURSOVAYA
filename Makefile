all:
	g++ main.cpp Client.cpp ./md5/md5.cpp -o Client -lcryptopp

test:
	g++ unitTest.cpp ./md5/md5.cpp Client.cpp -o test -lcryptopp -lUnitTest++ 
clean:
	rm -f Client

