#include "stdafx.h"
using namespace std;

const int  NPREF = 2;
const char NONWORD[] = "\n";	// cannot appear as real line: we remove newlines
const int  MAXGEN = 1500; // maximum words generated

typedef deque<string> Prefix;

size_t prefixHash(const Prefix & prefix) {
    size_t hash_ = 0;
    for (const auto &item : prefix) {
        hash_ ^= hash<string>()(item);
    }
    return hash_;
}

unordered_map<Prefix, vector<string>, decltype(&prefixHash) > statetab(1013, prefixHash); // prefix -> suffixes
//map<Prefix, vector<string> > statetab; // prefix -> suffixes

void		build(Prefix&, istream&);
void		generate();
void		add(Prefix&, const string&);

void initPrefix(Prefix &prefix){
    for (int i = 0; i < NPREF; i++)
        add(prefix, NONWORD);
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
    build(prefix, input_file_stream);
    bench_timer.stop();

    add(prefix, NONWORD);
    generate();
    statetab.clear( );
    cout << "\n\b\b\b\t" << format(bench_timer.elapsed( )) << "";
    getchar();

    return 0;
}

// build: read input words, build state table
void build(Prefix& prefix, istream& in) {
    string buf;

    while (in >> buf)
        add(prefix, buf);
}

// add: add word to suffix deque, update prefix
void add(Prefix& prefix, const string& s) {
    if (prefix.size() == NPREF) {
        statetab[prefix].push_back(s);
        prefix.pop_front( );
    }
    prefix.push_back(s);
}

// generate: produce output, one word per line
void generate() {
    Prefix prefix;
    initPrefix(prefix);

    for (int i = 0; i < MAXGEN; i++) {
        const auto &suf = statetab.at(prefix);
        const auto &w = suf.at(rand()% suf.size());
        if (w == NONWORD)
            break;
        cout << w << " ";
        prefix.pop_front();	// advance
        prefix.push_back(w);
    }
}