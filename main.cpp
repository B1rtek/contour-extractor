#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>

using namespace std;
using namespace sf;

string usuniecieRozszerzenia(string sciezka)
{
    while(sciezka.back()!='.')
    {
        sciezka.pop_back();
        if(sciezka.empty())
        {
            return "";
        }
    }
    sciezka.pop_back();
    return sciezka;
}

#ifdef _WIN32
string command = "dir /A-D /B > temp.txt";
#endif
#ifdef linux
string command = "ls -A1 > temp.txt";
#endif

vector<string> listaPlikow()
{
    set<string> dobreRozszerzenia = {
            "bmp", "png", "tga", "jpg", "gif", "psd", "hdr", "pic"
    };

    system(command.c_str());
    fstream lista;
    lista.open("temp.txt");
    string linia;
    vector<string> pliki;
    while(getline(lista, linia))
    {
        string kopiaLinii = linia;
        string rozszerzenie;
        while(kopiaLinii.back()!='.')
        {
            rozszerzenie = kopiaLinii.back() + rozszerzenie; // NOLINT(performance-inefficient-string-concatenation)
            kopiaLinii.pop_back();
            if(kopiaLinii.empty())
            {
                break;
            }
        }
        transform(rozszerzenie.begin(), rozszerzenie.end(), rozszerzenie.begin(), ::tolower);
        if(dobreRozszerzenia.count(rozszerzenie)) {
            pliki.push_back(linia);
        }

    }
    return pliki;
}

int wyborPliku(vector<string> lista)
{
    for(int i=0; i<lista.size(); i++)
    {
        cout<<i+1<<": "<<lista[i]<<endl;
    }
    int numer;
    cout<<"Podaj numer pliku: ";
    cin>>numer;
    return numer-1;
}

void progressbar(int progress, int calosc)
{
    string bar="|";
    float floatprogress = float(progress)/float(calosc);
    int kawalki = int((float(progress)/float(calosc))*50);
    for(int i=0; i<kawalki; i++)
    {
        bar+=char(219);
    }
    for(int i=0; i<50-kawalki; i++)
    {
        bar+=' ';
    }
    bar+="| ";
    string procent = to_string(round(floatprogress*10000.0f)/100.0f);
    for(int i=0; i<4; i++) procent.pop_back();
    procent+=" %";
    bar+=procent;
    cout<<bar<<endl;
}

int main()
{
    vector<string>lista = listaPlikow();
    string wybranyPlik = lista[wyborPliku(lista)];

    Image obraz, nowyObraz;
    if(!obraz.loadFromFile(wybranyPlik))
    {
        return 1;
    }
    nowyObraz.create(obraz.getSize().x, obraz.getSize().y);
    for(int y=0; y<obraz.getSize().y; y++)
    {
        for(int x=0; x<obraz.getSize().x; x++)
        {
            int sumar=0, sumag=0, sumab=0, przetworzone=8;
            for(int y1=y-1; y1<=y+1; y1++)
            {
                if(y1<0 || y1>=obraz.getSize().y)
                {
                    przetworzone--;
                    continue;
                }
                for(int x1=x-1; x1<=x+1; x1++)
                {
                    if(x1<0 || x1>=obraz.getSize().x)
                    {
                        przetworzone--;
                        continue;
                    }
                    if(x1==x && y1==y)
                    {
                        przetworzone--;
                        continue;
                    }
                    sumar+=abs(obraz.getPixel(x1, y1).r-obraz.getPixel(x,y).r);
                    sumag+=abs(obraz.getPixel(x1, y1).g-obraz.getPixel(x,y).g);
                    sumab+=abs(obraz.getPixel(x1, y1).b-obraz.getPixel(x,y).b);
                }
            }
            sumar/=przetworzone;
            sumag/=przetworzone;
            sumab/=przetworzone;
            int srednia = (sumar + sumag + sumab) / 3;
            sumar = srednia;
            sumag = srednia;
            sumab = srednia;
            nowyObraz.setPixel(x, y, Color(sumar, sumag, sumab));
        }
        progressbar(y+1, obraz.getSize().y); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }
    wybranyPlik = usuniecieRozszerzenia(wybranyPlik);
    wybranyPlik+="bw.png";
    nowyObraz.saveToFile(wybranyPlik);
    return 0;
}
