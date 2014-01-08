/* Copyright 2013 Alexandr Smirnov */

#include "library/sort_application.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>

#include "library/sort.h"

SorterApplication::SorterApplication() : message_("") {}

void SorterApplication::help(const char* appname) {
    message_ += std::string("This is an application of sorting arrays ")
             + "of integer numbers.\n\n"
             + "Please provide arguments in the following format:\n\n"
             + "  $ " + appname + " <sort method> <size of array>\n\n"
             + " <element[1]> <element[2]>... <element[size]> \n\n"
             + "Where all arguments but second are integer numbers, \n\n"
             + "third argument is the number of elements in the array,  \n\n"
             + "and second argument has type 'string' and values: \n\n"
             + "q - if you want to sort this by QUICKSORT \n\n"
             + "h - if you want to sort this by HEAPSORT \n\n"
             + "m - if you want to sort this by MERGESORT \n\n";
}

void parseInteger(const char* arg);
void parseInteger(const char* arg) {
    char* end;
    strtol(arg, &end, 10);

    if (end[0]) {
        throw "Wrong number format";
    }
}

void parseMethod(const char* arg);
void parseMethod(const char* arg) {
    if ((arg[0] != 'm') & (arg[0] != 'h') & (arg[0] != 'q')) {
      throw "Wrong method format";
    }
}

bool SorterApplication::parseArguments(int argc, const char** argv) {
    if (argc == 1) {
        help(argv[0]);
        return false;
    } else if (argc != (atoi(argv[2]) + 3)) {
        message_ = "ERROR: The number of arguments must match \n\n";
        message_ += "value of second argument + 3.\n\n";
        help(argv[0]);
        return false;
    }

    try {
        parseMethod(argv[1]);
        }
    catch(...) {
        message_ = "Wrong method format!\n";
        return false;
    }

    try {
      for (int i = 2; i < argc; i++) {
        parseInteger(argv[i]);
      }
    }
    catch(...) {
        message_ = "Wrong number format!\n";
        return false;
    }

    return true;
}

std::string SorterApplication::operator()(int argc, const char** argv) {
    bool returnCode = parseArguments(argc, argv);
    if (returnCode != true)
        return message_;

    if (atoi(argv[2]) > 0) {
      unsigned int N = static_cast<unsigned int>(atoi(argv[2]));
      std::vector <int> a(N);
      unsigned int i;

      for (i = 0; i < N; i++) {
        int64_t temp = atol(argv[i + 3]);
        if (temp >= 2147483647 || temp <= -2147483648) {
          message_ = "ERROR: The number is out of range!";
          return message_;
        }
        a[i] = atoi(argv[i + 3]);
      }

      Sorter sorter;

      if (argv[1][0] == 'q')
        sorter.Sort(&a[0], static_cast<int>(N), QUICKSORT);
      if (argv[1][0] == 'h')
        sorter.Sort(&a[0], static_cast<int>(N), HEAPSORT);
      if (argv[1][0] == 'm')
        sorter.Sort(&a[0], static_cast<int>(N), MERGESORT);

      std::ostringstream stream;
      stream << "Result of sorting:";

      for (i = 0; i < N; i++)
        stream << " " << a[i];

    message_ = stream.str();
    }
    return message_;
}