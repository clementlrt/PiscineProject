#include "graph.h"
#include "sstream"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    // bouton sauvegarder
    m_tool_box.add_child(m_save_box);
    m_save_box.set_pos(-10,600);
    m_save_box.set_dim(100,100);
    m_save.set_dim(100,100);
    m_save_image.set_pic_name("sauvegarde.bmp");
    m_save.add_child(m_save_image);
    m_save_box.add_child(m_save);

    // bouton retour
    m_tool_box.add_child(m_retour_box);
    m_retour_box.set_pos(-10,500);
    m_retour_box.set_dim(100,100);
    m_retour.set_dim(100,100);
    m_retour_image.set_pic_name("fleche.bmp");
    m_retour.add_child(m_retour_image);
    m_retour_box.add_child(m_retour);

    //bouton suppression

    m_tool_box.add_child(m_aj_box);
    m_aj_box.set_pos(-10,200);
    m_aj_box.set_dim(100,100);
    m_aj.set_dim(100,100);
    m_aj_image.set_pic_name("ARETE.bmp");
    m_aj.add_child(m_aj_image);
    m_aj_box.add_child(m_aj);

    m_tool_box.add_child(m_ajs_box);
    m_ajs_box.set_pos(-10,400);
    m_ajs_box.set_dim(100,100);
    m_ajs.set_dim(100,100);
    m_ajs_image.set_pic_name("SOMMET.bmp");
    m_ajs.add_child(m_ajs_image);
    m_ajs_box.add_child(m_ajs);

    m_tool_box.add_child(m_sups_box);
    m_sups_box.set_pos(-10,300);
    m_sups_box.set_dim(100,100);
    m_sups.set_dim(100,100);
    m_sups_image.set_pic_name("SOMMET2.bmp");
    m_sups.add_child(m_sups_image);
    m_sups_box.add_child(m_sups);

    m_tool_box.add_child(m_supa_box);
    m_supa_box.set_pos(-10,100);
    m_supa_box.set_dim(100,100);
    m_supa.set_dim(100,100);
    m_supa_image.set_pic_name("ARETE2.bmp");
    m_supa.add_child(m_supa_image);
    m_supa_box.add_child(m_supa);
}

void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(bool *prog2)
{
    m_interface->m_top_box.update();

    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    if  (m_interface->m_save.clicked())
        save();

    if(m_interface->m_retour.clicked())
        *prog2 = false;

    if(m_interface->m_supa.clicked())
    {
        retirerArc();

        for (auto &elt : m_vertices)
            elt.second.post_update();

        for (auto &elt : m_edges)
            elt.second.post_update();
    }

    if(m_interface->m_sups.clicked())
    {
        retirerSommet();

        for (auto &elt : m_vertices)
            elt.second.post_update();

        for (auto &elt : m_edges)
            elt.second.post_update();
    }

    if(m_interface->m_aj.clicked())
        Ajoutarcs();

    if(m_interface->m_ajs.clicked())
        ajoutSommet();
}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }

    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);
}

void Graph::charger_fichier(std::string nom )
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);


    std::ifstream file(nom, std::ios::in);
    int x,y, nbimage, nbarrete, a1, a2;
    std::string nomimage;

    if(file)
    {
        file>>nbimage;
        for(int i=0; i<nbimage; i++)
        {
            file >> x >> y >> nomimage;
            add_interfaced_vertex(i, 0, x, y, nomimage);
        }

        file>>nbarrete;
        for (int i=0; i<nbarrete; i++)
        {
            file >> a1 >> a2;
            add_interfaced_edge(i, a1, a2, 50);
        }
    }
    else
        std::cout << "erreur lors du chargement" << std::endl;

    file.close();

}

void Graph::save()
{
    std::string nom;

    if(m_choix == 1)
        nom = "images.txt";

    if(m_choix == 2)
        nom = "file.txt";

    if(m_choix == 3)
        nom = "image.txt";

    std::ofstream file(nom, std::ios::out | std::ios::trunc);

    if (file)
    {
        file << m_vertices.size() << std::endl;

        for(unsigned int i=0 ; i<m_vertices.size(); i++)
        {
            file << m_vertices[i].m_interface->m_top_box.get_posx() << " " << m_vertices[i].m_interface->m_top_box.get_posy() << " " << m_vertices[i].m_interface->m_img.get_pic_name() << std::endl;
        }

        file << m_edges.size() << std::endl;

        for (unsigned int i = 0; i < m_edges.size(); i++)
        {
            file << m_edges[i].m_from << " " << m_edges[i].m_to << std::endl;
        }
    }
    else
        std::cout << "erreur lors de la sauvegarde" << std::endl;

    file.close();
}


void Graph::test_remove_edge(int eidx)
{
    std::map<int, Edge>::iterator E;

    /// test : on a bien des éléments interfacés
    if (m_interface && m_edges[eidx].m_interface)
        m_interface->m_main_box.remove_child( m_edges[eidx].m_interface->m_top_edge );


    E = m_edges.find(eidx);
    m_edges.erase(E);
}

void Graph::test_remove_vertex(int vidx)
{
    std::map<int, Vertex>::iterator V;
    std::map<int, Edge>::iterator E;

    /// test : on a bien des éléments interfacés
    if (m_interface && m_vertices[vidx].m_interface)
        m_interface->m_main_box.remove_child( m_vertices[vidx].m_interface->m_top_box);

    V = m_vertices.find(vidx);
    m_vertices.erase(V);

    for(int i = 0 ; i<m_edges.size() ; i++)
    {
        if((m_edges[i].m_from == vidx) || (m_edges[i].m_to == vidx))
        {
             E = m_edges.find(i);

            /// test : on a bien des éléments interfacés
            if (m_interface && m_edges[i].m_interface)
                m_interface->m_main_box.remove_child( m_edges[i].m_interface->m_top_edge );

            m_edges.erase(E);
        }
    }
}

void Graph::videVecteur()

{
    int t1=m_edges.size() ;
    int t2=m_vertices.size();

    for(int i=0 ; i<t1; i++)
    {
        m_edges.erase(m_edges.find(i));
    }

    for(int i=0 ; i<t2; i++)
    {
        m_vertices.erase(m_vertices.find(i));
    }
}

void Graph::Ajoutarcs()
{

    int sommeta;
    int sommetb;
    double poidsommet;


    std::cout<<"Sommet de depart"<<std::endl;
    std::cin>>sommeta;

    std::cout<<"Sommet d'arrive"<<std::endl;
    std::cin>>sommetb;

    std::cout<<"Tapez le poid de l'arc"<<std::endl;
    std::cin>>poidsommet;


    add_interfaced_edge(m_edges.size(),sommeta,sommetb,poidsommet);
}

void Graph::ajoutSommet()
{
    int last_vertex=0;
    int photo;
    std::string affichage;

    std::cout<<"Choisissez le sommet que vous voulez rajouter"<<std::endl;
    std::cout<<"1. Aigle"<<std::endl;
    std::cout<<"2.Elan"<<std::endl;
    std::cout<<"3.Amphipodes"<<std::endl;
    std::cout<<"4.Caiman"<<std::endl;
    std::cout<<"5.Tapir"<<std::endl;
    std::cout<<"6.Thon"<<std::endl;
    std::cout<<"7.Vegetaux"<<std::endl;
    std::cout<<"8.Ours"<<std::endl;
    std::cout<<"9.Loup"<<std::endl;
    std::cin>>photo;

    switch(photo)
    {
    case 1:
        affichage="aigle.bmp";
        break;
    case 2:
        affichage="elan.bmp";
        break;
    case 3:
        affichage="amphipode.bmp";
        break;
    case 4:
        affichage="caiman.bmp";
        break;
    case 5:
        affichage="tapir.bmp";
        break;
    case 6:
        affichage="thon.bmp";
        break;
    case 7:
        affichage="vegetaux.bmp";
        break;
    case 8:
        affichage="ours.bmp";
        break;
    case 9:
        affichage="loup.bmp";
        break;
    }



    for(auto &elt : m_vertices)
    {
        if(last_vertex<=elt.first)
        {
            last_vertex=elt.first+1;
        }
    }

    add_interfaced_vertex(last_vertex, 30, 20, 20, affichage);
}

void Graph::retirerArc()
{
    int choix;

    std::cout << "Quelle arete voulez-vous supprimer ? :" << std::endl;
    std::cin >> choix;
    test_remove_edge(choix);
}

void Graph::retirerSommet()
{
    int choix;

    std::cout << "Quel sommet voulez-vous supprimer ? :" << std::endl;
    std::cin >> choix;
    test_remove_vertex(choix);
}
