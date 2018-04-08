#include "grman/grman.h"
#include <iostream>

#include "graph.h"

void menu(Graph &g, bool *prog, bool *prog2)
{
    bool work = true;

    BITMAP* page = load_bitmap("t.bmp",NULL);
    BITMAP* buffer = create_bitmap(800,600);

    if(!page)
    {
        allegro_message("pas trouver image");
        exit(EXIT_FAILURE);
    }

    clear_bitmap(buffer);

   while(work)
    {
        blit(page,buffer,0,0,0,0,800,600);

        if(mouse_b&1)
        {
            if (mouse_x>42&&mouse_y>503&&mouse_x<209&&mouse_y<550)
            {

                g.charger_fichier("images.txt");
                work = false;
                g.set_choix(1);
            }

            if (mouse_x>305&&mouse_y>503&&mouse_x<471&&mouse_y<550)
            {
                g.charger_fichier("file.txt");
                work = false;
                g.set_choix(2);
            }
            if (mouse_x>561&&mouse_y>503&&mouse_x<721&&mouse_y<550)
            {
                g.charger_fichier("image.txt");
                work = false;
                g.set_choix(3);
            }


            if(mouse_x > 25 && mouse_y > 30 && mouse_x < 90 && mouse_y < 71)
                exit (EXIT_FAILURE);
        }

         blit(buffer,screen,0,0,0,0,800,600);
    }
    *prog2=true;
}

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();
    grman::set_pictures_path("pics");


    Graph g;
    g.make_example();
    bool prog = true;
    bool prog2 = true;


    while (prog)
    {
        menu(g, &prog, &prog2);

        while(prog2)
        {
            /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
            g.update(&prog2);

            /// Mise à jour générale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }
        g.videVecteur();
    }
    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


