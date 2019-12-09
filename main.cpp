#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

class Obserwator {
    public:
        Obserwator(const char* s) : nazwa{s?s:"nieznany"} { /* nic nie robi */ }
        virtual ~Obserwator() { } // klasa bazowa wiec wirtualny destruktor
        const char* id() const { return nazwa.c_str(); }
        virtual void update() = 0; // metoda czysto wirtualna
    private:
        string nazwa;
};

class Manager {
    public:
        void dodaj(Obserwator* p);
        void usun(Obserwator* p);
        void powiadom();
        void obserwatorzy() const;
    private:
        list<Obserwator*> klient;
};

void Manager::obserwatorzy() const {
    if (klient.empty()) cout << "Brak obserwatorow!\n";
    auto it = klient.cbegin(); // stały iterator początku listy
    while (it != klient.cend()) {
        cout << "Obserwator " << (*it)->id() << endl;
        ++it;
    }
}

void Manager::powiadom() {
    for (auto p : klient) p->update();
}

void Manager::dodaj(Obserwator* p) {
    if (find(klient.begin(),klient.end(),p)==klient.end()) {
        klient.push_back(p);
    } else {
        cout << "Obserwator " << p->id() << " jest juz na liscie!\n";
    }
}

void Manager::usun(Obserwator* p) {
    klient.erase(find(klient.begin(),klient.end(),p));
}

class Meteo : public Manager {
    public:
        double getT() const { return t; }
        void operator()(size_t n); // n liczba losowan
    private:
        double t { 0. };
        double losuj(int a, int b);
};

double Meteo::losuj(int a, int b) {
    return static_cast<double>( rand() % (b - a + 1) + a );
}

void Meteo::operator()(size_t n) {
    while (n--) {
        t = losuj(5,25);
        powiadom();
    }
}

class TChwilowa : public Obserwator {
    public:
        TChwilowa(const char* s, const Meteo& r) : Obserwator(s),ref{r} { }
        void update() {
            t = ref.getT();
            cout << "Obserwator " << id() << " t = " << t << endl;
        }
    private:
        double t { 0. };
        const Meteo& ref;
};

int main() {
    
    Meteo stacja;
    Obserwator *p1 = new TChwilowa("PWSZ",stacja);
    Obserwator *p2 = new TChwilowa("NOWY SACZ",stacja);
    //Obserwator *p3 = new TSrednia("KRAKOW",stacja);
    //Obserwator *p4 = new TSrednia("WARSZAWA",stacja);
    stacja.obserwatorzy(); // pusta lista
    stacja.dodaj(p1);
    stacja.dodaj(p2);

    //stacja.dodaj(p3);
    //stacja.dodaj(p4);
    stacja.obserwatorzy();
    stacja(2); // dwa losowania, wywolanie stacja.operator()(2)
    stacja.dodaj(p1);
    //stacja.usun(p3);
    stacja(2); // kolejne losowania, juz bez obserwatora p3
    delete p1;
    delete p2;
    //delete p3;
    //delete p4;
}