#include <exception>
#include <string>

class PropertyException : public std::exception {
    protected:
        std::string errorMessage;
    public:
        explicit PropertyException(const std::string& msg) : errorMessage("[Property] " + msg) {}
        const char* what() const noexcept override { 
            return errorMessage.c_str(); 
        }
};

class StreetPropertyException : public PropertyException {
    public:
        using PropertyException::PropertyException;
};

class CantBuildException : public StreetPropertyException {
    public:
        explicit CantBuildException(const std::string& msg) : StreetPropertyException(msg) {}
};

class InvalidHouseCountException : public StreetPropertyException {
    public:
        explicit InvalidHouseCountException(const std::string& msg) : StreetPropertyException(msg) {}
};

class CantRemoveBuilding: public StreetPropertyException {
    public:
        explicit CantRemoveBuilding(const std::string& msg): StreetPropertyException(msg) {}      
};