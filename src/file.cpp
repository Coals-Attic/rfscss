#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "file.h"
#include "debug.h"

bool File::is_valid_path(const std::string& path) {
    // Check if the path is valid, i.e. it is not empty and does not contain
    // any invalid characters.
    // Invalid chars:
    //  : * ? " < > | and anything between 0 and 31 decimal
    // (even though most are only invalid on Windows, we don't care)

    if (path.empty()) {
        return false;
    } else {
        for (char c : path) {
            if (
                c < 32 || c == ':' || c == '*'|| 
                c == '"' || c == '<' || c == '>' ||
                c == '|' || c == '?'
            ) {
                return false;
            }
        }
    }

    return true;

}

std::vector<char> File::read(std::string file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "rfscss - file '" << file_path << "' not found" << std::endl;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    const std::string& s = ss.str();
    std::vector<char> content(s.begin(), s.end());

    return content;
}

void File::place_in(std::string file_path, std::string content) {
    // If the folder does not exist, create it
    struct stat info;
    
    std::string folder = file_path;
    std::string::size_type delim_pos = folder.find_last_of("/");
    if (delim_pos != std::string::npos) {
        folder = folder.substr(0, delim_pos);
    }

    if (folder != file_path) {
        mkdir(folder.c_str(), 0777);
    }

    std::ofstream file;
    file.open(file_path, std::ios::out | std::ios::app);
    if (file.fail()) {
        #if DEBUG
        throw std::ios_base::failure(std::strerror(errno));
        #endif

        std::cerr << "rfscss - fatal: failed to open '" << file_path 
                  << "' (" << std::strerror(errno) << ")" << std::endl;
        return;
    }

    file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);
    file << std::endl << content;
}