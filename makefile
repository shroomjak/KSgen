main:
	g++ -c ks.h
	g++ -c ks.cpp
	g++ all.cpp ks.cpp -o result
clean:
	rm -f result
