#include <fstream>
#include <sstream>

#include "utils/config/ConfigException.hpp"
#include "utils/config/ConfigLoader.hpp"

Config ConfigLoader::loadConfig(std::filesystem::path path = "config/")
{
    Config config;

    std::filesystem::path propertyPath = path.append("property.txt");
    std::filesystem::path railroadPath = path.append("railroad.txt");
    std::filesystem::path utilityPath = path.append("utility.txt");
    std::filesystem::path taxPath = path.append("tax.txt");
    std::filesystem::path specialPath = path.append("special.txt");
    std::filesystem::path miscPath = path.append("misc.txt");

    loadProperty(config, propertyPath);
    loadRailroad(config, railroadPath);
    loadUtility(config, utilityPath);
    loadTax(config, taxPath);
    loadSpecial(config, specialPath);
    loadMisc(config, miscPath);

    std::ifstream railroadStream(railroadPath);
    std::ifstream utilityStream(utilityPath);
    std::ifstream taxStream(taxPath);
    std::ifstream specialStream(specialPath);
    std::ifstream miscStream(miscPath);

    if (!railroadStream.is_open()) {
        throw SaveFileNotFoundException(railroadPath.string());
    }
    if (!utilityStream.is_open()) {
        throw SaveFileNotFoundException(utilityPath.string());
    }
    if (!taxStream.is_open()) {
        throw SaveFileNotFoundException(taxPath.string());
    }
    if (!specialStream.is_open()) {
        throw SaveFileNotFoundException(specialPath.string());
    }
    if (!miscStream.is_open()) {
        throw SaveFileNotFoundException(miscPath.string());
    }

    // Input path

    return config;
}

void ConfigLoader::loadProperty(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }
    // ID KODE NAMA JENIS WARNA HARGA_LAHAN NILAI_GADAI UPG_RUMAH UPG_HT RENT_L0…RENT_L5
    int line = 0;
    std::string buffer;
    while (std::getline(in, buffer)) {
        line++;
        int col = 0;

        PropertyConfig property;
        std::stringstream bufferStream(buffer);
        col++;
        if (!(bufferStream << property.id)) {
            throw SaveFileFormatException("ID", path.string(), col, line);
        }
        col++;
        if (!(bufferStream << property.code)) {
            throw SaveFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream << property.name)) {
            throw SaveFileFormatException("NAMA", path.string(), col, line);
        }
        col++;
        if (!(bufferStream << property.color)) {
            throw SaveFileFormatException("JENIS", path.string(), col, line);
        }
        col++;
        if (!(bufferStream << property.color)) {
            throw SaveFileFormatException("WARNA", path.string(), col, line);
        }
        col++;
        if (!(bufferStream << property.price)) {
            throw SaveFileFormatException("HARGA_LAHAN", path.string(), col, line);
        }
        col++;
        if (!(bufferStream << property.mortgageValue)) {
            throw SaveFileFormatException("NILAI_GADAI", path.string(), col, line);
        }

        if (property.type == "STREET") {
            col++;
            if (!(bufferStream << property.housePrice)) {
                throw SaveFileFormatException("HARGA_RUMAH", path.string(), col, line);
            }
            col++;
            if (!(bufferStream << property.hotelPrice)) {
                throw SaveFileFormatException("HARGA_HOTEL", path.string(), col, line);
            }
            for (int i = 0; i < 6; i++) {
                col++;
                if (!(bufferStream << property.rent[i])) {
                    throw SaveFileFormatException("SEWA_L" + std::to_string(i), path.string(), col, line);
                }
            }
        }
        config.properties.push_back(property);
    }
}
void ConfigLoader::loadRailroad(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    // JUMLAH_RAILROAD BIAYA_SEWA
    int line = 0;
    std::string buffer;
    while (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);
        int col = 0;
        int count;
        long long rent;

        col++;
        if (!(bufferStream >> count)) {
            throw SaveFileFormatException("JUMLAH_RAILROAD", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> rent)) {
            throw SaveFileFormatException("BIAYA_SEWA", path.string(), col, line);
        }
        config.railroadRent[count] = rent;
    }
}
void ConfigLoader::loadUtility(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    // JUMLAH_UTILITY BIAYA_SEWA
    int line = 0;
    std::string buffer;
    while (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);
        int col = 0;
        int count;
        long long rent;

        col++;
        if (!(bufferStream >> count)) {
            throw SaveFileFormatException("JUMLAH_UTILITY", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> rent)) {
            throw SaveFileFormatException("BIAYA_SEWA", path.string(), col, line);
        }
        config.utilityRent[count] = rent;
    }
}
void ConfigLoader::loadTax(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    // PPH_FLAT PPH_PERSENTASE PBM_FLAT
    int line = 0;
    std::string buffer;
    if (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> config.incomeFlatTax)) {
            throw SaveFileFormatException("PPH_FLAT", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.incomePercentageTax)) {
            throw SaveFileFormatException("PPH_PERSENTASE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.luxuryFlatTax)) {
            throw SaveFileFormatException("PBM_FLAT", path.string(), col, line);
        }
    }
    else {
        throw SaveFileFormatException("PPH_FLAT", path.string(), 1, line);
    }
}
void ConfigLoader::loadSpecial(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    // GO_SALARY JAIL_FINE
    int line = 0;
    std::string buffer;
    if (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> config.goSalary)) {
            throw SaveFileFormatException("GO_SALARY", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.jailFine)) {
            throw SaveFileFormatException("JAIL_FINE", path.string(), col, line);
        }
    }
    else {
        throw SaveFileFormatException("GO_SALARY", path.string(), 1, line);
    }
}
void ConfigLoader::loadMisc(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw SaveFileNotFoundException(path.string());
    }

    // MAX_TURN SALDO_AWAL
    int line = 0;
    std::string buffer;
    if (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);
        int col = 0;
        col++;
        if (!(bufferStream >> config.maxTurn)) {
            throw SaveFileFormatException("MAX_TURN", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.initialMoney)) {
            throw SaveFileFormatException("SALDO_AWAL", path.string(), col, line);
        }
    }
    else {
        throw SaveFileFormatException("GO_SALARY", path.string(), 1, line);
    }
}