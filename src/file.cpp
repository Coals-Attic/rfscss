#include <vector>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include "file.h"
#include "debug.h"

std::vector<char> File::read(std::string file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cout << "rfScss - File '" << file_path << "' not found" << std::endl;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    const std::string& s = ss.str();
    std::vector<char> content(s.begin(), s.end());

    return content;
}

void File::place_in(std::string folder_path, std::string folder_name, std::string content) {
    // If the folder does not exist, create it
    struct stat info;
    
    int ret = mkdir(folder_path.c_str(), 0777);
    if (ret == 0) {
        std::cout << "rfScss - Created folder '" << folder_path << "'" << std::endl;
    } else {
        std::cout << "rfScss - In folder '" << folder_path << "'" << std::endl;
    }

    // Create a .scss file for this selector and put the content in it
    std::string file_name = folder_name + ".scss";
    std::string file_path = folder_path + "/" + file_name;

    std::cout << "rfScss - Saving '" << file_path << "'" << std::endl;
    std::stringstream ss;
    ss << folder_name << " {";
    ss << content << "}\n";
    std::ofstream file;
    file.open(file_path, std::ios::out);
    file << ss.str();
    file.close();
}