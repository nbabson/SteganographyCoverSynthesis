all: draw CA embed makeCA extractRule extract createCover recoverMessage

draw:
	gcc -Wall -o draw draw.c -lssl -lcrypto

CA:
	g++ -Wall -o CA CA.cpp -g
	
embed:
	gcc -Wall -o embed embed.c -lm -lssl -lcrypto
	
makeCA:
	gcc -Wall -o makeCA makeCA.c -lm -lssl -lcrypto -g

extractRule:	
	gcc -Wall -o extractRule extractRule.c -lm -g

extract:
	gcc -Wall -o extract extract.c -lm -g

createCover:
	gcc -Wall -o createCover createCover.c 

recoverMessage:
	gcc -Wall -o recoverMessage recoverMessage.c

clean:
	rm -f *.o CA draw embed makeCA extractRule extract createCover recoverMessage
