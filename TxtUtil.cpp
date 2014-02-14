#include "stdafx.h"
using namespace std;

const int  NPREF = 2;
const char NONWORD[ ] = "\n";	// cannot appear as real line: we remove newlines
const int  MAXGEN = 5000; // maximum words generated

typedef deque<string> Prefix;

unordered_map<deque<string>, vector<string> > statetab; // prefix -> suffixes
//map<Prefix, vector<string> > statetab; // prefix -> suffixes

void		build(Prefix&, istream&);
void		generate(int nwords);
void		add(Prefix&, const string&);

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

    int	nwords = MAXGEN;
    Prefix prefix;	// current input prefix

    //srand(time(NULL));
    for (int i = 0; i < NPREF; i++)
        add(prefix, NONWORD);
    bench_timer.start( );
    cout << "\nBuild ...";
    build(prefix, input_file_stream);
    bench_timer.stop();

    add(prefix, NONWORD);
    generate(nwords);
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
void generate(int nwords) {
    Prefix prefix;

    for (int i = 0; i < NPREF; i++)
        add(prefix, NONWORD);
    for (int i = 0; i < nwords; i++) {
        vector<string>& suf = statetab[prefix];
        const string& w = suf[rand( ) % suf.size( )];
        if (w == NONWORD)
            break;
        cout << w << " ";
        prefix.pop_front( );	// advance
        prefix.push_back(w);
    }
}