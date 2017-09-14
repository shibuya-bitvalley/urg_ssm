//  gnuplot_stream.h  by KUOH Shunsuke (2012/04/27)
//    NOTE: This program works only with GCC.

#ifndef _GNUPLOT_STREAM_H_
#define _GNUPLOT_STREAM_H_

#include <ostream>
#include <cstdio>
#include <ext/stdio_filebuf.h>

class gnuplot_stream : public std::ostream {
public:
    typedef __gnu_cxx::stdio_filebuf<char> filebuf_type;

private:
    class PopenFP {
        FILE* fp_;

    public:
        PopenFP(const char* command) : fp_(0) {
            fp_ = popen(command, "w");
        }

        ~PopenFP() {
            pclose(fp_);
        }

        FILE* file_pointer() const {
            return const_cast<FILE*>(fp_);
        }
    };

    PopenFP fp_;
    filebuf_type fb_;

public:
    gnuplot_stream(const char* command = "gnuplot -p") : fp_(command), fb_(fp_.file_pointer(), ios_base::out) {
        this->init(&fb_);
        if (!fb_.is_open())
            this->setstate(ios_base::failbit);
        else
            this->clear();
    }

    ~gnuplot_stream() {}

    filebuf_type* rdbuf() const {
        return const_cast<filebuf_type*>(&fb_);
    }

    bool is_open() const {
        return fb_.is_open();
    }
};

#endif//_GNUPLOT_STREAM_H_
