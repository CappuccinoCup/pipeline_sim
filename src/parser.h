#ifndef ARM_PIPELINE_PARSER_H
#define ARM_PIPELINE_PARSER_H

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string.h>
#include <ctype.h>
#include <vector>

using namespace std;

extern vector <string> lexeme;
extern int prog;

int execute(int pc);

int compare(int pc);

#endif //ARM_PIPELINE_PARSER_H
