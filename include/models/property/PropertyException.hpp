#include <exception>
#include <string>

class PropertyException: public std::exception {
    public:
        PropertyException(const std::string& errorMessage): errorMessage(errorMessage) {}
        const char* what() const noexcept {
            return errorMessage.c_str();
        }
    private:
        std::string errorMessage;

};

class StreetProperty: virtual public PropertyException {
    public:
        StreetProperty(const std::string& errorMessage): PropertyException(errorMessage) {}
};

class CantBuildException: public StreetProperty {
    public:
        CantBuildException(const std::string& errorMessage): PropertyException(errorMessage), StreetProperty(errorMessage) {}
    };
    
class InvalidHouseCountException: public StreetProperty {
    public:
        InvalidHouseCountException(const std::string& errorMessage): PropertyException(errorMessage), StreetProperty(errorMessage) {}
    };
    
class CantRemoveBuilding: public StreetProperty {
    public:
        CantRemoveBuilding(const std::string& errorMessage): PropertyException(errorMessage), StreetProperty(errorMessage) {}      
};