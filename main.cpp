#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

// ======================================
// STRUCTURE D'UN NOEUD
// ======================================

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droite;
};

// ======================================
// CLASSE ABR
// ======================================

class ArbreBinaireRecherche {
private:
    Noeud* racine;

    Noeud* creerNoeud(int valeur) {
        Noeud* n = new Noeud;
        n->valeur = valeur;
        n->gauche = nullptr;
        n->droite = nullptr;
        return n;
    }

    Noeud* inserer(Noeud* courant, int valeur) {
        if (courant == nullptr)
            return creerNoeud(valeur);

        if (valeur < courant->valeur)
            courant->gauche = inserer(courant->gauche, valeur);
        else if (valeur > courant->valeur)
            courant->droite = inserer(courant->droite, valeur);

        return courant;
    }

    bool rechercher(Noeud* courant, int valeur) {
        if (courant == nullptr)
            return false;

        if (valeur == courant->valeur)
            return true;

        if (valeur < courant->valeur)
            return rechercher(courant->gauche, valeur);

        return rechercher(courant->droite, valeur);
    }

    Noeud* trouverMinimum(Noeud* courant) {
        while (courant != nullptr && courant->gauche != nullptr)
            courant = courant->gauche;

        return courant;
    }

    Noeud* supprimer(Noeud* courant, int valeur) {
        if (courant == nullptr)
            return nullptr;

        if (valeur < courant->valeur) {
            courant->gauche = supprimer(courant->gauche, valeur);
        }
        else if (valeur > courant->valeur) {
            courant->droite = supprimer(courant->droite, valeur);
        }
        else {
            if (courant->gauche == nullptr &&
                courant->droite == nullptr) {
                delete courant;
                return nullptr;
            }

            if (courant->gauche == nullptr) {
                Noeud* temporaire = courant->droite;
                delete courant;
                return temporaire;
            }

            if (courant->droite == nullptr) {
                Noeud* temporaire = courant->gauche;
                delete courant;
                return temporaire;
            }

            Noeud* successeur = trouverMinimum(courant->droite);
            courant->valeur = successeur->valeur;

            courant->droite = supprimer(
                courant->droite, successeur->valeur
            );
        }

        return courant;
    }

    void afficher(Noeud* courant, bool& premier) {
        if (courant == nullptr)
            return;

        afficher(courant->gauche, premier);

        if (!premier)
            cout << " -> ";

        cout << courant->valeur;
        premier = false;

        afficher(courant->droite, premier);
    }

    void libererArbre(Noeud* courant) {
        if (courant == nullptr)
            return;

        libererArbre(courant->gauche);
        libererArbre(courant->droite);
        delete courant;
    }

public:
    ArbreBinaireRecherche() {
        racine = nullptr;
    }

    ~ArbreBinaireRecherche() {
        libererArbre(racine);
    }

    void inserer(int valeur) {
        racine = inserer(racine, valeur);
    }

    bool rechercher(int valeur) {
        return rechercher(racine, valeur);
    }

    bool supprimer(int valeur) {
        if (!rechercher(racine, valeur))
            return false;

        racine = supprimer(racine, valeur);
        return true;
    }

    void afficher() {
        if (racine == nullptr) {
            cout << "Arbre vide." << endl;
            return;
        }

        bool premier = true;
        afficher(racine, premier);
        cout << endl;
    }
};

// ======================================
// FONCTION PRINCIPALE
// ======================================

int main() {
    int N;

    cout << "=== COMPARAISON ABR ET MAP ===" << endl;
    cout << "Nombre de valeurs a inserer : ";
    cin >> N;

    if (cin.fail() || N < 1) {
        cout << "Saisissez un entier superieur a 1 ." << endl;
        return 1;
    }

    // Creer les nombres de 1 a N
    vector<int> valeursOrdonnees;

    for (int i = 1; i <= N; i++)
        valeursOrdonnees.push_back(i);

    // Creer une copie melangee
    vector<int> valeursAleatoires = valeursOrdonnees;

    random_device rd;
    mt19937 generateur(rd());

    shuffle(
        valeursAleatoires.begin(),
        valeursAleatoires.end(),
        generateur
    );

    // Creer les deux ABR et les deux map
    ArbreBinaireRecherche abrOrdonne;
    ArbreBinaireRecherche abrAleatoire;

    map<int, int> mapOrdonne;
    map<int, int> mapAleatoire;

    // ======================================
    // EXPERIENCE 1 : ORDRE CROISSANT
    // ======================================

    cout << "\n======================================" << endl;
    cout << "EXPERIENCE 1 : ORDRE CROISSANT" << endl;
    cout << "======================================" << endl;

    auto debut = high_resolution_clock::now();

    for (int i = 0; i < N; i++)
        abrOrdonne.inserer(valeursOrdonnees[i]);

    auto fin = high_resolution_clock::now();

    auto tempsABRO = duration_cast<microseconds>(
        fin - debut
    ).count();

    debut = high_resolution_clock::now();

    for (int i = 0; i < N; i++)
        mapOrdonne[valeursOrdonnees[i]] = valeursOrdonnees[i];

    fin = high_resolution_clock::now();

    auto tempsMapO = duration_cast<microseconds>(
        fin - debut
    ).count();

    cout << "Insertion ABR  : " << tempsABRO << " microsecondes" << endl;
    cout << "Insertion map  : " << tempsMapO << " microsecondes" << endl;

    // ======================================
    // EXPERIENCE 2 : ORDRE ALEATOIRE
    // ======================================

    cout << "\n======================================" << endl;
    cout << "EXPERIENCE 2 : ORDRE ALEATOIRE" << endl;
    cout << "======================================" << endl;

    debut = high_resolution_clock::now();

    for (int i = 0; i < N; i++)
        abrAleatoire.inserer(valeursAleatoires[i]);

    fin = high_resolution_clock::now();

    auto tempsABRA = duration_cast<microseconds>(
        fin - debut
    ).count();

    debut = high_resolution_clock::now();

    for (int i = 0; i < N; i++)
        mapAleatoire[valeursAleatoires[i]] = valeursAleatoires[i];

    fin = high_resolution_clock::now();

    auto tempsMapA = duration_cast<microseconds>(
        fin - debut
    ).count();

    cout << "Insertion ABR  : " << tempsABRA << " microsecondes" << endl;
    cout << "Insertion map  : " << tempsMapA << " microsecondes" << endl;

    // Afficher les valeurs utilisees
    if (N <= 30) {
        cout << "\nOrdre croissant : ";

        for (int i = 0; i < N; i++)
            cout << valeursOrdonnees[i] << " ";

        cout << "\nOrdre aleatoire : ";

        for (int i = 0; i < N; i++)
            cout << valeursAleatoires[i] << " ";

        cout << endl;
    }

    // ======================================
    // CHOIX DE L'EXPERIENCE POUR LE MENU
    // ======================================

    int experience;

    cout << "\nChoisissez l'experience pour le menu :" << endl;
    cout << "1. Ordre croissant" << endl;
    cout << "2. Ordre aleatoire" << endl;
    cout << "Votre choix : ";
    cin >> experience;

    if (experience != 1 && experience != 2) {
        cout << "Choix invalide." << endl;
        return 1;
    }

    ArbreBinaireRecherche* arbre;
    map<int, int>* arbreMap;

    if (experience == 1) {
        arbre = &abrOrdonne;
        arbreMap = &mapOrdonne;
    } else {
        arbre = &abrAleatoire;
        arbreMap = &mapAleatoire;
    }

    // ======================================
    // MENU
    // ======================================

    int choix;
    int valeur;

    do {
        cout << "\n========== MENU ==========" << endl;
        cout << "1. Afficher les elements" << endl;
        cout << "2. Rechercher une valeur" << endl;
        cout << "3. Supprimer une valeur" << endl;
        cout << "4. Quitter" << endl;
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {

        case 1: {
            if (N > 100) {
                cout << "Trop de valeurs pour un affichage complet." << endl;
                break;
            }

            cout << "\nABR : ";
            arbre->afficher();

            cout << "map : ";

            bool premier = true;

            for (const auto& element : *arbreMap) {
                if (!premier)
                    cout << " -> ";

                cout << element.first;
                premier = false;
            }

            cout << endl;
            break;
        }

        case 2: {
            cout << "Valeur a rechercher : ";
            cin >> valeur;

            auto debutABR = high_resolution_clock::now();
            bool trouveABR = arbre->rechercher(valeur);
            auto finABR = high_resolution_clock::now();

            auto tempsABR = duration_cast<nanoseconds>(
                finABR - debutABR
            ).count();

            auto debutMap = high_resolution_clock::now();
            bool trouveMap = arbreMap->find(valeur) != arbreMap->end();
            auto finMap = high_resolution_clock::now();

            auto tempsMap = duration_cast<nanoseconds>(
                finMap - debutMap
            ).count();

            cout << "\nRecherche ABR : "
                 << (trouveABR ? "Trouvee" : "Non trouvee")
                 << " | Temps : " << tempsABR << " ns" << endl;

            cout << "Recherche map : "
                 << (trouveMap ? "Trouvee" : "Non trouvee")
                 << " | Temps : " << tempsMap << " ns" << endl;

            break;
        }

        case 3: {
            cout << "Valeur a supprimer : ";
            cin >> valeur;

            auto debutABR = high_resolution_clock::now();
            bool supprimeABR = arbre->supprimer(valeur);
            auto finABR = high_resolution_clock::now();

            auto tempsABR = duration_cast<nanoseconds>(
                finABR - debutABR
            ).count();

            auto debutMap = high_resolution_clock::now();
            size_t supprimeMap = arbreMap->erase(valeur);
            auto finMap = high_resolution_clock::now();

            auto tempsMap = duration_cast<nanoseconds>(
                finMap - debutMap
            ).count();

            cout << "\nSuppression ABR : "
                 << (supprimeABR ? "Effectuee" : "Valeur inexistante")
                 << " | Temps : " << tempsABR << " ns" << endl;

            cout << "Suppression map : "
                 << (supprimeMap > 0 ? "Effectuee" : "Valeur inexistante")
                 << " | Temps : " << tempsMap << " ns" << endl;

            break;
        }

        case 4:
            cout << "Fin du programme." << endl;
            break;

        default:
            cout << "Choix invalide." << endl;
        }

    } while (choix != 4);

    return 0;
}


