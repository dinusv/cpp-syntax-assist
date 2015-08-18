#include <iostream>
#include "linenoise.hpp"

#include "QCodeBase.hpp"
#include "QCSAPluginLoader.hpp"

using namespace std;

using namespace csa;
using namespace csa::ast;

void completeFunction(const char* editBuffer, std::vector<std::string>& completions){

    const char* dotPosition = strrchr(editBuffer, '.');
    if (dotPosition == NULL)
        dotPosition = editBuffer;

    std::string searchFilter = dotPosition;
    size_t i = 0;
    while ( i < searchFilter.size() ){
        char& c = searchFilter[i];
        if ( !isalnum(c) && c != '_' && c != '$' )
            break;
        ++i;
    }
    if ( i < searchFilter.size() )
        searchFilter = searchFilter.substr(0, i);

    if ( std::string("quit").find(searchFilter) != std::string::npos && dotPosition == editBuffer){
        completions.push_back("quit()");

    } else if ( std::string("nodes").find(searchFilter) != std::string::npos && dotPosition == editBuffer ){
        completions.push_back("nodes('");
    }

}

int main(){

    linenoise::SetCompletionCallback(&completeFunction);
    linenoise::SetHistoryMaxLen(10);

    while (true) {
        size_t suggestionPosition = 0;
        std::string line = linenoise::Readline("csa> ", false, &suggestionPosition);

        if (line == "quit()")
            break;

        std::cout <<  "echo: '" << line << "'" << std::endl;
        std::cout <<  "pos:  "  << suggestionPosition << std::endl;

        linenoise::AddHistory(line.c_str());
    }

    return 0;
}
