#include <fstream>
#include <sstream>

#include "utils/config/ConfigException.hpp"
#include "utils/config/ConfigLoader.hpp"

Config ConfigLoader::loadConfig(std::string path)
{
    path = "config/" + path;
    if (path.back() != '/') {
        path += "/";
    }
    if (!std::filesystem::exists(path)) {
        throw ConfigFileNotFoundException(path);
    }

    Config config;

    std::filesystem::path actionTilePath = path + "aksi.txt";
    std::filesystem::path propertyPath = path + "property.txt";
    std::filesystem::path railroadPath = path + "railroad.txt";
    std::filesystem::path utilityPath = path + "utility.txt";
    std::filesystem::path taxPath = path + "tax.txt";
    std::filesystem::path specialPath = path + "special.txt";
    std::filesystem::path miscPath = path + "misc.txt";

    loadActionTile(config, actionTilePath);
    loadProperty(config, propertyPath);
    loadRailroad(config, railroadPath);
    loadUtility(config, utilityPath);
    loadTax(config, taxPath);
    loadSpecial(config, specialPath);
    loadMisc(config, miscPath);

    return config;
}

void ConfigLoader::loadActionTile(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
    }

    // ID KODE NAMA JENIS WARNA
    int line = 0;
    std::string buffer;
    while (std::getline(in, buffer)) {
        line++;
        int col = 0;

        ActionTileConfig actionTile;
        std::stringstream bufferStream(buffer);
        col++;
        if (!(bufferStream >> actionTile.id)) {
            throw ConfigFileFormatException("ID", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> actionTile.code)) {
            throw ConfigFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> actionTile.name)) {
            throw ConfigFileFormatException("NAMA", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> actionTile.color)) {
            throw ConfigFileFormatException("JENIS", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> actionTile.color)) {
            throw ConfigFileFormatException("WARNA", path.string(), col, line);
        }
        config.actionTiles.push_back(actionTile);
    }
}

void ConfigLoader::loadProperty(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
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
        if (!(bufferStream >> property.id)) {
            throw ConfigFileFormatException("ID", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.code)) {
            throw ConfigFileFormatException("KODE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.name)) {
            throw ConfigFileFormatException("NAMA", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.color)) {
            throw ConfigFileFormatException("JENIS", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.color)) {
            throw ConfigFileFormatException("WARNA", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.price)) {
            throw ConfigFileFormatException("HARGA_LAHAN", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> property.mortgageValue)) {
            throw ConfigFileFormatException("NILAI_GADAI", path.string(), col, line);
        }

        if (property.type == "STREET") {
            col++;
            if (!(bufferStream >> property.housePrice)) {
                throw ConfigFileFormatException("HARGA_RUMAH", path.string(), col, line);
            }
            col++;
            if (!(bufferStream >> property.hotelPrice)) {
                throw ConfigFileFormatException("HARGA_HOTEL", path.string(), col, line);
            }
            for (int i = 0; i < 6; i++) {
                col++;
                if (!(bufferStream >> property.rent[i])) {
                    throw ConfigFileFormatException("SEWA_L" + std::to_string(i), path.string(), col, line);
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
        throw ConfigFileNotFoundException(path.string());
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
            throw ConfigFileFormatException("JUMLAH_RAILROAD", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> rent)) {
            throw ConfigFileFormatException("BIAYA_SEWA", path.string(), col, line);
        }
        config.railroadRent[count] = rent;
    }
}
void ConfigLoader::loadUtility(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
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
            throw ConfigFileFormatException("JUMLAH_UTILITY", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> rent)) {
            throw ConfigFileFormatException("BIAYA_SEWA", path.string(), col, line);
        }
        config.utilityRent[count] = rent;
    }
}
void ConfigLoader::loadTax(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
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
            throw ConfigFileFormatException("PPH_FLAT", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.incomePercentageTax)) {
            throw ConfigFileFormatException("PPH_PERSENTASE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.luxuryFlatTax)) {
            throw ConfigFileFormatException("PBM_FLAT", path.string(), col, line);
        }
    }
    else {
        throw ConfigFileFormatException("PPH_FLAT", path.string(), 1, line);
    }
}
void ConfigLoader::loadSpecial(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
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
            throw ConfigFileFormatException("GO_SALARY", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.jailFine)) {
            throw ConfigFileFormatException("JAIL_FINE", path.string(), col, line);
        }
    }
    else {
        throw ConfigFileFormatException("GO_SALARY", path.string(), 1, line);
    }
}
void ConfigLoader::loadMisc(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
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
            throw ConfigFileFormatException("MAX_TURN", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> config.initialMoney)) {
            throw ConfigFileFormatException("SALDO_AWAL", path.string(), col, line);
        }
    }
    else {
        throw ConfigFileFormatException("GO_SALARY", path.string(), 1, line);
    }
}

void ConfigLoader::loadChanceCard(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
    }

    // TIPE NILAI PESAN
    int line = 0;
    std::string buffer;
    while (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);

        CardConfig card;
        int col = 0;
        col++;
        if (!(bufferStream >> card.type)) {
            throw ConfigFileFormatException("TIPE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> card.value)) {
            throw ConfigFileFormatException("NILAI", path.string(), col, line);
        }
        col++;
        if (!(std::getline(bufferStream, card.message))) {
            throw ConfigFileFormatException("PESAN", path.string(), col, line);
        }

        config.chanceCards.push_back(card);
    }
}

void ConfigLoader::loadCommunityChestCard(Config &config, std::filesystem::path path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigFileNotFoundException(path.string());
    }

    // TIPE NILAI PESAN
    int line = 0;
    std::string buffer;
    while (std::getline(in, buffer)) {
        line++;
        std::stringstream bufferStream(buffer);

        CardConfig card;
        int col = 0;
        col++;
        if (!(bufferStream >> card.type)) {
            throw ConfigFileFormatException("TIPE", path.string(), col, line);
        }
        col++;
        if (!(bufferStream >> card.value)) {
            throw ConfigFileFormatException("NILAI", path.string(), col, line);
        }
        col++;
        if (!(std::getline(bufferStream, card.message))) {
            throw ConfigFileFormatException("PESAN", path.string(), col, line);
        }

        config.communityChestCards.push_back(card);
    }
}