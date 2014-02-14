#include "stdafx.h"
using namespace std;

const int  wordsInPrefix = 2;
const char nonWord[] = "\n";	// cannot appear as real line: we remove newlines
const int  maxGeneratedWords = 1500;

class PrefixWords{
public:
    PrefixWords(){};
    ~PrefixWords(){};

private:

};

class Suffix{
public:
    Suffix(){};
    ~Suffix(){};

    void push_back(const string &s){
        wordList.push_back(s);
    }

    const vector <string>& content()const{
        return wordList;
    }

private:
    vector<string> wordList;

};

typedef deque<string> Prefix;

size_t prefixHash(const Prefix & prefix) {
    size_t hash_ = 0;
    for (const auto &item : prefix) {
        hash_ ^= hash<string>()(item);
    }
    return hash_;
}

//unordered_map<Prefix, vector<string>, decltype(&prefixHash) > statetab(1013, prefixHash); // prefix -> suffixes
unordered_map<Prefix, Suffix, decltype(&prefixHash) > tabPrefixToSuffixes(1013, prefixHash);

void		readAndBuild(Prefix&, istream&);
void		generate();
void		addWordToSuffixUpdatePrefix(Prefix&, const string&);

void initPrefix(Prefix &prefix){
    for (int i = 0; i < wordsInPrefix; i++)
        addWordToSuffixUpdatePrefix(prefix, nonWord);
}


int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));
    setlocale(LC_ALL, "Russian");
#if 0
    ::testing::InitGoogleTest(&argc, argv);
    const int res = RUN_ALL_TESTS( );
    if (res != 0)
        return res;
#endif
    if (argc < 2) {
        std::cerr << "WARNING: Missing arg";
        return -1;
    }

    std::ifstream input_file_stream(argv[1]);
    if (!input_file_stream.is_open( )) {
        std::cerr << "ERROR file not opened";
        return -1;
    }
    boost::timer::cpu_timer bench_timer;
    bench_timer.stop( );

    Prefix prefix;	// current input prefix
    initPrefix(prefix);

    bench_timer.start( );
    cout << "\nBuild ...";
    readAndBuild(prefix, input_file_stream);
    bench_timer.stop();

    addWordToSuffixUpdatePrefix(prefix, nonWord);
    generate();
    tabPrefixToSuffixes.clear();
    //statetab.clear();
    cout << "\n\b\b\b\t" << format(bench_timer.elapsed( )) << "";
    getchar();

    return 0;
}

void readAndBuild(Prefix& prefix, istream& in) {
    string buf;

    while (in >> buf)
        addWordToSuffixUpdatePrefix(prefix, buf);
}

// add: add word to suffix deque, update prefix
void addWordToSuffixUpdatePrefix(Prefix& prefix, const string& s) {
    if (prefix.size() == wordsInPrefix) {
        //statetab[prefix].push_back(s);
        tabPrefixToSuffixes[prefix].push_back(s);
        prefix.pop_front();
    }
    prefix.push_back(s);
}

void generate() {
    for (const auto &it: tabPrefixToSuffixes){
        const auto suffixesList = it.second.content();
        if (suffixesList.size() < 2)
            continue;

        const auto &prefixWords = it.first;
        cout << '\n' << prefixWords.at(0) << " " << prefixWords.at(1) << "\n";
        for (const auto &suffixWord : suffixesList){
            cout << "\t" << suffixWord << "\n";
        }
    }

#if 0
    typedef std::multimap<int, string> word_index;
    word_index words_by_freq;
    for (const auto &state : statetab) {
        //words_by_freq.insert(std::pair<int, string>(state.second., state.first));
    }
#endif

#if 0
    Prefix prefix;
    initPrefix(prefix);
    for (int i = 0; i < maxGeneratedWords; i++) {
        const auto &suf = statetab.at(prefix);
        const auto &w = suf.at(rand()% suf.size());
        if (w == nonWord)
            break;
        cout << w << " ";
        prefix.pop_front();	// advance
        prefix.push_back(w);
    }
#endif
}