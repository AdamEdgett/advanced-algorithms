all: p2a p2b

p2a:
	g++ -I../common ../common/*.h p2a.cpp -o p2a.out -Wno-deprecated
p2b:
	g++ -I../common ../common/*.h p2b.cpp -o p2b.out -Wno-deprecated

clean:
	rm p2a.out p2b.out
