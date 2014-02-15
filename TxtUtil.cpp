#include "stdafx.h"
//using namespace std;
using std::string;
//using std::cout;


const int  wordsInPrefix = 1;
const char nonWord[] = "{!}";	// cannot appear as real line
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

    inline void push_back(const string &s){
        wordList.push_back(s);
    }

    inline const std::vector <string>& content()const{
        return wordList;
    }

private:
    std::vector<string> wordList;

};

typedef std::deque<string> Prefix;

//unordered_map<Prefix, vector<string>, decltype(&prefixHash) > statetab(1013, prefixHash); // prefix -> suffixes
std::unordered_map<Prefix, Suffix, std::function<size_t(const Prefix & prefix) >>
tablePrefixToSuffixes(1013, [](const Prefix & prefix){
    size_t h = 0;
    for (const string &x : prefix){
        h ^= std::hash<string>()(x);
    }
    return h;
});

void		generate();
void		add(Prefix&, const string&);

bool isNumber(const std::string& s){
    return (s.find_first_not_of("0123456789") == string::npos);
    //return std::all_of(s.begin(), s.end(), ::isdigit); //Debug assert (c >= -1 && c <= 255) filed! 
    //return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void initPrefix(Prefix &prefix){
    for (int i = 0; i < wordsInPrefix; i++)
        add(prefix, nonWord);
}

void readAndBuild(Prefix& prefix, std::ifstream &inputFile) {
    string newLine, buf, buf1;
    while (inputFile.good() || (inputFile.eof() && !newLine.empty())){
        std::getline(inputFile, newLine, '\n');
        if (newLine.empty())
            continue;
        std::stringstream ss(newLine);
        while (ss >> buf){
            //boost::trim_if(buf, boost::is_any_of("\"()?!.,«»-—“„")); //trimPunctuation
            boost::trim_if(buf, !boost::algorithm::is_alpha());
            if (buf.empty())
                continue;

            if (isNumber(buf)){
                add(prefix, "{#}");
                continue;
            }
            boost::algorithm::to_lower(buf);
            add(prefix, buf);
        }
        add(prefix, "{/P}");
    }
}

int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));
    setlocale(LC_ALL, "Russian");

    std::string s = "Владимир Высоцкий";
    std::cout << boost::algorithm::erase_first_copy(s, "и") << std::endl; 
    std::cout << boost::algorithm::erase_nth_copy(s, "и", 2) << std::endl; 
    std::cout << boost::algorithm::erase_last_copy(s, "и") << std::endl; 
    std::cout << boost::algorithm::erase_all_copy(s, "и") << std::endl; 
    std::cout << boost::algorithm::erase_head_copy(s, 5) << std::endl; 
    std::cout << boost::algorithm::erase_tail_copy(s, 8) << std::endl;
    //getchar();
    //return 0;

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

    std::cout << "\nBuild ...";
    bench_timer.start();
    readAndBuild(prefix, input_file_stream);
    bench_timer.stop();
    input_file_stream.close();

    add(prefix, nonWord);
    generate();
    tablePrefixToSuffixes.clear();
    //statetab.clear();
    std::cout << "\n\t" << format(bench_timer.elapsed()) << "";
    getchar();

    return 0;
}

// add: add word to suffix deque, update prefix
void add(Prefix& prefix, const string& newWordSuffix) {
    if (prefix.size() == wordsInPrefix) {
        //statetab[prefix].push_back(newWordSuffix);
        tablePrefixToSuffixes[prefix].push_back(newWordSuffix);
        prefix.pop_front();
    }
    prefix.push_back(newWordSuffix);
}

void generate() {
    std::cout << '\n';
    for (const auto &tablePair : tablePrefixToSuffixes){
        const auto suffixesList = tablePair.second.content();
        if (suffixesList.size() < 1)
            continue;

        const auto &prefixWords = tablePair.first;
        for (const auto &prefixWord : prefixWords){
            //if (prefixWord == )
            std::cout << prefixWord << ' ';
        }
        std::cout << "\n ";

        for (const auto &suffixWord : suffixesList){
            std::cout << "   " << suffixWord/* << "\n"*/;
        }
        std::cout << "\n";
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