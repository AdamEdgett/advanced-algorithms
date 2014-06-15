all: p5a p5b

p5a:
	g++ -I../common ../common/*.h p5a.cpp -o p5a.out -Wno-deprecated
p5b:
	g++ -I../common ../common/*.h p5b.cpp -o p5b.out -Wno-deprecated

clean:
	rm p5a.out p5b.out
