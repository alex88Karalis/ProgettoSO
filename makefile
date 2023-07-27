# file per backup del ProgettoSO

SOURCE = ~/ProgettoSO/
BACKUP_F = /media/sf_SOLabVM/Git_Repo/

$(BACKUP_F): $(SOURCE)
	make backup


backup: 
	cp -r $(SOURCE) $(BACKUP_F)
	ls -l  $(BACKUP_F)ProgettoSO
	

