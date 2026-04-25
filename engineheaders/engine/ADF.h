#pragma once
#include "engine/master.h"
#include <variant>
#include <optional>
#include <map>
#include <string>
#include <vector>

class ADFObject {
    enum class TokenType {
        String,
        StartObject,
        StartArray,
        End
    };
    struct Token {
        TokenType type;
        std::optional<std::string> content;
        inline Token() {}
        inline Token (TokenType Type, std::optional<std::string> Content = std::nullopt) { type = Type; content = Content; }
    };

    std::variant<std::map<std::string, ADFObject>, std::string, std::vector<ADFObject>> data;


    class Tokenizer {
        std::ifstream filestream;
    public:
        inline Tokenizer(std::string FilePath) {
            filestream = Filesystem::GetFile(FilePath, std::ios::in | std::ios_base::binary);
        }
        Token ReadToken();
    };

public:
    enum class ADFType {
        map,
        array
    };

    //! Creates an ADF tree from a .adf file.
    static ENGINEEXPORT ADFObject FromFile(std::string FilePath);

    inline std::string GetString() {
        if (!std::holds_alternative<std::string>(data)) {
            Engine::Error("Tried to get a string value from a different type of an ADF entry!");
        }
        return std::get<std::string>(data);
    }
    inline std::map<std::string, ADFObject> GetChildren() {
        if (!std::holds_alternative<std::map<std::string, ADFObject>>(data)) {
            Engine::Error("Tried to get a list of children from a different type of an ADF entry!");
        }
        return std::get<std::map<std::string, ADFObject>>(data);
    }
    inline std::vector<ADFObject> GetArray() {
        if (!std::holds_alternative<std::vector<ADFObject>>(data)) {
            Engine::Error("Tried to get an array from a different type of an ADF entry!");
        }
        return std::get<std::vector<ADFObject>>(data);
    }

    inline ADFObject& operator[](int i) {
        return GetArray()[i];
    }

    inline ADFObject& operator[](std::string name) {
        return GetChildren().at(name);
    }

    //! Used for manual adding of entries.
    inline ADFObject(decltype(data) Content, ADFType) { data = Content; }
private:
    ADFObject(ADFType Type, Tokenizer& Tokenizer);
    ADFObject(std::string Content) { data = Content; }
};