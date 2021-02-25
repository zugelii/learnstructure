#include "Helper.h"
//the Helper project is a .lib project

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


std::string GetConfigFilePath(std::string appName, std::string file)
{
    if(fs::exists(file))
        return (fs::current_path() / file).string();
    std::string etcFile = "/data/etc/" + appName + "/" + file;
    if(!fs::exists(etcFile))
        etcFile = "/etc/" + appName + "/" + file;
    return etcFile;
}
