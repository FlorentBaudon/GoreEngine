#CC -> compilateur command - LDFLAGS -> option edition de liens (include etc..)
CC=g++
CFLAG=-W -Wall -ansi -std=c++11

LDFLAGS =-lglew32 -lglfw3

LIBRAIRIES=-LDependencies/GLEW/lib/Release/x64/ -LDependencies/GLFW/lib-mingw-w64/

DEPS = -IDependencies/GLEW/include/
DEPS+= -IDependencies/GLFW/include/
DEPS+= -IDependencies/glm/

SRCDIR=GorEngine/Src/
OBJDIR=obj/
OUTDIR=bin/

SRC=$(subst $(SRCDIR), ,$(wildcard $(SRCDIR)*.cpp))
#get all cpp files in src folders, then remove src/ fom filename

OBJ=$(SRC:.cpp=.o)
# prend la list de fichiers dans SRC finnissant par .c et refait la même list mais en .o



all : directories hello

hello : $(OBJ)
	$(CC) -o $(OUTDIR)hello $(foreach o,$^,$(OBJDIR)$(o)) $(LIBRAIRIES) $(LDFLAGS)

#$(foreach o,$^,$(OBJDIR)$(o)) pour chaque element dans $^ (tout les depedances de la cible) j'ajoute la variable OBJDIR

#main.o : hello.h
#force la generation de main.o si hello.h est modifié (~?~)

%.o : $(SRCDIR)%.cpp
	$(CC) -o $(OBJDIR)$@ -c $< $(CFLAG) $(DEPS)
# %.o et %.cpp est une commande générique qui cherche tout le .c pour les transofrmer en .o en conservant le nom
#  $@ nom de la cible (ici le .o)
#  $< -> nom de la 1er dependance (ici le .c) 
#  $^ -> nom de toutes les dependances (ici le .c)

#.PHONE spécifie que ses commandes ne sont pas associés à des fichiers, en gros avc un make all clean ne sera jamais executé. Il faudra faire explicitement un "make clean" pour le faire. Ce la evite de déclencher la commande clean à cause d'un fichiers nommée clean par exemple.
.PHONY : clean wipe directories

clean :
	rm -f $(OBJDIR)*.o
	rm -f $(OUTDIR)*.exe
wipe :
	rm -rf $(OBJDIR)
	rm -rf $(OUTDIR)
directories :
	mkdir -p bin
	mkdir -p obj

# gcc -o hello -I/somewhere/include hello.c -> Permet de chercher les fichiers .h ailleurs -I = Includes
# gcc -o hello -L/somewhere/lib hello.c -> Permet de chercher les fichiers .h ailleurs -L = Libraries
# -Wall warning all
# -Werror traite les warning comme des erreurs, stop la compilation
# -ansi defini le standard (influe sur le type d'erreur) (maintenant on utilise -std=c++11 )
# -pedantic  affiche les ereurs dû à la norme
# -lstdc++ ajoute lib standard cpp (toujours obligatoire ?)
# ajouter @ devant la commande la rend silencieuse (la commande n'apparaitra pas dans les logs)
