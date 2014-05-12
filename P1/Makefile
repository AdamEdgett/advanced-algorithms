all: p1a p1b

p1a:
	g++ -I../common ../common/*.h p1a.cpp -o p1a.out -Wno-deprecated
p1b:
	g++ ../common/*.h p1b.cpp -o p1b.out -Wno-deprecated

clean:
	rm p1a.out p1b.out
