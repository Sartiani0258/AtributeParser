/**
 * Script najde promenne podle dotazu, napriklad
 * 4 3
 * <tag1 value = "HelloWorld">
 * <tag2 name = "Name1">
 * </tag2>
 * </tag1>
 * tag1.tag2~name
 * tag1~name
 * tag1~value
 * cislo 4 znamena 4 radky textu a 3 znamena 3 radky dotazu podle kterych budeme hledat promenne
 */
#include <vector>
#include <iostream>
#include <regex>

using namespace std;

vector<vector<string>> testovaci_data();

vector<string> nacti_radky(int pocet_nq);;

string vyhledej_dalsi_regex_zpusob(const string &regex_vyraz, string retezec);;

string vyhledej(const string &regex_vyraz, const string &retezec);;

vector<vector<string>> nacti_vstup();

void najdi_dotaz(const string &re_vyraz_tag, const string &re_vyraz_promenna, const string &dotaz,
                 const vector<string> &vyrazy);

void vyhledej_podle_dotazu(const vector<vector<string>> &dotazy_vyrazy);

/**
 * Vstupni testovaci data
 * @return vrati vector s nactenymi radky a dotazy
 */
vector<vector<string>> testovaci_data() {
    vector<string> vyrazy{"<tag1 value = \"HelloWorld\">", "<tag2 name = \"Name1\">", "</tag2>", "</tag1>"
    };
    vector<string> dotazy{"tag1.tag2~name", "tag1~name", "tag1~value"
    };
    return vector<vector<string>>{dotazy, vyrazy};
}

/**
 * Nacte radky ze vstupu
 * @param pocet_nq pocet radku ktere se nactou
 * @return vector s nactenymi radky
 */
vector<string> nacti_radky(int pocet_nq) {
    vector<string> na_vraceni;
    for (int i = 0; i < pocet_nq + 1; ++i) {
        string radek;
        getline(cin, radek);
        if (!radek.empty())
            na_vraceni.push_back(radek);
    };
    return na_vraceni;

}

string vyhledej_dalsi_regex_zpusob(const string &regex_vyraz, string retezec) {
    regex word_regex(regex_vyraz);
    auto words_begin =
            std::sregex_iterator(retezec.begin(), retezec.end(), word_regex);
    auto words_end = std::sregex_iterator();
    std::string match_str;
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        match_str = match.str();
    }
    return match_str;
}

/**
 * Vyhleda hledany vyraz v textovem retezci
 * @param regex_vyraz regex vyraz pomoci ktereho se vyhledava
 * @param retezec textovy retezec ve kterem budeme vyhledavat
 * @return nalezeny vyraz
 */
string vyhledej(const string &regex_vyraz, const string &retezec) {
    regex rgx(regex_vyraz);
    smatch matches;
    string match_str;
    if (regex_search(retezec, matches, rgx)) {
        return matches[matches.size() - 1].str();
    } else {
        return "";
    }
}

/**
 * Nacte vstupni data. Mame moznost simultannich dat, nebo dat vlozenych uzivatelem.
 * @return nactena vstupni data
 */
vector<vector<string>> nacti_vstup() {
    char y_n;
    cout << "Chcete vlozit data? Zvolte 'n' pokud ne a pouziju testovaci data: \n";
    cin >> y_n;
    if (y_n == 'n') {
        return testovaci_data();
    }
    int n, q;
    cout << "Vlozte data: " << endl;
    vector<string> vyrazy;
    vector<string> dotazy;
    cin >> n;
    cin >> q;
    vyrazy = nacti_radky(n); // vyrazy typu <tag1 value = "HelloWorld">
    dotazy = nacti_radky(q); // dotazy typu tag1.tag2~name ktere definuji co hledame
    return vector<vector<string>>{dotazy, vyrazy};
}

/**
 * Funkce ktera nam iteruje pres text a hleda promenne definovane v dotazech
 * @param re_vyraz_tag regex vyraz pomoci ktereho najdeme tagy
 * @param re_vyraz_promenna regex vyraz pomoci ktereho najdeme promenne v textu
 * @param dotaz dotazy ktere se hledaji v textu
 * @param vyrazy text ve kterym se bude hledat
 */
void najdi_dotaz(const string &re_vyraz_tag, const string &re_vyraz_promenna, const string &dotaz,
                 const vector<string> &vyrazy) {
    string tag = vyhledej(re_vyraz_tag, dotaz);
    string promenna = vyhledej(re_vyraz_promenna, dotaz);
    bool nasel_se = false;
    for (const auto &vyraz: vyrazy) {
        string vysledek = vyhledej(promenna + " = \"(.+)\"", vyraz);
        if (!vyhledej("^<tag" + tag, vyraz).empty() and !vysledek.empty()) {
            cout << "Promenna v radku '" << vyraz << "' se podle dotazu '" << dotaz << "' nasla: " << vysledek
                 << endl;
            nasel_se = true;
            break;
        }
    }
    if (!nasel_se) cout << "Dotaz '" << dotaz << "' se nenasel!\n";
}

/**
 * Vyhleda v textu hledane vyrazy
 * @param dotazy_vyrazy vector s textem a dotazmi podle kterych budeme vyhledavat v tomto textu
 */
void vyhledej_podle_dotazu(const vector<vector<string>> &dotazy_vyrazy) {
    string re_vyraz_tag = "(\\w)~"; // vyraz pro nalezeni tagu napr. tag2~
    string re_vyraz_promenna = "~(\\w*.)"; // vyraz pro zachyceni promenne napr ~name
    for (const auto &dotaz: dotazy_vyrazy.at(0))
        najdi_dotaz(re_vyraz_tag, re_vyraz_promenna, dotaz, dotazy_vyrazy.at(1));
}

/**
 * Funkce kterou vola compiler
 * @return 0
 */
int main() {
    vector<vector<string>> dotazy_vyrazy = nacti_vstup();
    vyhledej_podle_dotazu(dotazy_vyrazy);
    return 0;
}
