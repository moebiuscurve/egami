helloworld: helloworld.c
	gcc -o helloworld helloworld.c `pkg-config --libs gl glu` -lglut
clean:
	rm -rf helloworld
