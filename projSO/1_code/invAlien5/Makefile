
FILE_OUT = Game.out
SOURCE = main.c areaGioco.c  astronave.c bullet.c    #cannone.c
HEADER = areaGioco.h  astronave.h bullet.h header.h  #cannone.h
OBJECTS = main.o areaGioco.o  astronave.o bullet.o   #cannone.o
PWD = ~/EserciziSO/invasioneAliena/invAlien3
BACKUP_F = /media/sf_SOLabVM/invasioneAliena/

Game.out: $(OBJECTS)
	gcc $(OBJECTS) -lncurses -o $(FILE_OUT) 
	
areaGioco.o: areaGioco.c areaGioco.h bullet.c bullet.h header.h
	gcc -c areaGioco.c  
	
cannone.o: cannone.c cannone.h header.h
	gcc -c cannone.c  
	
astronave.o: astronave.c astronave.h header.h
	gcc -c astronave.c 

bullet.o: bullet.c bullet.h
	gcc -c bullet.c


main.o: main.c 
	gcc -c main.c 


clean:
	rm -fv *.o *.gch


backup:
	make clean
	cp -r $(PWD) $(BACKUP_F)
	ls -l $(BACKUP_F)

openAll: 
	gedit $(SOURCE) $(HEADER) &


