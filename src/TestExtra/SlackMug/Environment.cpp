#include "Environment.h"

#include "ThorSerialize/JsonThor.h"
#include <fstream>

Environment::Environment(std::string const& fileName)
{
    std::ifstream   file(fileName);
    file >> ThorsAnvil::Serialize::jsonImporter(*this);
}
