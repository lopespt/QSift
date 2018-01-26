// ------------------------------------------------------------------------
// Project: q-SIFT
// Authors:  Guilherme Wachs-Lopes <gwachs@fei.edu.br>
//           Miller Horvath <mhorvath@fei.edu.br>
//
// This is an open source code created for research and academic purposes.
// If you have used this code in you research, please cite:
//
//   P. S. Rodrigues, G. A. Wachs-Lopes, G. Antonio Giraldi and M. Horvath,
//   in press. q-SIFT: A Strategy Based on Non-Extensive Statistic to
//   Improve SIFT Algorithm Under Severe Conditions. Pattern Recognition.
//
// ------------------------------------------------------------------------

#include <tools.hpp>
#include <iostream>
#include <memory>

namespace Color {
    enum class Code : int {
        FG_BOLD = 1,
        FG_BLINK = 5,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };
    union UCode {
        Code code;
        int ch;
    };

    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) { }

        Modifier(UCode pCode) : code(pCode.code) { }


        friend std::ostream &
        operator<<(std::ostream &os, const Modifier &mod) {
            return os << "\033[" << (int) mod.code << "m";
        }
    };
}

struct Teste{
    int a;
    ~Teste(){
        cout << "Realmente deletou \n";
    }
};

int main(int argc, char *argv[]) {

    using namespace Color;

    vector<feature> f;

    //PDM_OpenCL(f,f);
    // printf("sdsd %ls\n", L"∂ƒ©ß");
    //std::cout << U"∂ƒ©ß";

    setlocale(LC_ALL, "");

    auto lambda = [](Teste *i) {
        cout << "deleting\n";
        // std::default_delete<Teste>()(i);
    };

    auto k = shared_ptr<Teste>(new Teste{5},lambda);
    {
        auto q = shared_ptr<Teste>(k);
        cout << q.use_count();
    }
    cout << k.use_count();


    return 0;
}